/* vi: set sw=4 ts=4: */
/*
 * Mini ps implementation(s) for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 * Fix for SELinux Support:(c)2007 Hiroshi Shinji <shiroshi@my.email.ne.jp>
 *                         (c)2007 Yuichi Nakamura <ynakam@hitachisoft.jp>
 *
 * Licensed under the GPL version 2, see the file LICENSE in this tarball.
 */

#include "libbb.h"

/* Absolute maximum on output line length */
enum { MAX_WIDTH = 2*1024 };

#if ENABLE_DESKTOP

#include <sys/times.h> /* for times() */
//#include <sys/sysinfo.h> /* for sysinfo() */
#ifndef AT_CLKTCK
#define AT_CLKTCK 17
#endif


#if ENABLE_SELINUX
#define SELINUX_O_PREFIX "label,"
#define DEFAULT_O_STR    (SELINUX_O_PREFIX "pid,user" IF_FEATURE_PS_TIME(",time") ",args")
#else
#define DEFAULT_O_STR    ("pid,user" IF_FEATURE_PS_TIME(",time") ",args")
#endif

typedef struct {
	uint16_t width;
	char name6[6];
	const char *header;
	void (*f)(char *buf, int size, const procps_status_t *ps);
	int ps_flags;
} ps_out_t;

struct globals {
	ps_out_t* out;
	int out_cnt;
	int print_header;
	int need_flags;
	char *buffer;
	unsigned terminal_width;
#if ENABLE_FEATURE_PS_TIME
	unsigned kernel_HZ;
	unsigned long long seconds_since_boot;
#endif
	char default_o[sizeof(DEFAULT_O_STR)];
};
#define G (*(struct globals*)&bb_common_bufsiz1)
#define out                (G.out               )
#define out_cnt            (G.out_cnt           )
#define print_header       (G.print_header      )
#define need_flags         (G.need_flags        )
#define buffer             (G.buffer            )
#define terminal_width     (G.terminal_width    )
#define kernel_HZ          (G.kernel_HZ         )
#define seconds_since_boot (G.seconds_since_boot)
#define default_o          (G.default_o         )

#if ENABLE_FEATURE_PS_TIME
/* for ELF executables, notes are pushed before environment and args */
static ptrdiff_t find_elf_note(ptrdiff_t findme)
{
	ptrdiff_t *ep = (ptrdiff_t *) environ;

	while (*ep++);
	while (*ep) {
		if (ep[0] == findme) {
			return ep[1];
		}
		ep += 2;
	}
	return -1;
}

#if ENABLE_FEATURE_PS_UNUSUAL_SYSTEMS
static unsigned get_HZ_by_waiting(void)
{
	struct timeval tv1, tv2;
	unsigned t1, t2, r, hz;
	unsigned cnt = cnt; /* for compiler */
	int diff;

	r = 0;

	/* Wait for times() to reach new tick */
	t1 = times(NULL);
	do {
		t2 = times(NULL);
	} while (t2 == t1);
	gettimeofday(&tv2, NULL);

	do {
		t1 = t2;
		tv1.tv_usec = tv2.tv_usec;

		/* Wait exactly one times() tick */
		do {
			t2 = times(NULL);
		} while (t2 == t1);
		gettimeofday(&tv2, NULL);

		/* Calculate ticks per sec, rounding up to even */
		diff = tv2.tv_usec - tv1.tv_usec;
		if (diff <= 0) diff += 1000000;
		hz = 1000000u / (unsigned)diff;
		hz = (hz+1) & ~1;

		/* Count how many same hz values we saw */
		if (r != hz) {
			r = hz;
			cnt = 0;
		}
		cnt++;
	} while (cnt < 3); /* exit if saw 3 same values */

	return r;
}
#else
static inline unsigned get_HZ_by_waiting(void)
{
	/* Better method? */
	return 100;
}
#endif

