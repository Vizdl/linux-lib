/* vi: set sw=4 ts=4: */
/*
 * Mini ping implementation for busybox
 *
 * Copyright (C) 1999 by Randolph Chung <tausq@debian.org>
 *
 * Adapted from the ping in netkit-base 0.10:
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Muuss.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */
/* from ping6.c:
 * Copyright (C) 1999 by Randolph Chung <tausq@debian.org>
 *
 * This version of ping is adapted from the ping in netkit-base 0.10,
 * which is:
 *
 * Original copyright notice is retained at the end of this file.
 *
 * This version is an adaptation of ping.c from busybox.
 * The code was modified by Bart Visscher <magick@linux-fan.com>
 */

#include <net/if.h>
#include <netinet/ip_icmp.h>
#include "libbb.h"

#if ENABLE_PING6
#include <netinet/icmp6.h>
/* I see RENUMBERED constants in bits/in.h - !!?
 * What a fuck is going on with libc? Is it a glibc joke? */
#ifdef IPV6_2292HOPLIMIT
#undef IPV6_HOPLIMIT
#define IPV6_HOPLIMIT IPV6_2292HOPLIMIT
#endif
#endif

enum {
	DEFDATALEN = 56,
	MAXIPLEN = 60,
	MAXICMPLEN = 76,
	MAXPACKET = 65468,
	MAX_DUP_CHK = (8 * 128),
	MAXWAIT = 10,
	PINGINTERVAL = 1, /* 1 second */
};

/* common routines */

static int in_cksum(unsigned short *buf, int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return ans;
}

#if !ENABLE_FEATURE_FANCY_PING

/* simple version */

static char *hostname;

static void noresp(int ign UNUSED_PARAM)
{
	printf("No response from %s\n", hostname);
	exit(EXIT_FAILURE);
}

static void ping4(len_and_sockaddr *lsa)
{
	struct sockaddr_in pingaddr;
	struct icmp *pkt;
	int pingsock, c;
	char packet[DEFDATALEN + MAXIPLEN + MAXICMPLEN];

	pingsock = create_icmp_socket();
	pingaddr = lsa->u.sin;

	pkt = (struct icmp *) packet;
	memset(pkt, 0, sizeof(packet));
	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_cksum = in_cksum((unsigned short *) pkt, sizeof(packet));

	c = xsendto(pingsock, packet, DEFDATALEN + ICMP_MINLEN,
			   (struct sockaddr *) &pingaddr, sizeof(pingaddr));

	/* listen for replies */
	while (1) {
		struct sockaddr_in from;
		socklen_t fromlen = sizeof(from);

		c = recvfrom(pingsock, packet, sizeof(packet), 0,
				(struct sockaddr *) &from, &fromlen);
		if (c < 0) {
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
			continue;
		}
		if (c >= 76) {			/* ip + icmp */
			struct iphdr *iphdr = (struct iphdr *) packet;

			pkt = (struct icmp *) (packet + (iphdr->ihl << 2));	/* skip ip hdr */
			if (pkt->icmp_type == ICMP_ECHOREPLY)
				break;
		}
	}
	if (ENABLE_FEATURE_CLEAN_UP)
		close(pingsock);
}

#if ENABLE_PING6
static void ping6(len_and_sockaddr *lsa)
{
	struct sockaddr_in6 pingaddr;
	struct icmp6_hdr *pkt;
	int pingsock, c;
	int sockopt;
	char packet[DEFDATALEN + MAXIPLEN + MAXICMPLEN];

	pingsock = create_icmp6_socket();
	pingaddr = lsa->u.sin6;

	pkt = (struct icmp6_hdr *) packet;
	memset(pkt, 0, sizeof(packet));
	pkt->icmp6_type = ICMP6_ECHO_REQUEST;

	sockopt = offsetof(struct icmp6_hdr, icmp6_cksum);
	setsockopt(pingsock, SOL_RAW, IPV6_CHECKSUM, &sockopt, sizeof(sockopt));

	c = xsendto(pingsock, packet, DEFDATALEN + sizeof (struct icmp6_hdr),
			   (struct sockaddr *) &pingaddr, sizeof(pingaddr));

	/* listen for replies */
	while (1) {
		struct sockaddr_in6 from;
		socklen_t fromlen = sizeof(from);

		c = recvfrom(pingsock, packet, sizeof(packet), 0,
				(struct sockaddr *) &from, &fromlen);
		if (c < 0) {
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
			continue;
		}
		if (c >= 8) {			/* icmp6_hdr */
			pkt = (struct icmp6_hdr *) packet;
			if (pkt->icmp6_type == ICMP6_ECHO_REPLY)
				break;
		}
	}
	if (ENABLE_FEATURE_CLEAN_UP)
		close(pingsock);
}
#endif

