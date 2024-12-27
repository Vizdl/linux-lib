/* vi: set sw=4 ts=4: */
/*
 * ifplugd for busybox
 *
 * Copyright (C) 2009 Maksym Kryzhanovskyy <xmaks@email.cz>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */
#include "libbb.h"

#include "fix_u32.h"
#include <linux/if.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <net/ethernet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/sockios.h>
#include <syslog.h>

#define __user
#include <linux/wireless.h>

/*
TODO: describe compat status here.

One questionable point of the design is netlink usage:

We have 1 second timeout by default to poll the link status,
it is short enough so that there are no real benefits in
using netlink to get "instantaneous" interface creation/deletion
notifications. We can check for interface existence by just
doing some fast ioctl using its name.

Netlink code then can be just dropped (1k or more?)
*/


#define IFPLUGD_ENV_PREVIOUS "IFPLUGD_PREVIOUS"
#define IFPLUGD_ENV_CURRENT "IFPLUGD_CURRENT"

enum {
	FLAG_NO_AUTO			= 1 <<  0, // -a, Do not enable interface automatically
	FLAG_NO_DAEMON			= 1 <<  1, // -n, Do not daemonize
	FLAG_NO_SYSLOG			= 1 <<  2, // -s, Do not use syslog, use stderr instead
	FLAG_IGNORE_FAIL		= 1 <<  3, // -f, Ignore detection failure, retry instead (failure is treated as DOWN)
	FLAG_IGNORE_FAIL_POSITIVE	= 1 <<  4, // -F, Ignore detection failure, retry instead (failure is treated as UP)
	FLAG_IFACE			= 1 <<  5, // -i, Specify ethernet interface
	FLAG_RUN			= 1 <<  6, // -r, Specify program to execute
	FLAG_IGNORE_RETVAL		= 1 <<  7, // -I, Don't exit on nonzero return value of program executed
	FLAG_POLL_TIME			= 1 <<  8, // -t, Specify poll time in seconds
	FLAG_DELAY_UP			= 1 <<  9, // -u, Specify delay for configuring interface
	FLAG_DELAY_DOWN			= 1 << 10, // -d, Specify delay for deconfiguring interface
	FLAG_API_MODE			= 1 << 11, // -m, Force API mode (mii, priv, ethtool, wlan, auto)
	FLAG_NO_STARTUP			= 1 << 12, // -p, Don't run script on daemon startup
	FLAG_NO_SHUTDOWN		= 1 << 13, // -q, Don't run script on daemon quit
	FLAG_INITIAL_DOWN		= 1 << 14, // -l, Run "down" script on startup if no cable is detected
	FLAG_EXTRA_ARG			= 1 << 15, // -x, Specify an extra argument for action script
	FLAG_MONITOR			= 1 << 16, // -M, Use interface monitoring
#if ENABLE_FEATURE_PIDFILE
	FLAG_KILL			= 1 << 17, // -k, Kill a running daemon
#endif
};
#if ENABLE_FEATURE_PIDFILE
# define OPTION_STR "+ansfFi:r:It:u:d:m:pqlx:Mk"
#else
# define OPTION_STR "+ansfFi:r:It:u:d:m:pqlx:M"
#endif

enum { // api mode
	API_AUTO	= 'a',
	API_ETHTOOL	= 'e',
	API_MII		= 'm',
	API_PRIVATE	= 'p',
	API_WLAN	= 'w',
	API_IFF		= 'i',
};

enum { // interface status
	IFSTATUS_ERR = -1,
	IFSTATUS_DOWN = 0,
	IFSTATUS_UP = 1,
};

enum { // constant fds
	ioctl_fd = 3,
	netlink_fd = 4,
};

struct globals {
	smallint iface_last_status;
	smallint iface_exists;

	/* Used in getopt32, must have sizeof == sizeof(int) */
	unsigned poll_time;
	unsigned delay_up;
	unsigned delay_down;

	const char *iface;
	const char *api_mode;
	const char *script_name;
	const char *extra_arg;

	smallint (*detect_link_func)(void);
	smallint (*cached_detect_link_func)(void);
};
#define G (*ptr_to_globals)
#define INIT_G() do { \
	SET_PTR_TO_GLOBALS(xzalloc(sizeof(G))); \
	G.iface_last_status = -1; \
	G.iface_exists   = 1; \
	G.poll_time      = 1; \
	G.delay_down     = 5; \
	G.iface          = "eth0"; \
	G.api_mode       = "a"; \
	G.script_name    = "/etc/ifplugd/ifplugd.action"; \
} while (0)


