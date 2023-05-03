/* vi: set sw=4 ts=4: */
/*
 * Mini chvt implementation for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"

int chvt_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int chvt_main(int argc, char **argv)
{
	int num;

	if (argc != 2) {
		bb_show_usage();
	}

	num = xatou_range(argv[1], 1, 63);
	console_make_active(get_console_fd_or_die(), num);
	return EXIT_SUCCESS;
}
