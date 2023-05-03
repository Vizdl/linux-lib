/* vi: set sw=4 ts=4: */
/*
 * Mini grep implementation for busybox using libc regex.
 *
 * Copyright (C) 1999,2000,2001 by Lineo, inc. and Mark Whitley
 * Copyright (C) 1999,2000,2001 by Mark Whitley <markw@codepoet.org>
 *
 * Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
 */
/* BB_AUDIT SUSv3 defects - unsupported option -x "match whole line only". */
/* BB_AUDIT GNU defects - always acts as -a.  */
/* http://www.opengroup.org/onlinepubs/007904975/utilities/grep.html */
/*
 * 2004,2006 (C) Vladimir Oleynik <dzo@simtreas.ru> -
 * correction "-e pattern1 -e pattern2" logic and more optimizations.
 * precompiled regex
 */
/*
 * (C) 2006 Jac Goudsmit added -o option
 */

#include "libbb.h"
#include "xregex.h"

/* options */
#define OPTSTR_GREP \
	"lnqvscFiHhe:f:Lorm:" \
	IF_FEATURE_GREP_CONTEXT("A:B:C:") \
	IF_FEATURE_GREP_EGREP_ALIAS("E") \
	IF_DESKTOP("w") \
	IF_EXTRA_COMPAT("z") \
	"aI"

/* ignored: -a "assume all files to be text" */
/* ignored: -I "assume binary files have no matches" */

enum {
	OPTBIT_l, /* list matched file names only */
	OPTBIT_n, /* print line# */
	OPTBIT_q, /* quiet - exit(EXIT_SUCCESS) of first match */
	OPTBIT_v, /* invert the match, to select non-matching lines */
	OPTBIT_s, /* suppress errors about file open errors */
	OPTBIT_c, /* count matches per file (suppresses normal output) */
	OPTBIT_F, /* literal match */
	OPTBIT_i, /* case-insensitive */
	OPTBIT_H, /* force filename display */
	OPTBIT_h, /* inhibit filename display */
	OPTBIT_e, /* -e PATTERN */
	OPTBIT_f, /* -f FILE_WITH_PATTERNS */
	OPTBIT_L, /* list unmatched file names only */
	OPTBIT_o, /* show only matching parts of lines */
	OPTBIT_r, /* recurse dirs */
	OPTBIT_m, /* -m MAX_MATCHES */
	IF_FEATURE_GREP_CONTEXT(    OPTBIT_A ,) /* -A NUM: after-match context */
	IF_FEATURE_GREP_CONTEXT(    OPTBIT_B ,) /* -B NUM: before-match context */
	IF_FEATURE_GREP_CONTEXT(    OPTBIT_C ,) /* -C NUM: -A and -B combined */
	IF_FEATURE_GREP_EGREP_ALIAS(OPTBIT_E ,) /* extended regexp */
	IF_DESKTOP(                 OPTBIT_w ,) /* whole word match */
	IF_EXTRA_COMPAT(            OPTBIT_z ,) /* input is NUL terminated */
	OPT_l = 1 << OPTBIT_l,
	OPT_n = 1 << OPTBIT_n,
	OPT_q = 1 << OPTBIT_q,
	OPT_v = 1 << OPTBIT_v,
	OPT_s = 1 << OPTBIT_s,
	OPT_c = 1 << OPTBIT_c,
	OPT_F = 1 << OPTBIT_F,
	OPT_i = 1 << OPTBIT_i,
	OPT_H = 1 << OPTBIT_H,
	OPT_h = 1 << OPTBIT_h,
	OPT_e = 1 << OPTBIT_e,
	OPT_f = 1 << OPTBIT_f,
	OPT_L = 1 << OPTBIT_L,
	OPT_o = 1 << OPTBIT_o,
	OPT_r = 1 << OPTBIT_r,
	OPT_m = 1 << OPTBIT_m,
	OPT_A = IF_FEATURE_GREP_CONTEXT(    (1 << OPTBIT_A)) + 0,
	OPT_B = IF_FEATURE_GREP_CONTEXT(    (1 << OPTBIT_B)) + 0,
	OPT_C = IF_FEATURE_GREP_CONTEXT(    (1 << OPTBIT_C)) + 0,
	OPT_E = IF_FEATURE_GREP_EGREP_ALIAS((1 << OPTBIT_E)) + 0,
	OPT_w = IF_DESKTOP(                 (1 << OPTBIT_w)) + 0,
	OPT_z = IF_EXTRA_COMPAT(            (1 << OPTBIT_z)) + 0,
};