static int run_script(const char *action)
{
	pid_t pid;
	int r;

	bb_error_msg("executing '%s %s %s'", G.script_name, G.iface, action);

#if 1
	pid = vfork();
	if (pid < 0) {
		bb_perror_msg("fork");
		return -1;
	}

	if (pid == 0) {
		/* child */
		execlp(G.script_name, G.script_name, G.iface, action, G.extra_arg, NULL);
		bb_perror_msg_and_die("can't execute '%s'", G.script_name);
	}

	/* parent */
	wait(&r);
	r = WEXITSTATUS(r);

	bb_error_msg("exit code: %u", r);
	return (option_mask32 & FLAG_IGNORE_RETVAL) ? 0 : r;

#else /* insanity */

	struct fd_pair pipe_pair;
	char buf[256];
	int i = 0;

	xpiped_pair(pipe_pair);

	pid = vfork();
	if (pid < 0) {
		bb_perror_msg("fork");
		return -1;
	}

	/* child */
	if (pid == 0) {
		xmove_fd(pipe_pair.wr, 1);
		xdup2(1, 2);
		if (pipe_pair.rd > 2)
			close(pipe_pair.rd);

		// umask(0022); // Set up a sane umask

		execlp(G.script_name, G.script_name, G.iface, action, G.extra_arg, NULL);
		_exit(EXIT_FAILURE);
	}

	/* parent */
	close(pipe_pair.wr);

	while (1) {
		if (bb_got_signal && bb_got_signal != SIGCHLD) {
			bb_error_msg("killing child");
			kill(pid, SIGTERM);
			bb_got_signal = 0;
			break;
		}

		r = read(pipe_pair.rd, &buf[i], 1);

		if (buf[i] == '\n' || i == sizeof(buf)-2 || r != 1) {
			if (r == 1 && buf[i] != '\n')
				i++;

			buf[i] = '\0';

			if (i > 0)
				bb_error_msg("client: %s", buf);

			i = 0;
		} else {
			i++;
		}

		if (r != 1)
			break;
	}

	close(pipe_pair.rd);

	wait(&r);

	if (!WIFEXITED(r) || WEXITSTATUS(r) != 0) {
		bb_error_msg("program execution failed, return value is %i",
			WEXITSTATUS(r));
		return option_mask32 & FLAG_IGNORE_RETVAL ? 0 : WEXITSTATUS(r);
	}
	bb_error_msg("program executed successfully");
	return 0;
#endif
}

static int network_ioctl(int request, void* data)
{
	return ioctl(ioctl_fd, request, data);
}

static void set_ifreq_to_ifname(struct ifreq *ifreq)
{
	memset(ifreq, 0, sizeof(struct ifreq));
	strncpy_IFNAMSIZ(ifreq->ifr_name, G.iface);
}

static const char *strstatus(int status)
{
	if (status == IFSTATUS_ERR)
		return "error";
	return "down\0up" + (status * 5);
}

static void up_iface(void)
{
	struct ifreq ifrequest;

	if (!G.iface_exists)
		return;

	set_ifreq_to_ifname(&ifrequest);
	if (network_ioctl(SIOCGIFFLAGS, &ifrequest) < 0) {
		bb_perror_msg("can't %cet interface flags", 'g');
		G.iface_exists = 0;
		return;
	}

	if (!(ifrequest.ifr_flags & IFF_UP)) {
		ifrequest.ifr_flags |= IFF_UP;
		/* Let user know we mess up with interface */
		bb_error_msg("upping interface");
		if (network_ioctl(SIOCSIFFLAGS, &ifrequest) < 0)
			bb_perror_msg_and_die("can't %cet interface flags", 's');
	}

#if 0 /* why do we mess with IP addr? It's not our business */
	if (network_ioctl(SIOCGIFADDR, &ifrequest) < 0) {
		bb_error_msg("can't get interface address");
	} else if (ifrequest.ifr_addr.sa_family != AF_INET) {
		bb_perror_msg("the interface is not IP-based");
	} else {
		((struct sockaddr_in*)(&ifrequest.ifr_addr))->sin_addr.s_addr = INADDR_ANY;
		if (network_ioctl(SIOCSIFADDR, &ifrequest) < 0)
			bb_perror_msg("can't set interface address");
	}
	if (network_ioctl(SIOCGIFFLAGS, &ifrequest) < 0) {
		bb_perror_msg("can't get interface flags");
		return;
	}
#endif
}

