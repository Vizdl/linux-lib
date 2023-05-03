/* vi: set sw=4 ts=4: */
/*
 * A tiny 'top' utility.
 *
 * This is written specifically for the linux /proc/<PID>/stat(m)
 * files format.
 *
 * This reads the PIDs of all processes and their status and shows
 * the status of processes (first ones that fit to screen) at given
 * intervals.
 *
 * NOTES:
 * - At startup this changes to /proc, all the reads are then
 *   relative to that.
 *
 * (C) Eero Tamminen <oak at welho dot com>
 *
 * Rewritten by Vladimir Oleynik (C) 2002 <dzo@simtreas.ru>
 *
 * Sept 2008: Vineet Gupta <vineet.gupta@arc.com>
 * Added Support for reporting SMP Information
 * - CPU where Process was last seen running
 *   (to see effect of sched_setaffinity() etc)
 * - CPU Time Split (idle/IO/wait etc) PER CPU
 *
 * Copyright (c) 1992 Branko Lankester
 * Copyright (c) 1992 Roger Binns
 * Copyright (C) 1994-1996 Charles L. Blake.
 * Copyright (C) 1992-1998 Michael K. Johnson
 *
 * Licensed under GPLv2, see file LICENSE in this tarball for details.
 */

#include "libbb.h"


typedef struct top_status_t {
	unsigned long vsz;
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
	unsigned long ticks;
	unsigned pcpu; /* delta of ticks */
#endif
	unsigned pid, ppid;
	unsigned uid;
	char state[4];
	char comm[COMM_LEN];
#if ENABLE_FEATURE_TOP_SMP_PROCESS
	int last_seen_on_cpu;
#endif
} top_status_t;

typedef struct jiffy_counts_t {
	/* Linux 2.4.x has only first four */
	unsigned long long usr, nic, sys, idle;
	unsigned long long iowait, irq, softirq, steal;
	unsigned long long total;
	unsigned long long busy;
} jiffy_counts_t;

/* This structure stores some critical information from one frame to
   the next. Used for finding deltas. */
typedef struct save_hist {
	unsigned long ticks;
	pid_t pid;
} save_hist;

typedef int (*cmp_funcp)(top_status_t *P, top_status_t *Q);


enum { SORT_DEPTH = 3 };


struct globals {
	top_status_t *top;
	int ntop;
#if ENABLE_FEATURE_TOPMEM
	smallint sort_field;
	smallint inverted;
#endif
#if ENABLE_FEATURE_TOP_SMP_CPU
	smallint smp_cpu_info; /* one/many cpu info lines? */
#endif
#if ENABLE_FEATURE_USE_TERMIOS
	struct termios initial_settings;
#endif
#if !ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
	cmp_funcp sort_function[1];
#else
	cmp_funcp sort_function[SORT_DEPTH];
	struct save_hist *prev_hist;
	int prev_hist_count;
	jiffy_counts_t cur_jif, prev_jif;
	/* int hist_iterations; */
	unsigned total_pcpu;
	/* unsigned long total_vsz; */
#endif
#if ENABLE_FEATURE_TOP_SMP_CPU
	/* Per CPU samples: current and last */
	jiffy_counts_t *cpu_jif, *cpu_prev_jif;
	int num_cpus;
#endif
	char line_buf[80];
};

enum { LINE_BUF_SIZE = COMMON_BUFSIZE - offsetof(struct globals, line_buf) };

#define G (*(struct globals*)&bb_common_bufsiz1)
#define INIT_G() do { \
	struct G_sizecheck { \
		char G_sizecheck[sizeof(G) > COMMON_BUFSIZE ? -1 : 1]; \
	}; \
} while (0)
#define top              (G.top               )
#define ntop             (G.ntop              )
#define sort_field       (G.sort_field        )
#define inverted         (G.inverted          )
#define smp_cpu_info     (G.smp_cpu_info      )
#define initial_settings (G.initial_settings  )
#define sort_function    (G.sort_function     )
#define prev_hist        (G.prev_hist         )
#define prev_hist_count  (G.prev_hist_count   )
#define cur_jif          (G.cur_jif           )
#define prev_jif         (G.prev_jif          )
#define cpu_jif          (G.cpu_jif           )
#define cpu_prev_jif     (G.cpu_prev_jif      )
#define num_cpus         (G.num_cpus          )
#define total_pcpu       (G.total_pcpu        )
#define line_buf         (G.line_buf          )

enum {
	OPT_d = (1 << 0),
	OPT_n = (1 << 1),
	OPT_b = (1 << 2),
	OPT_EOF = (1 << 3), /* pseudo: "we saw EOF in stdin" */
};
#define OPT_BATCH_MODE (option_mask32 & OPT_b)


#if ENABLE_FEATURE_USE_TERMIOS
static int pid_sort(top_status_t *P, top_status_t *Q)
{
	/* Buggy wrt pids with high bit set */
	/* (linux pids are in [1..2^15-1]) */
	return (Q->pid - P->pid);
}
#endif

