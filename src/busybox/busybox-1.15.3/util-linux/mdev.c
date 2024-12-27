/* vi: set sw=4 ts=4: */
/*
 * mdev - Mini udev for busybox
 *
 * Copyright 2005 Rob Landley <rob@landley.net>
 * Copyright 2005 Frank Sorenson <frank@tuxrocks.com>
 *
 * Licensed under GPL version 2, see file LICENSE in this tarball for details.
 */
#include "libbb.h"
#include "xregex.h"

/* "mdev -s" scans /sys/class/xxx, looking for directories which have dev
 * file (it is of the form "M:m\n"). Example: /sys/class/tty/tty0/dev
 * contains "4:0\n". Directory name is taken as device name, path component
 * directly after /sys/class/ as subsystem. In this example, "tty0" and "tty".
 * Then mdev creates the /dev/device_name node.
 * If /sys/class/.../dev file does not exist, mdev still may act
 * on this device: see "@|$|*command args..." parameter in config file.
 *
 * mdev w/o parameters is called as hotplug helper. It takes device
 * and subsystem names from $DEVPATH and $SUBSYSTEM, extracts
 * maj,min from "/sys/$DEVPATH/dev" and also examines
 * $ACTION ("add"/"delete") and $FIRMWARE.
 *
 * If action is "add", mdev creates /dev/device_name similarly to mdev -s.
 * (todo: explain "delete" and $FIRMWARE)
 *
 * If /etc/mdev.conf exists, it may modify /dev/device_name's properties.
 * /etc/mdev.conf file format:
 *
 * [-][subsystem/]device  user:grp  mode  [>|=path] [@|$|*command args...]
 * [-]@maj,min[-min2]     user:grp  mode  [>|=path] [@|$|*command args...]
 * [-]$envvar=val         user:grp  mode  [>|=path] [@|$|*command args...]
 *
 * Leading minus in 1st field means "don't stop on this line", otherwise
 * search is stopped after the matching line is encountered.
 *
 * The device name or "subsystem/device" combo is matched against 1st field
 * (which is a regex), or maj,min is matched against 1st field,
 * or specified environment variable (as regex) is matched against 1st field.
 *
 * $envvar=val format is useful for loading modules for hot-plugged devices
 * which do not have driver loaded yet. In this case /sys/class/.../dev
 * does not exist, but $MODALIAS is set to needed module's name
 * (actually, an alias to it) by kernel. This rule instructs mdev
 * to load the module and exit:
 *    $MODALIAS=.* 0:0 660 @modprobe "$MODALIAS"
 * The kernel will generate another hotplug event when /sys/class/.../dev
 * file appears.
 *
 * When line matches, the device node is created, chmod'ed and chown'ed,
 * moved to path, and if >path, a symlink to moved node is created,
 * all this if /sys/class/.../dev exists.
 *    Examples:
 *    =loop/      - moves to /dev/loop
 *    >disk/sda%1 - moves to /dev/disk/sdaN, makes /dev/sdaN a symlink
 *
 * Then "command args..." is executed (via sh -c 'command args...').
 * @:execute on creation, $:on deletion, *:on both.
 * This happens regardless of /sys/class/.../dev existence.
 */

struct globals {
	int root_major, root_minor;
	char *subsystem;
};
#define G (*(struct globals*)&bb_common_bufsiz1)
#define root_major (G.root_major)
#define root_minor (G.root_minor)
#define subsystem  (G.subsystem )

/* Prevent infinite loops in /sys symlinks */
#define MAX_SYSFS_DEPTH 3

/* We use additional 64+ bytes in make_device() */
#define SCRATCH_SIZE 80

/* Builds an alias path.
 * This function potentionally reallocates the alias parameter.
 * Only used for ENABLE_FEATURE_MDEV_RENAME
 */
static char *build_alias(char *alias, const char *device_name)
{
	char *dest;

	/* ">bar/": rename to bar/device_name */
	/* ">bar[/]baz": rename to bar[/]baz */
	dest = strrchr(alias, '/');
	if (dest) { /* ">bar/[baz]" ? */
		*dest = '\0'; /* mkdir bar */
		bb_make_directory(alias, 0755, FILEUTILS_RECUR);
		*dest = '/';
		if (dest[1] == '\0') { /* ">bar/" => ">bar/device_name" */
			dest = alias;
			alias = concat_path_file(alias, device_name);
			free(dest);
		}
	}

	return alias;
}