static void maybe_up_new_iface(void)
{
	if (!(option_mask32 & FLAG_NO_AUTO))
		up_iface();

#if 0 /* bloat */
	struct ifreq ifrequest;
	struct ethtool_drvinfo driver_info;

	set_ifreq_to_ifname(&ifrequest);
	driver_info.cmd = ETHTOOL_GDRVINFO;
	ifrequest.ifr_data = &driver_info;
	if (network_ioctl(SIOCETHTOOL, &ifrequest) == 0) {
		char buf[sizeof("/xx:xx:xx:xx:xx:xx")];

		/* Get MAC */
		buf[0] = '\0';
		set_ifreq_to_ifname(&ifrequest);
		if (network_ioctl(SIOCGIFHWADDR, &ifrequest) == 0) {
			sprintf(buf, "/%02X:%02X:%02X:%02X:%02X:%02X",
				(uint8_t)(ifrequest.ifr_hwaddr.sa_data[0]),
				(uint8_t)(ifrequest.ifr_hwaddr.sa_data[1]),
				(uint8_t)(ifrequest.ifr_hwaddr.sa_data[2]),
				(uint8_t)(ifrequest.ifr_hwaddr.sa_data[3]),
				(uint8_t)(ifrequest.ifr_hwaddr.sa_data[4]),
				(uint8_t)(ifrequest.ifr_hwaddr.sa_data[5]));
		}

		bb_error_msg("using interface %s%s with driver<%s> (version: %s)",
			G.iface, buf, driver_info.driver, driver_info.version);
	}
#endif

	G.cached_detect_link_func = NULL;
}

static smallint detect_link_mii(void)
{
	struct ifreq ifreq;
	struct mii_ioctl_data *mii = (void *)&ifreq.ifr_data;

	set_ifreq_to_ifname(&ifreq);

	if (network_ioctl(SIOCGMIIPHY, &ifreq) < 0) {
		bb_perror_msg("SIOCGMIIPHY failed");
		return IFSTATUS_ERR;
	}

	mii->reg_num = 1;

	if (network_ioctl(SIOCGMIIREG, &ifreq) < 0) {
		bb_perror_msg("SIOCGMIIREG failed");
		return IFSTATUS_ERR;
	}

	return (mii->val_out & 0x0004) ? IFSTATUS_UP : IFSTATUS_DOWN;
}

static smallint detect_link_priv(void)
{
	struct ifreq ifreq;
	struct mii_ioctl_data *mii = (void *)&ifreq.ifr_data;

	set_ifreq_to_ifname(&ifreq);

	if (network_ioctl(SIOCDEVPRIVATE, &ifreq) < 0) {
		bb_perror_msg("SIOCDEVPRIVATE failed");
		return IFSTATUS_ERR;
	}

	mii->reg_num = 1;

	if (network_ioctl(SIOCDEVPRIVATE+1, &ifreq) < 0) {
		bb_perror_msg("SIOCDEVPRIVATE+1 failed");
		return IFSTATUS_ERR;
	}

	return (mii->val_out & 0x0004) ? IFSTATUS_UP : IFSTATUS_DOWN;
}

static smallint detect_link_ethtool(void)
{
	struct ifreq ifreq;
	struct ethtool_value edata;

	set_ifreq_to_ifname(&ifreq);

	edata.cmd = ETHTOOL_GLINK;
	ifreq.ifr_data = &edata;

	if (network_ioctl(SIOCETHTOOL, &ifreq) < 0) {
		bb_perror_msg("ETHTOOL_GLINK failed");
		return IFSTATUS_ERR;
	}

	return edata.data ? IFSTATUS_UP : IFSTATUS_DOWN;
}

static smallint detect_link_iff(void)
{
	struct ifreq ifreq;

	set_ifreq_to_ifname(&ifreq);

	if (network_ioctl(SIOCGIFFLAGS, &ifreq) < 0) {
		bb_perror_msg("SIOCGIFFLAGS failed");
		return IFSTATUS_ERR;
	}

	return (ifreq.ifr_flags & IFF_RUNNING) ? IFSTATUS_UP : IFSTATUS_DOWN;
}

