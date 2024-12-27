/* vi: set sw=4 ts=4: */
/*
 * Mini start-stop-daemon implementation(s) for busybox
 *
 * Written by Marek Michalkiewicz <marekm@i17linuxb.ists.pwr.wroc.pl>,
 * Adapted for busybox David Kimdon <dwhedon@gordian.com>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/*
This is how it is supposed to work:

start-stop-daemon [OPTIONS] [--start|--stop] [[--] arguments...]

One (only) of these must be given:
        -S,--start              Start
        -K,--stop               Stop

Search for matching processes.
If --stop is given, stop all matching processes (by sending a signal).
If --start is given, start a new process unless a matching process was found.

Options controlling process matching
(if multiple conditions are specified, all must match):
        -u,--user USERNAME|UID  Only consider this user's processes
        -n,--name PROCESS_NAME  Look for processes by matching PROCESS_NAME
                                with comm field in /proc/$PID/stat.
                                Only basename is compared:
                                "ntpd" == "./ntpd" == "/path/to/ntpd".
[TODO: can PROCESS_NAME be a full pathname? Should we require full match then
with /proc/$PID/exe or argv[0] (comm can't be matched, it never contains path)]
        -x,--exec EXECUTABLE    Look for processes that were started with this
                                command in /proc/$PID/cmdline.
                                Unlike -n, we match against the full path:
                                "ntpd" != "./ntpd" != "/path/to/ntpd"
        -p,--pidfile PID_FILE   Look for processes with PID from this file

Options which are valid for --start only:
        -x,--exec EXECUTABLE    Program to run (1st arg of execvp). Mandatory.
        -a,--startas NAME       argv[0] (defaults to EXECUTABLE)
        -b,--background         Put process into background
        -N,--nicelevel N        Add N to process' nice level
        -c,--chuid USER[:[GRP]] Change to specified user [and group]
        -m,--make-pidfile       Write PID to the pidfile
                                (both -m and -p must be given!)

Options which are valid for --stop only:
        -s,--signal SIG         Signal to send (default:TERM)
        -t,--test               Exit with status 0 if process is found
                                (we don't actually start or stop daemons)

Misc options:
        -o,--oknodo             Exit with status 0 if nothing is done
        -q,--quiet              Quiet
        -v,--verbose            Verbose
*/

#include <sys/resource.h>

/* Override ENABLE_FEATURE_PIDFILE */
#define WANT_PIDFILE 1
#include "libbb.h"

struct pid_list {
	struct pid_list *next;
	pid_t pid;
};

enum {
	CTX_STOP       = (1 <<  0),
	CTX_START      = (1 <<  1),
	OPT_BACKGROUND = (1 <<  2), // -b
	OPT_QUIET      = (1 <<  3), // -q
	OPT_TEST       = (1 <<  4), // -t
	OPT_MAKEPID    = (1 <<  5), // -m
	OPT_a          = (1 <<  6), // -a
	OPT_n          = (1 <<  7), // -n
	OPT_s          = (1 <<  8), // -s
	OPT_u          = (1 <<  9), // -u
	OPT_c          = (1 << 10), // -c
	OPT_x          = (1 << 11), // -x
	OPT_p          = (1 << 12), // -p
	OPT_OKNODO     = (1 << 13) * ENABLE_FEATURE_START_STOP_DAEMON_FANCY, // -o
	OPT_VERBOSE    = (1 << 14) * ENABLE_FEATURE_START_STOP_DAEMON_FANCY, // -v
	OPT_NICELEVEL  = (1 << 15) * ENABLE_FEATURE_START_STOP_DAEMON_FANCY, // -N
};
#define QUIET (option_mask32 & OPT_QUIET)
#define TEST  (option_mask32 & OPT_TEST)

