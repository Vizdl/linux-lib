/* vi: set sw=4 ts=4: */
/*
 * Mini watch implementation for busybox
 *
 * Copyright (C) 2001 by Michael Habermann <mhabermann@gmx.de>
 * Copyrigjt (C) Mar 16, 2003 Manuel Novoa III   (mjn3@codepoet.org)
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/* BB_AUDIT SUSv3 N/A */
/* BB_AUDIT GNU defects -- only option -n is supported. */

#include "libbb.h"

// procps 2.0.18:
// watch [-d] [-n seconds]
//   [--differences[=cumulative]] [--interval=seconds] command
//
// procps-3.2.3:
// watch [-dt] [-n seconds]
//   [--differences[=cumulative]] [--interval=seconds] [--no-title] command
//
// (procps 3.x and procps 2.x are forks, not newer/older versions of the same)

int watch_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int watch_main(int argc UNUSED_PARAM, char **argv)
{
	unsigned opt;
	unsigned period = 2;
	unsigned width, new_width;
	char *header;
	char *cmd;

	opt_complementary = "-1:n+"; // at least one param; -n NUM
	// "+": stop at first non-option (procps 3.x only)
	opt = getopt32(argv, "+dtn:", &period);
	argv += optind;

	// watch from both procps 2.x and 3.x does concatenation. Example:
	// watch ls -l "a /tmp" "2>&1" -- ls won't see "a /tmp" as one param
	cmd = *argv;
	while (*++argv)
		cmd = xasprintf("%s %s", cmd, *argv); // leaks cmd

	width = (unsigned)-1; // make sure first time new_width != width
	header = NULL;
	while (1) {
		printf("\033[H\033[J");
		if (!(opt & 0x2)) { // no -t
			const unsigned time_len = sizeof("1234-67-90 23:56:89");
			time_t t;

			get_terminal_width_height(STDIN_FILENO, &new_width, NULL);
			if (new_width != width) {
				width = new_width;
				free(header);
				header = xasprintf("Every %us: %-*s", period, (int)width, cmd);
			}
			time(&t);
			if (time_len < width)
				strftime(header + width - time_len, time_len,
					"%Y-%m-%d %H:%M:%S", localtime(&t));

			puts(header);
		}
		fflush(stdout);
		// TODO: 'real' watch pipes cmd's output to itself
		// and does not allow it to overflow the screen
		// (taking into account linewrap!)
		system(cmd);
		sleep(period);
	}
	return 0; // gcc thinks we can reach this :)
}