#define PRINT_FILES_WITH_MATCHES    (option_mask32 & OPT_l)
#define PRINT_LINE_NUM              (option_mask32 & OPT_n)
#define BE_QUIET                    (option_mask32 & OPT_q)
#define SUPPRESS_ERR_MSGS           (option_mask32 & OPT_s)
#define PRINT_MATCH_COUNTS          (option_mask32 & OPT_c)
#define FGREP_FLAG                  (option_mask32 & OPT_F)
#define PRINT_FILES_WITHOUT_MATCHES (option_mask32 & OPT_L)
#define NUL_DELIMITED               (option_mask32 & OPT_z)

struct globals {
	int max_matches;
#if !ENABLE_EXTRA_COMPAT
	int reflags;
#else
	RE_TRANSLATE_TYPE case_fold; /* RE_TRANSLATE_TYPE is [[un]signed] char* */
#endif
	smalluint invert_search;
	smalluint print_filename;
	smalluint open_errors;
#if ENABLE_FEATURE_GREP_CONTEXT
	smalluint did_print_line;
	int lines_before;
	int lines_after;
	char **before_buf;
	IF_EXTRA_COMPAT(size_t *before_buf_size;)
	int last_line_printed;
#endif
	/* globals used internally */
	llist_t *pattern_head;   /* growable list of patterns to match */
	const char *cur_file;    /* the current file we are reading */
};
#define G (*(struct globals*)&bb_common_bufsiz1)
#define INIT_G() do { \
	struct G_sizecheck { \
		char G_sizecheck[sizeof(G) > COMMON_BUFSIZE ? -1 : 1]; \
	}; \
} while (0)
#define max_matches       (G.max_matches         )
#if !ENABLE_EXTRA_COMPAT
# define reflags          (G.reflags             )
#else
# define case_fold        (G.case_fold           )
/* http://www.delorie.com/gnu/docs/regex/regex_46.html */
# define reflags           re_syntax_options
# undef REG_NOSUB
# undef REG_EXTENDED
# undef REG_ICASE
# define REG_NOSUB    bug:is:here /* should not be used */
/* Just RE_SYNTAX_EGREP is not enough, need to enable {n[,[m]]} too */
# define REG_EXTENDED (RE_SYNTAX_EGREP | RE_INTERVALS | RE_NO_BK_BRACES)
# define REG_ICASE    bug:is:here /* should not be used */
#endif
#define invert_search     (G.invert_search       )
#define print_filename    (G.print_filename      )
#define open_errors       (G.open_errors         )
#define did_print_line    (G.did_print_line      )
#define lines_before      (G.lines_before        )
#define lines_after       (G.lines_after         )
#define before_buf        (G.before_buf          )
#define before_buf_size   (G.before_buf_size     )
#define last_line_printed (G.last_line_printed   )
#define pattern_head      (G.pattern_head        )
#define cur_file          (G.cur_file            )


typedef struct grep_list_data_t {
	char *pattern;
/* for GNU regex, matched_range must be persistent across grep_file() calls */
#if !ENABLE_EXTRA_COMPAT
	regex_t compiled_regex;
	regmatch_t matched_range;
#else
	struct re_pattern_buffer compiled_regex;
	struct re_registers matched_range;
#endif
#define ALLOCATED 1
#define COMPILED 2
	int flg_mem_alocated_compiled;
} grep_list_data_t;

#if !ENABLE_EXTRA_COMPAT
#define print_line(line, line_len, linenum, decoration) \
	print_line(line, linenum, decoration)
