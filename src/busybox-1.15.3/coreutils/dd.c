/* vi: set sw=4 ts=4: */
/*
 * Mini dd implementation for busybox
 *
 *
 * Copyright (C) 2000,2001  Matt Kraai
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"

/* This is a NOEXEC applet. Be very careful! */


enum {
	ifd = STDIN_FILENO,
	ofd = STDOUT_FILENO,
};

static const struct suffix_mult dd_suffixes[] = {
	{ "c", 1 },
	{ "w", 2 },
	{ "b", 512 },
	{ "kD", 1000 },
	{ "k", 1024 },
	{ "K", 1024 },	/* compat with coreutils dd */
	{ "MD", 1000000 },
	{ "M", 1048576 },
	{ "GD", 1000000000 },
	{ "G", 1073741824 },
	{ }
};

struct globals {
	off_t out_full, out_part, in_full, in_part;
};
#define G (*(struct globals*)&bb_common_bufsiz1)
/* We have to zero it out because of NOEXEC */
#define INIT_G() memset(&G, 0, sizeof(G))


static void dd_output_status(int UNUSED_PARAM cur_signal)
{
	/* Deliberately using %u, not %d */
	fprintf(stderr, "%"OFF_FMT"u+%"OFF_FMT"u records in\n"
			"%"OFF_FMT"u+%"OFF_FMT"u records out\n",
			G.in_full, G.in_part,
			G.out_full, G.out_part);
}

static ssize_t full_write_or_warn(const void *buf, size_t len,
	const char *const filename)
{
	ssize_t n = full_write(ofd, buf, len);
	if (n < 0)
		bb_perror_msg("writing '%s'", filename);
	return n;
}

static bool write_and_stats(const void *buf, size_t len, size_t obs,
	const char *filename)
{
	ssize_t n = full_write_or_warn(buf, len, filename);
	if (n < 0)
		return 1;
	if ((size_t)n == obs)
		G.out_full++;
	else if (n) /* > 0 */
		G.out_part++;
	return 0;
}

#if ENABLE_LFS
#define XATOU_SFX xatoull_sfx
#else
#define XATOU_SFX xatoul_sfx
#endif

