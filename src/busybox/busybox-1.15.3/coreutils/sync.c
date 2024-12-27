/* vi: set sw=4 ts=4: */
/*
 * Mini sync implementation for busybox
 *
 * Copyright (C) 1995, 1996 by Bruce Perens <bruce@pixar.com>.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/* BB_AUDIT SUSv3 N/A -- Matches GNU behavior. */

#include "libbb.h"

/* This is a NOFORK applet. Be very careful! */

int sync_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int sync_main(int argc, char **argv UNUSED_PARAM)
{
	/* coreutils-6.9 compat */
	bb_warn_ignoring_args(argc - 1);

	sync();

	return EXIT_SUCCESS;
}