static int mem_sort(top_status_t *P, top_status_t *Q)
{
	/* We want to avoid unsigned->signed and truncation errors */
	if (Q->vsz < P->vsz) return -1;
	return Q->vsz != P->vsz; /* 0 if ==, 1 if > */
}


#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE

static int pcpu_sort(top_status_t *P, top_status_t *Q)
{
	/* Buggy wrt ticks with high bit set */
	/* Affects only processes for which ticks overflow */
	return (int)Q->pcpu - (int)P->pcpu;
}

static int time_sort(top_status_t *P, top_status_t *Q)
{
	/* We want to avoid unsigned->signed and truncation errors */
	if (Q->ticks < P->ticks) return -1;
	return Q->ticks != P->ticks; /* 0 if ==, 1 if > */
}

static int mult_lvl_cmp(void* a, void* b)
{
	int i, cmp_val;

	for (i = 0; i < SORT_DEPTH; i++) {
		cmp_val = (*sort_function[i])(a, b);
		if (cmp_val != 0)
			return cmp_val;
	}
	return 0;
}

static NOINLINE int read_cpu_jiffy(FILE *fp, jiffy_counts_t *p_jif)
{
#if !ENABLE_FEATURE_TOP_SMP_CPU
	static const char fmt[] = "cpu %llu %llu %llu %llu %llu %llu %llu %llu";
#else
	static const char fmt[] = "cp%*s %llu %llu %llu %llu %llu %llu %llu %llu";
#endif
	int ret;

	if (!fgets(line_buf, LINE_BUF_SIZE, fp) || line_buf[0] != 'c' /* not "cpu" */)
		return 0;
	ret = sscanf(line_buf, fmt,
			&p_jif->usr, &p_jif->nic, &p_jif->sys, &p_jif->idle,
			&p_jif->iowait, &p_jif->irq, &p_jif->softirq,
			&p_jif->steal);
	if (ret >= 4) {
		p_jif->total = p_jif->usr + p_jif->nic + p_jif->sys + p_jif->idle
			+ p_jif->iowait + p_jif->irq + p_jif->softirq + p_jif->steal;
		/* procps 2.x does not count iowait as busy time */
		p_jif->busy = p_jif->total - p_jif->idle - p_jif->iowait;
	}

	return ret;
}

static void get_jiffy_counts(void)
{
	FILE* fp = xfopen_for_read("stat");

	/* We need to parse cumulative counts even if SMP CPU display is on,
	 * they are used to calculate per process CPU% */
	prev_jif = cur_jif;
	if (read_cpu_jiffy(fp, &cur_jif) < 4)
		bb_error_msg_and_die("can't read /proc/stat");

#if !ENABLE_FEATURE_TOP_SMP_CPU
	fclose(fp);
	return;
#else
	if (!smp_cpu_info) {
		fclose(fp);
		return;
	}

	if (!num_cpus) {
		/* First time here. How many CPUs?
		 * There will be at least 1 /proc/stat line with cpu%d
		 */
		while (1) {
			cpu_jif = xrealloc_vector(cpu_jif, 1, num_cpus);
			if (read_cpu_jiffy(fp, &cpu_jif[num_cpus]) <= 4)
				break;
			num_cpus++;
		}
		if (num_cpus == 0) /* /proc/stat with only "cpu ..." line?! */
			smp_cpu_info = 0;

		cpu_prev_jif = xzalloc(sizeof(cpu_prev_jif[0]) * num_cpus);

		/* Otherwise the first per cpu display shows all 100% idles */
		usleep(50000);
	} else { /* Non first time invocation */
		jiffy_counts_t *tmp;
		int i;

		/* First switch the sample pointers: no need to copy */
		tmp = cpu_prev_jif;
		cpu_prev_jif = cpu_jif;
		cpu_jif = tmp;

		/* Get the new samples */
		for (i = 0; i < num_cpus; i++)
			read_cpu_jiffy(fp, &cpu_jif[i]);
	}
#endif
	fclose(fp);
}

static void do_stats(void)
{
	top_status_t *cur;
	pid_t pid;
	int i, last_i, n;
	struct save_hist *new_hist;

	get_jiffy_counts();
	total_pcpu = 0;
	/* total_vsz = 0; */
	new_hist = xmalloc(sizeof(new_hist[0]) * ntop);
	/*
	 * Make a pass through the data to get stats.
	 */
	/* hist_iterations = 0; */
	i = 0;
	for (n = 0; n < ntop; n++) {
		cur = top + n;

		/*
		 * Calculate time in cur process.  Time is sum of user time
		 * and system time
		 */
		pid = cur->pid;
		new_hist[n].ticks = cur->ticks;
		new_hist[n].pid = pid;

		/* find matching entry from previous pass */
		cur->pcpu = 0;
		/* do not start at index 0, continue at last used one
		 * (brought hist_iterations from ~14000 down to 172) */
		last_i = i;
		if (prev_hist_count) do {
			if (prev_hist[i].pid == pid) {
				cur->pcpu = cur->ticks - prev_hist[i].ticks;
				total_pcpu += cur->pcpu;
				break;
			}
			i = (i+1) % prev_hist_count;
			/* hist_iterations++; */
		} while (i != last_i);
		/* total_vsz += cur->vsz; */
	}

	/*
	 * Save cur frame's information.
	 */
	free(prev_hist);
	prev_hist = new_hist;
	prev_hist_count = ntop;
}