#endif
static void print_line(const char *line, size_t line_len, int linenum, char decoration)
{
#if ENABLE_FEATURE_GREP_CONTEXT
	/* Happens when we go to next file, immediately hit match
	 * and try to print prev context... from prev file! Don't do it */
	if (linenum < 1)
		return;
	/* possibly print the little '--' separator */
	if ((lines_before || lines_after) && did_print_line
	 && last_line_printed != linenum - 1
	) {
		puts("--");
	}
	/* guard against printing "--" before first line of first file */
	did_print_line = 1;
	last_line_printed = linenum;
#endif
	if (print_filename)
		printf("%s%c", cur_file, decoration);
	if (PRINT_LINE_NUM)
		printf("%i%c", linenum, decoration);
	/* Emulate weird GNU grep behavior with -ov */
	if ((option_mask32 & (OPT_v|OPT_o)) != (OPT_v|OPT_o)) {
#if !ENABLE_EXTRA_COMPAT
		puts(line);
#else
		fwrite(line, 1, line_len, stdout);
		putchar(NUL_DELIMITED ? '\0' : '\n');
#endif
	}
}

#if ENABLE_EXTRA_COMPAT
/* Unlike getline, this one removes trailing '\n' */
static ssize_t FAST_FUNC bb_getline(char **line_ptr, size_t *line_alloc_len, FILE *file)
{
	ssize_t res_sz;
	char *line;
	int delim = (NUL_DELIMITED ? '\0' : '\n');

	res_sz = getdelim(line_ptr, line_alloc_len, delim, file);
	line = *line_ptr;

	if (res_sz > 0) {
		if (line[res_sz - 1] == delim)
			line[--res_sz] = '\0';
	} else {
		free(line); /* uclibc allocates a buffer even on EOF. WTF? */
	}
	return res_sz;
}
#endif