static smallint detect_link_wlan(void)
{
	struct iwreq iwrequest;
	uint8_t mac[ETH_ALEN];

	memset(&iwrequest, 0, sizeof(struct iwreq));
	strncpy_IFNAMSIZ(iwrequest.ifr_ifrn.ifrn_name, G.iface);

	if (network_ioctl(SIOCGIWAP, &iwrequest) < 0) {
		bb_perror_msg("SIOCGIWAP failed");
		return IFSTATUS_ERR;
	}

	memcpy(mac, &(iwrequest.u.ap_addr.sa_data), ETH_ALEN);

	if (mac[0] == 0xFF || mac[0] == 0x44 || mac[0] == 0x00) {
		for (int i = 1; i < ETH_ALEN; ++i) {
			if (mac[i] != mac[0])
				return IFSTATUS_UP;
		}
		return IFSTATUS_DOWN;
	}

	return IFSTATUS_UP;
}

static smallint detect_link_auto(void)
{
	const char *method;
	smallint iface_status;
	smallint sv_logmode;

	if (G.cached_detect_link_func) {
		iface_status = G.cached_detect_link_func();
		if (iface_status != IFSTATUS_ERR)
			return iface_status;
	}

	sv_logmode = logmode;
	logmode = LOGMODE_NONE;

	iface_status = detect_link_ethtool();
	if (iface_status != IFSTATUS_ERR) {
		G.cached_detect_link_func = detect_link_ethtool;
		method = "SIOCETHTOOL";
 found_method:
		logmode = sv_logmode;
		bb_error_msg("using %s detection mode", method);
		return iface_status;
	}

	iface_status = detect_link_mii();
	if (iface_status != IFSTATUS_ERR) {
		G.cached_detect_link_func = detect_link_mii;
		method = "SIOCGMIIPHY";
		goto found_method;
	}

	iface_status = detect_link_priv();
	if (iface_status != IFSTATUS_ERR) {
		G.cached_detect_link_func = detect_link_priv;
		method = "SIOCDEVPRIVATE";
		goto found_method;
	}

	iface_status = detect_link_wlan();
	if (iface_status != IFSTATUS_ERR) {
		G.cached_detect_link_func = detect_link_wlan;
		method = "wireless extension";
		goto found_method;
	}

	iface_status = detect_link_iff();
	if (iface_status != IFSTATUS_ERR) {
		G.cached_detect_link_func = detect_link_iff;
		method = "IFF_RUNNING";
		goto found_method;
	}

	logmode = sv_logmode;
	return iface_status; /* IFSTATUS_ERR */
}

static smallint detect_link(void)
{
	smallint status;

	if (!G.iface_exists)
		return (option_mask32 & FLAG_MONITOR) ? IFSTATUS_DOWN : IFSTATUS_ERR;

#if 0
/* Why? This behavior makes it hard to temporary down the iface.
 * It makes a bit more sense to do only in maybe_up_new_iface.
 * OTOH, maybe detect_link_wlan needs this. Then it should be done
 * _only_ there.
 */
	if (!(option_mask32 & FLAG_NO_AUTO))
		up_iface();
#endif

	status = G.detect_link_func();
	if (status == IFSTATUS_ERR) {
		if (option_mask32 & FLAG_IGNORE_FAIL)
			status = IFSTATUS_DOWN;
		if (option_mask32 & FLAG_IGNORE_FAIL_POSITIVE)
			status = IFSTATUS_UP;
	}

	if (status == IFSTATUS_ERR
	 && G.detect_link_func == detect_link_auto
	) {
		bb_error_msg("failed to detect link status");
	}

	if (status != G.iface_last_status) {
//TODO: is it safe to repeatedly do this?
		setenv(IFPLUGD_ENV_PREVIOUS, strstatus(G.iface_last_status), 1);
		setenv(IFPLUGD_ENV_CURRENT, strstatus(status), 1);
		G.iface_last_status = status;
	}

	return status;
}