#if !ENABLE_PING6
# define common_ping_main(af, argv) common_ping_main(argv)
#endif
static int common_ping_main(sa_family_t af, char **argv)
{
	len_and_sockaddr *lsa;

#if ENABLE_PING6
	while ((++argv)[0] && argv[0][0] == '-') {
		if (argv[0][1] == '4') {
			af = AF_INET;
			continue;
		}
		if (argv[0][1] == '6') {
			af = AF_INET6;
			continue;
		}
		bb_show_usage();
	}
#else
	argv++;
#endif

	hostname = *argv;
	if (!hostname)
		bb_show_usage();

#if ENABLE_PING6
	lsa = xhost_and_af2sockaddr(hostname, 0, af);
#else
	lsa = xhost_and_af2sockaddr(hostname, 0, AF_INET);
#endif
	/* Set timer _after_ DNS resolution */
	signal(SIGALRM, noresp);
	alarm(5); /* give the host 5000ms to respond */

#if ENABLE_PING6
	if (lsa->u.sa.sa_family == AF_INET6)
		ping6(lsa);
	else
#endif
		ping4(lsa);
	printf("%s is alive!\n", hostname);
	return EXIT_SUCCESS;
}


#else /* FEATURE_FANCY_PING */


/* full(er) version */

#define OPT_STRING ("qvc:s:w:W:I:4" IF_PING6("6"))
enum {
	OPT_QUIET = 1 << 0,
	OPT_VERBOSE = 1 << 1,
	OPT_c = 1 << 2,
	OPT_s = 1 << 3,
	OPT_w = 1 << 4,
	OPT_W = 1 << 5,
	OPT_I = 1 << 6,
	OPT_IPV4 = 1 << 7,
	OPT_IPV6 = (1 << 8) * ENABLE_PING6,
};


struct globals {
	int pingsock;
	int if_index;
	char *str_I;
	len_and_sockaddr *source_lsa;
	unsigned datalen;
	unsigned pingcount; /* must be int-sized */
	unsigned long ntransmitted, nreceived, nrepeats;
	uint16_t myid;
	unsigned tmin, tmax; /* in us */
	unsigned long long tsum; /* in us, sum of all times */
	unsigned deadline;
	unsigned timeout;
	unsigned total_secs;
	const char *hostname;
	const char *dotted;
	union {
		struct sockaddr sa;
		struct sockaddr_in sin;
#if ENABLE_PING6
		struct sockaddr_in6 sin6;
#endif
	} pingaddr;
	char rcvd_tbl[MAX_DUP_CHK / 8];
};
#define G (*(struct globals*)&bb_common_bufsiz1)
#define pingsock     (G.pingsock    )
#define if_index     (G.if_index    )
#define source_lsa   (G.source_lsa  )
#define str_I        (G.str_I       )
#define datalen      (G.datalen     )
#define ntransmitted (G.ntransmitted)
#define nreceived    (G.nreceived   )
#define nrepeats     (G.nrepeats    )
#define pingcount    (G.pingcount   )
#define myid         (G.myid        )
#define tmin         (G.tmin        )
#define tmax         (G.tmax        )
#define tsum         (G.tsum        )
#define deadline     (G.deadline    )
#define timeout      (G.timeout     )
#define total_secs   (G.total_secs  )
#define hostname     (G.hostname    )
#define dotted       (G.dotted      )
#define pingaddr     (G.pingaddr    )
#define rcvd_tbl     (G.rcvd_tbl    )
void BUG_ping_globals_too_big(void);
#define INIT_G() do { \
	if (sizeof(G) > COMMON_BUFSIZE) \
		BUG_ping_globals_too_big(); \
	pingsock = -1; \
	datalen = DEFDATALEN; \
	timeout = MAXWAIT; \
	tmin = UINT_MAX; \
} while (0)