static int grep_file(FILE *file)
{
	smalluint found;
	int linenum = 0;
	int nmatches = 0;
#if !ENABLE_EXTRA_COMPAT
	char *line;
#else
	char *line = NULL;
	ssize_t line_len;
	size_t line_alloc_len;
#define rm_so start[0]
#define rm_eo end[0]
#endif
#if ENABLE_FEATURE_GREP_CONTEXT
	int print_n_lines_after = 0;
	int curpos = 0; /* track where we are in the circular 'before' buffer */
	int idx = 0; /* used for iteration through the circular buffer */
#else
	enum { print_n_lines_after = 0 };
#endif /* ENABLE_FEATURE_GREP_CONTEXT */

	while (
#if !ENABLE_EXTRA_COMPAT
		(line = xmalloc_fgetline(file)) != NULL
#else
		(line_len = bb_getline(&line, &line_alloc_len, file)) >= 0
#endif
	) {
		llist_t *pattern_ptr = pattern_head;
		grep_list_data_t *gl = gl; /* for gcc */

		linenum++;
		found = 0;
		while (pattern_ptr) {
			gl = (grep_list_data_t *)pattern_ptr->data;
			if (FGREP_FLAG) {
				found |= (strstr(line, gl->pattern) != NULL);
			} else {
				if (!(gl->flg_mem_alocated_compiled & COMPILED)) {
					gl->flg_mem_alocated_compiled |= COMPILED;
#if !ENABLE_EXTRA_COMPAT
					xregcomp(&gl->compiled_regex, gl->pattern, reflags);
#else
					memset(&gl->compiled_regex, 0, sizeof(gl->compiled_regex));
					gl->compiled_regex.translate = case_fold; /* for -i */
					if (re_compile_pattern(gl->pattern, strlen(gl->pattern), &gl->compiled_regex))
						bb_error_msg_and_die("bad regex '%s'", gl->pattern);
#endif
				}
#if !ENABLE_EXTRA_COMPAT
				gl->matched_range.rm_so = 0;
				gl->matched_range.rm_eo = 0;
#endif
				if (
#if !ENABLE_EXTRA_COMPAT
					regexec(&gl->compiled_regex, line, 1, &gl->matched_range, 0) == 0
#else
					re_search(&gl->compiled_regex, line, line_len,
							/*start:*/ 0, /*range:*/ line_len,
							&gl->matched_range) >= 0
#endif
				) {
					if (!(option_mask32 & OPT_w))
						found = 1;
					else {
						char c = ' ';
						if (gl->matched_range.rm_so)
							c = line[gl->matched_range.rm_so - 1];
						if (!isalnum(c) && c != '_') {
							c = line[gl->matched_range.rm_eo];
							if (!c || (!isalnum(c) && c != '_'))
								found = 1;
						}
					}
				}
			}
			/* If it's non-inverted search, we can stop
			 * at first match */
			if (found && !invert_search)
				goto do_found;
			pattern_ptr = pattern_ptr->link;
		} /* while (pattern_ptr) */

		if (found ^ invert_search) {
 do_found:
			/* keep track of matches */
			nmatches++;

			/* quiet/print (non)matching file names only? */
			if (option_mask32 & (OPT_q|OPT_l|OPT_L)) {
				free(line); /* we don't need line anymore */
				if (BE_QUIET) {
					/* manpage says about -q:
					 * "exit immediately with zero status
					 * if any match is found,
					 * even if errors were detected" */
					exit(EXIT_SUCCESS);
				}
				/* if we're just printing filenames, we stop after the first match */
				if (PRINT_FILES_WITH_MATCHES) {
					puts(cur_file);
					/* fall through to "return 1" */
				}
				/* OPT_L aka PRINT_FILES_WITHOUT_MATCHES: return early */
				return 1; /* one match */
			}

#if ENABLE_FEATURE_GREP_CONTEXT
			/* Were we printing context and saw next (unwanted) match? */
			if ((option_mask32 & OPT_m) && nmatches > max_matches)
				break;
#endif

			/* print the matched line */
			if (PRINT_MATCH_COUNTS == 0) {
#if ENABLE_FEATURE_GREP_CONTEXT
				int prevpos = (curpos == 0) ? lines_before - 1 : curpos - 1;

				/* if we were told to print 'before' lines and there is at least
				 * one line in the circular buffer, print them */
				if (lines_before && before_buf[prevpos] != NULL) {
					int first_buf_entry_line_num = linenum - lines_before;

					/* advance to the first entry in the circular buffer, and
					 * figure out the line number is of the first line in the
					 * buffer */
					idx = curpos;
					while (before_buf[idx] == NULL) {
						idx = (idx + 1) % lines_before;
						first_buf_entry_line_num++;
					}

					/* now print each line in the buffer, clearing them as we go */
					while (before_buf[idx] != NULL) {
						print_line(before_buf[idx], before_buf_size[idx], first_buf_entry_line_num, '-');
						free(before_buf[idx]);
						before_buf[idx] = NULL;
						idx = (idx + 1) % lines_before;
						first_buf_entry_line_num++;
					}
				}

				/* make a note that we need to print 'after' lines */
				print_n_lines_after = lines_after;
#endif
				if (option_mask32 & OPT_o) {
					if (FGREP_FLAG) {
						/* -Fo just prints the pattern
						 * (unless -v: -Fov doesnt print anything at all) */
						if (found)
							print_line(gl->pattern, strlen(gl->pattern), linenum, ':');
					} else while (1) {
						unsigned end = gl->matched_range.rm_eo;
						char old = line[end];
						line[end] = '\0';
						print_line(line + gl->matched_range.rm_so,
								end - gl->matched_range.rm_so,
								linenum, ':');
						if (old == '\0')
							break;
						line[end] = old;
#if !ENABLE_EXTRA_COMPAT
						if (regexec(&gl->compiled_regex, line + end,
								1, &gl->matched_range, REG_NOTBOL) != 0)
							break;
						gl->matched_range.rm_so += end;
						gl->matched_range.rm_eo += end;
#else
						if (re_search(&gl->compiled_regex, line, line_len,
								end, line_len - end,
								&gl->matched_range) < 0)
							break;
#endif
					}
				} else {
					print_line(line, line_len, linenum, ':');
				}
			}
		}
#if ENABLE_FEATURE_GREP_CONTEXT
		else { /* no match */
			/* if we need to print some context lines after the last match, do so */
			if (print_n_lines_after) {
				print_line(line, strlen(line), linenum, '-');
				print_n_lines_after--;
			} else if (lines_before) {
				/* Add the line to the circular 'before' buffer */
				free(before_buf[curpos]);
				before_buf[curpos] = line;
				IF_EXTRA_COMPAT(before_buf_size[curpos] = line_len;)
				curpos = (curpos + 1) % lines_before;
				/* avoid free(line) - we took the line */
				line = NULL;
			}
		}

#endif /* ENABLE_FEATURE_GREP_CONTEXT */
#if !ENABLE_EXTRA_COMPAT
		free(line);
#endif
		/* Did we print all context after last requested match? */
		if ((option_mask32 & OPT_m)
		 && !print_n_lines_after
		 && nmatches == max_matches
		) {
			break;
		}
	} /* while (read line) */

	/* special-case file post-processing for options where we don't print line
	 * matches, just filenames and possibly match counts */

	/* grep -c: print [filename:]count, even if count is zero */
	if (PRINT_MATCH_COUNTS) {
		if (print_filename)
			printf("%s:", cur_file);
		printf("%d\n", nmatches);
	}

	/* grep -L: print just the filename */
	if (PRINT_FILES_WITHOUT_MATCHES) {
		/* nmatches is zero, no need to check it:
		 * we return 1 early if we detected a match
		 * and PRINT_FILES_WITHOUT_MATCHES is set */
		puts(cur_file);
	}

	return nmatches;
}