int dd_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int dd_main(int argc UNUSED_PARAM, char **argv)
{
	enum {
		/* Must be in the same order as OP_conv_XXX! */
		/* (see "flags |= (1 << what)" below) */
		FLAG_NOTRUNC = 1 << 0,
		FLAG_SYNC    = 1 << 1,
		FLAG_NOERROR = 1 << 2,
		FLAG_FSYNC   = 1 << 3,
		/* end of conv flags */
		FLAG_TWOBUFS = 1 << 4,
		FLAG_COUNT   = 1 << 5,
	};
	static const char keywords[] ALIGN1 =
		"bs\0""count\0""seek\0""skip\0""if\0""of\0"
#if ENABLE_FEATURE_DD_IBS_OBS
		"ibs\0""obs\0""conv\0"
#endif
		;
#if ENABLE_FEATURE_DD_IBS_OBS
	static const char conv_words[] ALIGN1 =
		"notrunc\0""sync\0""noerror\0""fsync\0";
#endif
	enum {
		OP_bs = 0,
		OP_count,
		OP_seek,
		OP_skip,
		OP_if,
		OP_of,
#if ENABLE_FEATURE_DD_IBS_OBS
		OP_ibs,
		OP_obs,
		OP_conv,
		/* Must be in the same order as FLAG_XXX! */
		OP_conv_notrunc = 0,
		OP_conv_sync,
		OP_conv_noerror,
		OP_conv_fsync,
	/* Unimplemented conv=XXX: */
	//nocreat       do not create the output file
	//excl          fail if the output file already exists
	//fdatasync     physically write output file data before finishing
	//swab          swap every pair of input bytes
	//lcase         change upper case to lower case
	//ucase         change lower case to upper case
	//block         pad newline-terminated records with spaces to cbs-size
	//unblock       replace trailing spaces in cbs-size records with newline
	//ascii         from EBCDIC to ASCII
	//ebcdic        from ASCII to EBCDIC
	//ibm           from ASCII to alternate EBCDIC
#endif
	};
	int exitcode = EXIT_FAILURE;
	size_t ibs = 512, obs = 512;
	ssize_t n, w;
	char *ibuf, *obuf;
	/* And these are all zeroed at once! */
	struct {
		int flags;
		size_t oc;
		off_t count;
		off_t seek, skip;
		const char *infile, *outfile;
	} Z;
#define flags   (Z.flags  )
#define oc      (Z.oc     )
#define count   (Z.count  )
#define seek    (Z.seek   )
#define skip    (Z.skip   )
#define infile  (Z.infile )
#define outfile (Z.outfile)

	memset(&Z, 0, sizeof(Z));
	INIT_G();
	//fflush(NULL); - is this needed because of NOEXEC?

#if ENABLE_FEATURE_DD_SIGNAL_HANDLING
	signal_SA_RESTART_empty_mask(SIGUSR1, dd_output_status);
#endif

	for (n = 1; argv[n]; n++) {
		int what;
		char *val;
		char *arg = argv[n];

#if ENABLE_DESKTOP
		/* "dd --". NB: coreutils 6.9 will complain if they see
		 * more than one of them. We wouldn't. */
		if (arg[0] == '-' && arg[1] == '-' && arg[2] == '\0')
			continue;
#endif
		val = strchr(arg, '=');
		if (val == NULL)
			bb_show_usage();
		*val = '\0';
		what = index_in_strings(keywords, arg);
		if (what < 0)
			bb_show_usage();
		/* *val = '='; - to preserve ps listing? */
		val++;
#if ENABLE_FEATURE_DD_IBS_OBS
		if (what == OP_ibs) {
			/* Must fit into positive ssize_t */
			ibs = xatoul_range_sfx(val, 1, ((size_t)-1L)/2, dd_suffixes);
			/*continue;*/
		}
		if (what == OP_obs) {
			obs = xatoul_range_sfx(val, 1, ((size_t)-1L)/2, dd_suffixes);
			/*continue;*/
		}
		if (what == OP_conv) {
			while (1) {
				/* find ',', replace them with NUL so we can use val for
				 * index_in_strings() without copying.
				 * We rely on val being non-null, else strchr would fault.
				 */
				arg = strchr(val, ',');
				if (arg)
					*arg = '\0';
				what = index_in_strings(conv_words, val);
				if (what < 0)
					bb_error_msg_and_die(bb_msg_invalid_arg, val, "conv");
				flags |= (1 << what);
				if (!arg) /* no ',' left, so this was the last specifier */
					break;
				/* *arg = ','; - to preserve ps listing? */
				val = arg + 1; /* skip this keyword and ',' */
			}
			continue; /* we trashed 'what', can't fall through */
		}
#endif
		if (what == OP_bs) {
			ibs = obs = xatoul_range_sfx(val, 1, ((size_t)-1L)/2, dd_suffixes);
			/*continue;*/
		}
		/* These can be large: */
		if (what == OP_count) {
			flags |= FLAG_COUNT;
			count = XATOU_SFX(val, dd_suffixes);
			/*continue;*/
		}
		if (what == OP_seek) {
			seek = XATOU_SFX(val, dd_suffixes);
			/*continue;*/
		}
		if (what == OP_skip) {
			skip = XATOU_SFX(val, dd_suffixes);
			/*continue;*/
		}
		if (what == OP_if) {
			infile = val;
			/*continue;*/
		}
		if (what == OP_of) {
			outfile = val;
			/*continue;*/
		}
	} /* end of "for (argv[n])" */

//XXX:FIXME for huge ibs or obs, malloc'ing them isn't the brightest idea ever
	ibuf = obuf = xmalloc(ibs);
	if (ibs != obs) {
		flags |= FLAG_TWOBUFS;
		obuf = xmalloc(obs);
	}
	if (infile != NULL)
		xmove_fd(xopen(infile, O_RDONLY), ifd);
	else {
		infile = bb_msg_standard_input;
	}
	if (outfile != NULL) {
		int oflag = O_WRONLY | O_CREAT;

		if (!seek && !(flags & FLAG_NOTRUNC))
			oflag |= O_TRUNC;

		xmove_fd(xopen(outfile, oflag), ofd);

		if (seek && !(flags & FLAG_NOTRUNC)) {
			if (ftruncate(ofd, seek * obs) < 0) {
				struct stat st;

				if (fstat(ofd, &st) < 0 || S_ISREG(st.st_mode) ||
						S_ISDIR(st.st_mode))
					goto die_outfile;
			}
		}
	} else {
		outfile = bb_msg_standard_output;
	}
	if (skip) {
		if (lseek(ifd, skip * ibs, SEEK_CUR) < 0) {
			while (skip-- > 0) {
				n = safe_read(ifd, ibuf, ibs);
				if (n < 0)
					goto die_infile;
				if (n == 0)
					break;
			}
		}
	}
	if (seek) {
		if (lseek(ofd, seek * obs, SEEK_CUR) < 0)
			goto die_outfile;
	}

	while (!(flags & FLAG_COUNT) || (G.in_full + G.in_part != count)) {
		n = safe_read(ifd, ibuf, ibs);
		if (n == 0)
			break;
		if (n < 0) {
			/* "Bad block" */
			if (!(flags & FLAG_NOERROR))
				goto die_infile;
			bb_simple_perror_msg(infile);
			/* GNU dd with conv=noerror skips over bad blocks */
			xlseek(ifd, ibs, SEEK_CUR);
			/* conv=noerror,sync writes NULs,
			 * conv=noerror just ignores input bad blocks */
			n = 0;
		}
		if ((size_t)n == ibs)
			G.in_full++;
		else {
			G.in_part++;
			if (flags & FLAG_SYNC) {
				memset(ibuf + n, 0, ibs - n);
				n = ibs;
			}
		}
		if (flags & FLAG_TWOBUFS) {
			char *tmp = ibuf;
			while (n) {
				size_t d = obs - oc;

				if (d > (size_t)n)
					d = n;
				memcpy(obuf + oc, tmp, d);
				n -= d;
				tmp += d;
				oc += d;
				if (oc == obs) {
					if (write_and_stats(obuf, obs, obs, outfile))
						goto out_status;
					oc = 0;
				}
			}
		} else if (write_and_stats(ibuf, n, obs, outfile))
			goto out_status;

		if (flags & FLAG_FSYNC) {
			if (fsync(ofd) < 0)
				goto die_outfile;
		}
	}

	if (ENABLE_FEATURE_DD_IBS_OBS && oc) {
		w = full_write_or_warn(obuf, oc, outfile);
		if (w < 0) goto out_status;
		if (w > 0) G.out_part++;
	}
	if (close(ifd) < 0) {
 die_infile:
		bb_simple_perror_msg_and_die(infile);
	}

	if (close(ofd) < 0) {
 die_outfile:
		bb_simple_perror_msg_and_die(outfile);
	}

	exitcode = EXIT_SUCCESS;
 out_status:
	dd_output_status(0);

	return exitcode;
}