#define	A(bit)		rcvd_tbl[(bit)>>3]	/* identify byte in array */
#define	B(bit)		(1 << ((bit) & 0x07))	/* identify bit in byte */
#define	SET(bit)	(A(bit) |= B(bit))
#define	CLR(bit)	(A(bit) &= (~B(bit)))
#define	TST(bit)	(A(bit) & B(bit))

/**************************************************************************/

static void print_stats_and_exit(int junk) NORETURN;
static void print_stats_and_exit(int junk UNUSED_PARAM)
{
	signal(SIGINT, SIG_IGN);

	printf("\n--- %s ping statistics ---\n", hostname);
	printf("%lu packets transmitted, ", ntransmitted);
	printf("%lu packets received, ", nreceived);
	if (nrepeats)
		printf("%lu duplicates, ", nrepeats);
	if (ntransmitted)
		ntransmitted = (ntransmitted - nreceived) * 100 / ntransmitted;
	printf("%lu%% packet loss\n", ntransmitted);
	if (tmin != UINT_MAX) {
		unsigned tavg = tsum / (nreceived + nrepeats);
		printf("round-trip min/avg/max = %u.%03u/%u.%03u/%u.%03u ms\n",
			tmin / 1000, tmin % 1000,
			tavg / 1000, tavg % 1000,
			tmax / 1000, tmax % 1000);
	}
	/* if condition is true, exit with 1 -- 'failure' */
	exit(nreceived == 0 || (deadline && nreceived < pingcount));
}

static void sendping_tail(void (*sp)(int), const void *pkt, int size_pkt)
{
	int sz;

	CLR((uint16_t)ntransmitted % MAX_DUP_CHK);
	ntransmitted++;

	/* sizeof(pingaddr) can be larger than real sa size, but I think
	 * it doesn't matter */
	sz = xsendto(pingsock, pkt, size_pkt, &pingaddr.sa, sizeof(pingaddr));
	if (sz != size_pkt)
		bb_error_msg_and_die(bb_msg_write_error);

	if (pingcount == 0 || deadline || ntransmitted < pingcount) {
		/* Didn't send all pings yet - schedule next in 1s */
		signal(SIGALRM, sp);
		if (deadline) {
			total_secs += PINGINTERVAL;
			if (total_secs >= deadline)
				signal(SIGALRM, print_stats_and_exit);
		}
		alarm(PINGINTERVAL);
	} else { /* -c NN, and all NN are sent (and no deadline) */
		/* Wait for the last ping to come back.
		 * -W timeout: wait for a response in seconds.
		 * Affects only timeout in absense of any responses,
		 * otherwise ping waits for two RTTs. */
		unsigned expire = timeout;

		if (nreceived) {
			/* approx. 2*tmax, in seconds (2 RTT) */
			expire = tmax / (512*1024);
			if (expire == 0)
				expire = 1;
		}
		signal(SIGALRM, print_stats_and_exit);
		alarm(expire);
	}
}