#if ENABLE_FEATURE_CLEAN_UP
#define new_grep_list_data(p, m) add_grep_list_data(p, m)
static char *add_grep_list_data(char *pattern, int flg_used_mem)
#else
#define new_grep_list_data(p, m) add_grep_list_data(p)
static char *add_grep_list_data(char *pattern)
#endif
{
	grep_list_data_t *gl = xzalloc(sizeof(*gl));
	gl->pattern = pattern;
#if ENABLE_FEATURE_CLEAN_UP
	gl->flg_mem_alocated_compiled = flg_used_mem;
#else
	/*gl->flg_mem_alocated_compiled = 0;*/
#endif
	return (char *)gl;
}

static void load_regexes_from_file(llist_t *fopt)
{
	char *line;
	FILE *f;

	while (fopt) {
		llist_t *cur = fopt;
		char *ffile = cur->data;

		fopt = cur->link;
		free(cur);
		f = xfopen_stdin(ffile);
		while ((line = xmalloc_fgetline(f)) != NULL) {
			llist_add_to(&pattern_head,
				new_grep_list_data(line, ALLOCATED));
		}
	}
}

static int FAST_FUNC file_action_grep(const char *filename,
			struct stat *statbuf UNUSED_PARAM,
			void* matched,
			int depth UNUSED_PARAM)
{
	FILE *file = fopen_for_read(filename);
	if (file == NULL) {
		if (!SUPPRESS_ERR_MSGS)
			bb_simple_perror_msg(filename);
		open_errors = 1;
		return 0;
	}
	cur_file = filename;
	*(int*)matched += grep_file(file);
	fclose(file);
	return 1;
}

static int grep_dir(const char *dir)
{
	int matched = 0;
	recursive_action(dir,
		/* recurse=yes */ ACTION_RECURSE |
		/* followLinks=no */
		/* depthFirst=yes */ ACTION_DEPTHFIRST,
		/* fileAction= */ file_action_grep,
		/* dirAction= */ NULL,
		/* userData= */ &matched,
		/* depth= */ 0);
	return matched;
}