struct globals {
	struct pid_list *found;
	char *userspec;
	char *cmdname;
	char *execname;
	char *pidfile;
	int user_id;
	smallint signal_nr;
};
#define G (*(struct globals*)&bb_common_bufsiz1)
#define found             (G.found               )
#define userspec          (G.userspec            )
#define cmdname           (G.cmdname             )
#define execname          (G.execname            )
#define pidfile           (G.pidfile             )
#define user_id           (G.user_id             )
#define signal_nr         (G.signal_nr           )
#define INIT_G() do { \
	user_id = -1; \
	signal_nr = 15; \
} while (0)

#ifdef OLDER_VERSION_OF_X
/* -x,--exec EXECUTABLE
 * Look for processes with matching /proc/$PID/exe.
 * Match is performed using device+inode.
 */
static int pid_is_exec(pid_t pid)
{
	struct stat st;
	char buf[sizeof("/proc//exe") + sizeof(int)*3];

	sprintf(buf, "/proc/%u/exe", (unsigned)pid);
	if (stat(buf, &st) < 0)
		return 0;
	if (st.st_dev == execstat.st_dev
	 && st.st_ino == execstat.st_ino)
		return 1;
	return 0;
}
#endif

static int pid_is_exec(pid_t pid)
{
	ssize_t bytes;
	char buf[PATH_MAX];

	sprintf(buf, "/proc/%u/cmdline", (unsigned)pid);
	bytes = open_read_close(buf, buf, sizeof(buf) - 1);
	if (bytes > 0) {
		buf[bytes] = '\0';
		return strcmp(buf, execname) == 0;
	}
	return 0;
}

static int pid_is_name(pid_t pid)
{
	/* /proc/PID/stat is "PID (comm_15_bytes_max) ..." */
	char buf[32]; /* should be enough */
	char *p, *pe;

	sprintf(buf, "/proc/%u/stat", (unsigned)pid);
	if (open_read_close(buf, buf, sizeof(buf) - 1) < 0)
		return 0;
	buf[sizeof(buf) - 1] = '\0'; /* paranoia */
	p = strchr(buf, '(');
	if (!p)
		return 0;
	pe = strrchr(++p, ')');
	if (!pe)
		return 0;
	*pe = '\0';
	/* we require comm to match and to not be truncated */
	/* in Linux, if comm is 15 chars, it may be a truncated
	 * name, so we don't allow that to match */
	if (strlen(p) >= COMM_LEN - 1) /* COMM_LEN is 16 */
		return 0;
	return strcmp(p, cmdname) == 0;
}

static int pid_is_user(int pid)
{
	struct stat sb;
	char buf[sizeof("/proc/") + sizeof(int)*3];

	sprintf(buf, "/proc/%u", (unsigned)pid);
	if (stat(buf, &sb) != 0)
		return 0;
	return (sb.st_uid == (uid_t)user_id);
}

static void check(int pid)
{
	struct pid_list *p;

	if (execname && !pid_is_exec(pid)) {
		return;
	}
	if (cmdname && !pid_is_name(pid)) {
		return;
	}
	if (userspec && !pid_is_user(pid)) {
		return;
	}
	p = xmalloc(sizeof(*p));
	p->next = found;
	p->pid = pid;
	found = p;
}

static void do_pidfile(void)
{
	FILE *f;
	unsigned pid;

	f = fopen_for_read(pidfile);
	if (f) {
		if (fscanf(f, "%u", &pid) == 1)
			check(pid);
		fclose(f);
	} else if (errno != ENOENT)
		bb_perror_msg_and_die("open pidfile %s", pidfile);
}

static void do_procinit(void)
{
	DIR *procdir;
	struct dirent *entry;
	int pid;

	if (pidfile) {
		do_pidfile();
		return;
	}

	procdir = xopendir("/proc");

	pid = 0;
	while (1) {
		errno = 0; /* clear any previous error */
		entry = readdir(procdir);
// TODO: this check is too generic, it's better
// to check for exact errno(s) which mean that we got stale entry
		if (errno) /* Stale entry, process has died after opendir */
			continue;
		if (!entry) /* EOF, no more entries */
			break;
		pid = bb_strtou(entry->d_name, NULL, 10);
		if (errno) /* NaN */
			continue;
		check(pid);
	}
	closedir(procdir);
	if (!pid)
		bb_error_msg_and_die("nothing in /proc - not mounted?");
}