/* mknod in /dev based on a path like "/sys/block/hda/hda1"
 * NB1: path parameter needs to have SCRATCH_SIZE scratch bytes
 * after NUL, but we promise to not mangle (IOW: to restore if needed)
 * path string.
 * NB2: "mdev -s" may call us many times, do not leak memory/fds!
 */
static void make_device(char *path, int delete)
{
	char *device_name;
	int major, minor, type, len;
	int mode;
	parser_t *parser;

	/* Try to read major/minor string.  Note that the kernel puts \n after
	 * the data, so we don't need to worry about null terminating the string
	 * because sscanf() will stop at the first nondigit, which \n is.
	 * We also depend on path having writeable space after it.
	 */
	major = -1;
	if (!delete) {
		char *dev_maj_min = path + strlen(path);

		strcpy(dev_maj_min, "/dev");
		len = open_read_close(path, dev_maj_min + 1, 64);
		*dev_maj_min = '\0';
		if (len < 1) {
			if (!ENABLE_FEATURE_MDEV_EXEC)
				return;
			/* no "dev" file, but we can still run scripts
			 * based on device name */
		} else if (sscanf(++dev_maj_min, "%u:%u", &major, &minor) != 2) {
			major = -1;
		}
	}

	/* Determine device name, type, major and minor */
	device_name = (char*) bb_basename(path);
	/* http://kernel.org/doc/pending/hotplug.txt says that only
	 * "/sys/block/..." is for block devices. "/sys/bus" etc is not.
	 * But since 2.6.25 block devices are also in /sys/class/block,
	 * we use strstr("/block/") to forestall future surprises. */
	type = S_IFCHR;
	if (strstr(path, "/block/"))
		type = S_IFBLK;

	/* Make path point to "subsystem/device_name" */
	if (path[5] == 'b') /* legacy /sys/block? */
		path += sizeof("/sys/") - 1;
	else
		path += sizeof("/sys/class/") - 1;

	/* If we have config file, look up user settings */
	if (ENABLE_FEATURE_MDEV_CONF)
		parser = config_open2("/etc/mdev.conf", fopen_for_read);

	do {
		int keep_matching;
		struct bb_uidgid_t ugid;
		char *tokens[4];
		char *command = NULL;
		char *alias = NULL;
		char aliaslink = aliaslink; /* for compiler */

		/* Defaults in case we won't match any line */
		ugid.uid = ugid.gid = 0;
		keep_matching = 0;
		mode = 0660;

		if (ENABLE_FEATURE_MDEV_CONF
		 && config_read(parser, tokens, 4, 3, "# \t", PARSE_NORMAL)
		) {
			char *val;
			char *str_to_match;
			regmatch_t off[1 + 9 * ENABLE_FEATURE_MDEV_RENAME_REGEXP];

			val = tokens[0];
			keep_matching = ('-' == val[0]);
			val += keep_matching; /* swallow leading dash */

			/* Match against either "subsystem/device_name"
			 * or "device_name" alone */
			str_to_match = strchr(val, '/') ? path : device_name;

			/* Fields: regex uid:gid mode [alias] [cmd] */

			if (val[0] == '@') {
				/* @major,minor[-minor2] */
				/* (useful when name is ambiguous:
				 * "/sys/class/usb/lp0" and
				 * "/sys/class/printer/lp0") */
				int cmaj, cmin0, cmin1, sc;
				if (major < 0)
					continue; /* no dev, no match */
				sc = sscanf(val, "@%u,%u-%u", &cmaj, &cmin0, &cmin1);
				if (sc < 1 || major != cmaj
				 || (sc == 2 && minor != cmin0)
				 || (sc == 3 && (minor < cmin0 || minor > cmin1))
				) {
					continue; /* this line doesn't match */
				}
				goto line_matches;
			}
			if (val[0] == '$') {
				/* regex to match an environment variable */
				char *eq = strchr(++val, '=');
				if (!eq)
					continue;
				*eq = '\0';
				str_to_match = getenv(val);
				if (!str_to_match)
					continue;
				str_to_match -= strlen(val) + 1;
				*eq = '=';
			}
			/* else: regex to match [subsystem/]device_name */

			{
				regex_t match;
				int result;

				xregcomp(&match, val, REG_EXTENDED);
				result = regexec(&match, str_to_match, ARRAY_SIZE(off), off, 0);
				regfree(&match);
				//bb_error_msg("matches:");
				//for (int i = 0; i < ARRAY_SIZE(off); i++) {
				//	if (off[i].rm_so < 0) continue;
				//	bb_error_msg("match %d: '%.*s'\n", i,
				//		(int)(off[i].rm_eo - off[i].rm_so),
				//		device_name + off[i].rm_so);
				//}

				/* If no match, skip rest of line */
				/* (regexec returns whole pattern as "range" 0) */
				if (result || off[0].rm_so
				 || ((int)off[0].rm_eo != (int)strlen(str_to_match))
				) {
					continue; /* this line doesn't match */
				}
			}
 line_matches:
			/* This line matches. Stop parsing after parsing
			 * the rest the line unless keep_matching == 1 */

			/* 2nd field: uid:gid - device ownership */
			if (get_uidgid(&ugid, tokens[1], 1) == 0)
				bb_error_msg("unknown user/group %s", tokens[1]);

			/* 3rd field: mode - device permissions */
			mode = strtoul(tokens[2], NULL, 8);

			val = tokens[3];
			/* 4th field (opt): >|=alias */

			if (ENABLE_FEATURE_MDEV_RENAME && val) {
				aliaslink = val[0];
				if (aliaslink == '>' || aliaslink == '=') {
					char *a, *s, *st;
					char *p;
					unsigned i, n;

					a = val;
					s = strchrnul(val, ' ');
					st = strchrnul(val, '\t');
					if (st < s)
						s = st;
					val = (s[0] && s[1]) ? s+1 : NULL;
					s[0] = '\0';

					if (ENABLE_FEATURE_MDEV_RENAME_REGEXP) {
						/* substitute %1..9 with off[1..9], if any */
						n = 0;
						s = a;
						while (*s)
							if (*s++ == '%')
								n++;

						p = alias = xzalloc(strlen(a) + n * strlen(str_to_match));
						s = a + 1;
						while (*s) {
							*p = *s;
							if ('%' == *s) {
								i = (s[1] - '0');
								if (i <= 9 && off[i].rm_so >= 0) {
									n = off[i].rm_eo - off[i].rm_so;
									strncpy(p, str_to_match + off[i].rm_so, n);
									p += n - 1;
									s++;
								}
							}
							p++;
							s++;
						}
					} else {
						alias = xstrdup(a + 1);
					}
				}
			}

			if (ENABLE_FEATURE_MDEV_EXEC && val) {
				const char *s = "$@*";
				const char *s2 = strchr(s, val[0]);

				if (!s2) {
					bb_error_msg("bad line %u", parser->lineno);
					if (ENABLE_FEATURE_MDEV_RENAME)
						free(alias);
					continue;
				}

				/* Are we running this command now?
				 * Run $cmd on delete, @cmd on create, *cmd on both
				 */
				if (s2-s != delete)
					command = xstrdup(val + 1);
			}
		}

		/* End of field parsing */

		/* "Execute" the line we found */
		{
			const char *node_name;

			node_name = device_name;
			if (ENABLE_FEATURE_MDEV_RENAME && alias)
				node_name = alias = build_alias(alias, device_name);

			if (!delete && major >= 0) {
				if (mknod(node_name, mode | type, makedev(major, minor)) && errno != EEXIST)
					bb_perror_msg_and_die("mknod %s", node_name);
				if (major == root_major && minor == root_minor)
					symlink(node_name, "root");
				if (ENABLE_FEATURE_MDEV_CONF) {
					chmod(node_name, mode);
					chown(node_name, ugid.uid, ugid.gid);
				}
				if (ENABLE_FEATURE_MDEV_RENAME && alias) {
					if (aliaslink == '>')
						symlink(node_name, device_name);
				}
			}

			if (ENABLE_FEATURE_MDEV_EXEC && command) {
				/* setenv will leak memory, use putenv/unsetenv/free */
				char *s = xasprintf("%s=%s", "MDEV", node_name);
				char *s1 = xasprintf("%s=%s", "SUBSYSTEM", subsystem);
				putenv(s);
				putenv(s1);
				if (system(command) == -1)
					bb_perror_msg("can't run '%s'", command);
				unsetenv("SUBSYSTEM");
				free(s1);
				unsetenv("MDEV");
				free(s);
				free(command);
			}

			if (delete) {
				if (ENABLE_FEATURE_MDEV_RENAME && alias) {
					if (aliaslink == '>')
						unlink(device_name);
				}
				unlink(node_name);
			}

			if (ENABLE_FEATURE_MDEV_RENAME)
				free(alias);
		}

		/* We found matching line.
		 * Stop unless it was prefixed with '-' */
		if (ENABLE_FEATURE_MDEV_CONF && !keep_matching)
			break;

	/* end of "while line is read from /etc/mdev.conf" */
	} while (ENABLE_FEATURE_MDEV_CONF);

	if (ENABLE_FEATURE_MDEV_CONF)
		config_close(parser);
}

