/* vi: set sw=4 ts=4: */
/*
 * seq implementation for busybox
 *
 * Copyright (C) 2004, Glenn McGrath
 *
 * Licensed under the GPL v2, see the file LICENSE in this tarball.
 */
#include "libbb.h"

/* This is a NOFORK applet. Be very careful! */

int seq_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int seq_main(int argc, char **argv)
{
	enum {
		OPT_w = (1 << 0),
		OPT_s = (1 << 1),
	};
	double first, last, increment, v;
	unsigned n;
	unsigned width;
	unsigned frac_part;
	const char *sep, *opt_s = "\n";
	unsigned opt;

#if ENABLE_LOCALE_SUPPORT
	/* Undo busybox.c: on input, we want to use dot
	 * as fractional separator, regardless of current locale */
	setlocale(LC_NUMERIC, "C");
#endif

	opt = getopt32(argv, "+ws:", &opt_s);
	argc -= optind;
	argv += optind;
	first = increment = 1;
	errno = 0;
	switch (argc) {
			char *pp;
		case 3:
			increment = strtod(argv[1], &pp);
			errno |= *pp;
		case 2:
			first = strtod(argv[0], &pp);
			errno |= *pp;
		case 1:
			last = strtod(argv[argc-1], &pp);
			if (!errno && *pp == '\0')
				break;
		default:
			bb_show_usage();
	}

#if ENABLE_LOCALE_SUPPORT
	setlocale(LC_NUMERIC, "");
#endif

	/* Last checked to be compatible with: coreutils-6.10 */
	width = 0;
	frac_part = 0;
	while (1) {
		char *dot = strchrnul(*argv, '.');
		int w = (dot - *argv);
		int f = strlen(dot);
		if (width < w)
			width = w;
		argv++;
		if (!*argv)
			break;
		/* Why do the above _before_ frac check below?
		 * Try "seq 1 2.0" and "seq 1.0 2.0":
		 * coreutils never pay attention to the number
		 * of fractional digits in last arg. */
		if (frac_part < f)
			frac_part = f;
	}
	if (frac_part) {
		frac_part--;
		if (frac_part)
			width += frac_part + 1;
	}
	if (!(opt & OPT_w))
		width = 0;

	sep = "";
	v = first;
	n = 0;
	while (increment >= 0 ? v <= last : v >= last) {
		printf("%s%0*.*f", sep, width, frac_part, v);
		sep = opt_s;
		/* v += increment; - would accumulate floating point errors */
		n++;
		v = first + n * increment;
	}
	if (n) /* if while loop executed at least once */
		bb_putchar('\n');

	return fflush(stdout);
}