#endif /* FEATURE_TOP_CPU_USAGE_PERCENTAGE */

#if ENABLE_FEATURE_TOP_CPU_GLOBAL_PERCENTS && ENABLE_FEATURE_TOP_DECIMALS
/* formats 7 char string (8 with terminating NUL) */
static char *fmt_100percent_8(char pbuf[8], unsigned value, unsigned total)
{
	unsigned t;
	if (value >= total) { /* 100% ? */
		strcpy(pbuf, "  100% ");
		return pbuf;
	}
	/* else generate " [N/space]N.N% " string */
	value = 1000 * value / total;
	t = value / 100;
	value = value % 100;
	pbuf[0] = ' ';
	pbuf[1] = t ? t + '0' : ' ';
	pbuf[2] = '0' + (value / 10);
	pbuf[3] = '.';
	pbuf[4] = '0' + (value % 10);
	pbuf[5] = '%';
	pbuf[6] = ' ';
	pbuf[7] = '\0';
	return pbuf;
}
#endif

#if ENABLE_FEATURE_TOP_CPU_GLOBAL_PERCENTS
static void display_cpus(int scr_width, char *scrbuf, int *lines_rem_p)
{
	/*
	 * xxx% = (cur_jif.xxx - prev_jif.xxx) / (cur_jif.total - prev_jif.total) * 100%
	 */
	unsigned total_diff;
	jiffy_counts_t *p_jif, *p_prev_jif;
	int i;

#if ENABLE_FEATURE_TOP_SMP_CPU
	int n_cpu_lines;
#endif

	/* using (unsigned) casts to make operations cheaper */
#define  CALC_TOT_DIFF  ((unsigned)(p_jif->total - p_prev_jif->total) ? : 1)

#if ENABLE_FEATURE_TOP_DECIMALS
#define CALC_STAT(xxx) char xxx[8]
#define SHOW_STAT(xxx) fmt_100percent_8(xxx, (unsigned)(p_jif->xxx - p_prev_jif->xxx), total_diff)
#define FMT "%s"
#else
#define CALC_STAT(xxx) unsigned xxx = 100 * (unsigned)(p_jif->xxx - p_prev_jif->xxx) / total_diff
#define SHOW_STAT(xxx) xxx
#define FMT "%4u%% "
#endif

#if !ENABLE_FEATURE_TOP_SMP_CPU
	{
		i = 1;
		p_jif = &cur_jif;
		p_prev_jif = &prev_jif;
#else
	/* Loop thru CPU(s) */
	n_cpu_lines = smp_cpu_info ? num_cpus : 1;
	if (n_cpu_lines > *lines_rem_p)
		n_cpu_lines = *lines_rem_p;

	for (i = 0; i < n_cpu_lines; i++) {
		p_jif = &cpu_jif[i];
		p_prev_jif = &cpu_prev_jif[i];
#endif
		total_diff = CALC_TOT_DIFF;

		{ /* Need a block: CALC_STAT are declarations */
			CALC_STAT(usr);
			CALC_STAT(sys);
			CALC_STAT(nic);
			CALC_STAT(idle);
			CALC_STAT(iowait);
			CALC_STAT(irq);
			CALC_STAT(softirq);
			/*CALC_STAT(steal);*/

			snprintf(scrbuf, scr_width,
				/* Barely fits in 79 chars when in "decimals" mode. */
#if ENABLE_FEATURE_TOP_SMP_CPU
				"CPU%s:"FMT"usr"FMT"sys"FMT"nic"FMT"idle"FMT"io"FMT"irq"FMT"sirq",
				(smp_cpu_info ? utoa(i) : ""),
#else
				"CPU:"FMT"usr"FMT"sys"FMT"nic"FMT"idle"FMT"io"FMT"irq"FMT"sirq",
#endif
				SHOW_STAT(usr), SHOW_STAT(sys), SHOW_STAT(nic), SHOW_STAT(idle),
				SHOW_STAT(iowait), SHOW_STAT(irq), SHOW_STAT(softirq)
				/*, SHOW_STAT(steal) - what is this 'steal' thing? */
				/* I doubt anyone wants to know it */
			);
			puts(scrbuf);
		}
	}
#undef SHOW_STAT
#undef CALC_STAT
#undef FMT
	*lines_rem_p -= i;
}
#else  /* !ENABLE_FEATURE_TOP_CPU_GLOBAL_PERCENTS */
#define display_cpus(scr_width, scrbuf, lines_rem) ((void)0)
#endif

static unsigned long display_header(int scr_width, int *lines_rem_p)
{
	FILE *fp;
	char buf[80];
	char scrbuf[80];
	unsigned long total, used, mfree, shared, buffers, cached;

	/* read memory info */
	fp = xfopen_for_read("meminfo");

	/*
	 * Old kernels (such as 2.4.x) had a nice summary of memory info that
	 * we could parse, however this is gone entirely in 2.6. Try parsing
	 * the old way first, and if that fails, parse each field manually.
	 *
	 * First, we read in the first line. Old kernels will have bogus
	 * strings we don't care about, whereas new kernels will start right
	 * out with MemTotal:
	 *                              -- PFM.
	 */
	if (fscanf(fp, "MemTotal: %lu %s\n", &total, buf) != 2) {
		fgets(buf, sizeof(buf), fp);    /* skip first line */

		fscanf(fp, "Mem: %lu %lu %lu %lu %lu %lu",
			&total, &used, &mfree, &shared, &buffers, &cached);
		/* convert to kilobytes */
		used /= 1024;
		mfree /= 1024;
		shared /= 1024;
		buffers /= 1024;
		cached /= 1024;
		total /= 1024;
	} else {
		/*
		 * Revert to manual parsing, which incidentally already has the
		 * sizes in kilobytes. This should be safe for both 2.4 and
		 * 2.6.
		 */
		fscanf(fp, "MemFree: %lu %s\n", &mfree, buf);

		/*
		 * MemShared: is no longer present in 2.6. Report this as 0,
		 * to maintain consistent behavior with normal procps.
		 */
		if (fscanf(fp, "MemShared: %lu %s\n", &shared, buf) != 2)
			shared = 0;

		fscanf(fp, "Buffers: %lu %s\n", &buffers, buf);
		fscanf(fp, "Cached: %lu %s\n", &cached, buf);

		used = total - mfree;
	}
	fclose(fp);

	/* output memory info */
	if (scr_width > (int)sizeof(scrbuf))
		scr_width = sizeof(scrbuf);
	snprintf(scrbuf, scr_width,
		"Mem: %luK used, %luK free, %luK shrd, %luK buff, %luK cached",
		used, mfree, shared, buffers, cached);
	/* clear screen & go to top */
	printf(OPT_BATCH_MODE ? "%s\n" : "\e[H\e[J%s\n", scrbuf);
	(*lines_rem_p)--;

	/* Display CPU time split as percentage of total time
	 * This displays either a cumulative line or one line per CPU
	 */
	display_cpus(scr_width, scrbuf, lines_rem_p);

	/* read load average as a string */
	buf[0] = '\0';
	open_read_close("loadavg", buf, sizeof(buf) - 1);
	buf[sizeof(buf) - 1] = '\n';
	*strchr(buf, '\n') = '\0';
	snprintf(scrbuf, scr_width, "Load average: %s", buf);
	puts(scrbuf);
	(*lines_rem_p)--;

	return total;
}

static NOINLINE void display_process_list(int lines_rem, int scr_width)
{
	enum {
		BITS_PER_INT = sizeof(int) * 8
	};

	top_status_t *s;
	char vsz_str_buf[8];
	unsigned long total_memory = display_header(scr_width, &lines_rem); /* or use total_vsz? */
	/* xxx_shift and xxx_scale variables allow us to replace
	 * expensive divides with multiply and shift */
	unsigned pmem_shift, pmem_scale, pmem_half;
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
	unsigned pcpu_shift, pcpu_scale, pcpu_half;
	unsigned busy_jifs;
#endif

	/* what info of the processes is shown */
	printf(OPT_BATCH_MODE ? "%.*s" : "\e[7m%.*s\e[0m", scr_width,
		"  PID  PPID USER     STAT   VSZ %MEM"
#if ENABLE_FEATURE_TOP_SMP_PROCESS
		" CPU"
#endif
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
		" %CPU"
#endif
		" COMMAND");
	lines_rem--;

#if ENABLE_FEATURE_TOP_DECIMALS
#define UPSCALE 1000
#define CALC_STAT(name, val) div_t name = div((val), 10)
#define SHOW_STAT(name) name.quot, '0'+name.rem
#define FMT "%3u.%c"
#else
#define UPSCALE 100
#define CALC_STAT(name, val) unsigned name = (val)
#define SHOW_STAT(name) name
#define FMT "%4u%%"
#endif
	/*
	 * MEM% = s->vsz/MemTotal
	 */
	pmem_shift = BITS_PER_INT-11;
	pmem_scale = UPSCALE*(1U<<(BITS_PER_INT-11)) / total_memory;
	/* s->vsz is in kb. we want (s->vsz * pmem_scale) to never overflow */
	while (pmem_scale >= 512) {
		pmem_scale /= 4;
		pmem_shift -= 2;
	}
	pmem_half = (1U << pmem_shift) / (ENABLE_FEATURE_TOP_DECIMALS? 20 : 2);
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
	busy_jifs = cur_jif.busy - prev_jif.busy;
	/* This happens if there were lots of short-lived processes
	 * between two top updates (e.g. compilation) */
	if (total_pcpu < busy_jifs) total_pcpu = busy_jifs;

	/*
	 * CPU% = s->pcpu/sum(s->pcpu) * busy_cpu_ticks/total_cpu_ticks
	 * (pcpu is delta of sys+user time between samples)
	 */
	/* (cur_jif.xxx - prev_jif.xxx) and s->pcpu are
	 * in 0..~64000 range (HZ*update_interval).
	 * we assume that unsigned is at least 32-bit.
	 */
	pcpu_shift = 6;
	pcpu_scale = (UPSCALE*64 * (uint16_t)busy_jifs ? : 1);
	while (pcpu_scale < (1U << (BITS_PER_INT-2))) {
		pcpu_scale *= 4;
		pcpu_shift += 2;
	}
	pcpu_scale /= ( (uint16_t)(cur_jif.total - prev_jif.total) * total_pcpu ? : 1);
	/* we want (s->pcpu * pcpu_scale) to never overflow */
	while (pcpu_scale >= 1024) {
		pcpu_scale /= 4;
		pcpu_shift -= 2;
	}
	pcpu_half = (1U << pcpu_shift) / (ENABLE_FEATURE_TOP_DECIMALS? 20 : 2);
	/* printf(" pmem_scale=%u pcpu_scale=%u ", pmem_scale, pcpu_scale); */
#endif

	/* Ok, all preliminary data is ready, go through the list */
	scr_width += 2; /* account for leading '\n' and trailing NUL */
	if (lines_rem > ntop)
		lines_rem = ntop;
	s = top;
	while (--lines_rem >= 0) {
		unsigned col;
		CALC_STAT(pmem, (s->vsz*pmem_scale + pmem_half) >> pmem_shift);
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
		CALC_STAT(pcpu, (s->pcpu*pcpu_scale + pcpu_half) >> pcpu_shift);
#endif

		if (s->vsz >= 100000)
			sprintf(vsz_str_buf, "%6ldm", s->vsz/1024);
		else
			sprintf(vsz_str_buf, "%7ld", s->vsz);
		/* PID PPID USER STAT VSZ %MEM [%CPU] COMMAND */
		col = snprintf(line_buf, scr_width,
				"\n" "%5u%6u %-8.8s %s%s" FMT
#if ENABLE_FEATURE_TOP_SMP_PROCESS
				" %3d"
#endif
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
				FMT
#endif
				" ",
				s->pid, s->ppid, get_cached_username(s->uid),
				s->state, vsz_str_buf,
				SHOW_STAT(pmem)
#if ENABLE_FEATURE_TOP_SMP_PROCESS
				, s->last_seen_on_cpu
#endif
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
				, SHOW_STAT(pcpu)
#endif
		);
		if ((int)(col + 1) < scr_width)
			read_cmdline(line_buf + col, scr_width - col - 1, s->pid, s->comm);
		fputs(line_buf, stdout);
		/* printf(" %d/%d %lld/%lld", s->pcpu, total_pcpu,
			cur_jif.busy - prev_jif.busy, cur_jif.total - prev_jif.total); */
		s++;
	}
	/* printf(" %d", hist_iterations); */
	bb_putchar(OPT_BATCH_MODE ? '\n' : '\r');
	fflush(stdout);
}
#undef UPSCALE
#undef SHOW_STAT
#undef CALC_STAT
#undef FMT

static void clearmems(void)
{
	clear_username_cache();
	free(top);
	top = NULL;
	ntop = 0;
}

#if ENABLE_FEATURE_USE_TERMIOS

static void reset_term(void)
{
	tcsetattr_stdin_TCSANOW(&initial_settings);
	if (ENABLE_FEATURE_CLEAN_UP) {
		clearmems();
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
		free(prev_hist);
#endif
	}
}

static void sig_catcher(int sig UNUSED_PARAM)
{
	reset_term();
	exit(EXIT_FAILURE);
}
#endif /* FEATURE_USE_TERMIOS */

/*
 * TOPMEM support
 */

typedef unsigned long mem_t;

typedef struct topmem_status_t {
	unsigned pid;
	char comm[COMM_LEN];
	/* vsz doesn't count /dev/xxx mappings except /dev/zero */
	mem_t vsz     ;
	mem_t vszrw   ;
	mem_t rss     ;
	mem_t rss_sh  ;
	mem_t dirty   ;
	mem_t dirty_sh;
	mem_t stack   ;
} topmem_status_t;

enum { NUM_SORT_FIELD = 7 };

#define topmem ((topmem_status_t*)top)

#if ENABLE_FEATURE_TOPMEM

static int topmem_sort(char *a, char *b)
{
	int n;
	mem_t l, r;

	n = offsetof(topmem_status_t, vsz) + (sort_field * sizeof(mem_t));
	l = *(mem_t*)(a + n);
	r = *(mem_t*)(b + n);
//	if (l == r) {
//		l = a->mapped_rw;
//		r = b->mapped_rw;
//	}
	/* We want to avoid unsigned->signed and truncation errors */
	/* l>r: -1, l=r: 0, l<r: 1 */
	n = (l > r) ? -1 : (l != r);
	return inverted ? -n : n;
}

/* Cut "NNNN " out of "    NNNN kb" */
static char *grab_number(char *str, const char *match, unsigned sz)
{
	if (strncmp(str, match, sz) == 0) {
		str = skip_whitespace(str + sz);
		(skip_non_whitespace(str))[1] = '\0';
		return xstrdup(str);
	}
	return NULL;
}

/* display header info (meminfo / loadavg) */
static void display_topmem_header(int scr_width, int *lines_rem_p)
{
	char linebuf[128];
	unsigned i;
	FILE *fp;
	union {
		struct {
			/*  1 */ char *total;
			/*  2 */ char *mfree;
			/*  3 */ char *buf;
			/*  4 */ char *cache;
			/*  5 */ char *swaptotal;
			/*  6 */ char *swapfree;
			/*  7 */ char *dirty;
			/*  8 */ char *mwrite;
			/*  9 */ char *anon;
			/* 10 */ char *map;
			/* 11 */ char *slab;
		} u;
		char *str[11];
	} Z;
#define total     Z.u.total
#define mfree     Z.u.mfree
#define buf       Z.u.buf
#define cache     Z.u.cache
#define swaptotal Z.u.swaptotal
#define swapfree  Z.u.swapfree
#define dirty     Z.u.dirty
#define mwrite    Z.u.mwrite
#define anon      Z.u.anon
#define map       Z.u.map
#define slab      Z.u.slab
#define str       Z.str

	memset(&Z, 0, sizeof(Z));

	/* read memory info */
	fp = xfopen_for_read("meminfo");
	while (fgets(linebuf, sizeof(linebuf), fp)) {
		char *p;

#define SCAN(match, name) \
		p = grab_number(linebuf, match, sizeof(match)-1); \
		if (p) { name = p; continue; }

		SCAN("MemTotal:", total);
		SCAN("MemFree:", mfree);
		SCAN("Buffers:", buf);
		SCAN("Cached:", cache);
		SCAN("SwapTotal:", swaptotal);
		SCAN("SwapFree:", swapfree);
		SCAN("Dirty:", dirty);
		SCAN("Writeback:", mwrite);
		SCAN("AnonPages:", anon);
		SCAN("Mapped:", map);
		SCAN("Slab:", slab);
#undef SCAN
	}
	fclose(fp);

#define S(s) (s ? s : "0 ")
	snprintf(linebuf, sizeof(linebuf),
		"Mem %stotal %sanon %smap %sfree",
		S(total), S(anon), S(map), S(mfree));
	printf(OPT_BATCH_MODE ? "%.*s\n" : "\e[H\e[J%.*s\n", scr_width, linebuf);

	snprintf(linebuf, sizeof(linebuf),
		" %sslab %sbuf %scache %sdirty %swrite",
		S(slab), S(buf), S(cache), S(dirty), S(mwrite));
	printf("%.*s\n", scr_width, linebuf);

	snprintf(linebuf, sizeof(linebuf),
		"Swap %stotal %sfree", // TODO: % used?
		S(swaptotal), S(swapfree));
	printf("%.*s\n", scr_width, linebuf);

	(*lines_rem_p) -= 3;
#undef S

	for (i = 0; i < ARRAY_SIZE(str); i++)
		free(str[i]);
#undef total
#undef free
#undef buf
#undef cache
#undef swaptotal
#undef swapfree
#undef dirty
#undef write
#undef anon
#undef map
#undef slab
#undef str
}

static void ulltoa6_and_space(unsigned long long ul, char buf[6])
{
	/* see http://en.wikipedia.org/wiki/Tera */
	smart_ulltoa5(ul, buf, " mgtpezy");
	buf[5] = ' ';
}

static NOINLINE void display_topmem_process_list(int lines_rem, int scr_width)
{
#define HDR_STR "  PID   VSZ VSZRW   RSS (SHR) DIRTY (SHR) STACK"
#define MIN_WIDTH sizeof(HDR_STR)
	const topmem_status_t *s = topmem;

	display_topmem_header(scr_width, &lines_rem);
	strcpy(line_buf, HDR_STR " COMMAND");
	line_buf[5 + sort_field * 6] = '*';
	printf(OPT_BATCH_MODE ? "%.*s" : "\e[7m%.*s\e[0m", scr_width, line_buf);
	lines_rem--;

	if (lines_rem > ntop)
		lines_rem = ntop;
	while (--lines_rem >= 0) {
		/* PID VSZ VSZRW RSS (SHR) DIRTY (SHR) COMMAND */
		ulltoa6_and_space(s->pid     , &line_buf[0*6]);
		ulltoa6_and_space(s->vsz     , &line_buf[1*6]);
		ulltoa6_and_space(s->vszrw   , &line_buf[2*6]);
		ulltoa6_and_space(s->rss     , &line_buf[3*6]);
		ulltoa6_and_space(s->rss_sh  , &line_buf[4*6]);
		ulltoa6_and_space(s->dirty   , &line_buf[5*6]);
		ulltoa6_and_space(s->dirty_sh, &line_buf[6*6]);
		ulltoa6_and_space(s->stack   , &line_buf[7*6]);
		line_buf[8*6] = '\0';
		if (scr_width > (int)MIN_WIDTH) {
			read_cmdline(&line_buf[8*6], scr_width - MIN_WIDTH, s->pid, s->comm);
		}
		printf("\n""%.*s", scr_width, line_buf);
		s++;
	}
	bb_putchar(OPT_BATCH_MODE ? '\n' : '\r');
	fflush(stdout);
#undef HDR_STR
#undef MIN_WIDTH
}

#else
void display_topmem_process_list(int lines_rem, int scr_width);
int topmem_sort(char *a, char *b);
#endif /* TOPMEM */

/*
 * end TOPMEM support
 */

enum {
	TOP_MASK = 0
		| PSSCAN_PID
		| PSSCAN_PPID
		| PSSCAN_VSZ
		| PSSCAN_STIME
		| PSSCAN_UTIME
		| PSSCAN_STATE
		| PSSCAN_COMM
		| PSSCAN_CPU
		| PSSCAN_UIDGID,
	TOPMEM_MASK = 0
		| PSSCAN_PID
		| PSSCAN_SMAPS
		| PSSCAN_COMM,
};

int top_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int top_main(int argc UNUSED_PARAM, char **argv)
{
	int iterations;
	unsigned lines, col;
	int lines_rem;
	unsigned interval;
	char *str_interval, *str_iterations;
	IF_NOT_FEATURE_TOPMEM(const) unsigned scan_mask = TOP_MASK;
#if ENABLE_FEATURE_USE_TERMIOS
	struct termios new_settings;
	struct pollfd pfd[1];
	unsigned char c;

	pfd[0].fd = 0;
	pfd[0].events = POLLIN;
#endif /* FEATURE_USE_TERMIOS */

	INIT_G();

	interval = 5; /* default update interval is 5 seconds */
	iterations = 0; /* infinite */
#if ENABLE_FEATURE_TOP_SMP_CPU
	/*num_cpus = 0;*/
	/*smp_cpu_info = 0;*/  /* to start with show aggregate */
	cpu_jif = &cur_jif;
	cpu_prev_jif = &prev_jif;
#endif

	/* all args are options; -n NUM */
	opt_complementary = "-";
	col = getopt32(argv, "d:n:b", &str_interval, &str_iterations);
	if (col & OPT_d) {
		/* work around for "-d 1" -> "-d -1" done by getopt32 */
		if (str_interval[0] == '-')
			str_interval++;
		/* Need to limit it to not overflow poll timeout */
		interval = xatou16(str_interval);
	}
	if (col & OPT_n) {
		if (str_iterations[0] == '-')
			str_iterations++;
		iterations = xatou(str_iterations);
	}

	/* change to /proc */
	xchdir("/proc");
#if ENABLE_FEATURE_USE_TERMIOS
	tcgetattr(0, (void *) &initial_settings);
	memcpy(&new_settings, &initial_settings, sizeof(new_settings));
	/* unbuffered input, turn off echo */
	new_settings.c_lflag &= ~(ISIG | ICANON | ECHO | ECHONL);

	bb_signals(BB_FATAL_SIGS, sig_catcher);
	tcsetattr_stdin_TCSANOW(&new_settings);
#endif /* FEATURE_USE_TERMIOS */

#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
	sort_function[0] = pcpu_sort;
	sort_function[1] = mem_sort;
	sort_function[2] = time_sort;
#else
	sort_function[0] = mem_sort;
#endif /* FEATURE_TOP_CPU_USAGE_PERCENTAGE */

	while (1) {
		procps_status_t *p = NULL;

		lines = 24; /* default */
		col = 79;
#if ENABLE_FEATURE_USE_TERMIOS
		/* We output to stdout, we need size of stdout (not stdin)! */
		get_terminal_width_height(STDOUT_FILENO, &col, &lines);
		if (lines < 5 || col < 10) {
			sleep(interval);
			continue;
		}
#endif /* FEATURE_USE_TERMIOS */
		if (col > LINE_BUF_SIZE-2) /* +2 bytes for '\n', NUL, */
			col = LINE_BUF_SIZE-2;

		/* read process IDs & status for all the processes */
		while ((p = procps_scan(p, scan_mask)) != NULL) {
			int n;
			if (scan_mask == TOP_MASK) {
				n = ntop;
				top = xrealloc_vector(top, 6, ntop++);
				top[n].pid = p->pid;
				top[n].ppid = p->ppid;
				top[n].vsz = p->vsz;
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
				top[n].ticks = p->stime + p->utime;
#endif
				top[n].uid = p->uid;
				strcpy(top[n].state, p->state);
				strcpy(top[n].comm, p->comm);
#if ENABLE_FEATURE_TOP_SMP_PROCESS
				top[n].last_seen_on_cpu = p->last_seen_on_cpu;
#endif
			} else { /* TOPMEM */
#if ENABLE_FEATURE_TOPMEM
				if (!(p->mapped_ro | p->mapped_rw))
					continue; /* kernel threads are ignored */
				n = ntop;
				/* No bug here - top and topmem are the same */
				top = xrealloc_vector(topmem, 6, ntop++);
				strcpy(topmem[n].comm, p->comm);
				topmem[n].pid      = p->pid;
				topmem[n].vsz      = p->mapped_rw + p->mapped_ro;
				topmem[n].vszrw    = p->mapped_rw;
				topmem[n].rss_sh   = p->shared_clean + p->shared_dirty;
				topmem[n].rss      = p->private_clean + p->private_dirty + topmem[n].rss_sh;
				topmem[n].dirty    = p->private_dirty + p->shared_dirty;
				topmem[n].dirty_sh = p->shared_dirty;
				topmem[n].stack    = p->stack;
#endif
			}
		} /* end of "while we read /proc" */
		if (ntop == 0) {
			bb_error_msg("no process info in /proc");
			break;
		}

		if (scan_mask == TOP_MASK) {
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
			if (!prev_hist_count) {
				do_stats();
				usleep(100000);
				clearmems();
				continue;
			}
			do_stats();
			/* TODO: we don't need to sort all 10000 processes, we need to find top 24! */
			qsort(top, ntop, sizeof(top_status_t), (void*)mult_lvl_cmp);
#else
			qsort(top, ntop, sizeof(top_status_t), (void*)(sort_function[0]));
#endif /* FEATURE_TOP_CPU_USAGE_PERCENTAGE */
		}
#if ENABLE_FEATURE_TOPMEM
		else { /* TOPMEM */
			qsort(topmem, ntop, sizeof(topmem_status_t), (void*)topmem_sort);
		}
#endif
		lines_rem = lines;
		if (OPT_BATCH_MODE) {
			lines_rem = INT_MAX;
		}
		if (scan_mask == TOP_MASK)
			display_process_list(lines_rem, col);
#if ENABLE_FEATURE_TOPMEM
		else
			display_topmem_process_list(lines_rem, col);
#endif
		clearmems();
		if (iterations >= 0 && !--iterations)
			break;
#if !ENABLE_FEATURE_USE_TERMIOS
		sleep(interval);
#else
		if (option_mask32 & (OPT_b|OPT_EOF))
			 /* batch mode, or EOF on stdin ("top </dev/null") */
			sleep(interval);
		else if (safe_poll(pfd, 1, interval * 1000) > 0) {
			if (safe_read(STDIN_FILENO, &c, 1) != 1) { /* error/EOF? */
				option_mask32 |= OPT_EOF;
				continue;
			}
			if (c == initial_settings.c_cc[VINTR])
				break;
			c |= 0x20; /* lowercase */
			if (c == 'q')
				break;
			if (c == 'n') {
				IF_FEATURE_TOPMEM(scan_mask = TOP_MASK;)
				sort_function[0] = pid_sort;
			}
			if (c == 'm') {
				IF_FEATURE_TOPMEM(scan_mask = TOP_MASK;)
				sort_function[0] = mem_sort;
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
				sort_function[1] = pcpu_sort;
				sort_function[2] = time_sort;
#endif
			}
#if ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE
			if (c == 'p') {
				IF_FEATURE_TOPMEM(scan_mask = TOP_MASK;)
				sort_function[0] = pcpu_sort;
				sort_function[1] = mem_sort;
				sort_function[2] = time_sort;
			}
			if (c == 't') {
				IF_FEATURE_TOPMEM(scan_mask = TOP_MASK;)
				sort_function[0] = time_sort;
				sort_function[1] = mem_sort;
				sort_function[2] = pcpu_sort;
			}
#if ENABLE_FEATURE_TOPMEM
			if (c == 's') {
				scan_mask = TOPMEM_MASK;
				free(prev_hist);
				prev_hist = NULL;
				prev_hist_count = 0;
				sort_field = (sort_field + 1) % NUM_SORT_FIELD;
			}
			if (c == 'r')
				inverted ^= 1;
#endif
#if ENABLE_FEATURE_TOP_SMP_CPU
			/* procps-2.0.18 uses 'C', 3.2.7 uses '1' */
			if (c == 'c' || c == '1') {
				/* User wants to toggle per cpu <> aggregate */
				if (smp_cpu_info) {
					free(cpu_prev_jif);
					free(cpu_jif);
					cpu_jif = &cur_jif;
					cpu_prev_jif = &prev_jif;
				} else {
					/* Prepare for xrealloc() */
					cpu_jif = cpu_prev_jif = NULL;
				}
				num_cpus = 0;
				smp_cpu_info = !smp_cpu_info;
				get_jiffy_counts();
			}
#endif
#endif
		}
#endif /* FEATURE_USE_TERMIOS */
	} /* end of "while (1)" */

	bb_putchar('\n');
#if ENABLE_FEATURE_USE_TERMIOS
	reset_term();
#endif
	return EXIT_SUCCESS;
}
