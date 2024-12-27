/* vi: set sw=4 ts=4: */
/*
 * Mini fsync implementation for busybox
 *
 * Copyright (C) 2008 Nokia Corporation. All rights reserved.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */
#include "libbb.h"

/* This is a NOFORK applet. Be very careful! */

int fsync_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int fsync_main(int argc UNUSED_PARAM, char **argv)
{
	int status;
	int opts;

	opts = getopt32(argv, "d"); /* fdatasync */
	argv += optind;
	if (!*argv) {
		bb_show_usage();
	}

	status = EXIT_SUCCESS;
	do {
		int fd = open3_or_warn(*argv, O_NOATIME | O_NOCTTY | O_RDONLY, 0);

		if (fd == -1) {
			status = EXIT_FAILURE;
			continue;
		}
		if ((opts ? fdatasync(fd) : fsync(fd))) {
			//status = EXIT_FAILURE; - do we want this?
			bb_simple_perror_msg(*argv);
		}
		close(fd);
	} while (*++argv);

	return status;
}