static void sendping4(int junk UNUSED_PARAM)
{
	/* +4 reserves a place for timestamp, which may end up sitting
	 * *after* packet. Saves one if() */
	struct icmp *pkt = alloca(datalen + ICMP_MINLEN + 4);

	memset(pkt, 0, datalen + ICMP_MINLEN + 4);
	pkt->icmp_type = ICMP_ECHO;
	/*pkt->icmp_code = 0;*/
	/*pkt->icmp_cksum = 0;*/
	pkt->icmp_seq = htons(ntransmitted); /* don't ++ here, it can be a macro */
	pkt->icmp_id = myid;

	/* We don't do hton, because we will read it back on the same machine */
	/*if (datalen >= 4)*/
		*(uint32_t*)&pkt->icmp_dun = monotonic_us();

	pkt->icmp_cksum = in_cksum((unsigned short *) pkt, datalen + ICMP_MINLEN);

	sendping_tail(sendping4, pkt, datalen + ICMP_MINLEN);
}
#if ENABLE_PING6
static void sendping6(int junk UNUSED_PARAM)
{
	struct icmp6_hdr *pkt = alloca(datalen + sizeof(struct icmp6_hdr) + 4);

	memset(pkt, 0, datalen + sizeof(struct icmp6_hdr) + 4);
	pkt->icmp6_type = ICMP6_ECHO_REQUEST;
	/*pkt->icmp6_code = 0;*/
	/*pkt->icmp6_cksum = 0;*/
	pkt->icmp6_seq = htons(ntransmitted); /* don't ++ here, it can be a macro */
	pkt->icmp6_id = myid;

	/*if (datalen >= 4)*/
		*(uint32_t*)(&pkt->icmp6_data8[4]) = monotonic_us();

	sendping_tail(sendping6, pkt, datalen + sizeof(struct icmp6_hdr));
}
#endif

static const char *icmp_type_name(int id)
{
	switch (id) {
	case ICMP_ECHOREPLY:      return "Echo Reply";
	case ICMP_DEST_UNREACH:   return "Destination Unreachable";
	case ICMP_SOURCE_QUENCH:  return "Source Quench";
	case ICMP_REDIRECT:       return "Redirect (change route)";
	case ICMP_ECHO:           return "Echo Request";
	case ICMP_TIME_EXCEEDED:  return "Time Exceeded";
	case ICMP_PARAMETERPROB:  return "Parameter Problem";
	case ICMP_TIMESTAMP:      return "Timestamp Request";
	case ICMP_TIMESTAMPREPLY: return "Timestamp Reply";
	case ICMP_INFO_REQUEST:   return "Information Request";
	case ICMP_INFO_REPLY:     return "Information Reply";
	case ICMP_ADDRESS:        return "Address Mask Request";
	case ICMP_ADDRESSREPLY:   return "Address Mask Reply";
	default:                  return "unknown ICMP type";
	}
}
#if ENABLE_PING6
/* RFC3542 changed some definitions from RFC2292 for no good reason, whee!
 * the newer 3542 uses a MLD_ prefix where as 2292 uses ICMP6_ prefix */
#ifndef MLD_LISTENER_QUERY
# define MLD_LISTENER_QUERY ICMP6_MEMBERSHIP_QUERY
#endif
#ifndef MLD_LISTENER_REPORT
# define MLD_LISTENER_REPORT ICMP6_MEMBERSHIP_REPORT
#endif
#ifndef MLD_LISTENER_REDUCTION
# define MLD_LISTENER_REDUCTION ICMP6_MEMBERSHIP_REDUCTION
#endif
static const char *icmp6_type_name(int id)
{
	switch (id) {
	case ICMP6_DST_UNREACH:      return "Destination Unreachable";
	case ICMP6_PACKET_TOO_BIG:   return "Packet too big";
	case ICMP6_TIME_EXCEEDED:    return "Time Exceeded";
	case ICMP6_PARAM_PROB:       return "Parameter Problem";
	case ICMP6_ECHO_REPLY:       return "Echo Reply";
	case ICMP6_ECHO_REQUEST:     return "Echo Request";
	case MLD_LISTENER_QUERY:     return "Listener Query";
	case MLD_LISTENER_REPORT:    return "Listener Report";
	case MLD_LISTENER_REDUCTION: return "Listener Reduction";
	default:                     return "unknown ICMP type";
	}
}
#endif