static int do_stop(void)
{
	char *what;
	struct pid_list *p;
	int killed = 0;

	if (cmdname) {
		if (ENABLE_FEATURE_CLEAN_UP) what = xstrdup(cmdname);
		if (!ENABLE_FEATURE_CLEAN_UP) what = cmdname;
	} else if (execname) {
		if (ENABLE_FEATURE_CLEAN_UP) what = xstrdup(execname);
		if (!ENABLE_FEATURE_CLEAN_UP) what = execname;
	} else if (pidfile) {
		what = xasprintf("process in pidfile '%s'", pidfile);
	} else if (userspec) {
		what = xasprintf("process(es) owned by '%s'", userspec);
	} else {
		bb_error_msg_and_die("internal error, please report");
	}

	if (!found) {
		if (!QUIET)
			printf("no %s found; none killed\n", what);
		killed = -1;
		goto ret;
	}
	for (p = found; p; p = p->next) {
		if (TEST || kill(p->pid, signal_nr) == 0) {
			killed++;
		} else {
			p->pid = 0;
			bb_perror_msg("warning: killing process %u", (unsigned)p->pid);
		}
	}
	if (!QUIET && killed) {
		printf("stopped %s (pid", what);
		for (p = found; p; p = p->next)
			if (p->pid)
				printf(" %u", (unsigned)p->pid);
		puts(")");
	}
 ret:
	if (ENABLE_FEATURE_CLEAN_UP)
		free(what);
	return killed;
}

#if ENABLE_FEATURE_START_STOP_DAEMON_LONG_OPTIONS
static const char start_stop_daemon_longopts[] ALIGN1 =
	"stop\0"         No_argument       "K"
	"start\0"        No_argument       "S"
	"background\0"   No_argument       "b"
	"quiet\0"        No_argument       "q"
	"test\0"         No_argument       "t"
	"make-pidfile\0" No_argument       "m"
#if ENABLE_FEATURE_START_STOP_DAEMON_FANCY
	"oknodo\0"       No_argument       "o"
	"verbose\0"      No_argument       "v"
	"nicelevel\0"    Required_argument "N"
#endif
	"startas\0"      Required_argument "a"
	"name\0"         Required_argument "n"
	"signal\0"       Required_argument "s"
	"user\0"         Required_argument "u"
	"chuid\0"        Required_argument "c"
	"exec\0"         Required_argument "x"
	"pidfile\0"      Required_argument "p"
#if ENABLE_FEATURE_START_STOP_DAEMON_FANCY
	"retry\0"        Required_argument "R"
#endif
	;
#endif

