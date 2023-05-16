/* vi: set sw=4 ts=4: */
/*
 * fixup implementation for vizdl
 *
 * Copyright (C) 2023  Lei Ding <dinglei@kernelsoft.com>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */


//config:config FIXUP
//config:	bool "fixup test"
//config:	default y
//config:	help
//config:	linux fixup test

//applet:IF_FIXUP(APPLET_NOEXEC(fixup, fixup, BB_DIR_BIN, BB_SUID_DROP, fixup))

//kbuild:lib-$(CONFIG_FIXUP) += fixup.o

//usage:#define fixup_trivial_usage
//usage:       "[-t] [-T] [DEVICE]"
//usage:#define fixup_full_usage "\n\n"
//usage:       "Eject DEVICE or default /dev/cdrom\n"
//usage:     "\n	-t	Close tray"
//usage:     "\n	-T	Open/close tray (toggle)"

#include "libbb.h"     /* 包含busybox公共头文件 */
#include <sched.h>

int fixup_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
char *data = "/dev/null";
char path[256] = "/dev/null";


void test_copy_from_user(void)
{
    int ret;
    struct sched_param* param = (struct sched_param*)0x5000000;
    printf("test_copy_from_user begin, argc point[%lx]\n", param);
    ret = sched_setscheduler(getpid(), SCHED_FIFO, param);
    printf("test_copy_from_user end, ret %d.\n", ret);
}

void test_get_name(void)
{
	char* buff = (char *)0x40000000;
    int fd = 0;
    printf("fixup begin\n");
    fd = open(buff, O_WRONLY);
    printf("fixup end, ret = %d\n", fd);
}

int fixup_main(int argc UNUSED_PARAM, char **argv)
{
    test_copy_from_user();
    test_get_name();
    return 0;
}