/* File callback for /sys/ traversal */
static int FAST_FUNC fileAction(const char *fileName,
		struct stat *statbuf UNUSED_PARAM,
		void *userData,
		int depth UNUSED_PARAM)
{
	size_t len = strlen(fileName) - 4; /* can't underflow */
	char *scratch = userData;

	/* len check is for paranoid reasons */
	if (strcmp(fileName + len, "/dev") != 0 || len >= PATH_MAX)
		return FALSE;

	strcpy(scratch, fileName);
	scratch[len] = '\0';
	make_device(scratch, 0);

	return TRUE;
}

/* Directory callback for /sys/ traversal */
static int FAST_FUNC dirAction(const char *fileName UNUSED_PARAM,
		struct stat *statbuf UNUSED_PARAM,
		void *userData UNUSED_PARAM,
		int depth)
{
	/* Extract device subsystem -- the name of the directory
	 * under /sys/class/ */
	if (1 == depth) {
		free(subsystem);
		subsystem = strrchr(fileName, '/');
		if (subsystem)
			subsystem = xstrdup(subsystem + 1);
	}

	return (depth >= MAX_SYSFS_DEPTH ? SKIP : TRUE);
}

/* For the full gory details, see linux/Documentation/firmware_class/README
 *
 * Firmware loading works like this:
 * - kernel sets FIRMWARE env var
 * - userspace checks /lib/firmware/$FIRMWARE
 * - userspace waits for /sys/$DEVPATH/loading to appear
 * - userspace writes "1" to /sys/$DEVPATH/loading
 * - userspace copies /lib/firmware/$FIRMWARE into /sys/$DEVPATH/data
 * - userspace writes "0" (worked) or "-1" (failed) to /sys/$DEVPATH/loading
 * - kernel loads firmware into device
 */