static unsigned get_kernel_HZ(void)
{
	//char buf[64];
	struct sysinfo info;

	if (kernel_HZ)
		return kernel_HZ;

	/* Works for ELF only, Linux 2.4.0+ */
	kernel_HZ = find_elf_note(AT_CLKTCK);
	if (kernel_HZ == (unsigned)-1)
		kernel_HZ = get_HZ_by_waiting();

	//if (open_read_close("/proc/uptime", buf, sizeof(buf) <= 0)
	//	bb_perror_msg_and_die("cannot read %s", "/proc/uptime");
	//buf[sizeof(buf)-1] = '\0';
	///sscanf(buf, "%llu", &seconds_since_boot);
	sysinfo(&info);
	seconds_since_boot = info.uptime;

	return kernel_HZ;
}
#endif

/* Print value to buf, max size+1 chars (including trailing '\0') */

static void func_user(char *buf, int size, const procps_status_t *ps)
{
#if 1
	safe_strncpy(buf, get_cached_username(ps->uid), size+1);
#else
	/* "compatible" version, but it's larger */
	/* procps 2.18 shows numeric UID if name overflows the field */
	/* TODO: get_cached_username() returns numeric string if
	 * user has no passwd record, we will display it
	 * left-justified here; too long usernames are shown
	 * as _right-justified_ IDs. Is it worth fixing? */
	const char *user = get_cached_username(ps->uid);
	if (strlen(user) <= size)
		safe_strncpy(buf, user, size+1);
	else
		sprintf(buf, "%*u", size, (unsigned)ps->uid);
#endif
}

static void func_group(char *buf, int size, const procps_status_t *ps)
{
	safe_strncpy(buf, get_cached_groupname(ps->gid), size+1);
}

static void func_comm(char *buf, int size, const procps_status_t *ps)
{
	safe_strncpy(buf, ps->comm, size+1);
}

static void func_args(char *buf, int size, const procps_status_t *ps)
{
	read_cmdline(buf, size, ps->pid, ps->comm);
}

static void func_pid(char *buf, int size, const procps_status_t *ps)
{
	sprintf(buf, "%*u", size, ps->pid);
}

static void func_ppid(char *buf, int size, const procps_status_t *ps)
{
	sprintf(buf, "%*u", size, ps->ppid);
}

static void func_pgid(char *buf, int size, const procps_status_t *ps)
{
	sprintf(buf, "%*u", size, ps->pgid);
}

static void put_lu(char *buf, int size, unsigned long u)
{
	char buf4[5];

	/* see http://en.wikipedia.org/wiki/Tera */
	smart_ulltoa4(u, buf4, " mgtpezy");
	buf4[4] = '\0';
	sprintf(buf, "%.*s", size, buf4);
}

static void func_vsz(char *buf, int size, const procps_status_t *ps)
{
	put_lu(buf, size, ps->vsz);
}

static void func_rss(char *buf, int size, const procps_status_t *ps)
{
	put_lu(buf, size, ps->rss);
}

static void func_tty(char *buf, int size, const procps_status_t *ps)
{
	buf[0] = '?';
	buf[1] = '\0';
	if (ps->tty_major) /* tty field of "0" means "no tty" */
		snprintf(buf, size+1, "%u,%u", ps->tty_major, ps->tty_minor);
}


#if ENABLE_FEATURE_PS_ADDITIONAL_COLUMNS

static void func_rgroup(char *buf, int size, const procps_status_t *ps)
{
	safe_strncpy(buf, get_cached_groupname(ps->rgid), size+1);
}

static void func_ruser(char *buf, int size, const procps_status_t *ps)
{
	safe_strncpy(buf, get_cached_username(ps->ruid), size+1);
}

static void func_nice(char *buf, int size, const procps_status_t *ps)
{
	sprintf(buf, "%*d", size, ps->niceness);
}

#endif /* FEATURE_PS_ADDITIONAL_COLUMNS */