static void unpack_tail(int sz, uint32_t *tp,
		const char *from_str,
		uint16_t recv_seq, int ttl)
{
	const char *dupmsg = " (DUP!)";
	unsigned triptime = triptime; /* for gcc */

	++nreceived;

	if (tp) {
		/* (int32_t) cast is for hypothetical 64-bit unsigned */
		/* (doesn't hurt 32-bit real-world anyway) */
		triptime = (int32_t) ((uint32_t)monotonic_us() - *tp);
		tsum += triptime;
		if (triptime < tmin)
			tmin = triptime;
		if (triptime > tmax)
			tmax = triptime;
	}

	if (TST(recv_seq % MAX_DUP_CHK)) {
		++nrepeats;
		--nreceived;
	} else {
		SET(recv_seq % MAX_DUP_CHK);
		dupmsg += 7;
	}

	if (option_mask32 & OPT_QUIET)
		return;

	printf("%d bytes from %s: seq=%u ttl=%d", sz,
		from_str, recv_seq, ttl);
	if (tp)
		printf(" time=%u.%03u ms", triptime / 1000, triptime % 1000);
	puts(dupmsg);
	fflush(stdout);
}
static void unpack4(char *buf, int sz, struct sockaddr_in *from)
{
	struct icmp *icmppkt;
	struct iphdr *iphdr;
	int hlen;

	/* discard if too short */
	if (sz < (datalen + ICMP_MINLEN))
		return;

	/* check IP header */
	iphdr = (struct iphdr *) buf;
	hlen = iphdr->ihl << 2;
	sz -= hlen;
	icmppkt = (struct icmp *) (buf + hlen);
	if (icmppkt->icmp_id != myid)
		return;				/* not our ping */

	if (icmppkt->icmp_type == ICMP_ECHOREPLY) {
		uint16_t recv_seq = ntohs(icmppkt->icmp_seq);
		uint32_t *tp = NULL;

		if (sz >= ICMP_MINLEN + sizeof(uint32_t))
			tp = (uint32_t *) icmppkt->icmp_data;
		unpack_tail(sz, tp,
			inet_ntoa(*(struct in_addr *) &from->sin_addr.s_addr),
			recv_seq, iphdr->ttl);
	} else if (icmppkt->icmp_type != ICMP_ECHO) {
		bb_error_msg("warning: got ICMP %d (%s)",
				icmppkt->icmp_type,
				icmp_type_name(icmppkt->icmp_type));
	}
}
#if ENABLE_PING6
static void unpack6(char *packet, int sz, /*struct sockaddr_in6 *from,*/ int hoplimit)
{
	struct icmp6_hdr *icmppkt;
	char buf[INET6_ADDRSTRLEN];

	/* discard if too short */
	if (sz < (datalen + sizeof(struct icmp6_hdr)))
		return;

	icmppkt = (struct icmp6_hdr *) packet;
	if (icmppkt->icmp6_id != myid)
		return;				/* not our ping */

	if (icmppkt->icmp6_type == ICMP6_ECHO_REPLY) {
		uint16_t recv_seq = ntohs(icmppkt->icmp6_seq);
		uint32_t *tp = NULL;

		if (sz >= sizeof(struct icmp6_hdr) + sizeof(uint32_t))
			tp = (uint32_t *) &icmppkt->icmp6_data8[4];
		unpack_tail(sz, tp,
			inet_ntop(AF_INET6, &pingaddr.sin6.sin6_addr,
					buf, sizeof(buf)),
			recv_seq, hoplimit);
	} else if (icmppkt->icmp6_type != ICMP6_ECHO_REQUEST) {
		bb_error_msg("warning: got ICMP %d (%s)",
				icmppkt->icmp6_type,
				icmp6_type_name(icmppkt->icmp6_type));
	}
}
#endif

