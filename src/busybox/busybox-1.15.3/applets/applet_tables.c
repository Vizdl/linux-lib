/* vi: set sw=4 ts=4: */
/*
 * Applet table generator.
 * Runs on host and produces include/applet_tables.h
 *
 * Copyright (C) 2007 Denys Vlasenko <vda.linux@googlemail.com>
 *
 * Licensed under GPLv2, see file License in this tarball for details.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/autoconf.h"
#include "../include/busybox.h"

struct bb_applet {
	const char *name;
	const char *main;
	enum bb_install_loc_t install_loc;
	enum bb_suid_t need_suid;
	/* true if instead of fork(); exec("applet"); waitpid();
	 * one can do fork(); exit(applet_main(argc,argv)); waitpid(); */
	unsigned char noexec;
	/* Even nicer */
	/* true if instead of fork(); exec("applet"); waitpid();
	 * one can simply call applet_main(argc,argv); */
	unsigned char nofork;
};

/* Define struct bb_applet applets[] */
#include "../include/applets.h"

enum { NUM_APPLETS = ARRAY_SIZE(applets) };

static int offset[NUM_APPLETS];

static int cmp_name(const void *a, const void *b)
{
	const struct bb_applet *aa = a;
	const struct bb_applet *bb = b;
	return strcmp(aa->name, bb->name);
}

int main(int argc, char **argv)
{
	int i;
	int ofs;
	unsigned MAX_APPLET_NAME_LEN = 1;

	qsort(applets, NUM_APPLETS, sizeof(applets[0]), cmp_name);

	ofs = 0;
	for (i = 0; i < NUM_APPLETS; i++) {
		offset[i] = ofs;
		ofs += strlen(applets[i].name) + 1;
	}
	/* We reuse 4 high-order bits of offset array for other purposes,
	 * so if they are indeed needed, refuse to proceed */
	if (ofs > 0xfff)
		return 1;
	if (!argv[1])
		return 1;

	i = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (i < 0)
		return 1;
	dup2(i, 1);

	/* Keep in sync with include/busybox.h! */

	puts("/* This is a generated file, don't edit */\n");

	printf("#define NUM_APPLETS %u\n", NUM_APPLETS);
	if (NUM_APPLETS == 1) {
		printf("#define SINGLE_APPLET_STR \"%s\"\n", applets[0].name);
		printf("#define SINGLE_APPLET_MAIN %s_main\n", applets[0].name);
	}

	puts("\nconst char applet_names[] ALIGN1 = \"\"");
	for (i = 0; i < NUM_APPLETS; i++) {
		printf("\"%s\" \"\\0\"\n", applets[i].name);
		if (MAX_APPLET_NAME_LEN < strlen(applets[i].name))
			MAX_APPLET_NAME_LEN = strlen(applets[i].name);
	}
	puts(";");

	puts("\nint (*const applet_main[])(int argc, char **argv) = {");
	for (i = 0; i < NUM_APPLETS; i++) {
		printf("%s_main,\n", applets[i].main);
	}
	puts("};");

	puts("const uint16_t applet_nameofs[] ALIGN2 = {");
	for (i = 0; i < NUM_APPLETS; i++) {
		printf("0x%04x,\n",
			offset[i]
#if ENABLE_FEATURE_PREFER_APPLETS
			+ (applets[i].nofork << 12)
			+ (applets[i].noexec << 13)
#endif
#if ENABLE_FEATURE_SUID
			+ (applets[i].need_suid << 14) /* 2 bits */
#endif
		);
	}
	puts("};");

#if ENABLE_FEATURE_INSTALLER
	puts("const uint8_t applet_install_loc[] ALIGN1 = {");
	i = 0;
	while (i < NUM_APPLETS) {
		int v = applets[i].install_loc; /* 3 bits */
		if (++i < NUM_APPLETS)
			v |= applets[i].install_loc << 4; /* 3 bits */
		printf("0x%02x,\n", v);
		i++;
	}
	puts("};\n");
#endif

	printf("#define MAX_APPLET_NAME_LEN %u\n", MAX_APPLET_NAME_LEN);

	return 0;
}