#if ENABLE_FEATURE_PS_TIME
static void func_etime(char *buf, int size, const procps_status_t *ps)
{
	/* elapsed time [[dd-]hh:]mm:ss; here only mm:ss */
	unsigned long mm;
	unsigned ss;

	mm = ps->start_time / get_kernel_HZ();
	/* must be after get_kernel_HZ()! */
	mm = seconds_since_boot - mm;
	ss = mm % 60;
	mm /= 60;
	snprintf(buf, size+1, "%3lu:%02u", mm, ss);
}

static void func_time(char *buf, int size, const procps_status_t *ps)
{
	/* cumulative time [[dd-]hh:]mm:ss; here only mm:ss */
	unsigned long mm;
	unsigned ss;

	mm = (ps->utime + ps->stime) / get_kernel_HZ();
	ss = mm % 60;
	mm /= 60;
	snprintf(buf, size+1, "%3lu:%02u", mm, ss);
}
#endif

#if ENABLE_SELINUX
static void func_label(char *buf, int size, const procps_status_t *ps)
{
	safe_strncpy(buf, ps->context ? ps->context : "unknown", size+1);
}
#endif

/*
static void func_nice(char *buf, int size, const procps_status_t *ps)
{
	ps->???
}

static void func_pcpu(char *buf, int size, const procps_status_t *ps)
{
}
*/

static const ps_out_t out_spec[] = {
// Mandated by POSIX:
	{ 8                  , "user"  ,"USER"   ,func_user  ,PSSCAN_UIDGID  },
	{ 8                  , "group" ,"GROUP"  ,func_group ,PSSCAN_UIDGID  },
	{ 16                 , "comm"  ,"COMMAND",func_comm  ,PSSCAN_COMM    },
	{ 256                , "args"  ,"COMMAND",func_args  ,PSSCAN_COMM    },
	{ 5                  , "pid"   ,"PID"    ,func_pid   ,PSSCAN_PID     },
	{ 5                  , "ppid"  ,"PPID"   ,func_ppid  ,PSSCAN_PPID    },
	{ 5                  , "pgid"  ,"PGID"   ,func_pgid  ,PSSCAN_PGID    },
#if ENABLE_FEATURE_PS_TIME
	{ sizeof("ELAPSED")-1, "etime" ,"ELAPSED",func_etime ,PSSCAN_START_TIME },
#endif
#if ENABLE_FEATURE_PS_ADDITIONAL_COLUMNS
	{ 5                  , "nice"  ,"NI"     ,func_nice  ,PSSCAN_NICE    },
	{ 8                  , "rgroup","RGROUP" ,func_rgroup,PSSCAN_RUIDGID },
	{ 8                  , "ruser" ,"RUSER"  ,func_ruser ,PSSCAN_RUIDGID },
//	{ 5                  , "pcpu"  ,"%CPU"   ,func_pcpu  ,PSSCAN_        },
#endif
#if ENABLE_FEATURE_PS_TIME
	{ 6                  , "time"  ,"TIME"   ,func_time  ,PSSCAN_STIME | PSSCAN_UTIME },
#endif
	{ 6                  , "tty"   ,"TT"     ,func_tty   ,PSSCAN_TTY     },
	{ 4                  , "vsz"   ,"VSZ"    ,func_vsz   ,PSSCAN_VSZ     },
// Not mandated by POSIX, but useful:
	{ 4                  , "rss"   ,"RSS"    ,func_rss   ,PSSCAN_RSS     },
#if ENABLE_SELINUX
	{ 35                 , "label" ,"LABEL"  ,func_label ,PSSCAN_CONTEXT },
#endif
};

static ps_out_t* new_out_t(void)
{
	out = xrealloc_vector(out, 2, out_cnt);
	return &out[out_cnt++];
}