static NOINLINE int check_existence_through_netlink(void)
{
	char replybuf[1024];

	while (1) {
		struct nlmsghdr *mhdr;
		ssize_t bytes;

		bytes = recv(netlink_fd, &replybuf, sizeof(replybuf), MSG_DONTWAIT);
		if (bytes < 0) {
			if (errno == EAGAIN)
				return G.iface_exists;
			if (errno == EINTR)
				continue;

			bb_perror_msg("netlink: recv");
			return -1;
		}

		mhdr = (struct nlmsghdr*)replybuf;
		while (bytes > 0) {
			if (!NLMSG_OK(mhdr, bytes)
			 || bytes < sizeof(struct nlmsghdr)
			 || bytes < mhdr->nlmsg_len
			) {
				bb_error_msg("netlink packet too small or truncated");
				return -1;
			}

			if (mhdr->nlmsg_type == RTM_NEWLINK || mhdr->nlmsg_type == RTM_DELLINK) {
				struct rtattr *attr;
				struct ifinfomsg *imsg;
				int attr_len;

				imsg = NLMSG_DATA(mhdr);

				if (mhdr->nlmsg_len < NLMSG_LENGTH(sizeof(struct ifinfomsg))) {
					bb_error_msg("netlink packet too small or truncated");
					return -1;
				}

				attr = (struct rtattr*)((char*)imsg + NLMSG_ALIGN(sizeof(struct ifinfomsg)));
				attr_len = NLMSG_PAYLOAD(mhdr, sizeof(struct ifinfomsg));

				while (RTA_OK(attr, attr_len)) {
					if (attr->rta_type == IFLA_IFNAME) {
						char ifname[IFNAMSIZ + 1];
						int len = RTA_PAYLOAD(attr);

						if (len > IFNAMSIZ)
							len = IFNAMSIZ;
						memcpy(ifname, RTA_DATA(attr), len);
						if (strcmp(G.iface, ifname) == 0) {
							G.iface_exists = (mhdr->nlmsg_type == RTM_NEWLINK);
						}
					}
					attr = RTA_NEXT(attr, attr_len);
				}
			}

			mhdr = NLMSG_NEXT(mhdr, bytes);
		}
	}

	return G.iface_exists;
}

static NOINLINE int netlink_open(void)
{
	int fd;
	struct sockaddr_nl addr;

	fd = xsocket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);

	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_groups = RTMGRP_LINK;
	addr.nl_pid = getpid();

	xbind(fd, (struct sockaddr*)&addr, sizeof(addr));

	return fd;
}

#if ENABLE_FEATURE_PIDFILE
static NOINLINE pid_t read_pid(const char *filename)
{
	int len;
	char buf[128];

	len = open_read_close(filename, buf, 127);
	if (len > 0) {
		buf[len] = '\0';
		/* returns ULONG_MAX on error => -1 */
		return bb_strtoul(buf, NULL, 10);
	}
	return 0;
}
#endif