static void load_firmware(const char *firmware, const char *sysfs_path)
{
	int cnt;
	int firmware_fd, loading_fd, data_fd;

	/* check for /lib/firmware/$FIRMWARE */
	xchdir("/lib/firmware");
	firmware_fd = xopen(firmware, O_RDONLY);

	/* in case we goto out ... */
	data_fd = -1;

	/* check for /sys/$DEVPATH/loading ... give 30 seconds to appear */
	xchdir(sysfs_path);
	for (cnt = 0; cnt < 30; ++cnt) {
		loading_fd = open("loading", O_WRONLY);
		if (loading_fd != -1)
			goto loading;
		sleep(1);
	}
	goto out;

 loading:
	/* tell kernel we're loading by "echo 1 > /sys/$DEVPATH/loading" */
	if (full_write(loading_fd, "1", 1) != 1)
		goto out;

	/* load firmware into /sys/$DEVPATH/data */
	data_fd = open("data", O_WRONLY);
	if (data_fd == -1)
		goto out;
	cnt = bb_copyfd_eof(firmware_fd, data_fd);

	/* tell kernel result by "echo [0|-1] > /sys/$DEVPATH/loading" */
	if (cnt > 0)
		full_write(loading_fd, "0", 1);
	else
		full_write(loading_fd, "-1", 2);

 out:
	if (ENABLE_FEATURE_CLEAN_UP) {
		close(firmware_fd);
		close(loading_fd);
		close(data_fd);
	}
}