static void ping4(len_and_sockaddr *lsa)
{
	char packet[datalen + MAXIPLEN + MAXICMPLEN];
	int sockopt;

	pingsock = create_icmp_socket();
	pingaddr.sin = lsa->u.sin;
	if (source_lsa) {
		if (setsockopt(pingsock, IPPROTO_IP, IP_MULTICAST_IF,
				&source_lsa->u.sa, source_lsa->len))
			bb_error_msg_and_die("can't set multicast source interface");
		xbind(pingsock, &source_lsa->u.sa, source_lsa->len);
	}
	if (str_I)
		setsockopt_bindtodevice(pingsock, str_I);

	/* enable broadcast pings */
	setsockopt_broadcast(pingsock);

	/* set recv buf (needed if we can get lots of responses: flood ping,
	 * broadcast ping etc) */
	sockopt = (datalen * 2) + 7 * 1024; /* giving it a bit of extra room */
	setsockopt(pingsock, SOL_SOCKET, SO_RCVBUF, &sockopt, sizeof(sockopt));

	signal(SIGINT, print_stats_and_exit);

	/* start the ping's going ... */
	sendping4(0);

	/* listen for replies */
	while (1) {
		struct sockaddr_in from;
		socklen_t fromlen = (socklen_t) sizeof(from);
		int c;

		c = recvfrom(pingsock, packet, sizeof(packet), 0,
				(struct sockaddr *) &from, &fromlen);
		if (c < 0) {
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
			continue;
		}
		unpack4(packet, c, &from);
		if (pingcount && nreceived >= pingcount)
			break;
	}
}
#if ENABLE_PING6
extern int BUG_bad_offsetof_icmp6_cksum(void);
static void ping6(len_and_sockaddr *lsa)
{
	char packet[datalen + MAXIPLEN + MAXICMPLEN];
	int sockopt;
	struct msghdr msg;
	struct sockaddr_in6 from;
	struct iovec iov;
	char control_buf[CMSG_SPACE(36)];

	pingsock = create_icmp6_socket();
	pingaddr.sin6 = lsa->u.sin6;
	/* untested whether "-I addr" really works for IPv6: */
	if (source_lsa)
		xbind(pingsock, &source_lsa->u.sa, source_lsa->len);
	if (str_I)
		setsockopt_bindtodevice(pingsock, str_I);

#ifdef ICMP6_FILTER
	{
		struct icmp6_filter filt;
		if (!(option_mask32 & OPT_VERBOSE)) {
			ICMP6_FILTER_SETBLOCKALL(&filt);
			ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &filt);
		} else {
			ICMP6_FILTER_SETPASSALL(&filt);
		}
		if (setsockopt(pingsock, IPPROTO_ICMPV6, ICMP6_FILTER, &filt,
					   sizeof(filt)) < 0)
			bb_error_msg_and_die("setsockopt(ICMP6_FILTER)");
	}
#endif /*ICMP6_FILTER*/

	/* enable broadcast pings */
	setsockopt_broadcast(pingsock);

	/* set recv buf (needed if we can get lots of responses: flood ping,
	 * broadcast ping etc) */
	sockopt = (datalen * 2) + 7 * 1024; /* giving it a bit of extra room */
	setsockopt(pingsock, SOL_SOCKET, SO_RCVBUF, &sockopt, sizeof(sockopt));

	sockopt = offsetof(struct icmp6_hdr, icmp6_cksum);
	if (offsetof(struct icmp6_hdr, icmp6_cksum) != 2)
		BUG_bad_offsetof_icmp6_cksum();
	setsockopt(pingsock, SOL_RAW, IPV6_CHECKSUM, &sockopt, sizeof(sockopt));

	/* request ttl info to be returned in ancillary data */
	setsockopt(pingsock, SOL_IPV6, IPV6_HOPLIMIT, &const_int_1, sizeof(const_int_1));

	if (if_index)
		pingaddr.sin6.sin6_scope_id = if_index;

	signal(SIGINT, print_stats_and_exit);

	/* start the ping's going ... */
	sendping6(0);

	/* listen for replies */
	msg.msg_name = &from;
	msg.msg_namelen = sizeof(from);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control_buf;
	iov.iov_base = packet;
	iov.iov_len = sizeof(packet);
	while (1) {
		int c;
		struct cmsghdr *mp;
		int hoplimit = -1;
		msg.msg_controllen = sizeof(control_buf);

		c = recvmsg(pingsock, &msg, 0);
		if (c < 0) {
			if (errno != EINTR)
				bb_perror_msg("recvfrom");
			continue;
		}
		for (mp = CMSG_FIRSTHDR(&msg); mp; mp = CMSG_NXTHDR(&msg, mp)) {
			if (mp->cmsg_level == SOL_IPV6
			 && mp->cmsg_type == IPV6_HOPLIMIT
			 /* don't check len - we trust the kernel: */
			 /* && mp->cmsg_len >= CMSG_LEN(sizeof(int)) */
			) {
				/*hoplimit = *(int*)CMSG_DATA(mp); - unaligned access */
				move_from_unaligned_int(hoplimit, CMSG_DATA(mp));
			}
		}
		unpack6(packet, c, /*&from,*/ hoplimit);
		if (pingcount && nreceived >= pingcount)
			break;
	}
}
#endif