int grep_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int grep_main(int argc, char **argv)
{
	FILE *file;
	int matched;
	llist_t *fopt = NULL;

	/* do normal option parsing */
#if ENABLE_FEATURE_GREP_CONTEXT
	int Copt;

	/* -H unsets -h; -C unsets -A,-B; -e,-f are lists;
	 * -m,-A,-B,-C have numeric param */
	opt_complementary = "H-h:C-AB:e::f::m+:A+:B+:C+";
	getopt32(argv,
		OPTSTR_GREP,
		&pattern_head, &fopt, &max_matches,
		&lines_after, &lines_before, &Copt);

	if (option_mask32 & OPT_C) {
		/* -C unsets prev -A and -B, but following -A or -B
		   may override it */
		if (!(option_mask32 & OPT_A)) /* not overridden */
			lines_after = Copt;
		if (!(option_mask32 & OPT_B)) /* not overridden */
			lines_before = Copt;
	}
	/* sanity checks */
	if (option_mask32 & (OPT_c|OPT_q|OPT_l|OPT_L)) {
		option_mask32 &= ~OPT_n;
		lines_before = 0;
		lines_after = 0;
	} else if (lines_before > 0) {
		before_buf = xzalloc(lines_before * sizeof(before_buf[0]));
		IF_EXTRA_COMPAT(before_buf_size = xzalloc(lines_before * sizeof(before_buf_size[0]));)
	}
#else
	/* with auto sanity checks */
	/* -H unsets -h; -c,-q or -l unset -n; -e,-f are lists; -m N */
	opt_complementary = "H-h:c-n:q-n:l-n:e::f::m+";
	getopt32(argv, OPTSTR_GREP,
		&pattern_head, &fopt, &max_matches);
#endif
	invert_search = ((option_mask32 & OPT_v) != 0); /* 0 | 1 */

	if (pattern_head != NULL) {
		/* convert char **argv to grep_list_data_t */
		llist_t *cur;

		for (cur = pattern_head; cur; cur = cur->link)
			cur->data = new_grep_list_data(cur->data, 0);
	}
	if (option_mask32 & OPT_f)
		load_regexes_from_file(fopt);

	if (ENABLE_FEATURE_GREP_FGREP_ALIAS && applet_name[0] == 'f')
		option_mask32 |= OPT_F;

#if !ENABLE_EXTRA_COMPAT
	if (!(option_mask32 & (OPT_o | OPT_w)))
		reflags = REG_NOSUB;
#endif

	if (ENABLE_FEATURE_GREP_EGREP_ALIAS
	 && (applet_name[0] == 'e' || (option_mask32 & OPT_E))
	) {
		reflags |= REG_EXTENDED;
	}
#if ENABLE_EXTRA_COMPAT
	else {
		reflags = RE_SYNTAX_GREP;
	}
#endif

	if (option_mask32 & OPT_i) {
#if !ENABLE_EXTRA_COMPAT
		reflags |= REG_ICASE;
#else
		int i;
		case_fold = xmalloc(256);
		for (i = 0; i < 256; i++)
			case_fold[i] = (unsigned char)i;
		for (i = 'a'; i <= 'z'; i++)
			case_fold[i] = (unsigned char)(i - ('a' - 'A'));
#endif
	}

	argv += optind;
	argc -= optind;

	/* if we didn't get a pattern from -e and no command file was specified,
	 * first parameter should be the pattern. no pattern, no worky */
	if (pattern_head == NULL) {
		char *pattern;
		if (*argv == NULL)
			bb_show_usage();
		pattern = new_grep_list_data(*argv++, 0);
		llist_add_to(&pattern_head, pattern);
		argc--;
	}

	/* argv[0..(argc-1)] should be names of file to grep through. If
	 * there is more than one file to grep, we will print the filenames. */
	if (argc > 1)
		print_filename = 1;
	/* -H / -h of course override */
	if (option_mask32 & OPT_H)
		print_filename = 1;
	if (option_mask32 & OPT_h)
		print_filename = 0;

	/* If no files were specified, or '-' was specified, take input from
	 * stdin. Otherwise, we grep through all the files specified. */
	matched = 0;
	do {
		cur_file = *argv++;
		file = stdin;
		if (!cur_file || LONE_DASH(cur_file)) {
			cur_file = "(standard input)";
		} else {
			if (option_mask32 & OPT_r) {
				struct stat st;
				if (stat(cur_file, &st) == 0 && S_ISDIR(st.st_mode)) {
					if (!(option_mask32 & OPT_h))
						print_filename = 1;
					matched += grep_dir(cur_file);
					goto grep_done;
				}
			}
			/* else: fopen(dir) will succeed, but reading won't */
			file = fopen_for_read(cur_file);
			if (file == NULL) {
				if (!SUPPRESS_ERR_MSGS)
					bb_simple_perror_msg(cur_file);
				open_errors = 1;
				continue;
			}
		}
		matched += grep_file(file);
		fclose_if_not_stdin(file);
 grep_done: ;
	} while (--argc > 0);

	/* destroy all the elments in the pattern list */
	if (ENABLE_FEATURE_CLEAN_UP) {
		while (pattern_head) {
			llist_t *pattern_head_ptr = pattern_head;
			grep_list_data_t *gl = (grep_list_data_t *)pattern_head_ptr->data;

			pattern_head = pattern_head->link;
			if (gl->flg_mem_alocated_compiled & ALLOCATED)
				free(gl->pattern);
			if (gl->flg_mem_alocated_compiled & COMPILED)
				regfree(&gl->compiled_regex);
			free(gl);
			free(pattern_head_ptr);
		}
	}
	/* 0 = success, 1 = failed, 2 = error */
	if (open_errors)
		return 2;
	return !matched; /* invert return value: 0 = success, 1 = failed */
}