int mdev_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int mdev_main(int argc UNUSED_PARAM, char **argv)
{
	RESERVE_CONFIG_BUFFER(temp, PATH_MAX + SCRATCH_SIZE);

	/* We can be called as hotplug helper */
	/* Kernel cannot provide suitable stdio fds for us, do it ourself */
	bb_sanitize_stdio();

	/* Force the configuration file settings exactly */
	umask(0);

	xchdir("/dev");

	if (argv[1] && strcmp(argv[1], "-s") == 0) {
		/* Scan:
		 * mdev -s
		 */
		struct stat st;

		xstat("/", &st);
		root_major = major(st.st_dev);
		root_minor = minor(st.st_dev);

		/* ACTION_FOLLOWLINKS is needed since in newer kernels
		 * /sys/block/loop* (for example) are symlinks to dirs,
		 * not real directories.
		 * (kernel's CONFIG_SYSFS_DEPRECATED makes them real dirs,
		 * but we can't enforce that on users)
		 */
		if (access("/sys/class/block", F_OK) != 0) {
			/* Scan obsolete /sys/block only if /sys/class/block
			 * doesn't exist. Otherwise we'll have dupes.
			 * Also, do not complain if it doesn't exist.
			 * Some people configure kernel to have no blockdevs.
			 */
			recursive_action("/sys/block",
				ACTION_RECURSE | ACTION_FOLLOWLINKS | ACTION_QUIET,
				fileAction, dirAction, temp, 0);
		}
		recursive_action("/sys/class",
			ACTION_RECURSE | ACTION_FOLLOWLINKS,
			fileAction, dirAction, temp, 0);
	} else {
		char *fw;
		char *seq;
		char *action;
		char *env_path;

		/* Hotplug:
		 * env ACTION=... DEVPATH=... SUBSYSTEM=... [SEQNUM=...] mdev
		 * ACTION can be "add" or "remove"
		 * DEVPATH is like "/block/sda" or "/class/input/mice"
		 */
		action = getenv("ACTION");
		env_path = getenv("DEVPATH");
		subsystem = getenv("SUBSYSTEM");
		if (!action || !env_path /*|| !subsystem*/)
			bb_show_usage();
		fw = getenv("FIRMWARE");

		/* If it exists, does /dev/mdev.seq match $SEQNUM?
		 * If it does not match, earlier mdev is running
		 * in parallel, and we need to wait */
		seq = getenv("SEQNUM");
		if (seq) {
			int timeout = 2000 / 32; /* 2000 msec */
			do {
				int seqlen;
				char seqbuf[sizeof(int)*3 + 2];

				seqlen = open_read_close("mdev.seq", seqbuf, sizeof(seqbuf-1));
				if (seqlen < 0) {
					seq = NULL;
					break;
				}
				seqbuf[seqlen] = '\0';
				if (seqbuf[0] == '\n' /* seed file? */
				 || strcmp(seq, seqbuf) == 0 /* correct idx? */
				) {
					break;
				}
				usleep(32*1000);
			} while (--timeout);
		}

		snprintf(temp, PATH_MAX, "/sys%s", env_path);
		if (strcmp(action, "remove") == 0) {
			/* Ignoring "remove firmware". It was reported
			 * to happen and to cause erroneous deletion
			 * of device nodes. */
			if (!fw)
				make_device(temp, 1);
		}
		else if (strcmp(action, "add") == 0) {
			make_device(temp, 0);
			if (ENABLE_FEATURE_MDEV_LOAD_FIRMWARE) {
				if (fw)
					load_firmware(fw, temp);
			}
		}

		if (seq) {
			xopen_xwrite_close("mdev.seq", utoa(xatou(seq) + 1));
		}
	}

	if (ENABLE_FEATURE_CLEAN_UP)
		RELEASE_CONFIG_BUFFER(temp);

	return EXIT_SUCCESS;
}