int start_stop_daemon_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int start_stop_daemon_main(int argc UNUSED_PARAM, char **argv)
{
	unsigned opt;
	char *signame;
	char *startas;
	char *chuid;
#ifdef OLDER_VERSION_OF_X
	struct stat execstat;
#endif
#if ENABLE_FEATURE_START_STOP_DAEMON_FANCY
//	char *retry_arg = NULL;
//	int retries = -1;
	char *opt_N;
#endif

	INIT_G();

#if ENABLE_FEATURE_START_STOP_DAEMON_LONG_OPTIONS
	applet_long_options = start_stop_daemon_longopts;
#endif

	/* -K or -S is required; they are mutually exclusive */
	/* -p is required if -m is given */
	/* -xpun (at least one) is required if -K is given */
	/* -xa (at least one) is required if -S is given */
	/* -q turns off -v */
	opt_complementary = "K:S:K--S:S--K:m?p:K?xpun:S?xa"
		IF_FEATURE_START_STOP_DAEMON_FANCY("q-v");
	opt = getopt32(argv, "KSbqtma:n:s:u:c:x:p:"
		IF_FEATURE_START_STOP_DAEMON_FANCY("ovN:R:"),
		&startas, &cmdname, &signame, &userspec, &chuid, &execname, &pidfile
		IF_FEATURE_START_STOP_DAEMON_FANCY(,&opt_N)
		/* We accept and ignore -R <param> / --retry <param> */
		IF_FEATURE_START_STOP_DAEMON_FANCY(,NULL)
	);

	if (opt & OPT_s) {
		signal_nr = get_signum(signame);
		if (signal_nr < 0) bb_show_usage();
	}

	if (!(opt & OPT_a))
		startas = execname;
	if (!execname) /* in case -a is given and -x is not */
		execname = startas;

//	IF_FEATURE_START_STOP_DAEMON_FANCY(
//		if (retry_arg)
//			retries = xatoi_u(retry_arg);
//	)
	//argc -= optind;
	argv += optind;

	if (userspec) {
		user_id = bb_strtou(userspec, NULL, 10);
		if (errno)
			user_id = xuname2uid(userspec);
	}
	/* Both start and stop need to know current processes */
	do_procinit();

	if (opt & CTX_STOP) {
		int i = do_stop();
		return (opt & OPT_OKNODO) ? 0 : (i <= 0);
	}

	if (found) {
		if (!QUIET)
			printf("%s is already running\n%u\n", execname, (unsigned)found->pid);
		return !(opt & OPT_OKNODO);
	}

#ifdef OLDER_VERSION_OF_X
	if (execname)
		xstat(execname, &execstat);
#endif

	*--argv = startas;
	if (opt & OPT_BACKGROUND) {
#if BB_MMU
		bb_daemonize(DAEMON_DEVNULL_STDIO + DAEMON_CLOSE_EXTRA_FDS);
		/* DAEMON_DEVNULL_STDIO is superfluous -
		 * it's always done by bb_daemonize() */
#else
		pid_t pid = vfork();
		if (pid < 0) /* error */
			bb_perror_msg_and_die("vfork");
		if (pid != 0) {
			/* parent */
			/* why _exit? the child may have changed the stack,
			 * so "return 0" may do bad things */
			_exit(EXIT_SUCCESS);
		}
		/* Child */
		setsid(); /* detach from controlling tty */
		/* Redirect stdio to /dev/null, close extra FDs.
		 * We do not actually daemonize because of DAEMON_ONLY_SANITIZE */
		bb_daemonize_or_rexec(DAEMON_DEVNULL_STDIO
			+ DAEMON_CLOSE_EXTRA_FDS
			+ DAEMON_ONLY_SANITIZE,
			NULL /* argv, unused */ );
#endif
	}
	if (opt & OPT_MAKEPID) {
		/* User wants _us_ to make the pidfile */
		write_pidfile(pidfile);
	}
	if (opt & OPT_c) {
		struct bb_uidgid_t ugid = { -1, -1 };
		parse_chown_usergroup_or_die(&ugid, chuid);
		if (ugid.gid != (gid_t) -1) xsetgid(ugid.gid);
		if (ugid.uid != (uid_t) -1) xsetuid(ugid.uid);
	}
#if ENABLE_FEATURE_START_STOP_DAEMON_FANCY
	if (opt & OPT_NICELEVEL) {
		/* Set process priority */
		int prio = getpriority(PRIO_PROCESS, 0) + xatoi_range(opt_N, INT_MIN/2, INT_MAX/2);
		if (setpriority(PRIO_PROCESS, 0, prio) < 0) {
			bb_perror_msg_and_die("setpriority(%d)", prio);
		}
	}
#endif
	execvp(startas, argv);
	bb_perror_msg_and_die("cannot start %s", startas);
}