int ifplugd_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ifplugd_main(int argc UNUSED_PARAM, char **argv)
{
	int iface_status;
	int delay_time;
	const char *iface_status_str;
	struct pollfd netlink_pollfd[1];
	unsigned opts;
#if ENABLE_FEATURE_PIDFILE
	char *pidfile_name;
	pid_t pid_from_pidfile;
#endif

	INIT_G();

	opt_complementary = "t+:u+:d+";
	opts = getopt32(argv, OPTION_STR,
		&G.iface, &G.script_name, &G.poll_time, &G.delay_up,
		&G.delay_down, &G.api_mode, &G.extra_arg);

	applet_name = xasprintf("ifplugd(%s)", G.iface);

#if ENABLE_FEATURE_PIDFILE
	pidfile_name = xasprintf(_PATH_VARRUN"ifplugd.%s.pid", G.iface);
	pid_from_pidfile = read_pid(pidfile_name);

	if (opts & FLAG_KILL) {
		if (pid_from_pidfile > 0)
			kill(pid_from_pidfile, SIGQUIT);
		return EXIT_SUCCESS;
	}

	if (pid_from_pidfile > 0 && kill(pid_from_pidfile, 0) == 0)
		bb_error_msg_and_die("daemon already running");
#endif

	switch (G.api_mode[0]) {
	case API_AUTO:
		G.detect_link_func = detect_link_auto;
		break;
	case API_ETHTOOL:
		G.detect_link_func = detect_link_ethtool;
		break;
	case API_MII:
		G.detect_link_func = detect_link_mii;
		break;
	case API_PRIVATE:
		G.detect_link_func = detect_link_priv;
		break;
	case API_WLAN:
		G.detect_link_func = detect_link_wlan;
		break;
	case API_IFF:
		G.detect_link_func = detect_link_iff;
		break;
	default:
		bb_error_msg_and_die("unknown API mode '%s'", G.api_mode);
	}

	if (!(opts & FLAG_NO_DAEMON))
		bb_daemonize_or_rexec(DAEMON_CHDIR_ROOT, argv);

	xmove_fd(xsocket(AF_INET, SOCK_DGRAM, 0), ioctl_fd);
	if (opts & FLAG_MONITOR) {
		xmove_fd(netlink_open(), netlink_fd);
	}

	write_pidfile(pidfile_name);

	/* this can't be moved before socket creation */
	if (!(opts & FLAG_NO_SYSLOG)) {
		openlog(applet_name, 0, LOG_DAEMON);
		logmode |= LOGMODE_SYSLOG;
	}

	bb_signals(0
		| (1 << SIGINT )
		| (1 << SIGTERM)
		| (1 << SIGQUIT)
		| (1 << SIGHUP ) /* why we ignore it? */
		/* | (1 << SIGCHLD) - run_script does not use it anymore */
		, record_signo);

	bb_error_msg("started: %s", bb_banner);

	if (opts & FLAG_MONITOR) {
		struct ifreq ifrequest;
		set_ifreq_to_ifname(&ifrequest);
		G.iface_exists = (network_ioctl(SIOCGIFINDEX, &ifrequest) == 0);
	}

	if (G.iface_exists)
		maybe_up_new_iface();

	iface_status = detect_link();
	if (iface_status == IFSTATUS_ERR)
		goto exiting;
	iface_status_str = strstatus(iface_status);

	if (opts & FLAG_MONITOR) {
		bb_error_msg("interface %s",
			G.iface_exists ? "exists"
			: "doesn't exist, waiting");
	}
	/* else we assume it always exists, but don't mislead user
	 * by potentially lying that it really exists */

	if (G.iface_exists) {
		bb_error_msg("link is %s", iface_status_str);
	}

	if ((!(opts & FLAG_NO_STARTUP)
	     && iface_status == IFSTATUS_UP
	    )
	 || (opts & FLAG_INITIAL_DOWN)
	) {
		if (run_script(iface_status_str) != 0)
			goto exiting;
	}

	/* Main loop */
	netlink_pollfd[0].fd = netlink_fd;
	netlink_pollfd[0].events = POLLIN;
	delay_time = 0;
	while (1) {
		int iface_status_old;
		int iface_exists_old;

		switch (bb_got_signal) {
		case SIGINT:
		case SIGTERM:
			bb_got_signal = 0;
			goto cleanup;
		case SIGQUIT:
			bb_got_signal = 0;
			goto exiting;
		default:
			bb_got_signal = 0;
			break;
		}

		if (poll(netlink_pollfd,
				(opts & FLAG_MONITOR) ? 1 : 0,
				G.poll_time * 1000
			) < 0
		) {
			if (errno == EINTR)
				continue;
			bb_perror_msg("poll");
			goto exiting;
		}

		iface_status_old = iface_status;
		iface_exists_old = G.iface_exists;

		if ((opts & FLAG_MONITOR)
		 && (netlink_pollfd[0].revents & POLLIN)
		) {
			G.iface_exists = check_existence_through_netlink();
			if (G.iface_exists < 0) /* error */
				goto exiting;
			if (iface_exists_old != G.iface_exists) {
				bb_error_msg("interface %sappeared",
						G.iface_exists ? "" : "dis");
				if (G.iface_exists)
					maybe_up_new_iface();
			}
		}

		/* note: if !G.iface_exists, returns DOWN */
		iface_status = detect_link();
		if (iface_status == IFSTATUS_ERR) {
			if (!(opts & FLAG_MONITOR))
				goto exiting;
			iface_status = IFSTATUS_DOWN;
		}
		iface_status_str = strstatus(iface_status);

		if (iface_status_old != iface_status) {
			bb_error_msg("link is %s", iface_status_str);

			if (delay_time) {
				/* link restored its old status before
				 * we run script. don't run the script: */
				delay_time = 0;
			} else {
				delay_time = monotonic_sec();
				if (iface_status == IFSTATUS_UP)
					delay_time += G.delay_up;
				if (iface_status == IFSTATUS_DOWN)
					delay_time += G.delay_down;
				if (delay_time == 0)
					delay_time++;
			}
		}

		if (delay_time && (int)(monotonic_sec() - delay_time) >= 0) {
			delay_time = 0;
			if (run_script(iface_status_str) != 0)
				goto exiting;
		}
	} /* while (1) */

 cleanup:
	if (!(opts & FLAG_NO_SHUTDOWN)
	 && (iface_status == IFSTATUS_UP
	     || (iface_status == IFSTATUS_DOWN && delay_time)
	    )
	) {
		setenv(IFPLUGD_ENV_PREVIOUS, strstatus(iface_status), 1);
		setenv(IFPLUGD_ENV_CURRENT, strstatus(-1), 1);
		run_script("down\0up"); /* reusing string */
	}

 exiting:
	remove_pidfile(pidfile_name);
	bb_error_msg_and_die("exiting");
}