static const ps_out_t* find_out_spec(const char *name)
{
	unsigned i;
	for (i = 0; i < ARRAY_SIZE(out_spec); i++) {
		if (!strncmp(name, out_spec[i].name6, 6))
			return &out_spec[i];
	}
	bb_error_msg_and_die("bad -o argument '%s'", name);
}

static void parse_o(char* opt)
{
	ps_out_t* new;
	// POSIX: "-o is blank- or comma-separated list" (FIXME)
	char *comma, *equal;
	while (1) {
		comma = strchr(opt, ',');
		equal = strchr(opt, '=');
		if (comma && (!equal || equal > comma)) {
			*comma = '\0';
			*new_out_t() = *find_out_spec(opt);
			*comma = ',';
			opt = comma + 1;
			continue;
		}
		break;
	}
	// opt points to last spec in comma separated list.
	// This one can have =HEADER part.
	new = new_out_t();
	if (equal)
		*equal = '\0';
	*new = *find_out_spec(opt);
	if (equal) {
		*equal = '=';
		new->header = equal + 1;
		// POSIX: the field widths shall be ... at least as wide as
		// the header text (default or overridden value).
		// If the header text is null, such as -o user=,
		// the field width shall be at least as wide as the
		// default header text
		if (new->header[0]) {
			new->width = strlen(new->header);
			print_header = 1;
		}
	} else
		print_header = 1;
}

static void post_process(void)
{
	int i;
	int width = 0;
	for (i = 0; i < out_cnt; i++) {
		need_flags |= out[i].ps_flags;
		if (out[i].header[0]) {
			print_header = 1;
		}
		width += out[i].width + 1; /* "FIELD " */
	}
#if ENABLE_SELINUX
	if (!is_selinux_enabled())
		need_flags &= ~PSSCAN_CONTEXT;
#endif
	buffer = xmalloc(width + 1); /* for trailing \0 */
}

static void format_header(void)
{
	int i;
	ps_out_t* op;
	char *p;

	if (!print_header)
		return;
	p = buffer;
	i = 0;
	if (out_cnt) {
		while (1) {
			op = &out[i];
			if (++i == out_cnt) /* do not pad last field */
				break;
			p += sprintf(p, "%-*s ", op->width, op->header);
		}
		strcpy(p, op->header);
	}
	printf("%.*s\n", terminal_width, buffer);
}

static void format_process(const procps_status_t *ps)
{
	int i, len;
	char *p = buffer;
	i = 0;
	if (out_cnt) while (1) {
		out[i].f(p, out[i].width, ps);
		// POSIX: Any field need not be meaningful in all
		// implementations. In such a case a hyphen ( '-' )
		// should be output in place of the field value.
		if (!p[0]) {
			p[0] = '-';
			p[1] = '\0';
		}
		len = strlen(p);
		p += len;
		len = out[i].width - len + 1;
		if (++i == out_cnt) /* do not pad last field */
			break;
		p += sprintf(p, "%*s", len, "");
	}
	printf("%.*s\n", terminal_width, buffer);
}

int ps_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ps_main(int argc UNUSED_PARAM, char **argv)
{
	procps_status_t *p;
	llist_t* opt_o = NULL;
	IF_SELINUX(int opt;)

	// POSIX:
	// -a  Write information for all processes associated with terminals
	//     Implementations may omit session leaders from this list
	// -A  Write information for all processes
	// -d  Write information for all processes, except session leaders
	// -e  Write information for all processes (equivalent to -A.)
	// -f  Generate a full listing
	// -l  Generate a long listing
	// -o col1,col2,col3=header
	//     Select which columns to display
	/* We allow (and ignore) most of the above. FIXME */
	opt_complementary = "o::";
	IF_SELINUX(opt =) getopt32(argv, "Zo:aAdefl", &opt_o);
	if (opt_o) {
		do {
			parse_o(llist_pop(&opt_o));
		} while (opt_o);
	} else {
		/* Below: parse_o() needs char*, NOT const char*... */
#if ENABLE_SELINUX
		if (!(opt & 1) || !is_selinux_enabled()) {
			/* no -Z or no SELinux: do not show LABEL */
			strcpy(default_o, DEFAULT_O_STR + sizeof(SELINUX_O_PREFIX)-1);
		} else
#endif
		{
			strcpy(default_o, DEFAULT_O_STR);
		}
		parse_o(default_o);
	}
	post_process();

	/* Was INT_MAX, but some libc's go belly up with printf("%.*s")
	 * and such large widths */
	terminal_width = MAX_WIDTH;
	if (isatty(1)) {
		get_terminal_width_height(0, &terminal_width, NULL);
		if (--terminal_width > MAX_WIDTH)
			terminal_width = MAX_WIDTH;
	}
	format_header();

	p = NULL;
	while ((p = procps_scan(p, need_flags))) {
		format_process(p);
	}

	return EXIT_SUCCESS;
}