static void ping(len_and_sockaddr *lsa)
{
	printf("PING %s (%s)", hostname, dotted);
	if (source_lsa) {
		printf(" from %s",
			xmalloc_sockaddr2dotted_noport(&source_lsa->u.sa));
	}
	printf(": %d data bytes\n", datalen);

#if ENABLE_PING6
	if (lsa->u.sa.sa_family == AF_INET6)
		ping6(lsa);
	else
#endif
		ping4(lsa);
}

static int common_ping_main(int opt, char **argv)
{
	len_and_sockaddr *lsa;
	char *str_s;

	INIT_G();

	/* exactly one argument needed; -v and -q don't mix; -c NUM, -w NUM, -W NUM */
	opt_complementary = "=1:q--v:v--q:c+:w+:W+";
	opt |= getopt32(argv, OPT_STRING, &pingcount, &str_s, &deadline, &timeout, &str_I);
	if (opt & OPT_s)
		datalen = xatou16(str_s); // -s
	if (opt & OPT_I) { // -I
		if_index = if_nametoindex(str_I);
		if (!if_index) {
			/* TODO: I'm not sure it takes IPv6 unless in [XX:XX..] format */
			source_lsa = xdotted2sockaddr(str_I, 0);
			str_I = NULL; /* don't try to bind to device later */
		}
	}
	myid = (uint16_t) getpid();
	hostname = argv[optind];
#if ENABLE_PING6
	{
		sa_family_t af = AF_UNSPEC;
		if (opt & OPT_IPV4)
			af = AF_INET;
		if (opt & OPT_IPV6)
			af = AF_INET6;
		lsa = xhost_and_af2sockaddr(hostname, 0, af);
	}
#else
	lsa = xhost_and_af2sockaddr(hostname, 0, AF_INET);
#endif

	if (source_lsa && source_lsa->u.sa.sa_family != lsa->u.sa.sa_family)
		/* leaking it here... */
		source_lsa = NULL;

	dotted = xmalloc_sockaddr2dotted_noport(&lsa->u.sa);
	ping(lsa);
	print_stats_and_exit(EXIT_SUCCESS);
	/*return EXIT_SUCCESS;*/
}
#endif /* FEATURE_FANCY_PING */


int ping_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ping_main(int argc UNUSED_PARAM, char **argv)
{
#if !ENABLE_FEATURE_FANCY_PING
	return common_ping_main(AF_UNSPEC, argv);
#else
	return common_ping_main(0, argv);
#endif
}

#if ENABLE_PING6
int ping6_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ping6_main(int argc UNUSED_PARAM, char **argv)
{
# if !ENABLE_FEATURE_FANCY_PING
	return common_ping_main(AF_INET6, argv);
# else
	return common_ping_main(OPT_IPV6, argv);
# endif
}
#endif

/* from ping6.c:
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Muuss.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. <BSD Advertising Clause omitted per the July 22, 1999 licensing change
 *		ftp://ftp.cs.berkeley.edu/pub/4bsd/README.Impt.License.Change>
 *
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