#else /* !ENABLE_DESKTOP */


int ps_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int ps_main(int argc UNUSED_PARAM, char **argv UNUSED_PARAM)
{
	procps_status_t *p = NULL;
	int len;
	IF_NOT_SELINUX(const) int use_selinux = 0;
	IF_SELINUX(int i;)
#if !ENABLE_FEATURE_PS_WIDE
	enum { terminal_width = 79 };
#else
	unsigned terminal_width;
	int w_count = 0;
#endif

#if ENABLE_FEATURE_PS_WIDE || ENABLE_SELINUX
#if ENABLE_FEATURE_PS_WIDE
	opt_complementary = "-:ww";
	IF_SELINUX(i =) getopt32(argv, IF_SELINUX("Z") "w", &w_count);
	/* if w is given once, GNU ps sets the width to 132,
	 * if w is given more than once, it is "unlimited"
	 */
	if (w_count) {
		terminal_width = (w_count==1) ? 132 : MAX_WIDTH;
	} else {
		get_terminal_width_height(0, &terminal_width, NULL);
		/* Go one less... */
		if (--terminal_width > MAX_WIDTH)
			terminal_width = MAX_WIDTH;
	}
#else /* only ENABLE_SELINUX */
	i = getopt32(argv, "Z");
#endif
#if ENABLE_SELINUX
	if ((i & 1) && is_selinux_enabled())
		use_selinux = PSSCAN_CONTEXT;
#endif
#endif /* ENABLE_FEATURE_PS_WIDE || ENABLE_SELINUX */

	if (use_selinux)
		puts("  PID CONTEXT                          STAT COMMAND");
	else
		puts("  PID USER       VSZ STAT COMMAND");

	while ((p = procps_scan(p, 0
			| PSSCAN_PID
			| PSSCAN_UIDGID
			| PSSCAN_STATE
			| PSSCAN_VSZ
			| PSSCAN_COMM
			| use_selinux
	))) {
#if ENABLE_SELINUX
		if (use_selinux) {
			len = printf("%5u %-32.32s %s  ",
					p->pid,
					p->context ? p->context : "unknown",
					p->state);
		} else
#endif
		{
			const char *user = get_cached_username(p->uid);
			//if (p->vsz == 0)
			//	len = printf("%5u %-8.8s        %s ",
			//		p->pid, user, p->state);
			//else
			{
				char buf6[6];
				smart_ulltoa5(p->vsz, buf6, " mgtpezy");
				buf6[5] = '\0';
				len = printf("%5u %-8.8s %s %s  ",
					p->pid, user, buf6, p->state);
			}
		}

		{
			int sz = terminal_width - len;
			char buf[sz + 1];
			read_cmdline(buf, sz, p->pid, p->comm);
			puts(buf);
		}
	}
	if (ENABLE_FEATURE_CLEAN_UP)
		clear_username_cache();
	return EXIT_SUCCESS;
}

#endif /* ENABLE_DESKTOP */
