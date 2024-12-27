/* DO NOT EDIT. This file is generated from usage.src.h */
/* vi: set sw=8 ts=8: */
/*
 * This file suffers from chronically incorrect tabification
 * of messages. Before editing this file:
 * 1. Switch you editor to 8-space tab mode.
 * 2. Do not use \t in messages, use real tab character.
 * 3. Start each source line with message as follows:
 *    |<7 spaces>"text with tabs"....
 * or
 *    |<5 spaces>"\ntext with tabs"....
 */
#ifndef BB_USAGE_H
#define BB_USAGE_H 1

#define NOUSAGE_STR "\b"

#define scripted_trivial_usage NOUSAGE_STR
#define scripted_full_usage ""

#if !ENABLE_USE_BB_CRYPT || ENABLE_USE_BB_CRYPT_SHA
# define CRYPT_METHODS_HELP_STR "des,md5,sha256/512" \
	" (default "CONFIG_FEATURE_DEFAULT_PASSWD_ALGO")"
#else
# define CRYPT_METHODS_HELP_STR "des,md5" \
	" (default "CONFIG_FEATURE_DEFAULT_PASSWD_ALGO")"
#endif


#define ar_trivial_usage \
       "[-o] [-v] [-p] [-t] [-x] ARCHIVE FILES" \

#define ar_full_usage "\n\n" \
       "Extract or list FILES from an ar archive\n" \
     "\n	-o	Preserve original dates" \
     "\n	-p	Extract to stdout" \
     "\n	-t	List" \
     "\n	-x	Extract" \
     "\n	-v	Verbose" \

#define uncompress_trivial_usage \
       "[-cf] [FILE]..." \

#define uncompress_full_usage "\n\n" \
       "Decompress .Z file[s]\n" \
     "\n	-c	Write to stdout" \
     "\n	-f	Overwrite" \

#define gunzip_trivial_usage \
       "[-cfkt] [FILE]..." \

#define gunzip_full_usage "\n\n" \
       "Decompress FILEs (or stdin)\n" \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \
     "\n	-t	Test file integrity" \

#define gunzip_example_usage \
       "$ ls -la /tmp/BusyBox*\n" \
       "-rw-rw-r--    1 andersen andersen   557009 Apr 11 10:55 /tmp/BusyBox-0.43.tar.gz\n" \
       "$ gunzip /tmp/BusyBox-0.43.tar.gz\n" \
       "$ ls -la /tmp/BusyBox*\n" \
       "-rw-rw-r--    1 andersen andersen  1761280 Apr 14 17:47 /tmp/BusyBox-0.43.tar\n" \

#define zcat_trivial_usage \
       "[FILE]..." \

#define zcat_full_usage "\n\n" \
       "Decompress to stdout" \

#define bunzip2_trivial_usage \
       "[-cfk] [FILE]..." \

#define bunzip2_full_usage "\n\n" \
       "Decompress FILEs (or stdin)\n" \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \

#define bzcat_trivial_usage \
       "[FILE]..." \

#define bzcat_full_usage "\n\n" \
       "Decompress to stdout" \

#define unlzma_trivial_usage \
       "[-cfk] [FILE]..." \

#define unlzma_full_usage "\n\n" \
       "Decompress FILE (or stdin)\n" \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \

#define lzma_trivial_usage \
       "-d [-cfk] [FILE]..." \

#define lzma_full_usage "\n\n" \
       "Decompress FILE (or stdin)\n" \
     "\n	-d	Decompress" \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \

#define lzcat_trivial_usage \
       "[FILE]..." \

#define lzcat_full_usage "\n\n" \
       "Decompress to stdout" \

#define unxz_trivial_usage \
       "[-cfk] [FILE]..." \

#define unxz_full_usage "\n\n" \
       "Decompress FILE (or stdin)\n" \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \

#define xz_trivial_usage \
       "-d [-cfk] [FILE]..." \

#define xz_full_usage "\n\n" \
       "Decompress FILE (or stdin)\n" \
     "\n	-d	Decompress" \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \

#define xzcat_trivial_usage \
       "[FILE]..." \

#define xzcat_full_usage "\n\n" \
       "Decompress to stdout" \

#define bzip2_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define bzip2_full_usage "\n\n" \
       "Compress FILEs (or stdin) with bzip2 algorithm\n" \
     "\n	-1..9	Compression level" \
	IF_FEATURE_BZIP2_DECOMPRESS( \
     "\n	-d	Decompress" \
     "\n	-t	Test file integrity" \
	) \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \

#define cpio_trivial_usage \
       "[-dmvu] [-F FILE] [-R USER[:GRP]]" IF_FEATURE_CPIO_O(" [-H newc]") \
       " [-ti"IF_FEATURE_CPIO_O("o")"]" IF_FEATURE_CPIO_P(" [-p DIR]") \
       " [EXTR_FILE]..." \

#define cpio_full_usage "\n\n" \
       "Extract (-i) or list (-t) files from a cpio archive" \
	IF_FEATURE_CPIO_O(", or" \
     "\ntake file list from stdin and create an archive (-o)" \
                IF_FEATURE_CPIO_P(" or copy files (-p)") \
	) \
     "\n" \
     "\nMain operation mode:" \
     "\n	-t	List" \
     "\n	-i	Extract EXTR_FILEs (or all)" \
	IF_FEATURE_CPIO_O( \
     "\n	-o	Create (requires -H newc)" \
	) \
	IF_FEATURE_CPIO_P( \
     "\n	-p DIR	Copy files to DIR" \
	) \
     "\nOptions:" \
	IF_FEATURE_CPIO_O( \
     "\n	-H newc	Archive format" \
	) \
     "\n	-d	Make leading directories" \
     "\n	-m	Preserve mtime" \
     "\n	-v	Verbose" \
     "\n	-u	Overwrite" \
     "\n	-F FILE	Input (-t,-i,-p) or output (-o) file" \
     "\n	-R USER[:GRP]	Set owner of created files" \
     "\n	-L	Dereference symlinks" \
     "\n	-0	Input is separated by NULs" \

#define dpkg_trivial_usage \
       "[-ilCPru] [-F OPT] PACKAGE" \

#define dpkg_full_usage "\n\n" \
       "Install, remove and manage Debian packages\n" \
	IF_LONG_OPTS( \
     "\n	-i,--install	Install the package" \
     "\n	-l,--list	List of installed packages" \
     "\n	--configure	Configure an unpackaged package" \
     "\n	-P,--purge	Purge all files of a package" \
     "\n	-r,--remove	Remove all but the configuration files for a package" \
     "\n	--unpack	Unpack a package, but don't configure it" \
     "\n	--force-depends	Ignore dependency problems" \
     "\n	--force-confnew	Overwrite existing config files when installing" \
     "\n	--force-confold	Keep old config files when installing" \
	) \
	IF_NOT_LONG_OPTS( \
     "\n	-i		Install the package" \
     "\n	-l		List of installed packages" \
     "\n	-C		Configure an unpackaged package" \
     "\n	-P		Purge all files of a package" \
     "\n	-r		Remove all but the configuration files for a package" \
     "\n	-u		Unpack a package, but don't configure it" \
     "\n	-F depends	Ignore dependency problems" \
     "\n	-F confnew	Overwrite existing config files when installing" \
     "\n	-F confold	Keep old config files when installing" \
	) \

#define dpkg_deb_trivial_usage \
       "[-cefxX] FILE [DIR]" \

#define dpkg_deb_full_usage "\n\n" \
       "Perform actions on Debian packages (.deb)\n" \
     "\n	-c	List files" \
     "\n	-f	Print control fields" \
     "\n	-e	Extract control files to DIR (default: ./DEBIAN)" \
     "\n	-x	Extract files to DIR (no default)" \
     "\n	-X	Verbose -x" \

#define dpkg_deb_example_usage \
       "$ dpkg-deb -X ./busybox_0.48-1_i386.deb /tmp\n" \

#define gzip_trivial_usage \
       "[-cfk" IF_FEATURE_GZIP_DECOMPRESS("dt") IF_FEATURE_GZIP_LEVELS("123456789") "] [FILE]..." \

#define gzip_full_usage "\n\n" \
       "Compress FILEs (or stdin)\n" \
	IF_FEATURE_GZIP_LEVELS( \
     "\n	-1..9	Compression level" \
	) \
	IF_FEATURE_GZIP_DECOMPRESS( \
     "\n	-d	Decompress" \
     "\n	-t	Test file integrity" \
	) \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-k	Keep input files" \

#define gzip_example_usage \
       "$ ls -la /tmp/busybox*\n" \
       "-rw-rw-r--    1 andersen andersen  1761280 Apr 14 17:47 /tmp/busybox.tar\n" \
       "$ gzip /tmp/busybox.tar\n" \
       "$ ls -la /tmp/busybox*\n" \
       "-rw-rw-r--    1 andersen andersen   554058 Apr 14 17:49 /tmp/busybox.tar.gz\n" \

#define lzop_trivial_usage \
       "[-cfUvd123456789CF] [FILE]..." \

#define lzop_full_usage "\n\n" \
       "	-1..9	Compression level" \
     "\n	-d	Decompress" \
     "\n	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-U	Delete input files" \
     "\n	-v	Verbose" \
     "\n	-F	Don't store or verify checksum" \
     "\n	-C	Also write checksum of compressed block" \

#define lzopcat_trivial_usage \
       "[-vF] [FILE]..." \

#define lzopcat_full_usage "\n\n" \
       "	-v	Verbose" \
     "\n	-F	Don't verify checksum" \

#define unlzop_trivial_usage \
       "[-cfUvF] [FILE]..." \

#define unlzop_full_usage "\n\n" \
       "	-c	Write to stdout" \
     "\n	-f	Force" \
     "\n	-U	Delete input files" \
     "\n	-v	Verbose" \
     "\n	-F	Don't verify checksum" \

#define rpm_trivial_usage \
       "-i PACKAGE.rpm; rpm -qp[ildc] PACKAGE.rpm" \

#define rpm_full_usage "\n\n" \
       "Manipulate RPM packages\n" \
     "\nCommands:" \
     "\n	-i	Install package" \
     "\n	-qp	Query package" \
     "\n	-qpi	Show information" \
     "\n	-qpl	List contents" \
     "\n	-qpd	List documents" \
     "\n	-qpc	List config files" \

#define rpm2cpio_trivial_usage \
       "PACKAGE.rpm" \

#define rpm2cpio_full_usage "\n\n" \
       "Output a cpio archive of the rpm file" \

#define tar_trivial_usage \
	IF_FEATURE_TAR_CREATE("c|") "x|t [-" \
	IF_FEATURE_SEAMLESS_Z("Z") \
	IF_FEATURE_SEAMLESS_GZ("z") \
	IF_FEATURE_SEAMLESS_XZ("J") \
	IF_FEATURE_SEAMLESS_BZ2("j") \
	IF_FEATURE_SEAMLESS_LZMA("a") \
	IF_FEATURE_TAR_CREATE("h") \
	IF_FEATURE_TAR_NOPRESERVE_TIME("m") \
	"vokO] " \
	"[-f TARFILE] [-C DIR] " \
	IF_FEATURE_TAR_FROM("[-T FILE] [-X FILE] "IF_FEATURE_TAR_LONG_OPTIONS("[--exclude PATTERN]... ")) \
	"[FILE]..." \

#define tar_full_usage "\n\n" \
	IF_FEATURE_TAR_CREATE("Create, extract, ") \
	IF_NOT_FEATURE_TAR_CREATE("Extract ") \
	"or list files from a tar file" \
     "\n" \
	IF_FEATURE_TAR_CREATE( \
     "\n	c	Create" \
	) \
     "\n	x	Extract" \
     "\n	t	List" \
     "\n	-f FILE	Name of TARFILE ('-' for stdin/out)" \
     "\n	-C DIR	Change to DIR before operation" \
     "\n	-v	Verbose" \
     "\n	-O	Extract to stdout" \
	IF_FEATURE_TAR_NOPRESERVE_TIME( \
     "\n	-m	Don't restore mtime" \
	) \
     "\n	-o	Don't restore user:group" \
     "\n	-k	Don't replace existing files" \
	IF_FEATURE_SEAMLESS_Z( \
     "\n	-Z	(De)compress using compress" \
	) \
	IF_FEATURE_SEAMLESS_GZ( \
     "\n	-z	(De)compress using gzip" \
	) \
	IF_FEATURE_SEAMLESS_XZ( \
     "\n	-J	(De)compress using xz" \
	) \
	IF_FEATURE_SEAMLESS_BZ2( \
     "\n	-j	(De)compress using bzip2" \
	) \
	IF_FEATURE_SEAMLESS_LZMA( \
     "\n	-a	(De)compress using lzma" \
	) \
	IF_FEATURE_TAR_CREATE( \
     "\n	-h	Follow symlinks" \
	) \
	IF_FEATURE_TAR_FROM( \
     "\n	-T FILE	File with names to include" \
     "\n	-X FILE	File with glob patterns to exclude" \
	IF_FEATURE_TAR_LONG_OPTIONS( \
     "\n	--exclude PATTERN	Glob pattern to exclude" \
	) \
	) \

#define tar_example_usage \
       "$ zcat /tmp/tarball.tar.gz | tar -xf -\n" \
       "$ tar -cf /tmp/tarball.tar /usr/local\n" \

#define unzip_trivial_usage \
       "[-lnojpq] FILE[.zip] [FILE]... [-x FILE...] [-d DIR]" \

#define unzip_full_usage "\n\n" \
       "Extract FILEs from ZIP archive\n" \
     "\n	-l	List contents (with -q for short form)" \
     "\n	-n	Never overwrite files (default: ask)" \
     "\n	-o	Overwrite" \
     "\n	-j	Do not restore paths" \
     "\n	-p	Print to stdout" \
     "\n	-q	Quiet" \
     "\n	-x FILE	Exclude FILEs" \
     "\n	-d DIR	Extract into DIR" \

#define chvt_trivial_usage \
       "N" \

#define chvt_full_usage "\n\n" \
       "Change the foreground virtual terminal to /dev/ttyN" \

#define clear_trivial_usage \
       "" \

#define clear_full_usage "\n\n" \
       "Clear screen" \

#define deallocvt_trivial_usage \
       "[N]" \

#define deallocvt_full_usage "\n\n" \
       "Deallocate unused virtual terminal /dev/ttyN" \

#define dumpkmap_trivial_usage \
       "> keymap" \

#define dumpkmap_full_usage "\n\n" \
       "Print a binary keyboard translation table to stdout" \

#define dumpkmap_example_usage \
       "$ dumpkmap > keymap\n" \

#define fgconsole_trivial_usage \
	"" \

#define fgconsole_full_usage "\n\n" \
	"Get active console" \

#define kbd_mode_trivial_usage \
       "[-a|k|s|u] [-C TTY]" \

#define kbd_mode_full_usage "\n\n" \
       "Report or set VT console keyboard mode\n" \
     "\n	-a	Default (ASCII)" \
     "\n	-k	Medium-raw (keycode)" \
     "\n	-s	Raw (scancode)" \
     "\n	-u	Unicode (utf-8)" \
     "\n	-C TTY	Affect TTY" \

#define loadfont_trivial_usage \
       "< font" \

#define loadfont_full_usage "\n\n" \
       "Load a console font from stdin" \

#define loadfont_example_usage \
       "$ loadfont < /etc/i18n/fontname\n" \

#define setfont_trivial_usage \
       "FONT [-m MAPFILE] [-C TTY]" \

#define setfont_full_usage "\n\n" \
       "Load a console font\n" \
     "\n	-m MAPFILE	Load console screen map" \
     "\n	-C TTY		Affect TTY instead of /dev/tty" \

#define setfont_example_usage \
       "$ setfont -m koi8-r /etc/i18n/fontname\n" \

#define loadkmap_trivial_usage \
       "< keymap" \

#define loadkmap_full_usage "\n\n" \
       "Load a binary keyboard translation table from stdin" \

#define loadkmap_example_usage \
       "$ loadkmap < /etc/i18n/lang-keymap\n" \

#define openvt_trivial_usage \
       "[-c N] [-sw] [PROG ARGS]" \

#define openvt_full_usage "\n\n" \
       "Start PROG on a new virtual terminal\n" \
     "\n	-c N	Use specified VT" \
     "\n	-s	Switch to the VT" \
     "\n	-w	Wait for PROG to exit" \

#define openvt_example_usage \
       "openvt 2 /bin/ash\n" \

#define reset_trivial_usage \
       "" \

#define reset_full_usage "\n\n" \
       "Reset the screen" \

#define resize_trivial_usage \
       "" \

#define resize_full_usage "\n\n" \
       "Resize the screen" \

#define setconsole_trivial_usage \
       "[-r] [DEVICE]" \

#define setconsole_full_usage "\n\n" \
       "Make writes to /dev/console appear on DEVICE (default: /dev/tty)." \
   "\n""Does not redirect kernel log output or reads from /dev/console." \
   "\n" \
   "\n""	-r	Reset: writes to /dev/console go to kernel log tty(s)" \

#define setkeycodes_trivial_usage \
       "{ SCANCODE KEYCODE }..." \

#define setkeycodes_full_usage "\n\n" \
       "Modify kernel's scancode-to-keycode map,\n" \
       "allowing unusual keyboards to generate usable keycodes.\n\n" \
       "SCANCODE is either xx or e0xx (hexadecimal), KEYCODE is decimal." \

#define setkeycodes_example_usage \
       "$ setkeycodes e030 127\n" \

#define setlogcons_trivial_usage \
       "[N]" \

#define setlogcons_full_usage "\n\n" \
       "Pin kernel output to VT console N. Default:0 (do not pin)" \

#define showkey_trivial_usage \
       "[-a | -k | -s]" \

#define showkey_full_usage "\n\n" \
       "Show keys pressed\n" \
     "\n	-a	Display decimal/octal/hex values of the keys" \
     "\n	-k	Display interpreted keycodes (default)" \
     "\n	-s	Display raw scan-codes" \

#define basename_trivial_usage \
       "FILE [SUFFIX]" \

#define basename_full_usage "\n\n" \
       "Strip directory path and .SUFFIX from FILE" \

#define basename_example_usage \
       "$ basename /usr/local/bin/foo\n" \
       "foo\n" \
       "$ basename /usr/local/bin/\n" \
       "bin\n" \
       "$ basename /foo/bar.txt .txt\n" \
       "bar" \

#if ENABLE_FEATURE_CATN || ENABLE_FEATURE_CATV \

#define cat_trivial_usage \
       "[-" IF_FEATURE_CATN("nb") IF_FEATURE_CATV("vteA") "] [FILE]..." \

#else \

#define cat_trivial_usage \
       "[FILE]..." \

#endif \

#define cat_full_usage "\n\n" \
       "Print FILEs to stdout\n" \
	IF_FEATURE_CATN( \
     "\n	-n	Number output lines" \
     "\n	-b	Number nonempty lines" \
	) \
	IF_FEATURE_CATV( \
     "\n	-v	Show nonprinting characters as ^x or M-x" \
     "\n	-t	...and tabs as ^I" \
     "\n	-e	...and end lines with $" \
     "\n	-A	Same as -vte" \
	) \

#define cat_example_usage \
       "$ cat /proc/uptime\n" \
       "110716.72 17.67" \

#define chgrp_trivial_usage \
       "[-RhLHP"IF_DESKTOP("cvf")"]... GROUP FILE..." \

#define chgrp_full_usage "\n\n" \
       "Change the group membership of each FILE to GROUP\n" \
     "\n	-R	Recurse" \
     "\n	-h	Affect symlinks instead of symlink targets" \
     "\n	-L	Traverse all symlinks to directories" \
     "\n	-H	Traverse symlinks on command line only" \
     "\n	-P	Don't traverse symlinks (default)" \
	IF_DESKTOP( \
     "\n	-c	List changed files" \
     "\n	-v	Verbose" \
     "\n	-f	Hide errors" \
	) \

#define chgrp_example_usage \
       "$ ls -l /tmp/foo\n" \
       "-r--r--r--    1 andersen andersen        0 Apr 12 18:25 /tmp/foo\n" \
       "$ chgrp root /tmp/foo\n" \
       "$ ls -l /tmp/foo\n" \
       "-r--r--r--    1 andersen root            0 Apr 12 18:25 /tmp/foo\n" \

#define chmod_trivial_usage \
       "[-R"IF_DESKTOP("cvf")"] MODE[,MODE]... FILE..." \

#define chmod_full_usage "\n\n" \
       "Each MODE is one or more of the letters ugoa, one of the\n" \
       "symbols +-= and one or more of the letters rwxst\n" \
     "\n	-R	Recurse" \
	IF_DESKTOP( \
     "\n	-c	List changed files" \
     "\n	-v	List all files" \
     "\n	-f	Hide errors" \
	) \

#define chmod_example_usage \
       "$ ls -l /tmp/foo\n" \
       "-rw-rw-r--    1 root     root            0 Apr 12 18:25 /tmp/foo\n" \
       "$ chmod u+x /tmp/foo\n" \
       "$ ls -l /tmp/foo\n" \
       "-rwxrw-r--    1 root     root            0 Apr 12 18:25 /tmp/foo*\n" \
       "$ chmod 444 /tmp/foo\n" \
       "$ ls -l /tmp/foo\n" \
       "-r--r--r--    1 root     root            0 Apr 12 18:25 /tmp/foo\n" \

#define chown_trivial_usage \
       "[-Rh"IF_DESKTOP("LHPcvf")"]... USER[:[GRP]] FILE..." \

#define chown_full_usage "\n\n" \
       "Change the owner and/or group of each FILE to USER and/or GRP\n" \
     "\n	-R	Recurse" \
     "\n	-h	Affect symlinks instead of symlink targets" \
	IF_DESKTOP( \
     "\n	-L	Traverse all symlinks to directories" \
     "\n	-H	Traverse symlinks on command line only" \
     "\n	-P	Don't traverse symlinks (default)" \
     "\n	-c	List changed files" \
     "\n	-v	List all files" \
     "\n	-f	Hide errors" \
	) \

#define chown_example_usage \
       "$ ls -l /tmp/foo\n" \
       "-r--r--r--    1 andersen andersen        0 Apr 12 18:25 /tmp/foo\n" \
       "$ chown root /tmp/foo\n" \
       "$ ls -l /tmp/foo\n" \
       "-r--r--r--    1 root     andersen        0 Apr 12 18:25 /tmp/foo\n" \
       "$ chown root.root /tmp/foo\n" \
       "ls -l /tmp/foo\n" \
       "-r--r--r--    1 root     root            0 Apr 12 18:25 /tmp/foo\n" \

#define chroot_trivial_usage \
       "NEWROOT [PROG ARGS]" \

#define chroot_full_usage "\n\n" \
       "Run PROG with root directory set to NEWROOT" \

#define chroot_example_usage \
       "$ ls -l /bin/ls\n" \
       "lrwxrwxrwx    1 root     root          12 Apr 13 00:46 /bin/ls -> /BusyBox\n" \
       "# mount /dev/hdc1 /mnt -t minix\n" \
       "# chroot /mnt\n" \
       "# ls -l /bin/ls\n" \
       "-rwxr-xr-x    1 root     root        40816 Feb  5 07:45 /bin/ls*\n" \

#define cksum_trivial_usage \
       "FILE..." \

#define cksum_full_usage "\n\n" \
       "Calculate the CRC32 checksums of FILEs" \

#define comm_trivial_usage \
       "[-123] FILE1 FILE2" \

#define comm_full_usage "\n\n" \
       "Compare FILE1 with FILE2\n" \
     "\n	-1	Suppress lines unique to FILE1" \
     "\n	-2	Suppress lines unique to FILE2" \
     "\n	-3	Suppress lines common to both files" \

#define cp_trivial_usage \
       "[OPTIONS] SOURCE... DEST" \

#define cp_full_usage "\n\n" \
       "Copy SOURCE(s) to DEST\n" \
     "\n	-a	Same as -dpR" \
	IF_SELINUX( \
     "\n	-c	Preserve security context" \
	) \
     "\n	-R,-r	Recurse" \
     "\n	-d,-P	Preserve symlinks (default if -R)" \
     "\n	-L	Follow all symlinks" \
     "\n	-H	Follow symlinks on command line" \
     "\n	-p	Preserve file attributes if possible" \
     "\n	-f	Overwrite" \
     "\n	-i	Prompt before overwrite" \
     "\n	-l,-s	Create (sym)links" \
     "\n	-T	Treat DEST as a normal file" \
     "\n	-u	Copy only newer files" \

#define cut_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define cut_full_usage "\n\n" \
       "Print selected fields from each input FILE to stdout\n" \
     "\n	-b LIST	Output only bytes from LIST" \
     "\n	-c LIST	Output only characters from LIST" \
     "\n	-d CHAR	Use CHAR instead of tab as the field delimiter" \
     "\n	-s	Output only the lines containing delimiter" \
     "\n	-f N	Print only these fields" \
     "\n	-n	Ignored" \

#define cut_example_usage \
       "$ echo \"Hello world\" | cut -f 1 -d ' '\n" \
       "Hello\n" \
       "$ echo \"Hello world\" | cut -f 2 -d ' '\n" \
       "world\n" \

#define date_trivial_usage \
       "[OPTIONS] [+FMT] [TIME]" \

#define date_full_usage "\n\n" \
       "Display time (using +FMT), or set time\n" \
	IF_NOT_LONG_OPTS( \
     "\n	[-s] TIME	Set time to TIME" \
     "\n	-u		Work in UTC (don't convert to local time)" \
     "\n	-R		Output RFC-2822 compliant date string" \
	) IF_LONG_OPTS( \
     "\n	[-s,--set] TIME	Set time to TIME" \
     "\n	-u,--utc	Work in UTC (don't convert to local time)" \
     "\n	-R,--rfc-2822	Output RFC-2822 compliant date string" \
	) \
	IF_FEATURE_DATE_ISOFMT( \
     "\n	-I[SPEC]	Output ISO-8601 compliant date string" \
     "\n			SPEC='date' (default) for date only," \
     "\n			'hours', 'minutes', or 'seconds' for date and" \
     "\n			time to the indicated precision" \
	) \
	IF_NOT_LONG_OPTS( \
     "\n	-r FILE		Display last modification time of FILE" \
     "\n	-d TIME		Display TIME, not 'now'" \
	) IF_LONG_OPTS( \
     "\n	-r,--reference FILE	Display last modification time of FILE" \
     "\n	-d,--date TIME	Display TIME, not 'now'" \
	) \
	IF_FEATURE_DATE_ISOFMT( \
     "\n	-D FMT		Use FMT for -d TIME conversion" \
	) \
     "\n" \
     "\nRecognized TIME formats:" \
     "\n	hh:mm[:ss]" \
     "\n	[YYYY.]MM.DD-hh:mm[:ss]" \
     "\n	YYYY-MM-DD hh:mm[:ss]" \
     "\n	[[[[[YY]YY]MM]DD]hh]mm[.ss]" \
	IF_FEATURE_DATE_COMPAT( \
     "\n	'date TIME' form accepts MMDDhhmm[[YY]YY][.ss] instead" \
	) \

#define date_example_usage \
       "$ date\n" \
       "Wed Apr 12 18:52:41 MDT 2000\n" \

#define dd_trivial_usage \
       "[if=FILE] [of=FILE] [" IF_FEATURE_DD_IBS_OBS("ibs=N obs=N/") "bs=N] [count=N] [skip=N] [seek=N]\n" \
	IF_FEATURE_DD_IBS_OBS( \
       "	[conv=notrunc|noerror|sync|fsync]\n" \
       "	[iflag=skip_bytes|fullblock] [oflag=seek_bytes]" \
	) \

#define dd_full_usage "\n\n" \
       "Copy a file with converting and formatting\n" \
     "\n	if=FILE		Read from FILE instead of stdin" \
     "\n	of=FILE		Write to FILE instead of stdout" \
     "\n	bs=N		Read and write N bytes at a time" \
	IF_FEATURE_DD_IBS_OBS( \
     "\n	ibs=N		Read N bytes at a time" \
	) \
	IF_FEATURE_DD_IBS_OBS( \
     "\n	obs=N		Write N bytes at a time" \
	) \
     "\n	count=N		Copy only N input blocks" \
     "\n	skip=N		Skip N input blocks" \
     "\n	seek=N		Skip N output blocks" \
	IF_FEATURE_DD_IBS_OBS( \
     "\n	conv=notrunc	Don't truncate output file" \
     "\n	conv=noerror	Continue after read errors" \
     "\n	conv=sync	Pad blocks with zeros" \
     "\n	conv=fsync	Physically write data out before finishing" \
     "\n	conv=swab	Swap every pair of bytes" \
     "\n	iflag=skip_bytes	skip=N is in bytes" \
     "\n	iflag=fullblock	Read full blocks" \
     "\n	oflag=seek_bytes	seek=N is in bytes" \
	) \
	IF_FEATURE_DD_STATUS( \
     "\n	status=noxfer	Suppress rate output" \
     "\n	status=none	Suppress all output" \
	) \
     "\n" \
     "\nN may be suffixed by c (1), w (2), b (512), kB (1000), k (1024), MB, M, GB, G" \

#define dd_example_usage \
       "$ dd if=/dev/zero of=/dev/ram1 bs=1M count=4\n" \
       "4+0 records in\n" \
       "4+0 records out\n" \

#define df_trivial_usage \
	"[-Pk" \
	IF_FEATURE_HUMAN_READABLE("mh") \
	"T" \
	IF_FEATURE_DF_FANCY("ai] [-B SIZE") \
	"] [FILESYSTEM]..." \

#define df_full_usage "\n\n" \
       "Print filesystem usage statistics\n" \
     "\n	-P	POSIX output format" \
     "\n	-k	1024-byte blocks (default)" \
	IF_FEATURE_HUMAN_READABLE( \
     "\n	-m	1M-byte blocks" \
     "\n	-h	Human readable (e.g. 1K 243M 2G)" \
	) \
     "\n	-T	Print filesystem type" \
	IF_FEATURE_DF_FANCY( \
     "\n	-a	Show all filesystems" \
     "\n	-i	Inodes" \
     "\n	-B SIZE	Blocksize" \
	) \

#define df_example_usage \
       "$ df\n" \
       "Filesystem           1K-blocks      Used Available Use% Mounted on\n" \
       "/dev/sda3              8690864   8553540    137324  98% /\n" \
       "/dev/sda1                64216     36364     27852  57% /boot\n" \
       "$ df /dev/sda3\n" \
       "Filesystem           1K-blocks      Used Available Use% Mounted on\n" \
       "/dev/sda3              8690864   8553540    137324  98% /\n" \
       "$ POSIXLY_CORRECT=sure df /dev/sda3\n" \
       "Filesystem         512B-blocks      Used Available Use% Mounted on\n" \
       "/dev/sda3             17381728  17107080    274648  98% /\n" \
       "$ POSIXLY_CORRECT=yep df -P /dev/sda3\n" \
       "Filesystem          512-blocks      Used Available Capacity Mounted on\n" \
       "/dev/sda3             17381728  17107080    274648      98% /\n" \

#define dirname_trivial_usage \
       "FILENAME" \

#define dirname_full_usage "\n\n" \
       "Strip non-directory suffix from FILENAME" \

#define dirname_example_usage \
       "$ dirname /tmp/foo\n" \
       "/tmp\n" \
       "$ dirname /tmp/foo/\n" \
       "/tmp\n" \

#define dos2unix_trivial_usage \
       "[-ud] [FILE]" \

#define dos2unix_full_usage "\n\n" \
       "Convert FILE in-place from DOS to Unix format.\n" \
       "When no file is given, use stdin/stdout.\n" \
     "\n	-u	dos2unix" \
     "\n	-d	unix2dos" \

#define unix2dos_trivial_usage \
       "[-ud] [FILE]" \

#define unix2dos_full_usage "\n\n" \
       "Convert FILE in-place from Unix to DOS format.\n" \
       "When no file is given, use stdin/stdout.\n" \
     "\n	-u	dos2unix" \
     "\n	-d	unix2dos" \

#define du_trivial_usage \
       "[-aHLdclsx" IF_FEATURE_HUMAN_READABLE("hm") "k] [FILE]..." \

#define du_full_usage "\n\n" \
       "Summarize disk space used for each FILE and/or directory\n" \
     "\n	-a	Show file sizes too" \
     "\n	-L	Follow all symlinks" \
     "\n	-H	Follow symlinks on command line" \
     "\n	-d N	Limit output to directories (and files with -a) of depth < N" \
     "\n	-c	Show grand total" \
     "\n	-l	Count sizes many times if hard linked" \
     "\n	-s	Display only a total for each argument" \
     "\n	-x	Skip directories on different filesystems" \
	IF_FEATURE_HUMAN_READABLE( \
     "\n	-h	Sizes in human readable format (e.g., 1K 243M 2G)" \
     "\n	-m	Sizes in megabytes" \
	) \
     "\n	-k	Sizes in kilobytes" IF_FEATURE_DU_DEFAULT_BLOCKSIZE_1K(" (default)") \
	IF_NOT_FEATURE_DU_DEFAULT_BLOCKSIZE_1K( \
     "\n		Default unit is 512 bytes" \
	) \

#define du_example_usage \
       "$ du\n" \
       "16      ./CVS\n" \
       "12      ./kernel-patches/CVS\n" \
       "80      ./kernel-patches\n" \
       "12      ./tests/CVS\n" \
       "36      ./tests\n" \
       "12      ./scripts/CVS\n" \
       "16      ./scripts\n" \
       "12      ./docs/CVS\n" \
       "104     ./docs\n" \
       "2417    .\n" \

#define echo_trivial_usage \
	IF_FEATURE_FANCY_ECHO("[-neE] ") "[ARG]..." \

#define echo_full_usage "\n\n" \
       "Print the specified ARGs to stdout" \
	IF_FEATURE_FANCY_ECHO( "\n" \
     "\n	-n	Suppress trailing newline" \
     "\n	-e	Interpret backslash escapes (i.e., \\t=tab)" \
     "\n	-E	Don't interpret backslash escapes (default)" \
	) \

#define echo_example_usage \
       "$ echo \"Erik is cool\"\n" \
       "Erik is cool\n" \
	IF_FEATURE_FANCY_ECHO("$ echo -e \"Erik\\nis\\ncool\"\n" \
       "Erik\n" \
       "is\n" \
       "cool\n" \
       "$ echo \"Erik\\nis\\ncool\"\n" \
       "Erik\\nis\\ncool\n") \

#define env_trivial_usage \
       "[-iu] [-] [name=value]... [PROG ARGS]" \

#define env_full_usage "\n\n" \
       "Print the current environment or run PROG after setting up\n" \
       "the specified environment\n" \
     "\n	-, -i	Start with an empty environment" \
     "\n	-u	Remove variable from the environment" \

#define expand_trivial_usage \
       "[-i] [-t N] [FILE]..." \

#define expand_full_usage "\n\n" \
       "Convert tabs to spaces, writing to stdout\n" \
     "\n	-i	Don't convert tabs after non blanks" \
     "\n	-t	Tabstops every N chars" \

#define unexpand_trivial_usage \
       "[-fa][-t N] [FILE]..." \

#define unexpand_full_usage "\n\n" \
       "Convert spaces to tabs, writing to stdout\n" \
     "\n	-a	Convert all blanks" \
     "\n	-f	Convert only leading blanks" \
     "\n	-t N	Tabstops every N chars" \

#define expr_trivial_usage \
       "EXPRESSION" \

#define expr_full_usage "\n\n" \
       "Print the value of EXPRESSION to stdout\n" \
    "\n" \
       "EXPRESSION may be:\n" \
       "	ARG1 | ARG2	ARG1 if it is neither null nor 0, otherwise ARG2\n" \
       "	ARG1 & ARG2	ARG1 if neither argument is null or 0, otherwise 0\n" \
       "	ARG1 < ARG2	1 if ARG1 is less than ARG2, else 0. Similarly:\n" \
       "	ARG1 <= ARG2\n" \
       "	ARG1 = ARG2\n" \
       "	ARG1 != ARG2\n" \
       "	ARG1 >= ARG2\n" \
       "	ARG1 > ARG2\n" \
       "	ARG1 + ARG2	Sum of ARG1 and ARG2. Similarly:\n" \
       "	ARG1 - ARG2\n" \
       "	ARG1 * ARG2\n" \
       "	ARG1 / ARG2\n" \
       "	ARG1 % ARG2\n" \
       "	STRING : REGEXP		Anchored pattern match of REGEXP in STRING\n" \
       "	match STRING REGEXP	Same as STRING : REGEXP\n" \
       "	substr STRING POS LENGTH Substring of STRING, POS counted from 1\n" \
       "	index STRING CHARS	Index in STRING where any CHARS is found, or 0\n" \
       "	length STRING		Length of STRING\n" \
       "	quote TOKEN		Interpret TOKEN as a string, even if\n" \
       "				it is a keyword like 'match' or an\n" \
       "				operator like '/'\n" \
       "	(EXPRESSION)		Value of EXPRESSION\n" \
       "\n" \
       "Beware that many operators need to be escaped or quoted for shells.\n" \
       "Comparisons are arithmetic if both ARGs are numbers, else\n" \
       "lexicographical. Pattern matches return the string matched between\n" \
       "\\( and \\) or null; if \\( and \\) are not used, they return the number\n" \
       "of characters matched or 0." \

#define factor_trivial_usage \
       "[NUMBER]..." \

#define factor_full_usage "\n\n" \
       "Print prime factors" \

#define false_trivial_usage NOUSAGE_STR \

#define false_full_usage "" \

#define false_example_usage \
       "$ false\n" \
       "$ echo $?\n" \
       "1\n" \

#define fold_trivial_usage \
       "[-bs] [-w WIDTH] [FILE]..." \

#define fold_full_usage "\n\n" \
       "Wrap input lines in each FILE (or stdin), writing to stdout\n" \
     "\n	-b	Count bytes rather than columns" \
     "\n	-s	Break at spaces" \
     "\n	-w	Use WIDTH columns instead of 80" \

#define fsync_trivial_usage \
       "[-d] FILE..." \

#define fsync_full_usage "\n\n" \
       "Write files' buffered blocks to disk\n" \
     "\n	-d	Avoid syncing metadata" \

#define head_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define head_full_usage "\n\n" \
       "Print first 10 lines of each FILE (or stdin) to stdout.\n" \
       "With more than one FILE, precede each with a filename header.\n" \
     "\n	-n N[kbm]	Print first N lines" \
	IF_FEATURE_FANCY_HEAD( \
     "\n	-n -N[kbm]	Print all except N last lines" \
     "\n	-c [-]N[kbm]	Print first N bytes" \
     "\n	-q		Never print headers" \
     "\n	-v		Always print headers" \
	) \
     "\n" \
     "\nN may be suffixed by k (x1024), b (x512), or m (x1024^2)." \

#define head_example_usage \
       "$ head -n 2 /etc/passwd\n" \
       "root:x:0:0:root:/root:/bin/bash\n" \
       "daemon:x:1:1:daemon:/usr/sbin:/bin/sh\n" \

#define hostid_trivial_usage \
       "" \

#define hostid_full_usage "\n\n" \
       "Print out a unique 32-bit identifier for the machine" \

#define id_trivial_usage \
       "[OPTIONS] [USER]" \

#define id_full_usage "\n\n" \
       "Print information about USER or the current user\n" \
	IF_SELINUX( \
     "\n	-Z	Security context" \
	) \
     "\n	-u	User ID" \
     "\n	-g	Group ID" \
     "\n	-G	Supplementary group IDs" \
     "\n	-n	Print names instead of numbers" \
     "\n	-r	Print real ID instead of effective ID" \

#define id_example_usage \
       "$ id\n" \
       "uid=1000(andersen) gid=1000(andersen)\n" \

#define groups_trivial_usage \
       "[USER]" \

#define groups_full_usage "\n\n" \
       "Print the group memberships of USER or for the current process" \

#define groups_example_usage \
       "$ groups\n" \
       "andersen lp dialout cdrom floppy\n" \

#define install_trivial_usage \
	"[-cdDsp] [-o USER] [-g GRP] [-m MODE] [-t DIR] [SOURCE]... DEST" \

#define install_full_usage "\n\n" \
       "Copy files and set attributes\n" \
     "\n	-c	Just copy (default)" \
     "\n	-d	Create directories" \
     "\n	-D	Create leading target directories" \
     "\n	-s	Strip symbol table" \
     "\n	-p	Preserve date" \
     "\n	-o USER	Set ownership" \
     "\n	-g GRP	Set group ownership" \
     "\n	-m MODE	Set permissions" \
     "\n	-t DIR	Install to DIR" \
	IF_SELINUX( \
     "\n	-Z	Set security context" \
	) \

#define link_trivial_usage \
       "FILE LINK" \

#define link_full_usage "\n\n" \
       "Create hard LINK to FILE" \

#define ln_trivial_usage \
       "[OPTIONS] TARGET... LINK|DIR" \

#define ln_full_usage "\n\n" \
       "Create a link LINK or DIR/TARGET to the specified TARGET(s)\n" \
     "\n	-s	Make symlinks instead of hardlinks" \
     "\n	-f	Remove existing destinations" \
     "\n	-n	Don't dereference symlinks - treat like normal file" \
     "\n	-b	Make a backup of the target (if exists) before link operation" \
     "\n	-S suf	Use suffix instead of ~ when making backup files" \
     "\n	-T	2nd arg must be a DIR" \
     "\n	-v	Verbose" \

#define ln_example_usage \
       "$ ln -s BusyBox /tmp/ls\n" \
       "$ ls -l /tmp/ls\n" \
       "lrwxrwxrwx    1 root     root            7 Apr 12 18:39 ls -> BusyBox*\n" \

#define logname_trivial_usage \
       "" \

#define logname_full_usage "\n\n" \
       "Print the name of the current user" \

#define logname_example_usage \
       "$ logname\n" \
       "root\n" \

#define ls_trivial_usage \
	"[-1AaCxd" \
	IF_FEATURE_LS_FOLLOWLINKS("LH") \
	IF_FEATURE_LS_RECURSIVE("R") \
	IF_FEATURE_LS_FILETYPES("Fp") "lins" \
	IF_FEATURE_HUMAN_READABLE("h") \
	IF_FEATURE_LS_SORTFILES("rSXv") \
	IF_FEATURE_LS_TIMESTAMPS("ctu") \
	IF_SELINUX("kZ") "]" \
	IF_FEATURE_LS_WIDTH(" [-w WIDTH]") " [FILE]..." \

#define ls_full_usage "\n\n" \
       "List directory contents\n" \
     "\n	-1	One column output" \
     "\n	-a	Include entries which start with ." \
     "\n	-A	Like -a, but exclude . and .." \
     "\n	-x	List by lines" \
     "\n	-d	List directory entries instead of contents" \
	IF_FEATURE_LS_FOLLOWLINKS( \
     "\n	-L	Follow symlinks" \
     "\n	-H	Follow symlinks on command line" \
	) \
	IF_FEATURE_LS_RECURSIVE( \
     "\n	-R	Recurse" \
	) \
	IF_FEATURE_LS_FILETYPES( \
     "\n	-p	Append / to dir entries" \
     "\n	-F	Append indicator (one of */=@|) to entries" \
	) \
     "\n	-l	Long listing format" \
     "\n	-i	List inode numbers" \
     "\n	-n	List numeric UIDs and GIDs instead of names" \
     "\n	-s	List allocated blocks" \
	IF_FEATURE_LS_TIMESTAMPS( \
     "\n	-lc	List ctime" \
     "\n	-lu	List atime" \
	) \
	IF_FEATURE_LS_TIMESTAMPS(IF_LONG_OPTS( \
     "\n	--full-time	List full date and time" \
	)) \
	IF_FEATURE_HUMAN_READABLE( \
     "\n	-h	Human readable sizes (1K 243M 2G)" \
	) \
	IF_FEATURE_LS_SORTFILES( \
	IF_LONG_OPTS( \
     "\n	--group-directories-first" \
	) \
     "\n	-S	Sort by size" \
     "\n	-X	Sort by extension" \
     "\n	-v	Sort by version" \
	) \
	IF_FEATURE_LS_TIMESTAMPS( \
     "\n	-t	Sort by mtime" \
     "\n	-tc	Sort by ctime" \
     "\n	-tu	Sort by atime" \
	) \
     "\n	-r	Reverse sort order" \
	IF_SELINUX( \
     "\n	-Z	List security context and permission" \
	) \
	IF_FEATURE_LS_WIDTH( \
     "\n	-w N	Format N columns wide" \
	) \
	IF_FEATURE_LS_COLOR( \
     "\n	--color[={always,never,auto}]	Control coloring" \
	) \

#define md5sum_trivial_usage \
	IF_FEATURE_MD5_SHA1_SUM_CHECK("[-c[sw]] ")"[FILE]..." \

#define md5sum_full_usage "\n\n" \
       "Print" IF_FEATURE_MD5_SHA1_SUM_CHECK(" or check") " MD5 checksums" \
	IF_FEATURE_MD5_SHA1_SUM_CHECK( "\n" \
     "\n	-c	Check sums against list in FILEs" \
     "\n	-s	Don't output anything, status code shows success" \
     "\n	-w	Warn about improperly formatted checksum lines" \
	) \

#define md5sum_example_usage \
       "$ md5sum < busybox\n" \
       "6fd11e98b98a58f64ff3398d7b324003\n" \
       "$ md5sum busybox\n" \
       "6fd11e98b98a58f64ff3398d7b324003  busybox\n" \
       "$ md5sum -c -\n" \
       "6fd11e98b98a58f64ff3398d7b324003  busybox\n" \
       "busybox: OK\n" \
       "^D\n" \

#define sha1sum_trivial_usage \
	IF_FEATURE_MD5_SHA1_SUM_CHECK("[-c[sw]] ")"[FILE]..." \

#define sha1sum_full_usage "\n\n" \
       "Print" IF_FEATURE_MD5_SHA1_SUM_CHECK(" or check") " SHA1 checksums" \
	IF_FEATURE_MD5_SHA1_SUM_CHECK( "\n" \
     "\n	-c	Check sums against list in FILEs" \
     "\n	-s	Don't output anything, status code shows success" \
     "\n	-w	Warn about improperly formatted checksum lines" \
	) \

#define sha256sum_trivial_usage \
	IF_FEATURE_MD5_SHA1_SUM_CHECK("[-c[sw]] ")"[FILE]..." \

#define sha256sum_full_usage "\n\n" \
       "Print" IF_FEATURE_MD5_SHA1_SUM_CHECK(" or check") " SHA256 checksums" \
	IF_FEATURE_MD5_SHA1_SUM_CHECK( "\n" \
     "\n	-c	Check sums against list in FILEs" \
     "\n	-s	Don't output anything, status code shows success" \
     "\n	-w	Warn about improperly formatted checksum lines" \
	) \

#define sha512sum_trivial_usage \
	IF_FEATURE_MD5_SHA1_SUM_CHECK("[-c[sw]] ")"[FILE]..." \

#define sha512sum_full_usage "\n\n" \
       "Print" IF_FEATURE_MD5_SHA1_SUM_CHECK(" or check") " SHA512 checksums" \
	IF_FEATURE_MD5_SHA1_SUM_CHECK( "\n" \
     "\n	-c	Check sums against list in FILEs" \
     "\n	-s	Don't output anything, status code shows success" \
     "\n	-w	Warn about improperly formatted checksum lines" \
	) \

#define sha3sum_trivial_usage \
	IF_FEATURE_MD5_SHA1_SUM_CHECK("[-c[sw]] ")"[-a BITS] [FILE]..." \

#define sha3sum_full_usage "\n\n" \
       "Print" IF_FEATURE_MD5_SHA1_SUM_CHECK(" or check") " SHA3 checksums" \
	IF_FEATURE_MD5_SHA1_SUM_CHECK( "\n" \
     "\n	-c	Check sums against list in FILEs" \
     "\n	-s	Don't output anything, status code shows success" \
     "\n	-w	Warn about improperly formatted checksum lines" \
     "\n	-a BITS	224 (default), 256, 384, 512" \
	) \

#define mkdir_trivial_usage \
       "[OPTIONS] DIRECTORY..." \

#define mkdir_full_usage "\n\n" \
       "Create DIRECTORY\n" \
     "\n	-m MODE	Mode" \
     "\n	-p	No error if exists; make parent directories as needed" \
	IF_SELINUX( \
     "\n	-Z	Set security context" \
	) \

#define mkdir_example_usage \
       "$ mkdir /tmp/foo\n" \
       "$ mkdir /tmp/foo\n" \
       "/tmp/foo: File exists\n" \
       "$ mkdir /tmp/foo/bar/baz\n" \
       "/tmp/foo/bar/baz: No such file or directory\n" \
       "$ mkdir -p /tmp/foo/bar/baz\n" \

#define mkfifo_trivial_usage \
       "[-m MODE] " IF_SELINUX("[-Z] ") "NAME" \

#define mkfifo_full_usage "\n\n" \
       "Create named pipe\n" \
     "\n	-m MODE	Mode (default a=rw)" \
	IF_SELINUX( \
     "\n	-Z	Set security context" \
	) \

#define mknod_trivial_usage \
       "[-m MODE] " IF_SELINUX("[-Z] ") "NAME TYPE [MAJOR MINOR]" \

#define mknod_full_usage "\n\n" \
       "Create a special file (block, character, or pipe)\n" \
     "\n	-m MODE	Creation mode (default a=rw)" \
	IF_SELINUX( \
     "\n	-Z	Set security context" \
	) \
     "\nTYPE:" \
     "\n	b	Block device" \
     "\n	c or u	Character device" \
     "\n	p	Named pipe (MAJOR MINOR must be omitted)" \

#define mknod_example_usage \
       "$ mknod /dev/fd0 b 2 0\n" \
       "$ mknod -m 644 /tmp/pipe p\n" \

#define mktemp_trivial_usage \
       "[-dt] [-p DIR] [TEMPLATE]" \

#define mktemp_full_usage "\n\n" \
       "Create a temporary file with name based on TEMPLATE and print its name.\n" \
       "TEMPLATE must end with XXXXXX (e.g. [/dir/]nameXXXXXX).\n" \
       "Without TEMPLATE, -t tmp.XXXXXX is assumed.\n" \
     "\n	-d	Make directory, not file" \
     "\n	-q	Fail silently on errors" \
     "\n	-t	Prepend base directory name to TEMPLATE" \
     "\n	-p DIR	Use DIR as a base directory (implies -t)" \
     "\n	-u	Do not create anything; print a name" \
     "\n" \
     "\nBase directory is: -p DIR, else $TMPDIR, else /tmp" \

#define mktemp_example_usage \
       "$ mktemp /tmp/temp.XXXXXX\n" \
       "/tmp/temp.mWiLjM\n" \
       "$ ls -la /tmp/temp.mWiLjM\n" \
       "-rw-------    1 andersen andersen        0 Apr 25 17:10 /tmp/temp.mWiLjM\n" \

#define mv_trivial_usage \
       "[-fin] SOURCE DEST\n" \
       "or: mv [-fin] SOURCE... DIRECTORY" \

#define mv_full_usage "\n\n" \
       "Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY\n" \
     "\n	-f	Don't prompt before overwriting" \
     "\n	-i	Interactive, prompt before overwrite" \
     "\n	-n	Don't overwrite an existing file" \

#define mv_example_usage \
       "$ mv /tmp/foo /bin/bar\n" \

#define nice_trivial_usage \
       "[-n ADJUST] [PROG ARGS]" \

#define nice_full_usage "\n\n" \
       "Change scheduling priority, run PROG\n" \
     "\n	-n ADJUST	Adjust priority by ADJUST" \

#define nl_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define nl_full_usage "\n\n" \
       "Write FILEs to standard output with line numbers added\n" \
     "\n	-b STYLE	Which lines to number - a: all, t: nonempty, n: none" \
     "\n	-i N		Line number increment" \
     "\n	-s STRING	Use STRING as line number separator" \
     "\n	-v N		Start from N" \
     "\n	-w N		Width of line numbers" \

#define nohup_trivial_usage \
       "PROG ARGS" \

#define nohup_full_usage "\n\n" \
       "Run PROG immune to hangups, with output to a non-tty" \

#define nohup_example_usage \
       "$ nohup make &" \

#define nproc_trivial_usage \
	""IF_LONG_OPTS("--all --ignore=N") \

#define nproc_full_usage "\n\n" \
	"Print number of available CPUs" \
	IF_LONG_OPTS( \
     "\n" \
     "\n	--all		Number of installed CPUs" \
     "\n	--ignore=N	Exclude N CPUs" \
	) \

#if !ENABLE_DESKTOP \

#define od_trivial_usage \
       "[-aBbcDdeFfHhIiLlOovXx] [FILE]" \

#define od_full_usage "\n\n" \
       "Print FILE (or stdin) unambiguously, as octal bytes by default" \

#endif \

#if ENABLE_DESKTOP \

#define od_trivial_usage \
       "[-abcdfhilovxs] [-t TYPE] [-A RADIX] [-N SIZE] [-j SKIP] [-S MINSTR] [-w WIDTH] [FILE]..." \

#define od_full_usage "\n\n" \
       "Print FILEs (or stdin) unambiguously, as octal bytes by default" \

#endif \

#define paste_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define paste_full_usage "\n\n" \
       "Paste lines from each input file, separated with tab\n" \
     "\n	-d LIST	Use delimiters from LIST, not tab" \
     "\n	-s      Serial: one file at a time" \

#define paste_example_usage \
       "# write out directory in four columns\n" \
       "$ ls | paste - - - -\n" \
       "# combine pairs of lines from a file into single lines\n" \
       "$ paste -s -d '\\t\\n' file\n" \

#define printenv_trivial_usage \
       "[VARIABLE]..." \

#define printenv_full_usage "\n\n" \
       "Print environment VARIABLEs.\n" \
       "If no VARIABLE specified, print all." \

#define printf_trivial_usage \
       "FORMAT [ARG]..." \

#define printf_full_usage "\n\n" \
       "Format and print ARG(s) according to FORMAT (a-la C printf)" \

#define printf_example_usage \
       "$ printf \"Val=%d\\n\" 5\n" \
       "Val=5\n" \

#define pwd_trivial_usage \
       "" \

#define pwd_full_usage "\n\n" \
       "Print the full filename of the current working directory" \

#define pwd_example_usage \
       "$ pwd\n" \
       "/root\n" \

#define readlink_trivial_usage \
	IF_FEATURE_READLINK_FOLLOW("[-fnv] ") "FILE" \

#define readlink_full_usage "\n\n" \
       "Display the value of a symlink" \
	IF_FEATURE_READLINK_FOLLOW( "\n" \
     "\n	-f	Canonicalize by following all symlinks" \
     "\n	-n	Don't add newline" \
     "\n	-v	Verbose" \
	) \

#define realpath_trivial_usage \
       "FILE..." \

#define realpath_full_usage "\n\n" \
       "Return the absolute pathnames of given FILE" \

#define rm_trivial_usage \
       "[-irf] FILE..." \

#define rm_full_usage "\n\n" \
       "Remove (unlink) FILEs\n" \
     "\n	-i	Always prompt before removing" \
     "\n	-f	Never prompt" \
     "\n	-R,-r	Recurse" \

#define rm_example_usage \
       "$ rm -rf /tmp/foo\n" \

#define rmdir_trivial_usage \
       "[OPTIONS] DIRECTORY..." \

#define rmdir_full_usage "\n\n" \
       "Remove DIRECTORY if it is empty\n" \
     "\n	-p	Include parents" \
	IF_LONG_OPTS( \
     "\n	--ignore-fail-on-non-empty" \
	) \

#define rmdir_example_usage \
       "# rmdir /tmp/foo\n" \

#define seq_trivial_usage \
       "[-w] [-s SEP] [FIRST [INC]] LAST" \

#define seq_full_usage "\n\n" \
       "Print numbers from FIRST to LAST, in steps of INC.\n" \
       "FIRST, INC default to 1.\n" \
     "\n	-w	Pad to last with leading zeros" \
     "\n	-s SEP	String separator" \

#define shred_trivial_usage \
       "FILE..." \

#define shred_full_usage "\n\n" \
       "Overwrite/delete FILEs\n" \
     "\n	-f	Chmod to ensure writability" \
     "\n	-n N	Overwrite N times (default 3)" \
     "\n	-z	Final overwrite with zeros" \
     "\n	-u	Remove file" \

#define shuf_trivial_usage \
       "[-e|-i L-H] [-n NUM] [-o FILE] [-z] [FILE|ARG...]" \

#define shuf_full_usage "\n\n" \
       "Randomly permute lines\n" \
     "\n	-e	Treat ARGs as lines" \
     "\n	-i L-H	Treat numbers L-H as lines" \
     "\n	-n NUM	Output at most NUM lines" \
     "\n	-o FILE	Write to FILE, not standard output" \
     "\n	-z	End lines with zero byte, not newline" \

#define sleep_trivial_usage \
	IF_FEATURE_FANCY_SLEEP("[") "N" IF_FEATURE_FANCY_SLEEP("]...") \

#define sleep_full_usage "\n\n" \
	IF_NOT_FEATURE_FANCY_SLEEP("Pause for N seconds") \
	IF_FEATURE_FANCY_SLEEP( \
       "Pause for a time equal to the total of the args given, where each arg can\n" \
       "have an optional suffix of (s)econds, (m)inutes, (h)ours, or (d)ays") \

#define sleep_example_usage \
       "$ sleep 2\n" \
       "[2 second delay results]\n" \
	IF_FEATURE_FANCY_SLEEP( \
       "$ sleep 1d 3h 22m 8s\n" \
       "[98528 second delay results]\n") \

#define sort_trivial_usage \
       "[-nru" \
	IF_FEATURE_SORT_BIG("gMcszbdfiokt] [-o FILE] [-k start[.offset][opts][,end[.offset][opts]] [-t CHAR") \
       "] [FILE]..." \

#define sort_full_usage "\n\n" \
       "Sort lines of text\n" \
	IF_FEATURE_SORT_BIG( \
     "\n	-o FILE	Output to FILE" \
     "\n	-c	Check whether input is sorted" \
     "\n	-b	Ignore leading blanks" \
     "\n	-f	Ignore case" \
     "\n	-i	Ignore unprintable characters" \
     "\n	-d	Dictionary order (blank or alphanumeric only)" \
	) \
     "\n	-n	Sort numbers" \
	IF_FEATURE_SORT_BIG( \
     "\n	-g	General numerical sort" \
     "\n	-M	Sort month" \
     "\n	-V	Sort version" \
     "\n	-t CHAR	Field separator" \
     "\n	-k N[,M] Sort by Nth field" \
	) \
     "\n	-r	Reverse sort order" \
     "\n	-s	Stable (don't sort ties alphabetically)" \
     "\n	-u	Suppress duplicate lines" \
     "\n	-z	Lines are terminated by NUL, not newline" \

#define sort_example_usage \
       "$ echo -e \"e\\nf\\nb\\nd\\nc\\na\" | sort\n" \
       "a\n" \
       "b\n" \
       "c\n" \
       "d\n" \
       "e\n" \
       "f\n" \
	IF_FEATURE_SORT_BIG( \
		"$ echo -e \"c 3\\nb 2\\nd 2\" | $SORT -k 2,2n -k 1,1r\n" \
		"d 2\n" \
		"b 2\n" \
		"c 3\n" \
	) \
       "" \

#define split_trivial_usage \
       "[OPTIONS] [INPUT [PREFIX]]" \

#define split_full_usage "\n\n" \
       "	-b N[k|m]	Split by N (kilo|mega)bytes" \
     "\n	-l N		Split by N lines" \
     "\n	-a N		Use N letters as suffix" \

#define split_example_usage \
       "$ split TODO foo\n" \
       "$ cat TODO | split -a 2 -l 2 TODO_\n" \

#define stat_trivial_usage \
       "[OPTIONS] FILE..." \

#define stat_full_usage "\n\n" \
       "Display file" \
            IF_FEATURE_STAT_FILESYSTEM(" (default) or filesystem") \
            " status\n" \
	IF_FEATURE_STAT_FORMAT( \
     "\n	-c FMT	Use the specified format" \
	) \
	IF_FEATURE_STAT_FILESYSTEM( \
     "\n	-f	Display filesystem status" \
	) \
     "\n	-L	Follow links" \
     "\n	-t	Terse display" \
	IF_SELINUX( \
     "\n	-Z	Print security context" \
	) \
	IF_FEATURE_STAT_FORMAT( \
       "\n\nFMT sequences"IF_FEATURE_STAT_FILESYSTEM(" for files")":\n" \
       " %a	Access rights in octal\n" \
       " %A	Access rights in human readable form\n" \
       " %b	Number of blocks allocated (see %B)\n" \
       " %B	Size in bytes of each block reported by %b\n" \
       " %d	Device number in decimal\n" \
       " %D	Device number in hex\n" \
       " %f	Raw mode in hex\n" \
       " %F	File type\n" \
       " %g	Group ID\n" \
       " %G	Group name\n" \
       " %h	Number of hard links\n" \
       " %i	Inode number\n" \
       " %n	File name\n" \
       " %N	File name, with -> TARGET if symlink\n" \
       " %o	I/O block size\n" \
       " %s	Total size in bytes\n" \
       " %t	Major device type in hex\n" \
       " %T	Minor device type in hex\n" \
       " %u	User ID\n" \
       " %U	User name\n" \
       " %x	Time of last access\n" \
       " %X	Time of last access as seconds since Epoch\n" \
       " %y	Time of last modification\n" \
       " %Y	Time of last modification as seconds since Epoch\n" \
       " %z	Time of last change\n" \
       " %Z	Time of last change as seconds since Epoch\n" \
	IF_FEATURE_STAT_FILESYSTEM( \
       "\nFMT sequences for file systems:\n" \
       " %a	Free blocks available to non-superuser\n" \
       " %b	Total data blocks\n" \
       " %c	Total file nodes\n" \
       " %d	Free file nodes\n" \
       " %f	Free blocks\n" \
	IF_SELINUX( \
       " %C	Security context in selinux\n" \
	) \
       " %i	File System ID in hex\n" \
       " %l	Maximum length of filenames\n" \
       " %n	File name\n" \
       " %s	Block size (for faster transfer)\n" \
       " %S	Fundamental block size (for block counts)\n" \
       " %t	Type in hex\n" \
       " %T	Type in human readable form" \
	) \
	) \

#define stty_trivial_usage \
       "[-a|g] [-F DEVICE] [SETTING]..." \

#define stty_full_usage "\n\n" \
       "Without arguments, prints baud rate, line discipline,\n" \
       "and deviations from stty sane\n" \
     "\n	-F DEVICE	Open device instead of stdin" \
     "\n	-a		Print all current settings in human-readable form" \
     "\n	-g		Print in stty-readable form" \
     "\n	[SETTING]	See manpage" \

#define sum_trivial_usage \
       "[-rs] [FILE]..." \

#define sum_full_usage "\n\n" \
       "Checksum and count the blocks in a file\n" \
     "\n	-r	Use BSD sum algorithm (1K blocks)" \
     "\n	-s	Use System V sum algorithm (512byte blocks)" \

#define sync_trivial_usage \
       ""IF_FEATURE_SYNC_FANCY("[-df] [FILE]...") \

#define sync_full_usage "\n\n" \
    IF_NOT_FEATURE_SYNC_FANCY( \
       "Write all buffered blocks to disk" \
    ) \
    IF_FEATURE_SYNC_FANCY( \
       "Write all buffered blocks (in FILEs) to disk" \
     "\n	-d	Avoid syncing metadata" \
     "\n	-f	Sync filesystems underlying FILEs" \
    ) \

#define tac_trivial_usage \
	"[FILE]..." \

#define tac_full_usage "\n\n" \
	"Concatenate FILEs and print them in reverse" \

#define tail_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define tail_full_usage "\n\n" \
       "Print last 10 lines of each FILE (or stdin) to stdout.\n" \
       "With more than one FILE, precede each with a filename header.\n" \
     "\n	-f		Print data as file grows" \
     "\n	-c [+]N[kbm]	Print last N bytes" \
     "\n	-n N[kbm]	Print last N lines" \
     "\n	-n +N[kbm]	Start on Nth line and print the rest" \
	IF_FEATURE_FANCY_TAIL( \
     "\n	-q		Never print headers" \
     "\n	-s SECONDS	Wait SECONDS between reads with -f" \
     "\n	-v		Always print headers" \
     "\n	-F		Same as -f, but keep retrying" \
     "\n" \
     "\nN may be suffixed by k (x1024), b (x512), or m (x1024^2)." \
	) \

#define tail_example_usage \
       "$ tail -n 1 /etc/resolv.conf\n" \
       "nameserver 10.0.0.1\n" \

#define tee_trivial_usage \
       "[-ai] [FILE]..." \

#define tee_full_usage "\n\n" \
       "Copy stdin to each FILE, and also to stdout\n" \
     "\n	-a	Append to the given FILEs, don't overwrite" \
     "\n	-i	Ignore interrupt signals (SIGINT)" \

#define tee_example_usage \
       "$ echo \"Hello\" | tee /tmp/foo\n" \
       "$ cat /tmp/foo\n" \
       "Hello\n" \

#define test_trivial_usage NOUSAGE_STR \

#define test_full_usage "" \

#define test_example_usage \
       "$ test 1 -eq 2\n" \
       "$ echo $?\n" \
       "1\n" \
       "$ test 1 -eq 1\n" \
       "$ echo $?\n" \
       "0\n" \
       "$ [ -d /etc ]\n" \
       "$ echo $?\n" \
       "0\n" \
       "$ [ -d /junk ]\n" \
       "$ echo $?\n" \
       "1\n" \

#define timeout_trivial_usage \
       "[-s SIG] SECS PROG ARGS" \

#define timeout_full_usage "\n\n" \
       "Runs PROG. Sends SIG to it if it is not gone in SECS seconds.\n" \
       "Default SIG: TERM." \

#define touch_trivial_usage \
       "[-c]" IF_FEATURE_TOUCH_SUSV3(" [-d DATE] [-t DATE] [-r FILE]") " FILE..." \

#define touch_full_usage "\n\n" \
       "Update the last-modified date on the given FILE[s]\n" \
     "\n	-c	Don't create files" \
	IF_FEATURE_TOUCH_NODEREF( \
     "\n	-h	Don't follow links" \
	) \
	IF_FEATURE_TOUCH_SUSV3( \
     "\n	-d DT	Date/time to use" \
     "\n	-t DT	Date/time to use" \
     "\n	-r FILE	Use FILE's date/time" \
	) \

#define touch_example_usage \
       "$ ls -l /tmp/foo\n" \
       "/bin/ls: /tmp/foo: No such file or directory\n" \
       "$ touch /tmp/foo\n" \
       "$ ls -l /tmp/foo\n" \
       "-rw-rw-r--    1 andersen andersen        0 Apr 15 01:11 /tmp/foo\n" \

#define tr_trivial_usage \
       "[-cds] STRING1 [STRING2]" \

#define tr_full_usage "\n\n" \
       "Translate, squeeze, or delete characters from stdin, writing to stdout\n" \
     "\n	-c	Take complement of STRING1" \
     "\n	-d	Delete input characters coded STRING1" \
     "\n	-s	Squeeze multiple output characters of STRING2 into one character" \

#define tr_example_usage \
       "$ echo \"gdkkn vnqkc\" | tr [a-y] [b-z]\n" \
       "hello world\n" \

#define true_trivial_usage NOUSAGE_STR \

#define true_full_usage "" \

#define true_example_usage \
       "$ true\n" \
       "$ echo $?\n" \
       "0\n" \

#define truncate_trivial_usage \
       "[-c] -s SIZE FILE..." \

#define truncate_full_usage "\n\n" \
	"Truncate FILEs to the given size\n" \
	"\n	-c	Do not create files" \
	"\n	-s SIZE	Truncate to SIZE" \

#define truncate_example_usage \
	"$ truncate -s 1G foo" \

#define tty_trivial_usage \
       "" \

#define tty_full_usage "\n\n" \
       "Print file name of stdin's terminal" \
	IF_INCLUDE_SUSv2( "\n" \
     "\n	-s	Print nothing, only return exit status" \
	) \

#define tty_example_usage \
       "$ tty\n" \
       "/dev/tty2\n" \

#define uname_trivial_usage \
       "[-amnrspvio]" \

#define uname_full_usage "\n\n" \
       "Print system information\n" \
     "\n	-a	Print all" \
     "\n	-m	The machine (hardware) type" \
     "\n	-n	Hostname" \
     "\n	-r	Kernel release" \
     "\n	-s	Kernel name (default)" \
     "\n	-p	Processor type" \
     "\n	-v	Kernel version" \
     "\n	-i	The hardware platform" \
     "\n	-o	OS name" \

#define uname_example_usage \
       "$ uname -a\n" \
       "Linux debian 2.4.23 #2 Tue Dec 23 17:09:10 MST 2003 i686 GNU/Linux\n" \

#define arch_trivial_usage \
       "" \

#define arch_full_usage "\n\n" \
       "Print system architecture" \

#define uniq_trivial_usage \
       "[-cdu][-f,s,w N] [INPUT [OUTPUT]]" \

#define uniq_full_usage "\n\n" \
       "Discard duplicate lines\n" \
     "\n	-c	Prefix lines by the number of occurrences" \
     "\n	-d	Only print duplicate lines" \
     "\n	-u	Only print unique lines" \
     "\n	-i	Ignore case" \
     "\n	-f N	Skip first N fields" \
     "\n	-s N	Skip first N chars (after any skipped fields)" \
     "\n	-w N	Compare N characters in line" \

#define uniq_example_usage \
       "$ echo -e \"a\\na\\nb\\nc\\nc\\na\" | sort | uniq\n" \
       "a\n" \
       "b\n" \
       "c\n" \

#define unlink_trivial_usage \
	"FILE" \

#define unlink_full_usage "\n\n" \
	"Delete FILE by calling unlink()" \

#define usleep_trivial_usage \
       "N" \

#define usleep_full_usage "\n\n" \
       "Pause for N microseconds" \

#define usleep_example_usage \
       "$ usleep 1000000\n" \
       "[pauses for 1 second]\n" \

#define uudecode_trivial_usage \
       "[-o OUTFILE] [INFILE]" \

#define uudecode_full_usage "\n\n" \
       "Uudecode a file\n" \
       "Finds OUTFILE in uuencoded source unless -o is given" \

#define uudecode_example_usage \
       "$ uudecode -o busybox busybox.uu\n" \
       "$ ls -l busybox\n" \
       "-rwxr-xr-x   1 ams      ams        245264 Jun  7 21:35 busybox\n" \

#define base64_trivial_usage \
	"[-d] [FILE]" \

#define base64_full_usage "\n\n" \
       "Base64 encode or decode FILE to standard output" \
     "\n	-d	Decode data" \

#define uuencode_trivial_usage \
       "[-m] [FILE] STORED_FILENAME" \

#define uuencode_full_usage "\n\n" \
       "Uuencode FILE (or stdin) to stdout\n" \
     "\n	-m	Use base64 encoding per RFC1521" \

#define uuencode_example_usage \
       "$ uuencode busybox busybox\n" \
       "begin 755 busybox\n" \
       "<encoded file snipped>\n" \
       "$ uudecode busybox busybox > busybox.uu\n" \
       "$\n" \

#define wc_trivial_usage \
       "[-c"IF_UNICODE_SUPPORT("m")"lwL] [FILE]..." \

#define wc_full_usage "\n\n" \
       "Count lines, words, and bytes for each FILE (or stdin)\n" \
     "\n	-c	Count bytes" \
	IF_UNICODE_SUPPORT( \
     "\n	-m	Count characters" \
	) \
     "\n	-l	Count newlines" \
     "\n	-w	Count words" \
     "\n	-L	Print longest line length" \

#define wc_example_usage \
       "$ wc /etc/passwd\n" \
       "     31      46    1365 /etc/passwd\n" \

#define users_trivial_usage \
       "" \

#define users_full_usage "\n\n" \
       "Print the users currently logged on" \

#define w_trivial_usage \
       "" \

#define w_full_usage "\n\n" \
       "Show who is logged on" \

#define who_trivial_usage \
       "[-a]" \

#define who_full_usage "\n\n" \
       "Show who is logged on\n" \
     "\n	-a	Show all" \
     "\n	-H	Print column headers" \

#define whoami_trivial_usage \
       "" \

#define whoami_full_usage "\n\n" \
       "Print the user name associated with the current effective user id" \

#define yes_trivial_usage \
       "[STRING]" \

#define yes_full_usage "\n\n" \
       "Repeatedly output a line with STRING, or 'y'" \

#define pipe_progress_trivial_usage NOUSAGE_STR \

#define pipe_progress_full_usage "" \

#define run_parts_trivial_usage \
       "[-a ARG]... [-u UMASK] " \
       IF_FEATURE_RUN_PARTS_LONG_OPTIONS("[--reverse] [--test] [--exit-on-error] "IF_FEATURE_RUN_PARTS_FANCY("[--list] ")) \
       "DIRECTORY" \

#define run_parts_full_usage "\n\n" \
       "Run a bunch of scripts in DIRECTORY\n" \
     "\n	-a ARG		Pass ARG as argument to scripts" \
     "\n	-u UMASK	Set UMASK before running scripts" \
	IF_FEATURE_RUN_PARTS_LONG_OPTIONS( \
     "\n	--reverse	Reverse execution order" \
     "\n	--test		Dry run" \
     "\n	--exit-on-error	Exit if a script exits with non-zero" \
	IF_FEATURE_RUN_PARTS_FANCY( \
     "\n	--list		Print names of matching files even if they are not executable" \
	) \
	) \

#define run_parts_example_usage \
       "$ run-parts -a start /etc/init.d\n" \
       "$ run-parts -a stop=now /etc/init.d\n\n" \
       "Let's assume you have a script foo/dosomething:\n" \
       "#!/bin/sh\n" \
       "for i in $*; do eval $i; done; unset i\n" \
       "case \"$1\" in\n" \
       "start*) echo starting something;;\n" \
       "stop*) set -x; shutdown -h $stop;;\n" \
       "esac\n\n" \
       "Running this yields:\n" \
       "$run-parts -a stop=+4m foo/\n" \
       "+ shutdown -h +4m" \

#define start_stop_daemon_trivial_usage \
       "[OPTIONS] [-S|-K] ... [-- ARGS...]" \

#define start_stop_daemon_full_usage "\n\n" \
       "Search for matching processes, and then\n" \
       "-K: stop all matching processes\n" \
       "-S: start a process unless a matching process is found\n" \
     "\nProcess matching:" \
     "\n	-u USERNAME|UID	Match only this user's processes" \
     "\n	-n NAME		Match processes with NAME" \
     "\n			in comm field in /proc/PID/stat" \
     "\n	-x EXECUTABLE	Match processes with this command" \
     "\n			command in /proc/PID/cmdline" \
     "\n	-p FILE		Match a process with PID from FILE" \
     "\n	All specified conditions must match" \
     "\n-S only:" \
     "\n	-x EXECUTABLE	Program to run" \
     "\n	-a NAME		Zeroth argument" \
     "\n	-b		Background" \
	IF_FEATURE_START_STOP_DAEMON_FANCY( \
     "\n	-N N		Change nice level" \
	) \
     "\n	-c USER[:[GRP]]	Change user/group" \
     "\n	-m		Write PID to pidfile specified by -p" \
     "\n-K only:" \
     "\n	-s SIG		Signal to send" \
     "\n	-t		Match only, exit with 0 if found" \
     "\nOther:" \
	IF_FEATURE_START_STOP_DAEMON_FANCY( \
     "\n	-o		Exit with status 0 if nothing is done" \
     "\n	-v		Verbose" \
	) \
     "\n	-q		Quiet" \

#define which_trivial_usage \
       "[COMMAND]..." \

#define which_full_usage "\n\n" \
       "Locate a COMMAND" \

#define which_example_usage \
       "$ which login\n" \
       "/bin/login\n" \

#define chattr_trivial_usage \
       "[-R] [-v VERSION] [-+=AacDdijsStTu] FILE..." \

#define chattr_full_usage "\n\n" \
       "Change ext2 file attributes\n" \
     "\n	-R	Recurse" \
     "\n	-v VER	Set version/generation number" \
     "\nModifiers:" \
     "\n	-,+,=	Remove/add/set attributes" \
     "\nAttributes:" \
     "\n	A	Don't track atime" \
     "\n	a	Append mode only" \
     "\n	c	Enable compress" \
     "\n	D	Write dir contents synchronously" \
     "\n	d	Don't backup with dump" \
     "\n	i	Cannot be modified (immutable)" \
     "\n	j	Write all data to journal first" \
     "\n	s	Zero disk storage when deleted" \
     "\n	S	Write synchronously" \
     "\n	t	Disable tail-merging of partial blocks with other files" \
     "\n	u	Allow file to be undeleted" \

#define fsck_trivial_usage \
       "[-ANPRTV] [-t FSTYPE] [FS_OPTS] [BLOCKDEV]..." \

#define fsck_full_usage "\n\n" \
       "Check and repair filesystems\n" \
     "\n	-A	Walk /etc/fstab and check all filesystems" \
     "\n	-N	Don't execute, just show what would be done" \
     "\n	-P	With -A, check filesystems in parallel" \
     "\n	-R	With -A, skip the root filesystem" \
     "\n	-T	Don't show title on startup" \
     "\n	-V	Verbose" \
     "\n	-t TYPE	List of filesystem types to check" \

#define lsattr_trivial_usage \
       "[-Radlv] [FILE]..." \

#define lsattr_full_usage "\n\n" \
       "List ext2 file attributes\n" \
     "\n	-R	Recurse" \
     "\n	-a	Don't hide entries starting with ." \
     "\n	-d	List directory entries instead of contents" \
     "\n	-l	List long flag names" \
     "\n	-v	List version/generation number" \

#define tune2fs_trivial_usage \
       "[-c MAX_MOUNT_COUNT] " \
       "[-i DAYS] " \
       "[-C MOUNT_COUNT] " \
       "[-L LABEL] " \
       "BLOCKDEV" \

#define tune2fs_full_usage "\n\n" \
       "Adjust filesystem options on ext[23] filesystems" \

#define awk_trivial_usage \
       "[OPTIONS] [AWK_PROGRAM] [FILE]..." \

#define awk_full_usage "\n\n" \
       "	-v VAR=VAL	Set variable" \
     "\n	-F SEP		Use SEP as field separator" \
     "\n	-f FILE		Read program from FILE" \
	IF_FEATURE_AWK_GNU_EXTENSIONS( \
     "\n	-e AWK_PROGRAM" \
	) \

#define cmp_trivial_usage \
       "[-l] [-s] FILE1 [FILE2" IF_DESKTOP(" [SKIP1 [SKIP2]]") "]" \

#define cmp_full_usage "\n\n" \
       "Compare FILE1 with FILE2 (or stdin)\n" \
     "\n	-l	Write the byte numbers (decimal) and values (octal)" \
     "\n		for all differing bytes" \
     "\n	-s	Quiet" \

#define diff_trivial_usage \
       "[-abBdiNqrTstw] [-L LABEL] [-S FILE] [-U LINES] FILE1 FILE2" \

#define diff_full_usage "\n\n" \
       "Compare files line by line and output the differences between them.\n" \
       "This implementation supports unified diffs only.\n" \
     "\n	-a	Treat all files as text" \
     "\n	-b	Ignore changes in the amount of whitespace" \
     "\n	-B	Ignore changes whose lines are all blank" \
     "\n	-d	Try hard to find a smaller set of changes" \
     "\n	-i	Ignore case differences" \
     "\n	-L	Use LABEL instead of the filename in the unified header" \
     "\n	-N	Treat absent files as empty" \
     "\n	-q	Output only whether files differ" \
     "\n	-r	Recurse" \
     "\n	-S	Start with FILE when comparing directories" \
     "\n	-T	Make tabs line up by prefixing a tab when necessary" \
     "\n	-s	Report when two files are the same" \
     "\n	-t	Expand tabs to spaces in output" \
     "\n	-U	Output LINES lines of context" \
     "\n	-w	Ignore all whitespace" \

#define ed_trivial_usage "[FILE]" \

#define ed_full_usage "" \

#define patch_trivial_usage \
       "[OPTIONS] [ORIGFILE [PATCHFILE]]" \

#define patch_full_usage "\n\n" \
       "	-p N	Strip N leading components from file names" \
     "\n	-i DIFF	Read DIFF instead of stdin" \
     "\n	-R	Reverse patch" \
     "\n	-N	Ignore already applied patches" \
     "\n	-E	Remove output files if they become empty" \
	IF_LONG_OPTS( \
     "\n	--dry-run	Don't actually change files" \
	) \

#define patch_example_usage \
       "$ patch -p1 < example.diff\n" \
       "$ patch -p0 -i example.diff" \

#define sed_trivial_usage \
       "[-i[SFX]] [-nrE] [-f FILE]... [-e CMD]... [FILE]...\n" \
       "or: sed [-i[SFX]] [-nrE] CMD [FILE]..." \

#define sed_full_usage "\n\n" \
       "	-e CMD	Add CMD to sed commands to be executed" \
     "\n	-f FILE	Add FILE contents to sed commands to be executed" \
     "\n	-i[SFX]	Edit files in-place (otherwise sends to stdout)" \
     "\n		Optionally back files up, appending SFX" \
     "\n	-n	Suppress automatic printing of pattern space" \
     "\n	-r,-E	Use extended regex syntax" \
     "\n" \
     "\nIf no -e or -f, the first non-option argument is the sed command string." \
     "\nRemaining arguments are input files (stdin if none)." \

#define sed_example_usage \
       "$ echo \"foo\" | sed -e 's/f[a-zA-Z]o/bar/g'\n" \
       "bar\n" \

#define vi_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define vi_full_usage "\n\n" \
       "Edit FILE\n" \
	IF_FEATURE_VI_COLON( \
     "\n	-c CMD	Initial command to run ($EXINIT also available)" \
	) \
	IF_FEATURE_VI_READONLY( \
     "\n	-R	Read-only" \
	) \
     "\n	-H	List available features" \

#define find_trivial_usage \
       "[-HL] [PATH]... [OPTIONS] [ACTIONS]" \

#define find_full_usage "\n\n" \
       "Search for files and perform actions on them.\n" \
       "First failed action stops processing of current file.\n" \
       "Defaults: PATH is current directory, action is '-print'\n" \
     "\n	-L,-follow	Follow symlinks" \
     "\n	-H		...on command line only" \
	IF_FEATURE_FIND_XDEV( \
     "\n	-xdev		Don't descend directories on other filesystems" \
	) \
	IF_FEATURE_FIND_MAXDEPTH( \
     "\n	-maxdepth N	Descend at most N levels. -maxdepth 0 applies" \
     "\n			actions to command line arguments only" \
     "\n	-mindepth N	Don't act on first N levels" \
	) \
	IF_FEATURE_FIND_DEPTH( \
     "\n	-depth		Act on directory *after* traversing it" \
	) \
     "\n" \
     "\nActions:" \
	IF_FEATURE_FIND_PAREN( \
     "\n	( ACTIONS )	Group actions for -o / -a" \
	) \
	IF_FEATURE_FIND_NOT( \
     "\n	! ACT		Invert ACT's success/failure" \
	) \
     "\n	ACT1 [-a] ACT2	If ACT1 fails, stop, else do ACT2" \
     "\n	ACT1 -o ACT2	If ACT1 succeeds, stop, else do ACT2" \
     "\n			Note: -a has higher priority than -o" \
     "\n	-name PATTERN	Match file name (w/o directory name) to PATTERN" \
     "\n	-iname PATTERN	Case insensitive -name" \
	IF_FEATURE_FIND_PATH( \
     "\n	-path PATTERN	Match path to PATTERN" \
     "\n	-ipath PATTERN	Case insensitive -path" \
	) \
	IF_FEATURE_FIND_REGEX( \
     "\n	-regex PATTERN	Match path to regex PATTERN" \
	) \
	IF_FEATURE_FIND_TYPE( \
     "\n	-type X		File type is X (one of: f,d,l,b,c,s,p)" \
	) \
	IF_FEATURE_FIND_EXECUTABLE( \
     "\n	-executable	File is executable" \
	) \
	IF_FEATURE_FIND_PERM( \
     "\n	-perm MASK	At least one mask bit (+MASK), all bits (-MASK)," \
     "\n			or exactly MASK bits are set in file's mode" \
	) \
	IF_FEATURE_FIND_MTIME( \
     "\n	-mtime DAYS	mtime is greater than (+N), less than (-N)," \
     "\n			or exactly N days in the past" \
	) \
	IF_FEATURE_FIND_MMIN( \
     "\n	-mmin MINS	mtime is greater than (+N), less than (-N)," \
     "\n			or exactly N minutes in the past" \
	) \
	IF_FEATURE_FIND_NEWER( \
     "\n	-newer FILE	mtime is more recent than FILE's" \
	) \
	IF_FEATURE_FIND_INUM( \
     "\n	-inum N		File has inode number N" \
	) \
	IF_FEATURE_FIND_USER( \
     "\n	-user NAME/ID	File is owned by given user" \
	) \
	IF_FEATURE_FIND_GROUP( \
     "\n	-group NAME/ID	File is owned by given group" \
	) \
	IF_FEATURE_FIND_SIZE( \
     "\n	-size N[bck]	File size is N (c:bytes,k:kbytes,b:512 bytes(def.))" \
     "\n			+/-N: file size is bigger/smaller than N" \
	) \
	IF_FEATURE_FIND_LINKS( \
     "\n	-links N	Number of links is greater than (+N), less than (-N)," \
     "\n			or exactly N" \
	) \
	IF_FEATURE_FIND_CONTEXT( \
     "\n	-context CTX	File has specified security context" \
	) \
	IF_FEATURE_FIND_PRUNE( \
     "\n	-prune		If current file is directory, don't descend into it" \
	) \
     "\nIf none of the following actions is specified, -print is assumed" \
     "\n	-print		Print file name" \
	IF_FEATURE_FIND_PRINT0( \
     "\n	-print0		Print file name, NUL terminated" \
	) \
	IF_FEATURE_FIND_EXEC( \
     "\n	-exec CMD ARG ;	Run CMD with all instances of {} replaced by" \
     "\n			file name. Fails if CMD exits with nonzero" \
	) \
	IF_FEATURE_FIND_EXEC_PLUS( \
     "\n	-exec CMD ARG + Run CMD with {} replaced by list of file names" \
	) \
	IF_FEATURE_FIND_DELETE( \
     "\n	-delete		Delete current file/directory. Turns on -depth option" \
	) \
	IF_FEATURE_FIND_QUIT( \
     "\n	-quit		Exit" \
	) \

#define find_example_usage \
       "$ find / -name passwd\n" \
       "/etc/passwd\n" \

#define grep_trivial_usage \
       "[-HhnlLoqvsriwFE" \
	IF_EXTRA_COMPAT("z") \
       "] [-m N] " \
	IF_FEATURE_GREP_CONTEXT("[-A/B/C N] ") \
       "PATTERN/-e PATTERN.../-f FILE [FILE]..." \

#define grep_full_usage "\n\n" \
       "Search for PATTERN in FILEs (or stdin)\n" \
     "\n	-H	Add 'filename:' prefix" \
     "\n	-h	Do not add 'filename:' prefix" \
     "\n	-n	Add 'line_no:' prefix" \
     "\n	-l	Show only names of files that match" \
     "\n	-L	Show only names of files that don't match" \
     "\n	-c	Show only count of matching lines" \
     "\n	-o	Show only the matching part of line" \
     "\n	-q	Quiet. Return 0 if PATTERN is found, 1 otherwise" \
     "\n	-v	Select non-matching lines" \
     "\n	-s	Suppress open and read errors" \
     "\n	-r	Recurse" \
     "\n	-i	Ignore case" \
     "\n	-w	Match whole words only" \
     "\n	-x	Match whole lines only" \
     "\n	-F	PATTERN is a literal (not regexp)" \
     "\n	-E	PATTERN is an extended regexp" \
	IF_EXTRA_COMPAT( \
     "\n	-z	Input is NUL terminated" \
	) \
     "\n	-m N	Match up to N times per file" \
	IF_FEATURE_GREP_CONTEXT( \
     "\n	-A N	Print N lines of trailing context" \
     "\n	-B N	Print N lines of leading context" \
     "\n	-C N	Same as '-A N -B N'" \
	) \
     "\n	-e PTRN	Pattern to match" \
     "\n	-f FILE	Read pattern from file" \

#define grep_example_usage \
       "$ grep root /etc/passwd\n" \
       "root:x:0:0:root:/root:/bin/bash\n" \
       "$ grep ^[rR]oo. /etc/passwd\n" \
       "root:x:0:0:root:/root:/bin/bash\n" \

#define egrep_trivial_usage NOUSAGE_STR \

#define egrep_full_usage "" \

#define fgrep_trivial_usage NOUSAGE_STR \

#define fgrep_full_usage "" \

#define xargs_trivial_usage \
       "[OPTIONS] [PROG ARGS]" \

#define xargs_full_usage "\n\n" \
       "Run PROG on every item given by stdin\n" \
	IF_FEATURE_XARGS_SUPPORT_CONFIRMATION( \
     "\n	-p	Ask user whether to run each command" \
	) \
     "\n	-r	Don't run command if input is empty" \
	IF_FEATURE_XARGS_SUPPORT_ZERO_TERM( \
     "\n	-0	Input is separated by NULs" \
	) \
	IF_FEATURE_XARGS_SUPPORT_ARGS_FILE( \
     "\n	-a FILE	Read from FILE instead of stdin" \
	) \
     "\n	-t	Print the command on stderr before execution" \
     "\n	-e[STR]	STR stops input processing" \
     "\n	-n N	Pass no more than N args to PROG" \
     "\n	-s N	Pass command line of no more than N bytes" \
	IF_FEATURE_XARGS_SUPPORT_REPL_STR( \
     "\n	-I STR	Replace STR within PROG ARGS with input line" \
	) \
	IF_FEATURE_XARGS_SUPPORT_PARALLEL( \
     "\n	-P N	Run up to N PROGs in parallel" \
	) \
	IF_FEATURE_XARGS_SUPPORT_TERMOPT( \
     "\n	-x	Exit if size is exceeded" \
	) \

#define xargs_example_usage \
       "$ ls | xargs gzip\n" \
       "$ find . -name '*.c' -print | xargs rm\n" \

#define bootchartd_trivial_usage \
       "start [PROG ARGS]|stop|init" \

#define bootchartd_full_usage "\n\n" \
       "Create /var/log/bootchart.tgz with boot chart data\n" \
     "\nstart: start background logging; with PROG, run PROG, then kill logging with USR1" \
     "\nstop: send USR1 to all bootchartd processes" \
     "\ninit: start background logging; stop when getty/xdm is seen (for init scripts)" \
     "\nUnder PID 1: as init, then exec $bootchart_init, /init, /sbin/init" \

#define halt_trivial_usage \
       "[-d DELAY] [-n] [-f]" IF_FEATURE_WTMP(" [-w]") \

#define halt_full_usage "\n\n" \
       "Halt the system\n" \
     "\n	-d SEC	Delay interval" \
     "\n	-n	Do not sync" \
     "\n	-f	Force (don't go through init)" \
	IF_FEATURE_WTMP( \
     "\n	-w	Only write a wtmp record" \
	) \

#define poweroff_trivial_usage \
       "[-d DELAY] [-n] [-f]" \

#define poweroff_full_usage "\n\n" \
       "Halt and shut off power\n" \
     "\n	-d SEC	Delay interval" \
     "\n	-n	Do not sync" \
     "\n	-f	Force (don't go through init)" \

#define reboot_trivial_usage \
       "[-d DELAY] [-n] [-f]" \

#define reboot_full_usage "\n\n" \
       "Reboot the system\n" \
     "\n	-d SEC	Delay interval" \
     "\n	-n	Do not sync" \
     "\n	-f	Force (don't go through init)" \

#define linuxrc_trivial_usage NOUSAGE_STR \

#define linuxrc_full_usage "" \

#define init_trivial_usage \
       "" \

#define init_full_usage "\n\n" \
       "Init is the first process started during boot. It never exits." \
	IF_FEATURE_USE_INITTAB( \
   "\n""It (re)spawns children according to /etc/inittab." \
	) \
	IF_NOT_FEATURE_USE_INITTAB( \
   "\n""This version of init doesn't use /etc/inittab," \
   "\n""has fixed set of processed to run." \
	) \

#define init_notes_usage \
	"This version of init is designed to be run only by the kernel.\n" \
	"\n" \
	"BusyBox init doesn't support multiple runlevels. The runlevels field of\n" \
	"the /etc/inittab file is completely ignored by BusyBox init. If you want\n" \
	"runlevels, use sysvinit.\n" \
	"\n" \
	"BusyBox init works just fine without an inittab. If no inittab is found,\n" \
	"it has the following default behavior:\n" \
	"\n" \
	"	::sysinit:/etc/init.d/rcS\n" \
	"	::askfirst:/bin/sh\n" \
	"	::ctrlaltdel:/sbin/reboot\n" \
	"	::shutdown:/sbin/swapoff -a\n" \
	"	::shutdown:/bin/umount -a -r\n" \
	"	::restart:/sbin/init\n" \
	"	tty2::askfirst:/bin/sh\n" \
	"	tty3::askfirst:/bin/sh\n" \
	"	tty4::askfirst:/bin/sh\n" \
	"\n" \
	"If you choose to use an /etc/inittab file, the inittab entry format is as follows:\n" \
	"\n" \
	"	<id>:<runlevels>:<action>:<process>\n" \
	"\n" \
	"	<id>:\n" \
	"\n" \
	"		WARNING: This field has a non-traditional meaning for BusyBox init!\n" \
	"		The id field is used by BusyBox init to specify the controlling tty for\n" \
	"		the specified process to run on. The contents of this field are\n" \
	"		appended to \"/dev/\" and used as-is. There is no need for this field to\n" \
	"		be unique, although if it isn't you may have strange results. If this\n" \
	"		field is left blank, then the init's stdin/out will be used.\n" \
	"\n" \
	"	<runlevels>:\n" \
	"\n" \
	"		The runlevels field is completely ignored.\n" \
	"\n" \
	"	<action>:\n" \
	"\n" \
	"		Valid actions include: sysinit, respawn, askfirst, wait,\n" \
	"		once, restart, ctrlaltdel, and shutdown.\n" \
	"\n" \
	"		The available actions can be classified into two groups: actions\n" \
	"		that are run only once, and actions that are re-run when the specified\n" \
	"		process exits.\n" \
	"\n" \
	"		Run only-once actions:\n" \
	"\n" \
	"			'sysinit' is the first item run on boot. init waits until all\n" \
	"			sysinit actions are completed before continuing. Following the\n" \
	"			completion of all sysinit actions, all 'wait' actions are run.\n" \
	"			'wait' actions, like 'sysinit' actions, cause init to wait until\n" \
	"			the specified task completes. 'once' actions are asynchronous,\n" \
	"			therefore, init does not wait for them to complete. 'restart' is\n" \
	"			the action taken to restart the init process. By default this should\n" \
	"			simply run /sbin/init, but can be a script which runs pivot_root or it\n" \
	"			can do all sorts of other interesting things. The 'ctrlaltdel' init\n" \
	"			actions are run when the system detects that someone on the system\n" \
	"			console has pressed the CTRL-ALT-DEL key combination. Typically one\n" \
	"			wants to run 'reboot' at this point to cause the system to reboot.\n" \
	"			Finally the 'shutdown' action specifies the actions to taken when\n" \
	"			init is told to reboot. Unmounting filesystems and disabling swap\n" \
	"			is a very good here.\n" \
	"\n" \
	"		Run repeatedly actions:\n" \
	"\n" \
	"			'respawn' actions are run after the 'once' actions. When a process\n" \
	"			started with a 'respawn' action exits, init automatically restarts\n" \
	"			it. Unlike sysvinit, BusyBox init does not stop processes from\n" \
	"			respawning out of control. The 'askfirst' actions acts just like\n" \
	"			respawn, except that before running the specified process it\n" \
	"			displays the line \"Please press Enter to activate this console.\"\n" \
	"			and then waits for the user to press enter before starting the\n" \
	"			specified process.\n" \
	"\n" \
	"		Unrecognized actions (like initdefault) will cause init to emit an\n" \
	"		error message, and then go along with its business. All actions are\n" \
	"		run in the order they appear in /etc/inittab.\n" \
	"\n" \
	"	<process>:\n" \
	"\n" \
	"		Specifies the process to be executed and its command line.\n" \
	"\n" \
	"Example /etc/inittab file:\n" \
	"\n" \
	"	# This is run first except when booting in single-user mode\n" \
	"	#\n" \
	"	::sysinit:/etc/init.d/rcS\n" \
	"	\n" \
	"	# /bin/sh invocations on selected ttys\n" \
	"	#\n" \
	"	# Start an \"askfirst\" shell on the console (whatever that may be)\n" \
	"	::askfirst:-/bin/sh\n" \
	"	# Start an \"askfirst\" shell on /dev/tty2-4\n" \
	"	tty2::askfirst:-/bin/sh\n" \
	"	tty3::askfirst:-/bin/sh\n" \
	"	tty4::askfirst:-/bin/sh\n" \
	"	\n" \
	"	# /sbin/getty invocations for selected ttys\n" \
	"	#\n" \
	"	tty4::respawn:/sbin/getty 38400 tty4\n" \
	"	tty5::respawn:/sbin/getty 38400 tty5\n" \
	"	\n" \
	"	\n" \
	"	# Example of how to put a getty on a serial line (for a terminal)\n" \
	"	#\n" \
	"	#::respawn:/sbin/getty -L ttyS0 9600 vt100\n" \
	"	#::respawn:/sbin/getty -L ttyS1 9600 vt100\n" \
	"	#\n" \
	"	# Example how to put a getty on a modem line\n" \
	"	#::respawn:/sbin/getty 57600 ttyS2\n" \
	"	\n" \
	"	# Stuff to do when restarting the init process\n" \
	"	::restart:/sbin/init\n" \
	"	\n" \
	"	# Stuff to do before rebooting\n" \
	"	::ctrlaltdel:/sbin/reboot\n" \
	"	::shutdown:/bin/umount -a -r\n" \
	"	::shutdown:/sbin/swapoff -a\n" \

#define nuke_trivial_usage \
       "DIR..." \

#define nuke_full_usage "\n\n" \
       "Remove DIRs" \

#define resume_trivial_usage \
       "BLOCKDEV [OFFSET]" \

#define resume_full_usage "\n" \
   "\n""Restore system state from 'suspend-to-disk' data in BLOCKDEV" \

#define busybox_trivial_usage NOUSAGE_STR \

#define busybox_full_usage "" \

#define unit_trivial_usage \
       "" \

#define unit_full_usage "\n\n" \
       "Run the unit-test suite" \

#define parse_trivial_usage \
       "[-x] [-n MAXTOKENS] [-m MINTOKENS] [-d DELIMS] [-f FLAGS] FILE..." \

#define parse_full_usage "\n\n" \
       "	-x	Suppress output (for benchmarking)" \

#define add_shell_trivial_usage \
       "SHELL..." \

#define add_shell_full_usage "\n\n" \
       "Add SHELLs to /etc/shells" \

#define remove_shell_trivial_usage \
       "SHELL..." \

#define remove_shell_full_usage "\n\n" \
       "Remove SHELLs from /etc/shells" \

#define addgroup_trivial_usage \
       "[-g GID] [-S] " IF_FEATURE_ADDUSER_TO_GROUP("[USER] ") "GROUP" \

#define addgroup_full_usage "\n\n" \
       "Add a group" IF_FEATURE_ADDUSER_TO_GROUP(" or add a user to a group") "\n" \
     "\n	-g GID	Group id" \
     "\n	-S	Create a system group" \

#define adduser_trivial_usage \
       "[OPTIONS] USER [GROUP]" \

#define adduser_full_usage "\n\n" \
       "Create new user, or add USER to GROUP\n" \
     "\n	-h DIR		Home directory" \
     "\n	-g GECOS	GECOS field" \
     "\n	-s SHELL	Login shell" \
     "\n	-G GRP		Group" \
     "\n	-S		Create a system user" \
     "\n	-D		Don't assign a password" \
     "\n	-H		Don't create home directory" \
     "\n	-u UID		User id" \
     "\n	-k SKEL		Skeleton directory (/etc/skel)" \

#define chpasswd_trivial_usage \
	IF_LONG_OPTS("[--md5|--encrypted|--crypt-method|--root]") IF_NOT_LONG_OPTS("[-m|-e|-c|-R]") \

#define chpasswd_full_usage "\n\n" \
       "Read user:password from stdin and update /etc/passwd\n" \
	IF_LONG_OPTS( \
     "\n	-e,--encrypted		Supplied passwords are in encrypted form" \
     "\n	-m,--md5		Encrypt using md5, not des" \
     "\n	-c,--crypt-method ALG	"CRYPT_METHODS_HELP_STR \
     "\n	-R,--root DIR		Directory to chroot into" \
	) \
	IF_NOT_LONG_OPTS( \
     "\n	-e	Supplied passwords are in encrypted form" \
     "\n	-m	Encrypt using md5, not des" \
     "\n	-c ALG	"CRYPT_METHODS_HELP_STR \
     "\n	-R DIR	Directory to chroot into" \
	) \

#define cryptpw_trivial_usage \
       "[OPTIONS] [PASSWORD] [SALT]" \

#define cryptpw_full_usage "\n\n" \
       "Print crypt(3) hashed PASSWORD\n" \
	IF_LONG_OPTS( \
     "\n	-P,--password-fd N	Read password from fd N" \
     "\n	-m,--method TYPE	"CRYPT_METHODS_HELP_STR \
     "\n	-S,--salt SALT" \
	) \
	IF_NOT_LONG_OPTS( \
     "\n	-P N	Read password from fd N" \
     "\n	-m TYPE	"CRYPT_METHODS_HELP_STR \
     "\n	-S SALT" \
	) \

#define deluser_trivial_usage \
       IF_LONG_OPTS("[--remove-home] ") "USER" \

#define deluser_full_usage "\n\n" \
       "Delete USER from the system" \

#define delgroup_trivial_usage \
	IF_FEATURE_DEL_USER_FROM_GROUP("[USER] ")"GROUP" \

#define delgroup_full_usage "\n\n" \
       "Delete group GROUP from the system" \
	IF_FEATURE_DEL_USER_FROM_GROUP(" or user USER from group GROUP") \

#define getty_trivial_usage \
       "[OPTIONS] BAUD_RATE[,BAUD_RATE]... TTY [TERMTYPE]" \

#define getty_full_usage "\n\n" \
       "Open TTY, prompt for login name, then invoke /bin/login\n" \
     "\n	-h		Enable hardware RTS/CTS flow control" \
     "\n	-L		Set CLOCAL (ignore Carrier Detect state)" \
     "\n	-m		Get baud rate from modem's CONNECT status message" \
     "\n	-n		Don't prompt for login name" \
     "\n	-w		Wait for CR or LF before sending /etc/issue" \
     "\n	-i		Don't display /etc/issue" \
     "\n	-f ISSUE_FILE	Display ISSUE_FILE instead of /etc/issue" \
     "\n	-l LOGIN	Invoke LOGIN instead of /bin/login" \
     "\n	-t SEC		Terminate after SEC if no login name is read" \
     "\n	-I INITSTR	Send INITSTR before anything else" \
     "\n	-H HOST		Log HOST into the utmp file as the hostname" \
     "\n" \
     "\nBAUD_RATE of 0 leaves it unchanged" \

#define login_trivial_usage \
       "[-p] [-h HOST] [[-f] USER]" \

#define login_full_usage "\n\n" \
       "Begin a new session on the system\n" \
     "\n	-f	Don't authenticate (user already authenticated)" \
     "\n	-h HOST	Host user came from (for network logins)" \
     "\n	-p	Preserve environment" \

#define passwd_trivial_usage \
       "[OPTIONS] [USER]" \

#define passwd_full_usage "\n\n" \
       "Change USER's password (default: current user)" \
     "\n" \
     "\n	-a ALG	"CRYPT_METHODS_HELP_STR \
     "\n	-d	Set password to ''" \
     "\n	-l	Lock (disable) account" \
     "\n	-u	Unlock (enable) account" \

#define su_trivial_usage \
       "[-lmp] [-] [-s SH] [USER [SCRIPT ARGS / -c 'CMD' ARG0 ARGS]]" \

#define su_full_usage "\n\n" \
       "Run shell under USER (by default, root)\n" \
     "\n	-,-l	Clear environment, go to home dir, run shell as login shell" \
     "\n	-p,-m	Do not set new $HOME, $SHELL, $USER, $LOGNAME" \
     "\n	-c CMD	Command to pass to 'sh -c'" \
     "\n	-s SH	Shell to use instead of user's default" \

#define sulogin_trivial_usage \
       "[-t N] [TTY]" \

#define sulogin_full_usage "\n\n" \
       "Single user login\n" \
     "\n	-t N	Timeout" \

#define vlock_trivial_usage \
       "[-a]" \

#define vlock_full_usage "\n\n" \
       "Lock a virtual terminal. A password is required to unlock.\n" \
     "\n	-a	Lock all VTs" \

#define makemime_trivial_usage \
       "[OPTIONS] [FILE]..." \

#define makemime_full_usage "\n\n" \
       "Create multipart MIME-encoded message from FILEs\n" \
     "\n	-o FILE	Output. Default: stdout" \
     "\n	-a HDR	Add header(s). Examples:" \
     "\n		\"From: user@host.org\", \"Date: `date -R`\"" \
     "\n	-c CT	Content type. Default: application/octet-stream" \
     "\n	-C CS	Charset. Default: " CONFIG_FEATURE_MIME_CHARSET \
     "\n" \
     "\nOther options are silently ignored" \

#define popmaildir_trivial_usage \
       "[OPTIONS] MAILDIR [CONN_HELPER ARGS]" \

#define popmaildir_full_usage "\n\n" \
       "Fetch content of remote mailbox to local maildir\n" \
     "\n	-s		Skip authorization" \
     "\n	-T		Get messages with TOP instead of RETR" \
     "\n	-k		Keep retrieved messages on the server" \
     "\n	-t SEC		Network timeout" \
	IF_FEATURE_POPMAILDIR_DELIVERY( \
     "\n	-F 'PROG ARGS'	Filter program (may be repeated)" \
     "\n	-M 'PROG ARGS'	Delivery program" \
	) \
     "\n" \
     "\nFetch from plain POP3 server:" \
     "\npopmaildir -k DIR nc pop3.server.com 110 <user_and_pass.txt" \
     "\nFetch from SSLed POP3 server and delete fetched emails:" \
     "\npopmaildir DIR -- openssl s_client -quiet -connect pop3.server.com:995 <user_and_pass.txt" \

#define popmaildir_example_usage \
       "$ popmaildir -k ~/Maildir -- nc pop.drvv.ru 110 [<password_file]\n" \
       "$ popmaildir ~/Maildir -- openssl s_client -quiet -connect pop.gmail.com:995 [<password_file]\n" \

#define reformime_trivial_usage \
       "[OPTIONS]" \

#define reformime_full_usage "\n\n" \
       "Parse MIME-encoded message on stdin\n" \
     "\n	-x PREFIX	Extract content of MIME sections to files" \
     "\n	-X PROG ARGS	Filter content of MIME sections through PROG" \
     "\n			Must be the last option" \
     "\n" \
     "\nOther options are silently ignored" \

#define sendmail_trivial_usage \
       "[-tv] [-f SENDER] [-amLOGIN 4<user_pass.txt | -auUSER -apPASS]" \
     "\n		[-w SECS] [-H 'PROG ARGS' | -S HOST] [RECIPIENT_EMAIL]..." \

#define sendmail_full_usage "\n\n" \
       "Read email from stdin and send it\n" \
     "\nStandard options:" \
     "\n	-t		Read additional recipients from message body" \
     "\n	-f SENDER	For use in MAIL FROM:<sender>. Can be empty string" \
     "\n			Default: -auUSER, or username of current UID" \
     "\n	-o OPTIONS	Various options. -oi implied, others are ignored" \
     "\n	-i		-oi synonym, implied and ignored" \
     "\n" \
     "\nBusybox specific options:" \
     "\n	-v		Verbose" \
     "\n	-w SECS		Network timeout" \
     "\n	-H 'PROG ARGS'	Run connection helper. Examples:" \
     "\n		openssl s_client -quiet -tls1 -starttls smtp -connect smtp.gmail.com:25" \
     "\n		openssl s_client -quiet -tls1 -connect smtp.gmail.com:465" \
     "\n			$SMTP_ANTISPAM_DELAY: seconds to wait after helper connect" \
     "\n	-S HOST[:PORT]	Server (default $SMTPHOST or 127.0.0.1)" \
     "\n	-amLOGIN	Log in using AUTH LOGIN" \
     "\n	-amPLAIN	or AUTH PLAIN" \
     "\n			(-amCRAM-MD5 not supported)" \
     "\n	-auUSER		Username for AUTH" \
     "\n	-apPASS 	Password for AUTH" \
     "\n" \
     "\nIf no -a options are given, authentication is not done." \
     "\nIf -amLOGIN is given but no -au/-ap, user/password is read from fd #4." \
     "\nOther options are silently ignored; -oi is implied." \
	IF_MAKEMIME( \
     "\nUse makemime to create emails with attachments." \
	) \

#define adjtimex_trivial_usage \
       "[-q] [-o OFF] [-f FREQ] [-p TCONST] [-t TICK]" \

#define adjtimex_full_usage "\n\n" \
       "Read or set kernel time variables. See adjtimex(2)\n" \
     "\n	-q	Quiet" \
     "\n	-o OFF	Time offset, microseconds" \
     "\n	-f FREQ	Frequency adjust, integer kernel units (65536 is 1ppm)" \
     "\n	-t TICK	Microseconds per tick, usually 10000" \
     "\n		(positive -t or -f values make clock run faster)" \
     "\n	-p TCONST" \

#define bbconfig_trivial_usage \
       "" \

#define bbconfig_full_usage "\n\n" \
       "Print the config file used by busybox build" \

#define bc_trivial_usage \
       "[-sqlw] FILE..." \

#define bc_full_usage "\n" \
     "\nArbitrary precision calculator" \
     "\n" \
     "\n	-q	Quiet" \
     "\n	-l	Load standard math library" \
     "\n	-s	Be POSIX compatible" \
     "\n	-w	Warn if extensions are used" \
     "\n" \
     "\n$BC_LINE_LENGTH changes output width" \

#define bc_example_usage \
       "3 + 4.129\n" \
       "1903 - 2893\n" \
       "-129 * 213.28935\n" \
       "12 / -1932\n" \
       "12 % 12\n" \
       "34 ^ 189\n" \
       "scale = 13\n" \
       "ibase = 2\n" \
       "obase = A\n" \

#define dc_trivial_usage \
       IF_FEATURE_DC_BIG("[-x] ")"[-eSCRIPT]... [-fFILE]... [FILE]..." \

#define dc_full_usage "\n" \
     "\nTiny RPN calculator. Operations:" \
     "\n+, -, *, /, %, ~, ^," IF_FEATURE_DC_BIG(" |,") \
     "\np - print top of the stack without popping" \
     "\nf - print entire stack" \
     "\nk - pop the value and set the precision" \
     "\ni - pop the value and set input radix" \
     "\no - pop the value and set output radix" \
     "\nExamples: dc -e'2 2 + p' -> 4, dc -e'8 8 * 2 2 + / p' -> 16" \

#define dc_example_usage \
       "$ dc -e'2 2 + p'\n" \
       "4\n" \
       "$ dc -e'8 8 \\* 2 2 + / p'\n" \
       "16\n" \
       "$ dc -e'0 1 & p'\n" \
       "0\n" \
       "$ dc -e'0 1 | p'\n" \
       "1\n" \
       "$ echo '72 9 / 8 * p' | dc\n" \
       "64\n" \

#define beep_trivial_usage \
       "-f FREQ -l LEN -d DELAY -r COUNT -n" \

#define beep_full_usage "\n\n" \
       "	-f	Frequency in Hz" \
     "\n	-l	Length in ms" \
     "\n	-d	Delay in ms" \
     "\n	-r	Repetitions" \
     "\n	-n	Start new tone" \

#define chat_trivial_usage \
       "EXPECT [SEND [EXPECT [SEND...]]]" \

#define chat_full_usage "\n\n" \
       "Useful for interacting with a modem connected to stdin/stdout.\n" \
       "A script consists of \"expect-send\" argument pairs.\n" \
       "Example:\n" \
       "chat '' ATZ OK ATD123456 CONNECT '' ogin: pppuser word: ppppass '~'" \

#define conspy_trivial_usage \
	"[-vcsndfFQ] [-x COL] [-y LINE] [CONSOLE_NO]" \

#define conspy_full_usage "\n\n" \
     "A text-mode VNC like program for Linux virtual consoles." \
     "\nTo exit, quickly press ESC 3 times." \
     "\n" \
     "\n	-v	Don't send keystrokes to the console" \
     "\n	-c	Create missing /dev/{tty,vcsa}N" \
     "\n	-s	Open a SHELL session" \
     "\n	-n	Black & white" \
     "\n	-d	Dump console to stdout" \
     "\n	-f	Follow cursor" \
     "\n	-F	Assume console is on a framebuffer device" \
     "\n	-Q	Disable exit on ESC-ESC-ESC" \
     "\n	-x COL	Starting column" \
     "\n	-y LINE	Starting line" \

#define crond_trivial_usage \
       "-fbS -l N " IF_FEATURE_CROND_D("-d N ") "-L LOGFILE -c DIR" \

#define crond_full_usage "\n\n" \
       "	-f	Foreground" \
     "\n	-b	Background (default)" \
     "\n	-S	Log to syslog (default)" \
     "\n	-l N	Set log level. Most verbose 0, default 8" \
	IF_FEATURE_CROND_D( \
     "\n	-d N	Set log level, log to stderr" \
	) \
     "\n	-L FILE	Log to FILE" \
     "\n	-c DIR	Cron dir. Default:"CONFIG_FEATURE_CROND_DIR"/crontabs" \

#define crontab_trivial_usage \
       "[-c DIR] [-u USER] [-ler]|[FILE]" \

#define crontab_full_usage "\n\n" \
       "	-c	Crontab directory" \
     "\n	-u	User" \
     "\n	-l	List crontab" \
     "\n	-e	Edit crontab" \
     "\n	-r	Delete crontab" \
     "\n	FILE	Replace crontab by FILE ('-': stdin)" \

#define devfsd_trivial_usage \
       "mntpnt [-v]" IF_DEVFSD_FG_NP("[-fg][-np]") \

#define devfsd_full_usage "\n\n" \
       "Manage devfs permissions and old device name symlinks\n" \
     "\n	mntpnt	The mount point where devfs is mounted" \
     "\n	-v	Print the protocol version numbers for devfsd" \
     "\n		and the kernel-side protocol version and exit" \
	IF_DEVFSD_FG_NP( \
     "\n	-fg	Run in foreground" \
     "\n	-np	Exit after parsing the configuration file" \
     "\n		and processing synthetic REGISTER events," \
     "\n		don't poll for events" \
	) \

#define devmem_trivial_usage \
	"ADDRESS [WIDTH [VALUE]]" \

#define devmem_full_usage "\n\n" \
       "Read/write from physical address\n" \
     "\n	ADDRESS	Address to act upon" \
     "\n	WIDTH	Width (8/16/...)" \
     "\n	VALUE	Data to be written" \

#define fbsplash_trivial_usage \
       "-s IMGFILE [-c] [-d DEV] [-i INIFILE] [-f CMD]" \

#define fbsplash_full_usage "\n\n" \
       "	-s	Image" \
     "\n	-c	Hide cursor" \
     "\n	-d	Framebuffer device (default /dev/fb0)" \
     "\n	-i	Config file (var=value):" \
     "\n			BAR_LEFT,BAR_TOP,BAR_WIDTH,BAR_HEIGHT" \
     "\n			BAR_R,BAR_G,BAR_B,IMG_LEFT,IMG_TOP" \
     "\n	-f	Control pipe (else exit after drawing image)" \
     "\n			commands: 'NN' (% for progress bar) or 'exit'" \

#define flash_eraseall_trivial_usage \
       "[-jNq] MTD_DEVICE" \

#define flash_eraseall_full_usage "\n\n" \
       "Erase an MTD device\n" \
     "\n	-j	Format the device for jffs2" \
     "\n	-N	Don't skip bad blocks" \
     "\n	-q	Don't display progress messages" \

#define flash_lock_trivial_usage \
       "MTD_DEVICE OFFSET SECTORS" \

#define flash_lock_full_usage "\n\n" \
       "Lock part or all of an MTD device. If SECTORS is -1, then all sectors\n" \
       "will be locked, regardless of the value of OFFSET" \

#define flash_unlock_trivial_usage \
       "MTD_DEVICE" \

#define flash_unlock_full_usage "\n\n" \
       "Unlock an MTD device" \

#define flashcp_trivial_usage \
       "-v FILE MTD_DEVICE" \

#define flashcp_full_usage "\n\n" \
       "Copy an image to MTD device\n" \
     "\n	-v	Verbose" \

#define hdparm_trivial_usage \
       "[OPTIONS] [DEVICE]" \

#define hdparm_full_usage "\n\n" \
       "	-a	Get/set fs readahead" \
     "\n	-A	Set drive read-lookahead flag (0/1)" \
     "\n	-b	Get/set bus state (0 == off, 1 == on, 2 == tristate)" \
     "\n	-B	Set Advanced Power Management setting (1-255)" \
     "\n	-c	Get/set IDE 32-bit IO setting" \
     "\n	-C	Check IDE power mode status" \
	IF_FEATURE_HDPARM_HDIO_GETSET_DMA( \
     "\n	-d	Get/set using_dma flag") \
     "\n	-D	Enable/disable drive defect-mgmt" \
     "\n	-f	Flush buffer cache for device on exit" \
     "\n	-g	Display drive geometry" \
     "\n	-h	Display terse usage information" \
	IF_FEATURE_HDPARM_GET_IDENTITY( \
     "\n	-i	Display drive identification") \
	IF_FEATURE_HDPARM_GET_IDENTITY( \
     "\n	-I	Detailed/current information directly from drive") \
     "\n	-k	Get/set keep_settings_over_reset flag (0/1)" \
     "\n	-K	Set drive keep_features_over_reset flag (0/1)" \
     "\n	-L	Set drive doorlock (0/1) (removable harddisks only)" \
     "\n	-m	Get/set multiple sector count" \
     "\n	-n	Get/set ignore-write-errors flag (0/1)" \
     "\n	-p	Set PIO mode on IDE interface chipset (0,1,2,3,4,...)" \
     "\n	-P	Set drive prefetch count" \
     "\n	-Q	Get/set DMA tagged-queuing depth (if supported)" \
     "\n	-r	Get/set readonly flag (DANGEROUS to set)" \
	IF_FEATURE_HDPARM_HDIO_SCAN_HWIF( \
     "\n	-R	Register an IDE interface (DANGEROUS)") \
     "\n	-S	Set standby (spindown) timeout" \
     "\n	-t	Perform device read timings" \
     "\n	-T	Perform cache read timings" \
     "\n	-u	Get/set unmaskirq flag (0/1)" \
	IF_FEATURE_HDPARM_HDIO_UNREGISTER_HWIF( \
     "\n	-U	Unregister an IDE interface (DANGEROUS)") \
     "\n	-v	Defaults; same as -mcudkrag for IDE drives" \
     "\n	-V	Display program version and exit immediately" \
	IF_FEATURE_HDPARM_HDIO_DRIVE_RESET( \
     "\n	-w	Perform device reset (DANGEROUS)") \
     "\n	-W	Set drive write-caching flag (0/1) (DANGEROUS)" \
	IF_FEATURE_HDPARM_HDIO_TRISTATE_HWIF( \
     "\n	-x	Tristate device for hotswap (0/1) (DANGEROUS)") \
     "\n	-X	Set IDE xfer mode (DANGEROUS)" \
     "\n	-y	Put IDE drive in standby mode" \
     "\n	-Y	Put IDE drive to sleep" \
     "\n	-Z	Disable Seagate auto-powersaving mode" \
     "\n	-z	Reread partition table" \

#define hexedit_trivial_usage \
	"FILE" \

#define hexedit_full_usage "\n\n" \
	"Edit FILE in hexadecimal" \

#define i2cget_trivial_usage \
       "[-fy] BUS CHIP-ADDRESS [DATA-ADDRESS [MODE]]" \

#define i2cget_full_usage "\n\n" \
       "Read from I2C/SMBus chip registers" \
     "\n" \
     "\n	I2CBUS	I2C bus number" \
     "\n	ADDRESS	0x03-0x77" \
     "\nMODE is:" \
     "\n	b	Read byte data (default)" \
     "\n	w	Read word data" \
     "\n	c	Write byte/read byte" \
     "\n	Append p for SMBus PEC" \
     "\n" \
     "\n	-f	Force access" \
     "\n	-y	Disable interactive mode" \

#define i2cset_trivial_usage \
       "[-fy] [-m MASK] BUS CHIP-ADDRESS DATA-ADDRESS [VALUE] ... [MODE]" \

#define i2cset_full_usage "\n\n" \
       "Set I2C registers" \
     "\n" \
     "\n	I2CBUS	I2C bus number" \
     "\n	ADDRESS	0x03-0x77" \
     "\nMODE is:" \
     "\n	c	Byte, no value" \
     "\n	b	Byte data (default)" \
     "\n	w	Word data" \
     "\n	i	I2C block data" \
     "\n	s	SMBus block data" \
     "\n	Append p for SMBus PEC" \
     "\n" \
     "\n	-f	Force access" \
     "\n	-y	Disable interactive mode" \
     "\n	-r	Read back and compare the result" \
     "\n	-m MASK	Mask specifying which bits to write" \

#define i2cdump_trivial_usage \
       "[-fy] [-r FIRST-LAST] BUS ADDR [MODE]" \

#define i2cdump_full_usage "\n\n" \
       "Examine I2C registers" \
     "\n" \
     "\n	I2CBUS	I2C bus number" \
     "\n	ADDRESS	0x03-0x77" \
     "\nMODE is:" \
     "\n	b	Byte (default)" \
     "\n	w	Word" \
     "\n	W	Word on even register addresses" \
     "\n	i	I2C block" \
     "\n	s	SMBus block" \
     "\n	c	Consecutive byte" \
     "\n	Append p for SMBus PEC" \
     "\n" \
     "\n	-f	Force access" \
     "\n	-y	Disable interactive mode" \
     "\n	-r	Limit the number of registers being accessed" \

#define i2cdetect_trivial_usage \
       "-l | -F I2CBUS | [-ya] [-q|-r] I2CBUS [FIRST LAST]" \

#define i2cdetect_full_usage "\n\n" \
       "Detect I2C chips" \
     "\n" \
     "\n	-l	List installed buses" \
     "\n	-F BUS#	List functionalities on this bus" \
     "\n	-y	Disable interactive mode" \
     "\n	-a	Force scanning of non-regular addresses" \
     "\n	-q	Use smbus quick write commands for probing (default)" \
     "\n	-r	Use smbus read byte commands for probing" \
     "\n	FIRST and LAST limit probing range" \

#define inotifyd_trivial_usage \
	"PROG FILE1[:MASK]..." \

#define inotifyd_full_usage "\n\n" \
       "Run PROG on filesystem changes." \
     "\nWhen a filesystem event matching MASK occurs on FILEn," \
     "\nPROG ACTUAL_EVENTS FILEn [SUBFILE] is run." \
     "\nIf PROG is -, events are sent to stdout." \
     "\nEvents:" \
     "\n	a	File is accessed" \
     "\n	c	File is modified" \
     "\n	e	Metadata changed" \
     "\n	w	Writable file is closed" \
     "\n	0	Unwritable file is closed" \
     "\n	r	File is opened" \
     "\n	D	File is deleted" \
     "\n	M	File is moved" \
     "\n	u	Backing fs is unmounted" \
     "\n	o	Event queue overflowed" \
     "\n	x	File can't be watched anymore" \
     "\nIf watching a directory:" \
     "\n	y	Subfile is moved into dir" \
     "\n	m	Subfile is moved out of dir" \
     "\n	n	Subfile is created" \
     "\n	d	Subfile is deleted" \
     "\n" \
     "\ninotifyd waits for PROG to exit." \
     "\nWhen x event happens for all FILEs, inotifyd exits." \

#define less_trivial_usage \
       "[-EF" IF_FEATURE_LESS_REGEXP("I")IF_FEATURE_LESS_FLAGS("Mm") \
       "N" IF_FEATURE_LESS_TRUNCATE("S") IF_FEATURE_LESS_RAW("R") "h~] [FILE]..." \

#define less_full_usage "\n\n" \
       "View FILE (or stdin) one screenful at a time\n" \
     "\n	-E	Quit once the end of a file is reached" \
     "\n	-F	Quit if entire file fits on first screen" \
	IF_FEATURE_LESS_REGEXP( \
     "\n	-I	Ignore case in all searches" \
	) \
	IF_FEATURE_LESS_FLAGS( \
     "\n	-M,-m	Display status line with line numbers" \
     "\n		and percentage through the file" \
	) \
     "\n	-N	Prefix line number to each line" \
	IF_FEATURE_LESS_TRUNCATE( \
     "\n	-S	Truncate long lines" \
	) \
	IF_FEATURE_LESS_RAW( \
     "\n	-R	Remove color escape codes in input" \
	) \
     "\n	-~	Suppress ~s displayed past EOF" \

#define lsscsi_trivial_usage NOUSAGE_STR \

#define lsscsi_full_usage "" \

#if ENABLE_FEATURE_MAKEDEVS_LEAF \

#define makedevs_trivial_usage \
       "NAME TYPE MAJOR MINOR FIRST LAST [s]" \

#define makedevs_full_usage "\n\n" \
       "Create a range of block or character special files" \
     "\n" \
     "\nTYPE is:" \
     "\n	b	Block device" \
     "\n	c	Character device" \
     "\n	f	FIFO, MAJOR and MINOR are ignored" \
     "\n" \
     "\nFIRST..LAST specify numbers appended to NAME." \
     "\nIf 's' is the last argument, the base device is created as well." \
     "\n" \
     "\nExamples:" \
     "\n	makedevs /dev/ttyS c 4 66 2 63   ->  ttyS2-ttyS63" \
     "\n	makedevs /dev/hda b 3 0 0 8 s    ->  hda,hda1-hda8" \

#define makedevs_example_usage \
       "# makedevs /dev/ttyS c 4 66 2 63\n" \
       "[creates ttyS2-ttyS63]\n" \
       "# makedevs /dev/hda b 3 0 0 8 s\n" \
       "[creates hda,hda1-hda8]\n" \

#endif \

#if ENABLE_FEATURE_MAKEDEVS_TABLE \

#define makedevs_trivial_usage \
       "[-d device_table] rootdir" \

#define makedevs_full_usage "\n\n" \
       "Create a range of special files as specified in a device table.\n" \
       "Device table entries take the form of:\n" \
       "<name> <type> <mode> <uid> <gid> <major> <minor> <start> <inc> <count>\n" \
       "Where name is the file name, type can be one of:\n" \
       "	f	Regular file\n" \
       "	d	Directory\n" \
       "	c	Character device\n" \
       "	b	Block device\n" \
       "	p	Fifo (named pipe)\n" \
       "uid is the user id for the target file, gid is the group id for the\n" \
       "target file. The rest of the entries (major, minor, etc) apply to\n" \
       "to device special files. A '-' may be used for blank entries." \

#define makedevs_example_usage \
       "For example:\n" \
       "<name>    <type> <mode><uid><gid><major><minor><start><inc><count>\n" \
       "/dev         d   755    0    0    -      -      -      -    -\n" \
       "/dev/console c   666    0    0    5      1      -      -    -\n" \
       "/dev/null    c   666    0    0    1      3      0      0    -\n" \
       "/dev/zero    c   666    0    0    1      5      0      0    -\n" \
       "/dev/hda     b   640    0    0    3      0      0      0    -\n" \
       "/dev/hda     b   640    0    0    3      1      1      1    15\n\n" \
       "Will Produce:\n" \
       "/dev\n" \
       "/dev/console\n" \
       "/dev/null\n" \
       "/dev/zero\n" \
       "/dev/hda\n" \
       "/dev/hda[0-15]\n" \

#endif \

#define man_trivial_usage \
       "[-aw] [MANPAGE]..." \

#define man_full_usage "\n\n" \
       "Format and display manual page\n" \
     "\n	-a	Display all pages" \
     "\n	-w	Show page locations" \
     "\n" \
     "\n$COLUMNS overrides output width" \

#define microcom_trivial_usage \
       "[-d DELAY] [-t TIMEOUT] [-s SPEED] [-X] TTY" \

#define microcom_full_usage "\n\n" \
       "Copy bytes for stdin to TTY and from TTY to stdout\n" \
     "\n	-d	Wait up to DELAY ms for TTY output before sending every" \
     "\n		next byte to it" \
     "\n	-t	Exit if both stdin and TTY are silent for TIMEOUT ms" \
     "\n	-s	Set serial line to SPEED" \
     "\n	-X	Disable special meaning of NUL and Ctrl-X from stdin" \

#define mt_trivial_usage \
       "[-f device] opcode value" \

#define mt_full_usage "\n\n" \
       "Control magnetic tape drive operation\n" \
       "\n" \
       "Available Opcodes:\n" \
       "\n" \
       "bsf bsfm bsr bss datacompression drvbuffer eof eom erase\n" \
       "fsf fsfm fsr fss load lock mkpart nop offline ras1 ras2\n" \
       "ras3 reset retension rewind rewoffline seek setblk setdensity\n" \
       "setpart tell unload unlock weof wset" \

#define nandwrite_trivial_usage \
	"[-np] [-s ADDR] MTD_DEVICE [FILE]" \

#define nandwrite_full_usage "\n\n" \
	"Write to MTD_DEVICE\n" \
     "\n	-n	Write without ecc" \
     "\n	-p	Pad to page size" \
     "\n	-s ADDR	Start address" \

#define nanddump_trivial_usage \
	"[-no]" IF_LONG_OPTS(" [--bb padbad|skipbad]") " [-s ADDR] [-l LEN] [-f FILE] MTD_DEVICE" \

#define nanddump_full_usage "\n\n" \
	"Dump MTD_DEVICE\n" \
     "\n	-n	Read without ecc" \
     "\n	-o	Dump oob data" \
     "\n	-s ADDR	Start address" \
     "\n	-l LEN	Length" \
     "\n	-f FILE	Dump to file ('-' for stdout)" \
     IF_LONG_OPTS( \
     "\n	--bb METHOD" \
     "\n		skipbad: skip bad blocks" \
     "\n		padbad: substitute bad blocks by 0xff (default)" \
     ) \

#define partprobe_trivial_usage \
	"DEVICE..." \

#define partprobe_full_usage "\n\n" \
	"Ask kernel to rescan partition table" \

#define raidautorun_trivial_usage \
       "DEVICE" \

#define raidautorun_full_usage "\n\n" \
       "Tell the kernel to automatically search and start RAID arrays" \

#define raidautorun_example_usage \
       "$ raidautorun /dev/md0" \

#define readahead_trivial_usage \
       "[FILE]..." \

#define readahead_full_usage "\n\n" \
       "Preload FILEs to RAM" \

#define rfkill_trivial_usage \
       "COMMAND [INDEX|TYPE]" \

#define rfkill_full_usage "\n\n" \
       "Enable/disable wireless devices\n" \
       "\nCommands:" \
     "\n	list [INDEX|TYPE]	List current state" \
     "\n	block INDEX|TYPE	Disable device" \
     "\n	unblock INDEX|TYPE	Enable device" \
     "\n" \
     "\n	TYPE: all, wlan(wifi), bluetooth, uwb(ultrawideband)," \
     "\n		wimax, wwan, gps, fm" \

#define runlevel_trivial_usage \
       "[FILE]" \

#define runlevel_full_usage "\n\n" \
       "Find the current and previous system runlevel\n" \
       "\n" \
       "If no utmp FILE exists or if no runlevel record can be found,\n" \
       "print \"unknown\"" \

#define runlevel_example_usage \
       "$ runlevel /var/run/utmp\n" \
       "N 2" \

#define rx_trivial_usage \
       "FILE" \

#define rx_full_usage "\n\n" \
       "Receive a file using the xmodem protocol" \

#define rx_example_usage \
       "$ rx /tmp/foo\n" \

#define setfattr_trivial_usage \
       "[-h] -n|-x ATTR [-v VALUE] FILE..." \

#define setfattr_full_usage "\n\n" \
       "Set extended attributes" \
     "\n" \
     "\n	-h		Do not follow symlinks" \
     "\n	-x ATTR		Remove attribute ATTR" \
     "\n	-n ATTR		Set attribute ATTR to VALUE" \
     "\n	-v VALUE	(default: empty)" \

#define setserial_trivial_usage \
	"[-abGvz] { DEVICE [PARAMETER [ARG]]... | -g DEVICE... }" \

#define setserial_full_usage "\n\n" \
	"Print or set serial port parameters" \
   "\n" \
   "\n""	-a	Print all" \
   "\n""	-b	Print summary" \
   "\n""	-G	Print as setserial PARAMETERs" \
   "\n""	-v	Verbose" \
   "\n""	-z	Zero out serial flags before setting" \
   "\n""	-g	All args are device names" \
   "\n" \
   "\n""PARAMETERs: (* = takes ARG, ^ = can be turned off by preceding ^)" \
   "\n""	*port, *irq, *divisor, *uart, *baud_base, *close_delay, *closing_wait," \
   "\n""	^fourport, ^auto_irq, ^skip_test, ^sak, ^session_lockout, ^pgrp_lockout," \
   "\n""	^callout_nohup, ^split_termios, ^hup_notify, ^low_latency, autoconfig," \
   "\n""	spd_normal, spd_hi, spd_vhi, spd_shi, spd_warp, spd_cust" \
   "\n""ARG for uart:" \
   "\n""	unknown, 8250, 16450, 16550, 16550A, Cirrus, 16650, 16650V2, 16750," \
   "\n""	16950, 16954, 16654, 16850, RSA, NS16550A, XSCALE, RM9000, OCTEON, AR7," \
   "\n""	U6_16550A" \

#define strings_trivial_usage \
       "[-fo] [-t o/d/x] [-n LEN] [FILE]..." \

#define strings_full_usage "\n\n" \
       "Display printable strings in a binary file\n" \
     "\n	-f		Precede strings with filenames" \
     "\n	-o		Precede strings with octal offsets" \
     "\n	-t o/d/x	Precede strings with offsets in base 8/10/16" \
     "\n	-n LEN		At least LEN characters form a string (default 4)" \

#define time_trivial_usage \
       "[-vpa] [-o FILE] PROG ARGS" \

#define time_full_usage "\n\n" \
       "Run PROG, display resource usage when it exits\n" \
     "\n	-v	Verbose" \
     "\n	-p	POSIX output format" \
     "\n	-f FMT	Custom format" \
     "\n	-o FILE	Write result to FILE" \
     "\n	-a	Append (else overwrite)" \

#define ttysize_trivial_usage \
       "[w] [h]" \

#define ttysize_full_usage "\n\n" \
       "Print dimensions of stdin tty, or 80x24" \

#define ubiattach_trivial_usage \
       "-m MTD_NUM [-d UBI_NUM] [-O VID_HDR_OFF] UBI_CTRL_DEV" \

#define ubiattach_full_usage "\n\n" \
       "Attach MTD device to UBI\n" \
     "\n	-m MTD_NUM	MTD device number to attach" \
     "\n	-d UBI_NUM	UBI device number to assign" \
     "\n	-O VID_HDR_OFF	VID header offset" \

#define ubidetach_trivial_usage \
       "-d UBI_NUM UBI_CTRL_DEV" \

#define ubidetach_full_usage "\n\n" \
       "Detach MTD device from UBI\n" \
     "\n	-d UBI_NUM	UBI device number" \

#define ubimkvol_trivial_usage \
       "-N NAME [-s SIZE | -m] UBI_DEVICE" \

#define ubimkvol_full_usage "\n\n" \
       "Create UBI volume\n" \
     "\n	-a ALIGNMENT	Volume alignment (default 1)" \
     "\n	-m		Set volume size to maximum available" \
     "\n	-n VOLID	Volume ID. If not specified," \
     "\n			assigned automatically" \
     "\n	-N NAME		Volume name" \
     "\n	-s SIZE		Size in bytes" \
     "\n	-t TYPE		Volume type (static|dynamic)" \

#define ubirmvol_trivial_usage \
       "-n VOLID / -N VOLNAME UBI_DEVICE" \

#define ubirmvol_full_usage "\n\n" \
       "Remove UBI volume\n" \
     "\n	-n VOLID	Volume ID" \
     "\n	-N VOLNAME	Volume name" \

#define ubirsvol_trivial_usage \
       "-n VOLID -s SIZE UBI_DEVICE" \

#define ubirsvol_full_usage "\n\n" \
       "Resize UBI volume\n" \
     "\n	-n VOLID	Volume ID" \
     "\n	-s SIZE		Size in bytes" \

#define ubiupdatevol_trivial_usage \
       "-t UBI_DEVICE | [-s SIZE] UBI_DEVICE IMG_FILE" \

#define ubiupdatevol_full_usage "\n\n" \
       "Update UBI volume\n" \
     "\n	-t	Truncate to zero size" \
     "\n	-s SIZE	Size in bytes to resize to" \

#define ubirename_trivial_usage \
	"UBI_DEVICE OLD_VOLNAME NEW_VOLNAME [OLD2 NEW2]..." \

#define ubirename_full_usage "\n\n" \
	"Rename UBI volumes on UBI_DEVICE" \

#define volname_trivial_usage \
       "[DEVICE]" \

#define volname_full_usage "\n\n" \
       "Show CD volume name of the DEVICE (default /dev/cdrom)" \

#define watchdog_trivial_usage \
       "[-t N[ms]] [-T N[ms]] [-F] DEV" \

#define watchdog_full_usage "\n\n" \
       "Periodically write to watchdog device DEV\n" \
     "\n	-T N	Reboot after N seconds if not reset (default 60)" \
     "\n	-t N	Reset every N seconds (default 30)" \
     "\n	-F	Run in foreground" \
     "\n" \
     "\nUse 500ms to specify period in milliseconds" \

#if !ENABLE_MODPROBE_SMALL \

#define depmod_trivial_usage "[-n] [-b BASE] [VERSION] [MODFILES]..." \

#define depmod_full_usage "\n\n" \
       "Generate modules.dep, alias, and symbols files" \
     "\n" \
     "\n	-b BASE	Use BASE/lib/modules/VERSION" \
     "\n	-n	Dry run: print files to stdout" \

#endif \

#if !ENABLE_MODPROBE_SMALL \

#define insmod_trivial_usage \
	IF_FEATURE_2_4_MODULES("[OPTIONS] MODULE") \
	IF_NOT_FEATURE_2_4_MODULES("FILE") \
	IF_FEATURE_CMDLINE_MODULE_OPTIONS(" [SYMBOL=VALUE]...") \

#define insmod_full_usage "\n\n" \
       "Load kernel module" \
	IF_FEATURE_2_4_MODULES( "\n" \
     "\n	-f	Force module to load into the wrong kernel version" \
     "\n	-k	Make module autoclean-able" \
     "\n	-v	Verbose" \
     "\n	-q	Quiet" \
     "\n	-L	Lock: prevent simultaneous loads" \
	IF_FEATURE_INSMOD_LOAD_MAP( \
     "\n	-m	Output load map to stdout" \
	) \
     "\n	-x	Don't export externs" \
	) \

#endif \

#if !ENABLE_MODPROBE_SMALL \

#define lsmod_trivial_usage \
       "" \

#define lsmod_full_usage "\n\n" \
       "List loaded kernel modules" \

#endif \

#define modinfo_trivial_usage \
       "[-adlpn0] [-F keyword] MODULE" \

#define modinfo_full_usage "\n\n" \
       "	-a		Shortcut for '-F author'" \
     "\n	-d		Shortcut for '-F description'" \
     "\n	-l		Shortcut for '-F license'" \
     "\n	-p		Shortcut for '-F parm'" \
     "\n	-F keyword	Keyword to look for" \
     "\n	-0		Separate output with NULs" \

#define modinfo_example_usage \
       "$ modinfo -F vermagic loop\n" \

#if ENABLE_MODPROBE_SMALL \

#define lsmod_trivial_usage \
       "" \

#define lsmod_full_usage "\n\n" \
       "List loaded kernel modules" \

#endif \

#if ENABLE_MODPROBE_SMALL \

#define depmod_trivial_usage "[-n]" \

#define depmod_full_usage "\n\n" \
       "Generate modules.dep.bb" \
     "\n" \
     "\n	-n	Dry run: print file to stdout" \

#define insmod_trivial_usage \
	"FILE" IF_FEATURE_CMDLINE_MODULE_OPTIONS(" [SYMBOL=VALUE]...") \

#define insmod_full_usage "\n\n" \
       "Load kernel module" \

#define rmmod_trivial_usage \
       "MODULE..." \

#define rmmod_full_usage "\n\n" \
       "Unload kernel modules" \

#define modprobe_trivial_usage \
	"[-rq] MODULE" IF_FEATURE_CMDLINE_MODULE_OPTIONS(" [SYMBOL=VALUE]...") \

#define modprobe_full_usage "\n\n" \
       "	-r	Remove MODULE" \
     "\n	-q	Quiet" \

#endif \

#if !ENABLE_MODPROBE_SMALL \

#define modprobe_notes_usage \
	"modprobe can (un)load a stack of modules, passing each module options (when\n" \
	"loading). modprobe uses a configuration file to determine what option(s) to\n" \
	"pass each module it loads.\n" \
	"\n" \
	"The configuration file is searched (in this order):\n" \
	"\n" \
	"    /etc/modprobe.conf (2.6 only)\n" \
	"    /etc/modules.conf\n" \
	"    /etc/conf.modules (deprecated)\n" \
	"\n" \
	"They all have the same syntax (see below). If none is present, it is\n" \
	"_not_ an error; each loaded module is then expected to load without\n" \
	"options. Once a file is found, the others are tested for.\n" \
	"\n" \
	"/etc/modules.conf entry format:\n" \
	"\n" \
	"  alias <alias_name> <mod_name>\n" \
	"    Makes it possible to modprobe alias_name, when there is no such module.\n" \
	"    It makes sense if your mod_name is long, or you want a more representative\n" \
	"    name for that module (eg. 'scsi' in place of 'aha7xxx').\n" \
	"    This makes it also possible to use a different set of options (below) for\n" \
	"    the module and the alias.\n" \
	"    A module can be aliased more than once.\n" \
	"\n" \
	"  options <mod_name|alias_name> <symbol=value...>\n" \
	"    When loading module mod_name (or the module aliased by alias_name), pass\n" \
	"    the \"symbol=value\" pairs as option to that module.\n" \
	"\n" \
	"Sample /etc/modules.conf file:\n" \
	"\n" \
	"  options tulip irq=3\n" \
	"  alias tulip tulip2\n" \
	"  options tulip2 irq=4 io=0x308\n" \
	"\n" \
	"Other functionality offered by 'classic' modprobe is not available in\n" \
	"this implementation.\n" \
	"\n" \
	"If module options are present both in the config file, and on the command line,\n" \
	"then the options from the command line will be passed to the module _after_\n" \
	"the options from the config file. That way, you can have defaults in the config\n" \
	"file, and override them for a specific usage from the command line.\n" \

#define modprobe_example_usage \
       "(with the above /etc/modules.conf):\n\n" \
       "$ modprobe tulip\n" \
       "   will load the module 'tulip' with default option 'irq=3'\n\n" \
       "$ modprobe tulip irq=5\n" \
       "   will load the module 'tulip' with option 'irq=5', thus overriding the default\n\n" \
       "$ modprobe tulip2\n" \
       "   will load the module 'tulip' with default options 'irq=4 io=0x308',\n" \
       "   which are the default for alias 'tulip2'\n\n" \
       "$ modprobe tulip2 irq=8\n" \
       "   will load the module 'tulip' with default options 'irq=4 io=0x308 irq=8',\n" \
       "   which are the default for alias 'tulip2' overridden by the option 'irq=8'\n\n" \
       "   from the command line\n\n" \
       "$ modprobe tulip2 irq=2 io=0x210\n" \
       "   will load the module 'tulip' with default options 'irq=4 io=0x308 irq=4 io=0x210',\n" \
       "   which are the default for alias 'tulip2' overridden by the options 'irq=2 io=0x210'\n\n" \
       "   from the command line\n" \

#define modprobe_trivial_usage \
	"[-alrqvsD" IF_FEATURE_MODPROBE_BLACKLIST("b") "]" \
	" MODULE" IF_FEATURE_CMDLINE_MODULE_OPTIONS(" [SYMBOL=VALUE]...") \

#define modprobe_full_usage "\n\n" \
       "	-a	Load multiple MODULEs" \
     "\n	-l	List (MODULE is a pattern)" \
     "\n	-r	Remove MODULE (stacks) or do autoclean" \
     "\n	-q	Quiet" \
     "\n	-v	Verbose" \
     "\n	-s	Log to syslog" \
     "\n	-D	Show dependencies" \
	IF_FEATURE_MODPROBE_BLACKLIST( \
     "\n	-b	Apply blacklist to module names too" \
	) \

#endif /* !ENABLE_MODPROBE_SMALL */ \

#if !ENABLE_MODPROBE_SMALL \

#define rmmod_trivial_usage \
       "[-wfa] [MODULE]..." \

#define rmmod_full_usage "\n\n" \
       "Unload kernel modules\n" \
     "\n	-w	Wait until the module is no longer used" \
     "\n	-f	Force unload" \
     "\n	-a	Remove all unused modules (recursively)" \

#define rmmod_example_usage \
       "$ rmmod tulip\n" \

#endif \

#define arp_trivial_usage \
     "\n[-vn]	[-H HWTYPE] [-i IF] -a [HOSTNAME]" \
     "\n[-v]		    [-i IF] -d HOSTNAME [pub]" \
     "\n[-v]	[-H HWTYPE] [-i IF] -s HOSTNAME HWADDR [temp]" \
     "\n[-v]	[-H HWTYPE] [-i IF] -s HOSTNAME HWADDR [netmask MASK] pub" \
     "\n[-v]	[-H HWTYPE] [-i IF] -Ds HOSTNAME IFACE [netmask MASK] pub" \

#define arp_full_usage "\n\n" \
       "Manipulate ARP cache\n" \
       "\n	-a		Display (all) hosts" \
       "\n	-d		Delete ARP entry" \
       "\n	-s		Set new entry" \
       "\n	-v		Verbose" \
       "\n	-n		Don't resolve names" \
       "\n	-i IF		Network interface" \
       "\n	-D		Read HWADDR from IFACE" \
       "\n	-A,-p AF	Protocol family" \
       "\n	-H HWTYPE	Hardware address type" \

#define arping_trivial_usage \
       "[-fqbDUA] [-c CNT] [-w TIMEOUT] [-I IFACE] [-s SRC_IP] DST_IP" \

#define arping_full_usage "\n\n" \
       "Send ARP requests/replies\n" \
     "\n	-f		Quit on first ARP reply" \
     "\n	-q		Quiet" \
     "\n	-b		Keep broadcasting, don't go unicast" \
     "\n	-D		Exit with 1 if DST_IP replies" \
     "\n	-U		Unsolicited ARP mode, update your neighbors" \
     "\n	-A		ARP answer mode, update your neighbors" \
     "\n	-c N		Stop after sending N ARP requests" \
     "\n	-w TIMEOUT	Seconds to wait for ARP reply" \
     "\n	-I IFACE	Interface to use (default eth0)" \
     "\n	-s SRC_IP	Sender IP address" \
     "\n	DST_IP		Target IP address" \

#define brctl_trivial_usage \
       "COMMAND [BRIDGE [INTERFACE]]" \

#define brctl_full_usage "\n\n" \
       "Manage ethernet bridges\n" \
     "\nCommands:" \
	IF_FEATURE_BRCTL_SHOW( \
     "\n	show			Show a list of bridges" \
	) \
     "\n	addbr BRIDGE		Create BRIDGE" \
     "\n	delbr BRIDGE		Delete BRIDGE" \
     "\n	addif BRIDGE IFACE	Add IFACE to BRIDGE" \
     "\n	delif BRIDGE IFACE	Delete IFACE from BRIDGE" \
	IF_FEATURE_BRCTL_FANCY( \
     "\n	setageing BRIDGE TIME		Set ageing time" \
     "\n	setfd BRIDGE TIME		Set bridge forward delay" \
     "\n	sethello BRIDGE TIME		Set hello time" \
     "\n	setmaxage BRIDGE TIME		Set max message age" \
     "\n	setpathcost BRIDGE COST		Set path cost" \
     "\n	setportprio BRIDGE PRIO		Set port priority" \
     "\n	setbridgeprio BRIDGE PRIO	Set bridge priority" \
     "\n	stp BRIDGE [1/yes/on|0/no/off]	STP on/off" \
	) \

#define dnsd_trivial_usage \
       "[-dvs] [-c CONFFILE] [-t TTL_SEC] [-p PORT] [-i ADDR]" \

#define dnsd_full_usage "\n\n" \
       "Small static DNS server daemon\n" \
     "\n	-c FILE	Config file" \
     "\n	-t SEC	TTL" \
     "\n	-p PORT	Listen on PORT" \
     "\n	-i ADDR	Listen on ADDR" \
     "\n	-d	Daemonize" \
     "\n	-v	Verbose" \
     "\n	-s	Send successful replies only. Use this if you want" \
     "\n		to use /etc/resolv.conf with two nameserver lines:" \
     "\n			nameserver DNSD_SERVER" \
     "\n			nameserver NORMAL_DNS_SERVER" \

#define ether_wake_trivial_usage \
       "[-b] [-i IFACE] [-p aa:bb:cc:dd[:ee:ff]/a.b.c.d] MAC" \

#define ether_wake_full_usage "\n\n" \
       "Send a magic packet to wake up sleeping machines.\n" \
       "MAC must be a station address (00:11:22:33:44:55) or\n" \
       "a hostname with a known 'ethers' entry.\n" \
     "\n	-b		Broadcast the packet" \
     "\n	-i IFACE	Interface to use (default eth0)" \
     "\n	-p PASSWORD	Append four or six byte PASSWORD to the packet" \

#define ftpd_trivial_usage \
       "[-wvS]"IF_FEATURE_FTPD_AUTHENTICATION(" [-a USER]")" [-t N] [-T N] [DIR]" \

#define ftpd_full_usage "\n\n" \
	IF_NOT_FEATURE_FTPD_AUTHENTICATION( \
       "Anonymous FTP server. Client access occurs under ftpd's UID.\n" \
	) \
	IF_FEATURE_FTPD_AUTHENTICATION( \
       "FTP server. " \
	) \
       "Chroots to DIR, if this fails (run by non-root), cds to it.\n" \
       "Should be used as inetd service, inetd.conf line:\n" \
       "	21 stream tcp nowait root ftpd ftpd /files/to/serve\n" \
       "Can be run from tcpsvd:\n" \
       "	tcpsvd -vE 0.0.0.0 21 ftpd /files/to/serve" \
     "\n" \
     "\n	-w	Allow upload" \
	IF_FEATURE_FTPD_AUTHENTICATION( \
     "\n	-A	No login required, client access occurs under ftpd's UID" \
     "\n	-a USER	Enable 'anonymous' login and map it to USER" \
	) \
     "\n	-v	Log errors to stderr. -vv: verbose log" \
     "\n	-S	Log errors to syslog. -SS: verbose log" \
     "\n	-t,-T N	Idle and absolute timeout" \

#define ftpget_trivial_usage \
       "[OPTIONS] HOST [LOCAL_FILE] REMOTE_FILE" \

#define ftpget_full_usage "\n\n" \
       "Download a file via FTP\n" \
     "\n	-c	Continue previous transfer" \
     "\n	-v	Verbose" \
     "\n	-u USER	Username" \
     "\n	-p PASS	Password" \
     "\n	-P NUM	Port" \

#define ftpput_trivial_usage \
       "[OPTIONS] HOST [REMOTE_FILE] LOCAL_FILE" \

#define ftpput_full_usage "\n\n" \
       "Upload a file to a FTP server\n" \
     "\n	-v	Verbose" \
     "\n	-u USER	Username" \
     "\n	-p PASS	Password" \
     "\n	-P NUM	Port number" \

#define hostname_trivial_usage \
       "[OPTIONS] [HOSTNAME | -F FILE]" \

#define hostname_full_usage "\n\n" \
       "Get or set hostname or DNS domain name\n" \
     "\n	-s	Short" \
     "\n	-i	Addresses for the hostname" \
     "\n	-d	DNS domain name" \
     "\n	-f	Fully qualified domain name" \
     "\n	-F FILE	Use FILE's content as hostname" \

#define hostname_example_usage \
       "$ hostname\n" \
       "sage\n" \

#define dnsdomainname_trivial_usage NOUSAGE_STR \

#define dnsdomainname_full_usage "" \

#define httpd_trivial_usage \
       "[-ifv[v]]" \
       " [-c CONFFILE]" \
       " [-p [IP:]PORT]" \
	IF_FEATURE_HTTPD_SETUID(" [-u USER[:GRP]]") \
	IF_FEATURE_HTTPD_BASIC_AUTH(" [-r REALM]") \
       " [-h HOME]\n" \
       "or httpd -d/-e" IF_FEATURE_HTTPD_AUTH_MD5("/-m") " STRING" \

#define httpd_full_usage "\n\n" \
       "Listen for incoming HTTP requests\n" \
     "\n	-i		Inetd mode" \
     "\n	-f		Don't daemonize" \
     "\n	-v[v]		Verbose" \
     "\n	-p [IP:]PORT	Bind to IP:PORT (default *:80)" \
	IF_FEATURE_HTTPD_SETUID( \
     "\n	-u USER[:GRP]	Set uid/gid after binding to port") \
	IF_FEATURE_HTTPD_BASIC_AUTH( \
     "\n	-r REALM	Authentication Realm for Basic Authentication") \
     "\n	-h HOME		Home directory (default .)" \
     "\n	-c FILE		Configuration file (default {/etc,HOME}/httpd.conf)" \
	IF_FEATURE_HTTPD_AUTH_MD5( \
     "\n	-m STRING	MD5 crypt STRING") \
     "\n	-e STRING	HTML encode STRING" \
     "\n	-d STRING	URL decode STRING" \

#define ifconfig_trivial_usage \
	IF_FEATURE_IFCONFIG_STATUS("[-a]") " interface [address]" \

#define ifconfig_full_usage "\n\n" \
       "Configure a network interface\n" \
     "\n" \
	IF_FEATURE_IPV6( \
       "	[add ADDRESS[/PREFIXLEN]]\n") \
	IF_FEATURE_IPV6( \
       "	[del ADDRESS[/PREFIXLEN]]\n") \
       "	[[-]broadcast [ADDRESS]] [[-]pointopoint [ADDRESS]]\n" \
       "	[netmask ADDRESS] [dstaddr ADDRESS]\n" \
	IF_FEATURE_IFCONFIG_SLIP( \
       "	[outfill NN] [keepalive NN]\n") \
       "	" IF_FEATURE_IFCONFIG_HW("[hw ether" IF_FEATURE_HWIB("|infiniband")" ADDRESS] ") "[metric NN] [mtu NN]\n" \
       "	[[-]trailers] [[-]arp] [[-]allmulti]\n" \
       "	[multicast] [[-]promisc] [txqueuelen NN] [[-]dynamic]\n" \
	IF_FEATURE_IFCONFIG_MEMSTART_IOADDR_IRQ( \
       "	[mem_start NN] [io_addr NN] [irq NN]\n") \
       "	[up|down] ..." \

#define ifenslave_trivial_usage \
       "[-cdf] MASTER_IFACE SLAVE_IFACE..." \

#define ifenslave_full_usage "\n\n" \
       "Configure network interfaces for parallel routing\n" \
     "\n	-c	Change active slave" \
     "\n	-d	Remove slave interface from bonding device" \
     "\n	-f	Force, even if interface is not Ethernet" \

#define ifenslave_example_usage \
       "To create a bond device, simply follow these three steps:\n" \
       "- ensure that the required drivers are properly loaded:\n" \
       "  # modprobe bonding ; modprobe <3c59x|eepro100|pcnet32|tulip|...>\n" \
       "- assign an IP address to the bond device:\n" \
       "  # ifconfig bond0 <addr> netmask <mask> broadcast <bcast>\n" \
       "- attach all the interfaces you need to the bond device:\n" \
       "  # ifenslave bond0 eth0 eth1 eth2\n" \
       "  If bond0 didn't have a MAC address, it will take eth0's. Then, all\n" \
       "  interfaces attached AFTER this assignment will get the same MAC addr.\n\n" \
       "  To detach a dead interface without setting the bond device down:\n" \
       "  # ifenslave -d bond0 eth1\n\n" \
       "  To set the bond device down and automatically release all the slaves:\n" \
       "  # ifconfig bond0 down\n\n" \
       "  To change active slave:\n" \
       "  # ifenslave -c bond0 eth0\n" \

#define ifplugd_trivial_usage \
       "[OPTIONS]" \

#define ifplugd_full_usage "\n\n" \
       "Network interface plug detection daemon\n" \
     "\n	-n		Don't daemonize" \
     "\n	-s		Don't log to syslog" \
     "\n	-i IFACE	Interface" \
     "\n	-f/-F		Treat link detection error as link down/link up" \
     "\n			(otherwise exit on error)" \
     "\n	-a		Don't up interface at each link probe" \
     "\n	-M		Monitor creation/destruction of interface" \
     "\n			(otherwise it must exist)" \
     "\n	-r PROG		Script to run" \
     "\n	-x ARG		Extra argument for script" \
     "\n	-I		Don't exit on nonzero exit code from script" \
     "\n	-p		Don't run \"up\" script on startup" \
     "\n	-q		Don't run \"down\" script on exit" \
     "\n	-l		Always run script on startup" \
     "\n	-t SECS		Poll time in seconds" \
     "\n	-u SECS		Delay before running script after link up" \
     "\n	-d SECS		Delay after link down" \
     "\n	-m MODE		API mode (mii, priv, ethtool, wlan, iff, auto)" \
     "\n	-k		Kill running daemon" \

#define ifup_trivial_usage \
       "[-an"IF_FEATURE_IFUPDOWN_MAPPING("m")"vf] [-i FILE] IFACE..." \

#define ifup_full_usage "\n\n" \
       "	-a	Configure all interfaces" \
     "\n	-i FILE	Use FILE instead of /etc/network/interfaces" \
     "\n	-n	Print out what would happen, but don't do it" \
	IF_FEATURE_IFUPDOWN_MAPPING( \
     "\n		(note: doesn't disable mappings)" \
     "\n	-m	Don't run any mappings" \
	) \
     "\n	-v	Print out what would happen before doing it" \
     "\n	-f	Force configuration" \

#define ifdown_trivial_usage \
       "[-an"IF_FEATURE_IFUPDOWN_MAPPING("m")"vf] [-i FILE] IFACE..." \

#define ifdown_full_usage "\n\n" \
       "	-a	Deconfigure all interfaces" \
     "\n	-i FILE	Use FILE for interface definitions" \
     "\n	-n	Print out what would happen, but don't do it" \
	IF_FEATURE_IFUPDOWN_MAPPING( \
     "\n		(note: doesn't disable mappings)" \
     "\n	-m	Don't run any mappings" \
	) \
     "\n	-v	Print out what would happen before doing it" \
     "\n	-f	Force deconfiguration" \

#define inetd_trivial_usage \
       "[-fe] [-q N] [-R N] [CONFFILE]" \

#define inetd_full_usage "\n\n" \
       "Listen for network connections and launch programs\n" \
     "\n	-f	Run in foreground" \
     "\n	-e	Log to stderr" \
     "\n	-q N	Socket listen queue (default 128)" \
     "\n	-R N	Pause services after N connects/min" \
     "\n		(default 0 - disabled)" \
     "\n	Default CONFFILE is /etc/inetd.conf" \

#define ipaddr_trivial_usage \
       "add|del IFADDR dev IFACE | show|flush [dev IFACE] [to PREFIX]" \

#define ipaddr_full_usage "\n\n" \
       "ipaddr add|change|replace|delete dev IFACE IFADDR\n" \
       "	IFADDR := PREFIX | ADDR peer PREFIX [broadcast ADDR|+|-]\n" \
       "		[anycast ADDR] [label STRING] [scope SCOPE]\n" \
       "	PREFIX := ADDR[/MASK]\n" \
       "	SCOPE := [host|link|global|NUMBER]\n" \
       "ipaddr show|flush [dev IFACE] [scope SCOPE] [to PREFIX] [label PATTERN]" \

#define iplink_trivial_usage \
       /*Usage:iplink*/"set IFACE [up|down] [arp on|off] [multicast on|off]\n" \
       "	[promisc on|off] [mtu NUM] [name NAME] [qlen NUM] [address MAC]\n" \
       "	[master IFACE | nomaster]" \

#define iplink_full_usage "\n" \
       "iplink add [link IFACE] IFACE [address MAC] type TYPE [ARGS]\n" \
       "iplink delete IFACE type TYPE [ARGS]\n" \
       "	TYPE ARGS := vlan VLANARGS | vrf table NUM\n" \
       "	VLANARGS := id VLANID [protocol 802.1q|802.1ad] [reorder_hdr on|off]\n" \
       "		[gvrp on|off] [mvrp on|off] [loose_binding on|off]\n" \
       "iplink show [IFACE]" \

#define iproute_trivial_usage \
       "list|flush|add|del|change|append|replace|test ROUTE" \

#define iproute_full_usage "\n\n" \
       "iproute list|flush SELECTOR\n" \
       "	SELECTOR := [root PREFIX] [match PREFIX] [proto RTPROTO]\n" \
       "	PREFIX := default|ADDR[/MASK]\n" \
       "iproute get ADDR [from ADDR iif IFACE]\n" \
       "	[oif IFACE] [tos TOS]\n" \
       "iproute add|del|change|append|replace|test ROUTE\n" \
       "	ROUTE := NODE_SPEC [INFO_SPEC]\n" \
       "	NODE_SPEC := PREFIX"IF_FEATURE_IP_RULE(" [table TABLE_ID]")" [proto RTPROTO] [scope SCOPE] [metric METRIC]\n" \
       "	INFO_SPEC := NH OPTIONS\n" \
       "	NH := [via [inet|inet6] ADDR] [dev IFACE] [src ADDR] [onlink]\n" \
       "	OPTIONS := [mtu [lock] NUM] [advmss [lock] NUM]" \

#define iprule_trivial_usage \
       "[list] | add|del SELECTOR ACTION" \

#define iprule_full_usage "\n\n" \
       "	SELECTOR := [from PREFIX] [to PREFIX] [tos TOS] [fwmark FWMARK]\n" \
       "			[dev IFACE] [pref NUMBER]\n" \
       "	ACTION := [table TABLE_ID] [nat ADDR]\n" \
       "			[prohibit|reject|unreachable]\n" \
       "			[realms [SRCREALM/]DSTREALM]\n" \
       "	TABLE_ID := [local|main|default|NUMBER]" \

#define iptunnel_trivial_usage \
       "add|change|del|show [NAME]\n" \
       "	[mode ipip|gre|sit] [remote ADDR] [local ADDR] [ttl TTL]" \

#define iptunnel_full_usage "\n\n" \
       "iptunnel add|change|del|show [NAME]\n" \
       "	[mode ipip|gre|sit] [remote ADDR] [local ADDR]\n" \
       "	[[i|o]seq] [[i|o]key KEY] [[i|o]csum]\n" \
       "	[ttl TTL] [tos TOS] [[no]pmtudisc] [dev PHYS_DEV]" \

#define ipneigh_trivial_usage \
       "show|flush [to PREFIX] [dev DEV] [nud STATE]" \

#define ipneigh_full_usage "" \

#if ENABLE_FEATURE_IP_ADDRESS || ENABLE_FEATURE_IP_ROUTE \

# define IP_BAR_LINK   "|" \

#else \

# define IP_BAR_LINK   "" \

#endif \

#if ENABLE_FEATURE_IP_ADDRESS || ENABLE_FEATURE_IP_ROUTE || ENABLE_FEATURE_IP_LINK \

# define IP_BAR_TUNNEL "|" \

#else \

# define IP_BAR_TUNNEL "" \

#endif \

#if ENABLE_FEATURE_IP_ADDRESS || ENABLE_FEATURE_IP_ROUTE || ENABLE_FEATURE_IP_LINK || ENABLE_FEATURE_IP_TUNNEL \

# define IP_BAR_NEIGH  "|" \

#else \

# define IP_BAR_NEIGH  "" \

#endif \

#if ENABLE_FEATURE_IP_ADDRESS || ENABLE_FEATURE_IP_ROUTE || ENABLE_FEATURE_IP_LINK || ENABLE_FEATURE_IP_TUNNEL || ENABLE_FEATURE_IP_NEIGH \

# define IP_BAR_RULE   "|" \

#else \

# define IP_BAR_RULE   "" \

#endif \

#define ip_trivial_usage \
       "[OPTIONS] " \
	IF_FEATURE_IP_ADDRESS("address") \
	IF_FEATURE_IP_ROUTE(  IF_FEATURE_IP_ADDRESS("|")"route") \
	IF_FEATURE_IP_LINK(   IP_BAR_LINK  "link") \
	IF_FEATURE_IP_TUNNEL( IP_BAR_TUNNEL"tunnel") \
	IF_FEATURE_IP_NEIGH(  IP_BAR_NEIGH "neigh") \
	IF_FEATURE_IP_RULE(   IP_BAR_RULE  "rule") \
       " [ARGS]" \

#define ip_full_usage "\n\n" \
       "OPTIONS := -f[amily] inet|inet6|link | -o[neline]\n" \
	IF_FEATURE_IP_ADDRESS("\n" \
	"ip addr "ipaddr_trivial_usage) \
	IF_FEATURE_IP_ROUTE("\n" \
	"ip route "iproute_trivial_usage) \
	IF_FEATURE_IP_LINK("\n" \
	"ip link "iplink_trivial_usage) \
	IF_FEATURE_IP_TUNNEL("\n" \
	"ip tunnel "iptunnel_trivial_usage) \
	IF_FEATURE_IP_NEIGH("\n" \
	"ip neigh "ipneigh_trivial_usage) \
	IF_FEATURE_IP_RULE("\n" \
	"ip rule "iprule_trivial_usage) \

#define ipcalc_trivial_usage \
       "[OPTIONS] ADDRESS" \
       IF_FEATURE_IPCALC_FANCY("[/PREFIX]") " [NETMASK]" \

#define ipcalc_full_usage "\n\n" \
       "Calculate and display network settings from IP address\n" \
     "\n	-b	Broadcast address" \
     "\n	-n	Network address" \
     "\n	-m	Default netmask for IP" \
	IF_FEATURE_IPCALC_FANCY( \
     "\n	-p	Prefix for IP/NETMASK" \
     "\n	-h	Resolved host name" \
     "\n	-s	No error messages" \
	) \

#define fakeidentd_trivial_usage \
       "[-fiw] [-b ADDR] [STRING]" \

#define fakeidentd_full_usage "\n\n" \
       "Provide fake ident (auth) service\n" \
     "\n	-f	Run in foreground" \
     "\n	-i	Inetd mode" \
     "\n	-w	Inetd 'wait' mode" \
     "\n	-b ADDR	Bind to specified address" \
     "\n	STRING	Ident answer string (default: nobody)" \

#define nameif_trivial_usage \
	IF_NOT_FEATURE_NAMEIF_EXTENDED( \
		"[-s] [-c FILE] [IFNAME HWADDR]..." \
	) \
	IF_FEATURE_NAMEIF_EXTENDED( \
		"[-s] [-c FILE] [IFNAME SELECTOR]..." \
	) \

#define nameif_full_usage "\n\n" \
	"Rename network interface while it in the down state." \
	IF_NOT_FEATURE_NAMEIF_EXTENDED( \
     "\nThe device with address HWADDR is renamed to IFACE." \
	) \
	IF_FEATURE_NAMEIF_EXTENDED( \
     "\nThe device matched by SELECTOR is renamed to IFACE." \
     "\nSELECTOR can be a combination of:" \
     "\n	driver=STRING" \
     "\n	bus=STRING" \
     "\n	phy_address=NUM" \
     "\n	[mac=]XX:XX:XX:XX:XX:XX" \
	) \
     "\n" \
     "\n	-c FILE	Configuration file (default: /etc/mactab)" \
     "\n	-s	Log to syslog" \

#define nameif_example_usage \
       "$ nameif -s dmz0 00:A0:C9:8C:F6:3F\n" \
       " or\n" \
       "$ nameif -c /etc/my_mactab_file\n" \

#define nbdclient_trivial_usage \
       "{ [-b BLKSIZE] [-N NAME] [-t SEC] [-p] HOST [PORT] | -d } BLOCKDEV" \

#define nbdclient_full_usage "\n\n" \
       "Connect to HOST and provide network block device on BLOCKDEV" \

#if !ENABLE_NC_110_COMPAT \

#if ENABLE_NC_SERVER || ENABLE_NC_EXTRA \

#define NC_OPTIONS_STR "\n" \

#else \

#define NC_OPTIONS_STR \

#endif \

#define nc_trivial_usage \
	IF_NC_EXTRA("[-iN] [-wN] ")IF_NC_SERVER("[-l] [-p PORT] ") \
       "["IF_NC_EXTRA("-f FILE|")"IPADDR PORT]"IF_NC_EXTRA(" [-e PROG]") \

#define nc_full_usage "\n\n" \
       "Open a pipe to IP:PORT" IF_NC_EXTRA(" or FILE") \
	NC_OPTIONS_STR \
	IF_NC_SERVER( \
     "\n	-l	Listen mode, for inbound connects" \
	IF_NC_EXTRA( \
     "\n		(use -ll with -e for persistent server)" \
	) \
     "\n	-p PORT	Local port" \
	) \
	IF_NC_EXTRA( \
     "\n	-w SEC	Connect timeout" \
     "\n	-i SEC	Delay interval for lines sent" \
     "\n	-f FILE	Use file (ala /dev/ttyS0) instead of network" \
     "\n	-e PROG	Run PROG after connect" \
	) \

#define nc_notes_usage "" \
	IF_NC_EXTRA( \
       "To use netcat as a terminal emulator on a serial port:\n\n" \
       "$ stty 115200 -F /dev/ttyS0\n" \
       "$ stty raw -echo -ctlecho && nc -f /dev/ttyS0\n" \
	) \

#define nc_example_usage \
       "$ nc foobar.somedomain.com 25\n" \
       "220 foobar ESMTP Exim 3.12 #1 Sat, 15 Apr 2000 00:03:02 -0600\n" \
       "help\n" \
       "214-Commands supported:\n" \
       "214-    HELO EHLO MAIL RCPT DATA AUTH\n" \
       "214     NOOP QUIT RSET HELP\n" \
       "quit\n" \
       "221 foobar closing connection\n" \

#endif \

#if ENABLE_NC_110_COMPAT \

#define nc_trivial_usage \
       "[OPTIONS] HOST PORT  - connect" \
	IF_NC_SERVER("\n" \
       "nc [OPTIONS] -l -p PORT [HOST] [PORT]  - listen" \
	) \

#define nc_full_usage "\n\n" \
       "	-e PROG	Run PROG after connect (must be last)" \
	IF_NC_SERVER( \
     "\n	-l	Listen mode, for inbound connects" \
     "\n	-lk	With -e, provides persistent server" \
	) \
     "\n	-p PORT	Local port" \
     "\n	-s ADDR	Local address" \
     "\n	-w SEC	Timeout for connects and final net reads" \
	IF_NC_EXTRA( \
     "\n	-i SEC	Delay interval for lines sent" /* ", ports scanned" */ \
	) \
     "\n	-n	Don't do DNS resolution" \
     "\n	-u	UDP mode" \
     "\n	-v	Verbose" \
	IF_NC_EXTRA( \
     "\n	-o FILE	Hex dump traffic" \
     "\n	-z	Zero-I/O mode (scanning)" \
	) \

#endif \

#define netstat_trivial_usage \
       "[-"IF_ROUTE("r")"al] [-tuwx] [-en"IF_FEATURE_NETSTAT_WIDE("W")IF_FEATURE_NETSTAT_PRG("p")"]" \

#define netstat_full_usage "\n\n" \
       "Display networking information\n" \
	IF_ROUTE( \
     "\n	-r	Routing table" \
	) \
     "\n	-a	All sockets" \
     "\n	-l	Listening sockets" \
     "\n		Else: connected sockets" \
     "\n	-t	TCP sockets" \
     "\n	-u	UDP sockets" \
     "\n	-w	Raw sockets" \
     "\n	-x	Unix sockets" \
     "\n		Else: all socket types" \
     "\n	-e	Other/more information" \
     "\n	-n	Don't resolve names" \
	IF_FEATURE_NETSTAT_WIDE( \
     "\n	-W	Wide display" \
	) \
	IF_FEATURE_NETSTAT_PRG( \
     "\n	-p	Show PID/program name for sockets" \
	) \

#define nslookup_trivial_usage \
       IF_FEATURE_NSLOOKUP_BIG("[-type=QUERY_TYPE] [-debug] ") "HOST [DNS_SERVER]" \

#define nslookup_full_usage "\n\n" \
       "Query DNS about HOST" \
       IF_FEATURE_NSLOOKUP_BIG("\n") \
       IF_FEATURE_NSLOOKUP_BIG("\nQUERY_TYPE: soa,ns,a,"IF_FEATURE_IPV6("aaaa,")"cname,mx,txt,ptr,any") \

#define nslookup_example_usage \
       "$ nslookup localhost\n" \
       "Server:     default\n" \
       "Address:    default\n" \
       "\n" \
       "Name:       debian\n" \
       "Address:    127.0.0.1\n" \

#define ntpd_trivial_usage \
	"[-dnqNw"IF_FEATURE_NTPD_SERVER("l] [-I IFACE")"] [-S PROG]" \
	IF_NOT_FEATURE_NTP_AUTH(" [-p PEER]...") \
	IF_FEATURE_NTP_AUTH(" [-k KEYFILE] [-p [keyno:N:]PEER]...") \

#define ntpd_full_usage "\n\n" \
       "NTP client/server\n" \
     "\n	-d	Verbose (may be repeated)" \
     "\n	-n	Do not daemonize" \
     "\n	-q	Quit after clock is set" \
     "\n	-N	Run at high priority" \
     "\n	-w	Do not set time (only query peers), implies -n" \
     "\n	-S PROG	Run PROG after stepping time, stratum change, and every 11 min" \
	IF_NOT_FEATURE_NTP_AUTH( \
     "\n	-p PEER	Obtain time from PEER (may be repeated)" \
	) \
	IF_FEATURE_NTP_AUTH( \
     "\n	-k FILE	Key file (ntp.keys compatible)" \
     "\n	-p [keyno:NUM:]PEER" \
     "\n		Obtain time from PEER (may be repeated)" \
     "\n		Use key NUM for authentication" \
	) \
	IF_FEATURE_NTPD_CONF( \
     "\n		If -p is not given, 'server HOST' lines" \
     "\n		from /etc/ntp.conf are used" \
	) \
	IF_FEATURE_NTPD_SERVER( \
     "\n	-l	Also run as server on port 123" \
     "\n	-I IFACE Bind server to IFACE, implies -l" \
	) \

#if !ENABLE_FEATURE_FANCY_PING \

# define ping_trivial_usage \
       "HOST" \

# define ping_full_usage "\n\n" \
       "Send ICMP ECHO_REQUEST packets to network hosts" \

# define ping6_trivial_usage \
       "HOST" \

# define ping6_full_usage "\n\n" \
       "Send ICMP ECHO_REQUEST packets to network hosts" \

#else \

# define ping_trivial_usage \
       "[OPTIONS] HOST" \

# define ping_full_usage "\n\n" \
       "Send ICMP ECHO_REQUEST packets to network hosts\n" \
	IF_PING6( \
     "\n	-4,-6		Force IP or IPv6 name resolution" \
	) \
     "\n	-c CNT		Send only CNT pings" \
     "\n	-s SIZE		Send SIZE data bytes in packets (default 56)" \
     "\n	-i SECS		Interval" \
     "\n	-A		Ping as soon as reply is recevied" \
     "\n	-t TTL		Set TTL" \
     "\n	-I IFACE/IP	Source interface or IP address" \
     "\n	-W SEC		Seconds to wait for the first response (default 10)" \
     "\n			(after all -c CNT packets are sent)" \
     "\n	-w SEC		Seconds until ping exits (default:infinite)" \
     "\n			(can exit earlier with -c CNT)" \
     "\n	-q		Quiet, only display output at start" \
     "\n			and when finished" \
     "\n	-p HEXBYTE	Pattern to use for payload" \

# define ping6_trivial_usage \
       "[OPTIONS] HOST" \

# define ping6_full_usage "\n\n" \
       "Send ICMP ECHO_REQUEST packets to network hosts\n" \
     "\n	-c CNT		Send only CNT pings" \
     "\n	-s SIZE		Send SIZE data bytes in packets (default 56)" \
     "\n	-i SECS		Interval" \
     "\n	-A		Ping as soon as reply is recevied" \
     "\n	-I IFACE/IP	Source interface or IP address" \
     "\n	-q		Quiet, only display output at start" \
     "\n			and when finished" \
     "\n	-p HEXBYTE	Pattern to use for payload" \

#endif \

#define ping_example_usage \
       "$ ping localhost\n" \
       "PING slag (127.0.0.1): 56 data bytes\n" \
       "64 bytes from 127.0.0.1: icmp_seq=0 ttl=255 time=20.1 ms\n" \
       "\n" \
       "--- debian ping statistics ---\n" \
       "1 packets transmitted, 1 packets received, 0% packet loss\n" \
       "round-trip min/avg/max = 20.1/20.1/20.1 ms\n" \

#define ping6_example_usage \
       "$ ping6 ip6-localhost\n" \
       "PING ip6-localhost (::1): 56 data bytes\n" \
       "64 bytes from ::1: icmp6_seq=0 ttl=64 time=20.1 ms\n" \
       "\n" \
       "--- ip6-localhost ping statistics ---\n" \
       "1 packets transmitted, 1 packets received, 0% packet loss\n" \
       "round-trip min/avg/max = 20.1/20.1/20.1 ms\n" \

#define pscan_trivial_usage \
       "[-cb] [-p MIN_PORT] [-P MAX_PORT] [-t TIMEOUT] [-T MIN_RTT] HOST" \

#define pscan_full_usage "\n\n" \
       "Scan a host, print all open ports\n" \
     "\n	-c	Show closed ports too" \
     "\n	-b	Show blocked ports too" \
     "\n	-p	Scan from this port (default 1)" \
     "\n	-P	Scan up to this port (default 1024)" \
     "\n	-t	Timeout (default 5000 ms)" \
     "\n	-T	Minimum rtt (default 5 ms, increase for congested hosts)" \

#define route_trivial_usage \
       "[{add|del|delete}]" \

#define route_full_usage "\n\n" \
       "Edit kernel routing tables\n" \
     "\n	-n	Don't resolve names" \
     "\n	-e	Display other/more information" \
     "\n	-A inet" IF_FEATURE_IPV6("{6}") "	Select address family" \

#define slattach_trivial_usage \
       "[-ehmLF] [-c SCRIPT] [-s BAUD] [-p PROTOCOL] SERIAL_DEVICE" \

#define slattach_full_usage "\n\n" \
       "Configure serial line as SLIP network interface\n" \
     "\n	-p PROT	Protocol: slip, cslip (default), slip6, clisp6, adaptive" \
     "\n	-s BAUD	Line speed" \
     "\n	-e	Exit after initialization" \
     "\n	-h	Exit if carrier is lost (else never exits)" \
     "\n	-c PROG	Run PROG on carrier loss" \
     "\n	-m	Do NOT set raw 8bit mode" \
     "\n	-L	Enable 3-wire operation" \
     "\n	-F	Disable RTS/CTS flow control" \

#define ssl_client_trivial_usage \
       "[-e] -s FD [-r FD] [-n SNI]" \

#define ssl_client_full_usage "" \

#define tc_trivial_usage \
	"OBJECT CMD [dev STRING]" \

#define tc_full_usage "\n\n" \
	"OBJECT: qdisc|class|filter\n" \
	"CMD: add|del|change|replace|show\n" \
	"\n" \
	"qdisc [handle QHANDLE] [root|"IF_FEATURE_TC_INGRESS("ingress|")"parent CLASSID]\n" \
	"	[[QDISC_KIND] [help|OPTIONS]]\n" \
	"	QDISC_KIND := [p|b]fifo|tbf|prio|cbq|red|etc.\n" \
	"qdisc show [dev STRING]"IF_FEATURE_TC_INGRESS(" [ingress]")"\n" \
	"class [classid CLASSID] [root|parent CLASSID]\n" \
	"	[[QDISC_KIND] [help|OPTIONS] ]\n" \
	"class show [ dev STRING ] [root|parent CLASSID]\n" \
	"filter [pref PRIO] [protocol PROTO]\n" \
	"	[root|classid CLASSID] [handle FILTERID]\n" \
	"	[[FILTER_TYPE] [help|OPTIONS]]\n" \
	"filter show [dev STRING] [root|parent CLASSID]" \

#define tcpsvd_trivial_usage \
       "[-hEv] [-c N] [-C N[:MSG]] [-b N] [-u USER] [-l NAME] IP PORT PROG" \

#define tcpsvd_full_usage "\n\n" \
       "Create TCP socket, bind to IP:PORT and listen for incoming connections.\n" \
       "Run PROG for each connection.\n" \
     "\n	IP PORT		IP:PORT to listen on" \
     "\n	PROG ARGS	Program to run" \
     "\n	-u USER[:GRP]	Change to user/group after bind" \
     "\n	-c N		Up to N connections simultaneously (default 30)" \
     "\n	-b N		Allow backlog of approximately N TCP SYNs (default 20)" \
     "\n	-C N[:MSG]	Allow only up to N connections from the same IP:" \
     "\n			new connections from this IP address are closed" \
     "\n			immediately, MSG is written to the peer before close" \
     "\n	-E		Don't set up environment" \
     "\n	-h		Look up peer's hostname" \
     "\n	-l NAME		Local hostname (else look up local hostname in DNS)" \
     "\n	-v		Verbose" \
     "\n" \
     "\nEnvironment if no -E:" \
     "\nPROTO='TCP'" \
     "\nTCPREMOTEADDR='ip:port'" IF_FEATURE_IPV6(" ('[ip]:port' for IPv6)") \
     "\nTCPLOCALADDR='ip:port'" \
     "\nTCPORIGDSTADDR='ip:port' of destination before firewall" \
     "\n	Useful for REDIRECTed-to-local connections:" \
     "\n	iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to 8080" \
     "\nTCPCONCURRENCY=num_of_connects_from_this_ip" \
     "\nIf -h:" \
     "\nTCPLOCALHOST='hostname' (-l NAME is used if specified)" \
     "\nTCPREMOTEHOST='hostname'" \

#define udpsvd_trivial_usage \
       "[-hEv] [-c N] [-u USER] [-l NAME] IP PORT PROG" \

#define udpsvd_full_usage "\n\n" \
       "Create UDP socket, bind to IP:PORT and wait for incoming packets.\n" \
       "Run PROG for each packet, redirecting all further packets with same\n" \
       "peer ip:port to it.\n" \
     "\n	IP PORT		IP:PORT to listen on" \
     "\n	PROG ARGS	Program to run" \
     "\n	-u USER[:GRP]	Change to user/group after bind" \
     "\n	-c N		Up to N connections simultaneously (default 30)" \
     "\n	-E		Don't set up environment" \
     "\n	-h		Look up peer's hostname" \
     "\n	-l NAME		Local hostname (else look up local hostname in DNS)" \
     "\n	-v		Verbose" \
     "\n" \
     "\nEnvironment if no -E:" \
     "\nPROTO='UDP'" \
     "\nUDPREMOTEADDR='ip:port'" IF_FEATURE_IPV6(" ('[ip]:port' for IPv6)") \
     "\nUDPLOCALADDR='ip:port'" \
     "\nIf -h:" \
     "\nUDPLOCALHOST='hostname' (-l NAME is used if specified)" \
     "\nUDPREMOTEHOST='hostname'" \

#if ENABLE_FEATURE_TELNET_AUTOLOGIN \

#define telnet_trivial_usage \
       "[-a] [-l USER] HOST [PORT]" \

#define telnet_full_usage "\n\n" \
       "Connect to telnet server\n" \
     "\n	-a	Automatic login with $USER variable" \
     "\n	-l USER	Automatic login as USER" \

#else \

#define telnet_trivial_usage \
       "HOST [PORT]" \

#define telnet_full_usage "\n\n" \
       "Connect to telnet server" \

#endif \

#define telnetd_trivial_usage \
       "[OPTIONS]" \

#define telnetd_full_usage "\n\n" \
       "Handle incoming telnet connections" \
	IF_NOT_FEATURE_TELNETD_STANDALONE(" via inetd") "\n" \
     "\n	-l LOGIN	Exec LOGIN on connect" \
     "\n	-f ISSUE_FILE	Display ISSUE_FILE instead of /etc/issue" \
     "\n	-K		Close connection as soon as login exits" \
     "\n			(normally wait until all programs close slave pty)" \
	IF_FEATURE_TELNETD_STANDALONE( \
     "\n	-p PORT		Port to listen on" \
     "\n	-b ADDR[:PORT]	Address to bind to" \
     "\n	-F		Run in foreground" \
     "\n	-i		Inetd mode" \
	IF_FEATURE_TELNETD_INETD_WAIT( \
     "\n	-w SEC		Inetd 'wait' mode, linger time SEC" \
     "\n	-S		Log to syslog (implied by -i or without -F and -w)" \
	) \
	) \

#define tftp_trivial_usage \
       "[OPTIONS] HOST [PORT]" \

#define tftp_full_usage "\n\n" \
       "Transfer a file from/to tftp server\n" \
     "\n	-l FILE	Local FILE" \
     "\n	-r FILE	Remote FILE" \
	IF_FEATURE_TFTP_GET( \
     "\n	-g	Get file" \
	) \
	IF_FEATURE_TFTP_PUT( \
     "\n	-p	Put file" \
	) \
	IF_FEATURE_TFTP_BLOCKSIZE( \
     "\n	-b SIZE	Transfer blocks of SIZE octets" \
	) \

#define tftpd_trivial_usage \
       "[-cr] [-u USER] [DIR]" \

#define tftpd_full_usage "\n\n" \
       "Transfer a file on tftp client's request\n" \
       "\n" \
       "tftpd should be used as an inetd service.\n" \
       "tftpd's line for inetd.conf:\n" \
       "	69 dgram udp nowait root tftpd tftpd -l /files/to/serve\n" \
       "It also can be ran from udpsvd:\n" \
       "	udpsvd -vE 0.0.0.0 69 tftpd /files/to/serve\n" \
     "\n	-r	Prohibit upload" \
     "\n	-c	Allow file creation via upload" \
     "\n	-u	Access files as USER" \
     "\n	-l	Log to syslog (inetd mode requires this)" \

#define traceroute_trivial_usage \
       "[-"IF_TRACEROUTE6("46")"FIlnrv] [-f 1ST_TTL] [-m MAXTTL] [-q PROBES] [-p PORT]\n" \
       "	[-t TOS] [-w WAIT_SEC] [-s SRC_IP] [-i IFACE]\n" \
       "	[-z PAUSE_MSEC] HOST [BYTES]" \

#define traceroute_full_usage "\n\n" \
       "Trace the route to HOST\n" \
	IF_TRACEROUTE6( \
     "\n	-4,-6	Force IP or IPv6 name resolution" \
	) \
     "\n	-F	Set don't fragment bit" \
	IF_FEATURE_TRACEROUTE_USE_ICMP( \
     "\n	-I	Use ICMP ECHO instead of UDP datagrams" \
	) \
     "\n	-l	Display TTL value of the returned packet" \
     "\n	-n	Print numeric addresses" \
     "\n	-r	Bypass routing tables, send directly to HOST" \
	IF_FEATURE_TRACEROUTE_VERBOSE( \
     "\n	-v	Verbose" \
	) \
     "\n	-f N	First number of hops (default 1)" \
     "\n	-m N	Max number of hops" \
     "\n	-q N	Number of probes per hop (default 3)" \
     "\n	-p N	Base UDP port number used in probes" \
     "\n		(default 33434)" \
     "\n	-s IP	Source address" \
     "\n	-i IFACE Source interface" \
     "\n	-t N	Type-of-service in probe packets (default 0)" \
     "\n	-w SEC	Time to wait for a response (default 3)" \
     "\n	-g IP	Loose source route gateway (8 max)" \

#define traceroute6_trivial_usage \
       "[-nrv] [-m MAXTTL] [-q PROBES] [-p PORT]\n" \
       "	[-t TOS] [-w WAIT_SEC] [-s SRC_IP] [-i IFACE]\n" \
       "	HOST [BYTES]" \

#define traceroute6_full_usage "\n\n" \
       "Trace the route to HOST\n" \
     "\n	-n	Print numeric addresses" \
     "\n	-r	Bypass routing tables, send directly to HOST" \
	IF_FEATURE_TRACEROUTE_VERBOSE( \
     "\n	-v	Verbose" \
	) \
     "\n	-m N	Max number of hops" \
     "\n	-q N	Number of probes per hop (default 3)" \
     "\n	-p N	Base UDP port number used in probes" \
     "\n		(default 33434)" \
     "\n	-s IP	Source address" \
     "\n	-i IFACE Source interface" \
     "\n	-t N	Type-of-service in probe packets (default 0)" \
     "\n	-w SEC	Time wait for a response (default 3)" \

#define tunctl_trivial_usage \
       "[-f device] ([-t name] | -d name)" IF_FEATURE_TUNCTL_UG(" [-u owner] [-g group] [-b]") \

#define tunctl_full_usage "\n\n" \
       "Create or delete tun interfaces\n" \
     "\n	-f name		tun device (/dev/net/tun)" \
     "\n	-t name		Create iface 'name'" \
     "\n	-d name		Delete iface 'name'" \
	IF_FEATURE_TUNCTL_UG( \
     "\n	-u owner	Set iface owner" \
     "\n	-g group	Set iface group" \
     "\n	-b		Brief output" \
	) \

#define tunctl_example_usage \
       "# tunctl\n" \
       "# tunctl -d tun0\n" \

#define vconfig_trivial_usage \
       "COMMAND [OPTIONS]" \

#define vconfig_full_usage "\n\n" \
       "Create and remove virtual ethernet devices\n" \
     "\n	add		IFACE VLAN_ID" \
     "\n	rem		VLAN_NAME" \
     "\n	set_flag	IFACE 0|1 VLAN_QOS" \
     "\n	set_egress_map	VLAN_NAME SKB_PRIO VLAN_QOS" \
     "\n	set_ingress_map	VLAN_NAME SKB_PRIO VLAN_QOS" \
     "\n	set_name_type	NAME_TYPE" \

#define wget_trivial_usage \
	IF_FEATURE_WGET_LONG_OPTIONS( \
       "[-c|--continue] [--spider] [-q|--quiet] [-O|--output-document FILE]\n" \
       "	[--header 'header: value'] [-Y|--proxy on/off] [-P DIR]\n" \
       "	[-S|--server-response] [-U|--user-agent AGENT]" IF_FEATURE_WGET_TIMEOUT(" [-T SEC]") " URL..." \
	) \
	IF_NOT_FEATURE_WGET_LONG_OPTIONS( \
       "[-cq] [-O FILE] [-Y on/off] [-P DIR] [-S] [-U AGENT]" \
			IF_FEATURE_WGET_TIMEOUT(" [-T SEC]") " URL..." \
	) \

#define wget_full_usage "\n\n" \
       "Retrieve files via HTTP or FTP\n" \
	IF_FEATURE_WGET_LONG_OPTIONS( \
     "\n	--spider	Only check URL existence: $? is 0 if exists" \
	) \
     "\n	-c		Continue retrieval of aborted transfer" \
     "\n	-q		Quiet" \
     "\n	-P DIR		Save to DIR (default .)" \
     "\n	-S    		Show server response" \
	IF_FEATURE_WGET_TIMEOUT( \
     "\n	-T SEC		Network read timeout is SEC seconds" \
	) \
     "\n	-O FILE		Save to FILE ('-' for stdout)" \
     "\n	-U STR		Use STR for User-Agent header" \
     "\n	-Y on/off	Use proxy" \

#define whois_trivial_usage \
       "[-i] [-h SERVER] [-p PORT] NAME..." \

#define whois_full_usage "\n\n" \
       "Query WHOIS info about NAME\n" \
     "\n	-i	Show redirect results too" \
     "\n	-h,-p	Server to query" \

#define zcip_trivial_usage \
       "[OPTIONS] IFACE SCRIPT" \

#define zcip_full_usage "\n\n" \
       "Manage a ZeroConf IPv4 link-local address\n" \
     "\n	-f		Run in foreground" \
     "\n	-q		Quit after obtaining address" \
     "\n	-r 169.254.x.x	Request this address first" \
     "\n	-l x.x.0.0	Use this range instead of 169.254" \
     "\n	-v		Verbose" \
     "\n" \
     "\n$LOGGING=none		Suppress logging" \
     "\n$LOGGING=syslog 	Log to syslog" \
     "\n" \
     "\nWith no -q, runs continuously monitoring for ARP conflicts," \
     "\nexits only on I/O errors (link down etc)" \

#define lpd_trivial_usage \
       "SPOOLDIR [HELPER [ARGS]]" \

#define lpd_full_usage "\n\n" \
       "SPOOLDIR must contain (symlinks to) device nodes or directories" \
     "\nwith names matching print queue names. In the first case, jobs are" \
     "\nsent directly to the device. Otherwise each job is stored in queue" \
     "\ndirectory and HELPER program is called. Name of file to print" \
     "\nis passed in $DATAFILE variable." \
     "\nExample:" \
     "\n	tcpsvd -E 0 515 softlimit -m 999999 lpd /var/spool ./print" \

#define lpr_trivial_usage \
       "-P queue[@host[:port]] -U USERNAME -J TITLE -Vmh [FILE]..." \

#define lpr_full_usage "\n\n" \
       "	-P	lp service to connect to (else uses $PRINTER)" \
     "\n	-m	Send mail on completion" \
     "\n	-h	Print banner page too" \
     "\n	-V	Verbose" \

#define lpq_trivial_usage \
       "[-P queue[@host[:port]]] [-U USERNAME] [-d JOBID]... [-fs]" \

#define lpq_full_usage "\n\n" \
       "	-P	lp service to connect to (else uses $PRINTER)" \
     "\n	-d	Delete jobs" \
     "\n	-f	Force any waiting job to be printed" \
     "\n	-s	Short display" \

#define free_trivial_usage \
       "" IF_DESKTOP("[-b/k/m/g]") \

#define free_full_usage "\n\n" \
       "Display the amount of free and used system memory" \

#define free_example_usage \
       "$ free\n" \
       "              total         used         free       shared      buffers\n" \
       "  Mem:       257628       248724         8904        59644        93124\n" \
       " Swap:       128516         8404       120112\n" \
       "Total:       386144       257128       129016\n" \

#define fuser_trivial_usage \
       "[OPTIONS] FILE or PORT/PROTO" \

#define fuser_full_usage "\n\n" \
       "Find processes which use FILEs or PORTs\n" \
     "\n	-m	Find processes which use same fs as FILEs" \
     "\n	-4,-6	Search only IPv4/IPv6 space" \
     "\n	-s	Don't display PIDs" \
     "\n	-k	Kill found processes" \
     "\n	-SIGNAL	Signal to send (default: KILL)" \

#define iostat_trivial_usage \
       "[-c] [-d] [-t] [-z] [-k|-m] [ALL|BLOCKDEV...] [INTERVAL [COUNT]]" \

#define iostat_full_usage "\n\n" \
       "Report CPU and I/O statistics\n" \
     "\n	-c	Show CPU utilization" \
     "\n	-d	Show device utilization" \
     "\n	-t	Print current time" \
     "\n	-z	Omit devices with no activity" \
     "\n	-k	Use kb/s" \
     "\n	-m	Use Mb/s" \

#define kill_trivial_usage \
       "[-l] [-SIG] PID..." \

#define kill_full_usage "\n\n" \
       "Send a signal (default: TERM) to given PIDs\n" \
     "\n	-l	List all signal names and numbers" \

#define kill_example_usage \
       "$ ps | grep apache\n" \
       "252 root     root     S [apache]\n" \
       "263 www-data www-data S [apache]\n" \
       "264 www-data www-data S [apache]\n" \
       "265 www-data www-data S [apache]\n" \
       "266 www-data www-data S [apache]\n" \
       "267 www-data www-data S [apache]\n" \
       "$ kill 252\n" \

#define killall_trivial_usage \
       "[-l] [-q] [-SIG] PROCESS_NAME..." \

#define killall_full_usage "\n\n" \
       "Send a signal (default: TERM) to given processes\n" \
     "\n	-l	List all signal names and numbers" \
     "\n	-q	Don't complain if no processes were killed" \

#define killall_example_usage \
       "$ killall apache\n" \

#define killall5_trivial_usage \
       "[-l] [-SIG] [-o PID]..." \

#define killall5_full_usage "\n\n" \
       "Send a signal (default: TERM) to all processes outside current session\n" \
     "\n	-l	List all signal names and numbers" \
     "\n	-o PID	Don't signal this PID" \

#define lsof_trivial_usage \
       "" \

#define lsof_full_usage "\n\n" \
       "Show all open files" \

#define mpstat_trivial_usage \
       "[-A] [-I SUM|CPU|ALL|SCPU] [-u] [-P num|ALL] [INTERVAL [COUNT]]" \

#define mpstat_full_usage "\n\n" \
       "Per-processor statistics\n" \
     "\n	-A			Same as -I ALL -u -P ALL" \
     "\n	-I SUM|CPU|ALL|SCPU	Report interrupt statistics" \
     "\n	-P num|ALL		Processor to monitor" \
     "\n	-u			Report CPU utilization" \

#define nmeter_trivial_usage \
       "[-d MSEC] FORMAT_STRING" \

#define nmeter_full_usage "\n\n" \
       "Monitor system in real time" \
     "\n" \
     "\n -d MSEC	Milliseconds between updates, default:1000, none:-1" \
     "\n" \
     "\nFormat specifiers:" \
     "\n %Nc or %[cN]	CPU. N - bar size (default 10)" \
     "\n		(displays: S:system U:user N:niced D:iowait I:irq i:softirq)" \
     "\n %[nINTERFACE]	Network INTERFACE" \
     "\n %m		Allocated memory" \
     "\n %[mf]		Free memory" \
     "\n %[mt]		Total memory" \
     "\n %s		Allocated swap" \
     "\n %f		Number of used file descriptors" \
     "\n %Ni		Total/specific IRQ rate" \
     "\n %x		Context switch rate" \
     "\n %p		Forks" \
     "\n %[pn]		# of processes" \
     "\n %b		Block io" \
     "\n %Nt		Time (with N decimal points)" \
     "\n %r		Print <cr> instead of <lf> at EOL" \

#define pgrep_trivial_usage \
       "[-flanovx] [-s SID|-P PPID|PATTERN]" \

#define pgrep_full_usage "\n\n" \
       "Display process(es) selected by regex PATTERN\n" \
     "\n	-l	Show command name too" \
     "\n	-a	Show command line too" \
     "\n	-f	Match against entire command line" \
     "\n	-n	Show the newest process only" \
     "\n	-o	Show the oldest process only" \
     "\n	-v	Negate the match" \
     "\n	-x	Match whole name (not substring)" \
     "\n	-s	Match session ID (0 for current)" \
     "\n	-P	Match parent process ID" \

#define pkill_trivial_usage \
       "[-l|-SIGNAL] [-fnovx] [-s SID|-P PPID|PATTERN]" \

#define pkill_full_usage "\n\n" \
       "Send a signal to process(es) selected by regex PATTERN\n" \
     "\n	-l	List all signals" \
     "\n	-f	Match against entire command line" \
     "\n	-n	Signal the newest process only" \
     "\n	-o	Signal the oldest process only" \
     "\n	-v	Negate the match" \
     "\n	-x	Match whole name (not substring)" \
     "\n	-s	Match session ID (0 for current)" \
     "\n	-P	Match parent process ID" \

#if (ENABLE_FEATURE_PIDOF_SINGLE || ENABLE_FEATURE_PIDOF_OMIT) \

#define pidof_trivial_usage \
       "[OPTIONS] [NAME]..." \

#define USAGE_PIDOF "\n" \

#else \

#define pidof_trivial_usage \
       "[NAME]..." \

#define USAGE_PIDOF /* none */ \

#endif \

#define pidof_full_usage "\n\n" \
       "List PIDs of all processes with names that match NAMEs" \
	USAGE_PIDOF \
	IF_FEATURE_PIDOF_SINGLE( \
     "\n	-s	Show only one PID" \
	) \
	IF_FEATURE_PIDOF_OMIT( \
     "\n	-o PID	Omit given pid" \
     "\n		Use %PPID to omit pid of pidof's parent" \
	) \

#define pidof_example_usage \
       "$ pidof init\n" \
       "1\n" \
	IF_FEATURE_PIDOF_OMIT( \
       "$ pidof /bin/sh\n20351 5973 5950\n") \
	IF_FEATURE_PIDOF_OMIT( \
       "$ pidof /bin/sh -o %PPID\n20351 5950") \

#define pmap_trivial_usage \
       "[-xq] PID..." \

#define pmap_full_usage "\n\n" \
       "Display process memory usage" \
     "\n" \
     "\n	-x	Show details" \
     "\n	-q	Quiet" \

#define powertop_trivial_usage \
       "" \

#define powertop_full_usage "\n\n" \
       "Analyze power consumption on Intel-based laptops" \

#if ENABLE_DESKTOP \

#define ps_trivial_usage \
       "[-o COL1,COL2=HEADER]" IF_FEATURE_SHOW_THREADS(" [-T]") \

#define ps_full_usage "\n\n" \
       "Show list of processes\n" \
     "\n	-o COL1,COL2=HEADER	Select columns for display" \
	IF_FEATURE_SHOW_THREADS( \
     "\n	-T			Show threads" \
	) \

#else /* !ENABLE_DESKTOP */ \

#if !ENABLE_SELINUX && !ENABLE_FEATURE_PS_WIDE \

#define USAGE_PS "\nThis version of ps accepts no options" \

#else \

#define USAGE_PS "" \

#endif \

#define ps_trivial_usage \
       "" \

#define ps_full_usage "\n\n" \
       "Show list of processes\n" \
	USAGE_PS \
	IF_SELINUX( \
     "\n	-Z	Show selinux context" \
	) \
	IF_FEATURE_PS_WIDE( \
     "\n	w	Wide output" \
	) \
	IF_FEATURE_PS_LONG( \
     "\n	l	Long output" \
	) \
	IF_FEATURE_SHOW_THREADS( \
     "\n	T	Show threads" \
	) \

#endif /* ENABLE_DESKTOP */ \

#define ps_example_usage \
       "$ ps\n" \
       "  PID  Uid      Gid State Command\n" \
       "    1 root     root     S init\n" \
       "    2 root     root     S [kflushd]\n" \
       "    3 root     root     S [kupdate]\n" \
       "    4 root     root     S [kpiod]\n" \
       "    5 root     root     S [kswapd]\n" \
       "  742 andersen andersen S [bash]\n" \
       "  743 andersen andersen S -bash\n" \
       "  745 root     root     S [getty]\n" \
       " 2990 andersen andersen R ps\n" \

#define pstree_trivial_usage \
	"[-p] [PID|USER]" \

#define pstree_full_usage "\n\n" \
       "Display process tree, optionally start from USER or PID\n" \
     "\n	-p	Show pids" \

#define pwdx_trivial_usage \
       "PID..." \

#define pwdx_full_usage "\n\n" \
       "Show current directory for PIDs" \

#define smemcap_trivial_usage ">SMEMDATA.TAR" \

#define smemcap_full_usage "\n\n" \
       "Collect memory usage data in /proc and write it to stdout" \

#define sysctl_trivial_usage \
       "-p [-enq] [FILE...] / [-enqaw] [KEY[=VALUE]]..." \

#define sysctl_full_usage "\n\n" \
       "Show/set kernel parameters\n" \
     "\n	-p	Set values from FILEs (default /etc/sysctl.conf)" \
     "\n	-e	Don't warn about unknown keys" \
     "\n	-n	Don't show key names" \
     "\n	-q      Quiet" \
     "\n	-a	Show all values" \
     "\n	-w	Set values" \

#define sysctl_example_usage \
       "sysctl [-n] [-e] variable...\n" \
       "sysctl [-n] [-e] [-q] -w variable=value...\n" \
       "sysctl [-n] [-e] -a\n" \
       "sysctl [-n] [-e] [-q] -p file	(default /etc/sysctl.conf)\n" \
       "sysctl [-n] [-e] -A\n" \

#if ENABLE_FEATURE_SHOW_THREADS || ENABLE_FEATURE_TOP_SMP_CPU \

# define IF_SHOW_THREADS_OR_TOP_SMP(...) __VA_ARGS__ \

#else \

# define IF_SHOW_THREADS_OR_TOP_SMP(...) \

#endif \

#define top_trivial_usage \
       "[-b] [-nCOUNT] [-dSECONDS]" IF_FEATURE_TOPMEM(" [-m]") \

#define top_full_usage "\n\n" \
       "Provide a view of process activity in real time." \
   "\n""Read the status of all processes from /proc each SECONDS" \
   "\n""and display a screenful of them." \
   "\n" \
	IF_FEATURE_TOP_INTERACTIVE( \
       "Keys:" \
   "\n""	N/M" \
                IF_FEATURE_TOP_CPU_USAGE_PERCENTAGE("/P") \
                IF_FEATURE_TOP_CPU_USAGE_PERCENTAGE("/T") \
           ": " IF_FEATURE_TOPMEM("show CPU usage, ") "sort by pid/mem" \
                IF_FEATURE_TOP_CPU_USAGE_PERCENTAGE("/cpu") \
                IF_FEATURE_TOP_CPU_USAGE_PERCENTAGE("/time") \
	IF_FEATURE_TOPMEM( \
   "\n""	S: show memory" \
	) \
   "\n""	R: reverse sort" \
	IF_SHOW_THREADS_OR_TOP_SMP( \
   "\n""	" \
                IF_FEATURE_SHOW_THREADS("H: toggle threads") \
                IF_FEATURE_SHOW_THREADS(IF_FEATURE_TOP_SMP_CPU(", ")) \
                IF_FEATURE_TOP_SMP_CPU("1: toggle SMP") \
	) \
   "\n""	Q,^C: exit" \
   "\n" \
   "\n""Options:" \
	) \
   "\n""	-b	Batch mode" \
   "\n""	-n N	Exit after N iterations" \
   "\n""	-d N	Delay between updates" \
	IF_FEATURE_TOPMEM( \
   "\n""	-m	Same as 's' key" \
	) \

#define uptime_trivial_usage \
       "" \

#define uptime_full_usage "\n\n" \
       "Display the time since the last boot" \

#define uptime_example_usage \
       "$ uptime\n" \
       "  1:55pm  up  2:30, load average: 0.09, 0.04, 0.00\n" \

#define watch_trivial_usage \
       "[-n SEC] [-t] PROG ARGS" \

#define watch_full_usage "\n\n" \
       "Run PROG periodically\n" \
     "\n	-n	Loop period in seconds (default 2)" \
     "\n	-t	Don't print header" \

#define watch_example_usage \
       "$ watch date\n" \
       "Mon Dec 17 10:31:40 GMT 2000\n" \
       "Mon Dec 17 10:31:42 GMT 2000\n" \
       "Mon Dec 17 10:31:44 GMT 2000" \

#define chpst_trivial_usage \
       "[-vP012] [-u USER[:GRP]] [-U USER[:GRP]] [-e DIR]\n" \
       "	[-/ DIR] [-n NICE] [-m BYTES] [-d BYTES] [-o N]\n" \
       "	[-p N] [-f BYTES] [-c BYTES] PROG ARGS" \

#define chpst_full_usage "\n\n" \
       "Change the process state, run PROG\n" \
     "\n	-u USER[:GRP]	Set uid and gid" \
     "\n	-U USER[:GRP]	Set $UID and $GID in environment" \
     "\n	-e DIR		Set environment variables as specified by files" \
     "\n			in DIR: file=1st_line_of_file" \
     "\n	-/ DIR		Chroot to DIR" \
     "\n	-n NICE		Add NICE to nice value" \
     "\n	-m BYTES	Same as -d BYTES -s BYTES -l BYTES" \
     "\n	-d BYTES	Limit data segment" \
     "\n	-o N		Limit number of open files per process" \
     "\n	-p N		Limit number of processes per uid" \
     "\n	-f BYTES	Limit output file sizes" \
     "\n	-c BYTES	Limit core file size" \
     "\n	-v		Verbose" \
     "\n	-P		Create new process group" \
     "\n	-0		Close stdin" \
     "\n	-1		Close stdout" \
     "\n	-2		Close stderr" \

#define envdir_trivial_usage \
       "DIR PROG ARGS" \

#define envdir_full_usage "\n\n" \
       "Set various environment variables as specified by files\n" \
       "in the directory DIR, run PROG" \

#define envuidgid_trivial_usage \
       "USER PROG ARGS" \

#define envuidgid_full_usage "\n\n" \
       "Set $UID to USER's uid and $GID to USER's gid, run PROG" \

#define setuidgid_trivial_usage \
       "USER PROG ARGS" \

#define setuidgid_full_usage "\n\n" \
       "Set uid and gid to USER's uid and gid, drop supplementary group ids,\n" \
       "run PROG" \

#define softlimit_trivial_usage \
       "[-a BYTES] [-m BYTES] [-d BYTES] [-s BYTES] [-l BYTES]\n" \
       "	[-f BYTES] [-c BYTES] [-r BYTES] [-o N] [-p N] [-t N]\n" \
       "	PROG ARGS" \

#define softlimit_full_usage "\n\n" \
       "Set soft resource limits, then run PROG\n" \
     "\n	-a BYTES	Limit total size of all segments" \
     "\n	-m BYTES	Same as -d BYTES -s BYTES -l BYTES -a BYTES" \
     "\n	-d BYTES	Limit data segment" \
     "\n	-s BYTES	Limit stack segment" \
     "\n	-l BYTES	Limit locked memory size" \
     "\n	-o N		Limit number of open files per process" \
     "\n	-p N		Limit number of processes per uid" \
     "\nOptions controlling file sizes:" \
     "\n	-f BYTES	Limit output file sizes" \
     "\n	-c BYTES	Limit core file size" \
     "\nEfficiency opts:" \
     "\n	-r BYTES	Limit resident set size" \
     "\n	-t N		Limit CPU time, process receives" \
     "\n			a SIGXCPU after N seconds" \

#define runsv_trivial_usage \
       "DIR" \

#define runsv_full_usage "\n\n" \
       "Start and monitor a service and optionally an appendant log service" \

#define runsvdir_trivial_usage \
       "[-P] [-s SCRIPT] DIR" \

#define runsvdir_full_usage "\n\n" \
       "Start a runsv process for each subdirectory. If it exits, restart it.\n" \
     "\n	-P		Put each runsv in a new session" \
     "\n	-s SCRIPT	Run SCRIPT <signo> after signal is processed" \

#define sv_trivial_usage \
       "[-v] [-w SEC] CMD SERVICE_DIR..." \

#define sv_full_usage "\n\n" \
       "Control services monitored by runsv supervisor.\n" \
       "Commands (only first character is enough):\n" \
       "\n" \
       "status: query service status\n" \
       "up: if service isn't running, start it. If service stops, restart it\n" \
       "once: like 'up', but if service stops, don't restart it\n" \
       "down: send TERM and CONT signals. If ./run exits, start ./finish\n" \
       "	if it exists. After it stops, don't restart service\n" \
       "exit: send TERM and CONT signals to service and log service. If they exit,\n" \
       "	runsv exits too\n" \
       "pause, cont, hup, alarm, interrupt, quit, 1, 2, term, kill: send\n" \
       "STOP, CONT, HUP, ALRM, INT, QUIT, USR1, USR2, TERM, KILL signal to service" \

#define svc_trivial_usage \
       "[-udopchaitkx] SERVICE_DIR..." \

#define svc_full_usage "\n\n" \
       "Control services monitored by runsv supervisor" \
   "\n" \
   "\n""	-u	If service is not running, start it; restart if it stops" \
   "\n""	-d	If service is running, send TERM+CONT signals; do not restart it" \
   "\n""	-o	Once: if service is not running, start it; do not restart it" \
   "\n""	-pchaitk Send STOP, CONT, HUP, ALRM, INT, TERM, KILL signal to service" \
   "\n""	-x	Exit: runsv will exit as soon as the service is down" \

#define svok_trivial_usage \
       "SERVICE_DIR" \

#define svok_full_usage "\n\n" \
       "Check whether runsv supervisor is running.\n" \
       "Exit code is 0 if it does, 100 if it does not,\n" \
       "111 (with error message) if SERVICE_DIR does not exist." \

#define svlogd_trivial_usage \
       "[-tttv] [-r C] [-R CHARS] [-l MATCHLEN] [-b BUFLEN] DIR..." \

#define svlogd_full_usage "\n\n" \
       "Read log data from stdin and write to rotated log files in DIRs" \
   "\n" \
   "\n""-r C		Replace non-printable characters with C" \
   "\n""-R CHARS	Also replace CHARS with C (default _)" \
   "\n""-t		Timestamp with @tai64n" \
   "\n""-tt		Timestamp with yyyy-mm-dd_hh:mm:ss.sssss" \
   "\n""-ttt		Timestamp with yyyy-mm-ddThh:mm:ss.sssss" \
   "\n""-v		Verbose" \
   "\n" \
   "\n""DIR/config file modifies behavior:" \
   "\n""sSIZE - when to rotate logs (default 1000000, 0 disables)" \
   "\n""nNUM - number of files to retain" \
   "\n""!PROG - process rotated log with PROG" \
   "\n""+,-PATTERN - (de)select line for logging" \
   "\n""E,ePATTERN - (de)select line for stderr" \

#define chcon_trivial_usage \
       "[OPTIONS] CONTEXT FILE..." \
       "\n	chcon [OPTIONS] [-u USER] [-r ROLE] [-l RANGE] [-t TYPE] FILE..." \
	IF_LONG_OPTS( \
       "\n	chcon [OPTIONS] --reference=RFILE FILE..." \
	) \

#define chcon_full_usage "\n\n" \
       "Change the security context of each FILE to CONTEXT\n" \
     "\n	-v	Verbose" \
     "\n	-c	Report changes made" \
     "\n	-h	Affect symlinks instead of their targets" \
     "\n	-f	Suppress most error messages" \
	IF_LONG_OPTS( \
     "\n	--reference RFILE Use RFILE's group instead of using a CONTEXT value" \
	) \
     "\n	-u USER	Set user/role/type/range in the target security context" \
     "\n	-r ROLE" \
     "\n	-t TYPE" \
     "\n	-l RNG" \
     "\n	-R	Recurse" \

#define getenforce_trivial_usage NOUSAGE_STR \

#define getenforce_full_usage "" \

#define getsebool_trivial_usage \
       "-a or getsebool boolean..." \

#define getsebool_full_usage "\n\n" \
       "	-a	Show all selinux booleans" \

#define load_policy_trivial_usage NOUSAGE_STR \

#define load_policy_full_usage "" \

#define matchpathcon_trivial_usage \
       "[-n] [-N] [-f file_contexts_file] [-p prefix] [-V]" \

#define matchpathcon_full_usage "\n\n" \
       "	-n	Don't display path" \
     "\n	-N	Don't use translations" \
     "\n	-f	Use alternate file_context file" \
     "\n	-p	Use prefix to speed translations" \
     "\n	-V	Verify file context on disk matches defaults" \

#define runcon_trivial_usage \
       "[-c] [-u USER] [-r ROLE] [-t TYPE] [-l RANGE] PROG ARGS\n" \
       "runcon CONTEXT PROG ARGS" \

#define runcon_full_usage "\n\n" \
       "Run PROG in a different security context\n" \
     "\n	CONTEXT		Complete security context\n" \
     "\n	-c	Compute process transition context before modifying" \
     "\n	-t TYPE	Type (for same role as parent)" \
     "\n	-u USER	User identity" \
     "\n	-r ROLE	Role" \
     "\n	-l RNG	Levelrange" \

#define selinuxenabled_trivial_usage NOUSAGE_STR \

#define selinuxenabled_full_usage "" \

#define sestatus_trivial_usage \
       "[-vb]" \

#define sestatus_full_usage "\n\n" \
       "	-v	Verbose" \
     "\n	-b	Display current state of booleans" \

#define setenforce_trivial_usage \
       "[Enforcing | Permissive | 1 | 0]" \

#define setenforce_full_usage "" \

#define setfiles_trivial_usage \
       "[-dnpqsvW] [-e DIR]... [-o FILE] [-r alt_root_path]" \
	IF_FEATURE_SETFILES_CHECK_OPTION( \
       " [-c policyfile] spec_file" \
	) \
       " pathname" \

#define setfiles_full_usage "\n\n" \
       "Reset file contexts under pathname according to spec_file\n" \
	IF_FEATURE_SETFILES_CHECK_OPTION( \
     "\n	-c FILE	Check the validity of the contexts against the specified binary policy" \
	) \
     "\n	-d	Show which specification matched each file" \
     "\n	-l	Log changes in file labels to syslog" \
     "\n	-n	Don't change any file labels" \
     "\n	-q	Suppress warnings" \
     "\n	-r DIR	Use an alternate root path" \
     "\n	-e DIR	Exclude DIR" \
     "\n	-F	Force reset of context to match file_context for customizable files" \
     "\n	-o FILE	Save list of files with incorrect context" \
     "\n	-s	Take a list of files from stdin (instead of command line)" \
     "\n	-v	Show changes in file labels, if type or role are changing" \
     "\n	-vv	Show changes in file labels, if type, role, or user are changing" \
     "\n	-W	Display warnings about entries that had no matching files" \

#define restorecon_trivial_usage \
       "[-iFnRv] [-e EXCLUDEDIR]... [-o FILE] [-f FILE]" \

#define restorecon_full_usage "\n\n" \
       "Reset security contexts of files in pathname\n" \
     "\n	-i	Ignore files that don't exist" \
     "\n	-f FILE	File with list of files to process" \
     "\n	-e DIR	Directory to exclude" \
     "\n	-R,-r	Recurse" \
     "\n	-n	Don't change any file labels" \
     "\n	-o FILE	Save list of files with incorrect context" \
     "\n	-v	Verbose" \
     "\n	-vv	Show changed labels" \
     "\n	-F	Force reset of context to match file_context" \
     "\n		for customizable files, or the user section," \
     "\n		if it has changed" \

#define setsebool_trivial_usage \
       "boolean value" \

#define setsebool_full_usage "\n\n" \
       "Change boolean setting" \

#define ash_trivial_usage \
	"[-/+OPTIONS] [-/+o OPT]... [-c 'SCRIPT' [ARG0 [ARGS]] / FILE [ARGS] / -s [ARGS]]" \

#define ash_full_usage "\n\n" \
	"Unix shell interpreter" \

#define cttyhack_trivial_usage \
       "[PROG ARGS]" \

#define cttyhack_full_usage "\n\n" \
       "Give PROG a controlling tty if possible." \
     "\nExample for /etc/inittab (for busybox init):" \
     "\n	::respawn:/bin/cttyhack /bin/sh" \
     "\nGiving controlling tty to shell running with PID 1:" \
     "\n	$ exec cttyhack sh" \
     "\nStarting interactive shell from boot shell script:" \
     "\n	setsid cttyhack sh" \

#define hush_trivial_usage \
	"[-enxl] [-c 'SCRIPT' [ARG0 [ARGS]] / FILE [ARGS] / -s [ARGS]]" \

#define hush_full_usage "\n\n" \
	"Unix shell interpreter" \

#define klogd_trivial_usage \
       "[-c N] [-n]" \

#define klogd_full_usage "\n\n" \
       "Log kernel messages to syslog\n" \
     "\n	-c N	Print to console messages more urgent than prio N (1-8)" \
     "\n	-n	Run in foreground" \

#define logger_trivial_usage \
       "[OPTIONS] [MESSAGE]" \

#define logger_full_usage "\n\n" \
       "Write MESSAGE (or stdin) to syslog\n" \
     "\n	-s	Log to stderr as well as the system log" \
     "\n	-t TAG	Log using the specified tag (defaults to user name)" \
     "\n	-p PRIO	Priority (numeric or facility.level pair)" \

#define logger_example_usage \
       "$ logger \"hello\"\n" \

#define logread_trivial_usage \
       "[-fF]" \

#define logread_full_usage "\n\n" \
       "Show messages in syslogd's circular buffer\n" \
     "\n	-f	Output data as log grows" \
     "\n	-F	Same as -f, but dump buffer first" \

#define syslogd_trivial_usage \
       "[OPTIONS]" \

#define syslogd_full_usage "\n\n" \
       "System logging utility\n" \
	IF_NOT_FEATURE_SYSLOGD_CFG( \
       "(this version of syslogd ignores /etc/syslog.conf)\n" \
	) \
     "\n	-n		Run in foreground" \
	IF_FEATURE_REMOTE_LOG( \
     "\n	-R HOST[:PORT]	Log to HOST:PORT (default PORT:514)" \
     "\n	-L		Log locally and via network (default is network only if -R)" \
	) \
	IF_FEATURE_IPC_SYSLOG( \
     "\n	-C[size_kb]	Log to shared mem buffer (use logread to read it)" \
	) \
	IF_FEATURE_KMSG_SYSLOG( \
     "\n	-K		Log to kernel printk buffer (use dmesg to read it)" \
	) \
     "\n	-O FILE		Log to FILE (default: /var/log/messages, stdout if -)" \
	IF_FEATURE_ROTATE_LOGFILE( \
     "\n	-s SIZE		Max size (KB) before rotation (default 200KB, 0=off)" \
     "\n	-b N		N rotated logs to keep (default 1, max 99, 0=purge)" \
	) \
     "\n	-l N		Log only messages more urgent than prio N (1-8)" \
     "\n	-S		Smaller output" \
     "\n	-t		Strip client-generated timestamps" \
	IF_FEATURE_SYSLOGD_DUP( \
     "\n	-D		Drop duplicates" \
	) \
	IF_FEATURE_SYSLOGD_CFG( \
     "\n	-f FILE		Use FILE as config (default:/etc/syslog.conf)" \
	) \

#define syslogd_example_usage \
       "$ syslogd -R masterlog:514\n" \
       "$ syslogd -R 192.168.1.1:601\n" \

#define acpid_trivial_usage \
       "[-df] [-c CONFDIR] [-l LOGFILE] [-a ACTIONFILE] [-M MAPFILE] [-e PROC_EVENT_FILE] [-p PIDFILE]" \

#define acpid_full_usage "\n\n" \
       "Listen to ACPI events and spawn specific helpers on event arrival\n" \
     "\n	-d	Log to stderr, not log file (implies -f)" \
     "\n	-f	Run in foreground" \
     "\n	-c DIR	Config directory [/etc/acpi]" \
     "\n	-e FILE	/proc event file [/proc/acpi/event]" \
     "\n	-l FILE	Log file [/var/log/acpid.log]" \
     "\n	-p FILE	Pid file [/var/run/acpid.pid]" \
     "\n	-a FILE	Action file [/etc/acpid.conf]" \
     "\n	-M FILE Map file [/etc/acpi.map]" \
	IF_FEATURE_ACPID_COMPAT( \
     "\n\nAccept and ignore compatibility options -g -m -s -S -v" \
	) \

#define acpid_example_usage \
       "Without -e option, acpid uses all /dev/input/event* files\n" \
       "# acpid\n" \
       "# acpid -l /var/log/my-acpi-log\n" \
       "# acpid -e /proc/acpi/event\n" \

#define blkdiscard_trivial_usage \
       "[-o OFS] [-l LEN] [-s] DEVICE" \

#define blkdiscard_full_usage "\n\n" \
	"Discard sectors on DEVICE\n" \
	"\n	-o OFS	Byte offset into device" \
	"\n	-l LEN	Number of bytes to discard" \
	"\n	-s	Perform a secure discard" \

#define blkdiscard_example_usage \
	"$ blkdiscard -o 0 -l 1G /dev/sdb" \

#define blkid_trivial_usage \
       "[BLOCKDEV]..." \

#define blkid_full_usage "\n\n" \
       "Print UUIDs of all filesystems" \

#define blockdev_trivial_usage \
	"OPTION BLOCKDEV" \

#define blockdev_full_usage "\n\n" \
       "	--setro		Set ro" \
     "\n	--setrw		Set rw" \
     "\n	--getro		Get ro" \
     "\n	--getss		Get sector size" \
     "\n	--getbsz	Get block size" \
     "\n	--setbsz BYTES	Set block size" \
     "\n	--getsz		Get device size in 512-byte sectors" \
     "\n	--getsize64	Get device size in bytes" \
     "\n	--flushbufs	Flush buffers" \
     "\n	--rereadpt	Reread partition table" \

#define cal_trivial_usage \
       "[-jy] [[MONTH] YEAR]" \

#define cal_full_usage "\n\n" \
       "Display a calendar\n" \
     "\n	-j	Use julian dates" \
     "\n	-y	Display the entire year" \

#define chrt_trivial_usage \
       "[-prfombi] [PRIO] [PID | PROG ARGS]" \

#define chrt_full_usage "\n\n" \
       "Change scheduling priority and class for a process\n" \
     "\n	-p	Operate on PID" \
     "\n	-r	Set SCHED_RR class" \
     "\n	-f	Set SCHED_FIFO class" \
     "\n	-o	Set SCHED_OTHER class" \
     "\n	-b	Set SCHED_BATCH class" \
     "\n	-i	Set SCHED_IDLE class" \
     "\n	-m	Show min/max priorities" \

#define chrt_example_usage \
       "$ chrt -r 4 sleep 900; x=$!\n" \
       "$ chrt -f -p 3 $x\n" \
       "You need CAP_SYS_NICE privileges to set scheduling attributes of a process" \

#define dmesg_trivial_usage \
       "[-c] [-n LEVEL] [-s SIZE]" \

#define dmesg_full_usage "\n\n" \
       "Print or control the kernel ring buffer\n" \
     "\n	-c		Clear ring buffer after printing" \
     "\n	-n LEVEL	Set console logging level" \
     "\n	-s SIZE		Buffer size" \
     "\n	-r		Print raw message buffer" \

#define eject_trivial_usage \
       "[-t] [-T] [DEVICE]" \

#define eject_full_usage "\n\n" \
       "Eject DEVICE or default /dev/cdrom\n" \
	IF_FEATURE_EJECT_SCSI( \
     "\n	-s	SCSI device" \
	) \
     "\n	-t	Close tray" \
     "\n	-T	Open/close tray (toggle)" \

#define fallocate_trivial_usage \
       "[-o OFS] -l LEN FILE" \

#define fallocate_full_usage "\n\n" \
	"Preallocate space for FILE\n" \
     "\n	-o OFS	Offset of range" \
     "\n	-l LEN	Length of range" \

#define fatattr_trivial_usage \
       "[-+rhsvda] FILE..." \

#define fatattr_full_usage "\n\n" \
       "Change file attributes on FAT filesystem\n" \
     "\n	-	Clear attributes" \
     "\n	+	Set attributes" \
     "\n	r	Read only" \
     "\n	h	Hidden" \
     "\n	s	System" \
     "\n	v	Volume label" \
     "\n	d	Directory" \
     "\n	a	Archive" \

#define fbset_trivial_usage \
       "[OPTIONS] [MODE]" \

#define fbset_full_usage "\n\n" \
       "Show and modify frame buffer settings" \

#define fbset_example_usage \
       "$ fbset\n" \
       "mode \"1024x768-76\"\n" \
       "	# D: 78.653 MHz, H: 59.949 kHz, V: 75.694 Hz\n" \
       "	geometry 1024 768 1024 768 16\n" \
       "	timings 12714 128 32 16 4 128 4\n" \
       "	accel false\n" \
       "	rgba 5/11,6/5,5/0,0/0\n" \
       "endmode\n" \

#define fdformat_trivial_usage \
       "[-n] DEVICE" \

#define fdformat_full_usage "\n\n" \
       "Format floppy disk\n" \
     "\n	-n	Don't verify after format" \

#ifndef ENABLE_FEATURE_FDISK_BLKSIZE \

# define ENABLE_FEATURE_FDISK_BLKSIZE 0 \

# define IF_FEATURE_FDISK_BLKSIZE(a) \

#endif \

#define fdisk_trivial_usage \
       "[-ul" IF_FEATURE_FDISK_BLKSIZE("s") "] " \
       "[-C CYLINDERS] [-H HEADS] [-S SECTORS] [-b SSZ] DISK" \

#define fdisk_full_usage "\n\n" \
       "Change partition table\n" \
     "\n	-u		Start and End are in sectors (instead of cylinders)" \
     "\n	-l		Show partition table for each DISK, then exit" \
	IF_FEATURE_FDISK_BLKSIZE( \
     "\n	-s		Show partition sizes in kb for each DISK, then exit" \
	) \
     "\n	-b 2048		(for certain MO disks) use 2048-byte sectors" \
     "\n	-C CYLINDERS	Set number of cylinders/heads/sectors" \
     "\n	-H HEADS	Typically 255" \
     "\n	-S SECTORS	Typically 63" \

#define findfs_trivial_usage \
       "LABEL=label or UUID=uuid" \

#define findfs_full_usage "\n\n" \
       "Find a filesystem device based on a label or UUID" \

#define findfs_example_usage \
       "$ findfs LABEL=MyDevice" \

#define flock_trivial_usage \
       "[-sxun] FD|{FILE [-c] PROG ARGS}" \

#define flock_full_usage "\n\n" \
       "[Un]lock file descriptor, or lock FILE, run PROG\n" \
     "\n	-s	Shared lock" \
     "\n	-x	Exclusive lock (default)" \
     "\n	-u	Unlock FD" \
     "\n	-n	Fail rather than wait" \

#define freeramdisk_trivial_usage \
       "DEVICE" \

#define freeramdisk_full_usage "\n\n" \
       "Free all memory used by the specified ramdisk" \

#define freeramdisk_example_usage \
       "$ freeramdisk /dev/ram2\n" \

#define fdflush_trivial_usage \
       "DEVICE" \

#define fdflush_full_usage "\n\n" \
       "Force floppy disk drive to detect disk change" \

#define fsck_minix_trivial_usage \
       "[-larvsmf] BLOCKDEV" \

#define fsck_minix_full_usage "\n\n" \
       "Check MINIX filesystem\n" \
     "\n	-l	List all filenames" \
     "\n	-r	Perform interactive repairs" \
     "\n	-a	Perform automatic repairs" \
     "\n	-v	Verbose" \
     "\n	-s	Output superblock information" \
     "\n	-m	Show \"mode not cleared\" warnings" \
     "\n	-f	Force file system check" \

#define fsfreeze_trivial_usage \
       "--[un]freeze MOUNTPOINT" \

#define fsfreeze_full_usage "\n\n" \
	"Flush and halt writes to MOUNTPOINT" \

#define fstrim_trivial_usage \
       "[OPTIONS] MOUNTPOINT" \

#define fstrim_full_usage "\n\n" \
	IF_LONG_OPTS( \
       "	-o,--offset OFFSET	Offset in bytes to discard from" \
     "\n	-l,--length LEN		Bytes to discard" \
     "\n	-m,--minimum MIN	Minimum extent length" \
     "\n	-v,--verbose		Print number of discarded bytes" \
	) \
	IF_NOT_LONG_OPTS( \
       "	-o OFFSET	Offset in bytes to discard from" \
     "\n	-l LEN		Bytes to discard" \
     "\n	-m MIN		Minimum extent length" \
     "\n	-v		Print number of discarded bytes" \
	) \

#define getopt_trivial_usage \
       "[OPTIONS] [--] OPTSTRING PARAMS" \

#define getopt_full_usage "\n\n" \
	IF_FEATURE_GETOPT_LONG( \
       "	-a		Allow long options starting with single -\n" \
       "	-l LOPT[,...]	Long options to recognize\n" \
	) \
       "	-n PROGNAME	The name under which errors are reported" \
     "\n	-o OPTSTRING	Short options to recognize" \
     "\n	-q		No error messages on unrecognized options" \
     "\n	-Q		No normal output" \
     "\n	-s SHELL	Set shell quoting conventions" \
     "\n	-T		Version test (exits with 4)" \
     "\n	-u		Don't quote output" \
	IF_FEATURE_GETOPT_LONG( /* example uses -l, needs FEATURE_GETOPT_LONG */ \
     "\n" \
     "\nExample:" \
     "\n" \
     "\nO=`getopt -l bb: -- ab:c:: \"$@\"` || exit 1" \
     "\neval set -- \"$O\"" \
     "\nwhile true; do" \
     "\n	case \"$1\" in" \
     "\n	-a)	echo A; shift;;" \
     "\n	-b|--bb) echo \"B:'$2'\"; shift 2;;" \
     "\n	-c)	case \"$2\" in" \
     "\n		\"\")	echo C; shift 2;;" \
     "\n		*)	echo \"C:'$2'\"; shift 2;;" \
     "\n		esac;;" \
     "\n	--)	shift; break;;" \
     "\n	*)	echo Error; exit 1;;" \
     "\n	esac" \
     "\ndone" \
	) \

#define getopt_example_usage \
       "$ cat getopt.test\n" \
       "#!/bin/sh\n" \
       "GETOPT=`getopt -o ab:c:: --long a-long,b-long:,c-long:: \\\n" \
       "       -n 'example.busybox' -- \"$@\"`\n" \
       "if [ $? != 0 ]; then exit 1; fi\n" \
       "eval set -- \"$GETOPT\"\n" \
       "while true; do\n" \
       " case $1 in\n" \
       "   -a|--a-long) echo \"Option a\"; shift;;\n" \
       "   -b|--b-long) echo \"Option b, argument '$2'\"; shift 2;;\n" \
       "   -c|--c-long)\n" \
       "     case \"$2\" in\n" \
       "       \"\") echo \"Option c, no argument\"; shift 2;;\n" \
       "       *)  echo \"Option c, argument '$2'\"; shift 2;;\n" \
       "     esac;;\n" \
       "   --) shift; break;;\n" \
       "   *) echo \"Internal error!\"; exit 1;;\n" \
       " esac\n" \
       "done\n" \

#define hexdump_trivial_usage \
       "[-bcCdefnosvx" IF_FEATURE_HEXDUMP_REVERSE("R") "] [FILE]..." \

#define hexdump_full_usage "\n\n" \
       "Display FILEs (or stdin) in a user specified format\n" \
     "\n	-b		1-byte octal display" \
     "\n	-c		1-byte character display" \
     "\n	-d		2-byte decimal display" \
     "\n	-o		2-byte octal display" \
     "\n	-x		2-byte hex display" \
     "\n	-C		hex+ASCII 16 bytes per line" \
     "\n	-v		Show all (no dup folding)" \
     "\n	-e FORMAT_STR	Example: '16/1 \"%02x|\"\"\\n\"'" \
     "\n	-f FORMAT_FILE" \
     "\n	-n LENGTH	Show only first LENGTH bytes" \
     "\n	-s OFFSET	Skip OFFSET bytes" \
	IF_FEATURE_HEXDUMP_REVERSE( \
     "\n	-R		Reverse of 'hexdump -Cv'") \

#define hd_trivial_usage \
       "FILE..." \

#define hd_full_usage "\n\n" \
       "hd is an alias for hexdump -C" \

#define xxd_trivial_usage \
       "[OPTIONS] [FILE]" \

#define xxd_full_usage "\n\n" \
       "Hex dump FILE (or stdin)\n" \
     "\n	-g N		Bytes per group" \
     "\n	-c N		Bytes per line" \
     "\n	-p		Show only hex bytes, assumes -c30" \
     "\n	-l LENGTH	Show only first LENGTH bytes" \
     "\n	-s OFFSET	Skip OFFSET bytes" \

#define hwclock_trivial_usage \
	IF_LONG_OPTS( \
       "[-r|--show] [-s|--hctosys] [-w|--systohc] [--systz]" \
       " [--localtime] [-u|--utc]" \
       " [-f|--rtc FILE]" \
	) \
	IF_NOT_LONG_OPTS( \
       "[-r] [-s] [-w] [-t] [-l] [-u] [-f FILE]" \
	) \

#define hwclock_full_usage "\n\n" \
       "Query and set hardware clock (RTC)\n" \
     "\n	-r	Show hardware clock time" \
     "\n	-s	Set system time from hardware clock" \
     "\n	-w	Set hardware clock from system time" \
	IF_LONG_OPTS( \
     "\n	--systz	Set in-kernel timezone, correct system time" \
	) \
     "\n		if hardware clock is in local time" \
     "\n	-u	Assume hardware clock is kept in UTC" \
	IF_LONG_OPTS( \
     "\n	--localtime	Assume hardware clock is kept in local time" \
	) \
     "\n	-f FILE	Use specified device (e.g. /dev/rtc2)" \

#define ionice_trivial_usage \
	"[-c 1-3] [-n 0-7] [-p PID] [PROG]" \

#define ionice_full_usage "\n\n" \
       "Change I/O priority and class\n" \
     "\n	-c	Class. 1:realtime 2:best-effort 3:idle" \
     "\n	-n	Priority" \

#define ipcrm_trivial_usage \
       "[-MQS key] [-mqs id]" \

#define ipcrm_full_usage "\n\n" \
       "Upper-case options MQS remove an object by shmkey value.\n" \
       "Lower-case options remove an object by shmid value.\n" \
     "\n	-mM	Remove memory segment after last detach" \
     "\n	-qQ	Remove message queue" \
     "\n	-sS	Remove semaphore" \

#define ipcs_trivial_usage \
       "[[-smq] -i SHMID] | [[-asmq] [-tcplu]]" \

#define ipcs_full_usage "\n\n" \
       "	-i ID	Show specific resource" \
     "\nResource specification:" \
     "\n	-m	Shared memory segments" \
     "\n	-q	Message queues" \
     "\n	-s	Semaphore arrays" \
     "\n	-a	All (default)" \
     "\nOutput format:" \
     "\n	-t	Time" \
     "\n	-c	Creator" \
     "\n	-p	Pid" \
     "\n	-l	Limits" \
     "\n	-u	Summary" \

#define last_trivial_usage \
       ""IF_FEATURE_LAST_FANCY("[-HW] [-f FILE]") \

#define last_full_usage "\n\n" \
       "Show listing of the last users that logged into the system" \
	IF_FEATURE_LAST_FANCY( "\n" \
     "\n	-W	Display with no host column truncation" \
     "\n	-f FILE Read from FILE instead of /var/log/wtmp" \
	) \

#define losetup_trivial_usage \
       "[-r] [-o OFS] {-f|LOOPDEV} FILE - associate loop devices\n" \
       "	losetup -d LOOPDEV - disassociate\n" \
       "	losetup -a - show status\n" \
       "	losetup -f - show next free loop device" \

#define losetup_full_usage "\n\n" \
       "	-o OFS	Start OFS bytes into FILE" \
     "\n	-r	Read-only" \
     "\n	-f	Show/use next free loop device" \

#define losetup_notes_usage \
       "One argument (losetup /dev/loop1) will display the current association\n" \
       "(if any), or disassociate it (with -d). The display shows the offset\n" \
       "and filename of the file the loop device is currently bound to.\n\n" \
       "Two arguments (losetup /dev/loop1 file.img) create a new association,\n" \
       "with an optional offset (-o 12345). Encryption is not yet supported.\n" \
       "losetup -f will show the first loop free loop device\n\n" \

#define lspci_trivial_usage \
       "[-mk]" \

#define lspci_full_usage "\n\n" \
       "List all PCI devices" \
     "\n" \
     "\n	-m	Parsable output" \
     "\n	-k	Show driver" \

#define lsusb_trivial_usage NOUSAGE_STR \

#define lsusb_full_usage "" \

#define mdev_trivial_usage \
       "[-s]" \

#define mdev_full_usage "\n\n" \
       "mdev -s is to be run during boot to scan /sys and populate /dev.\n" \
       "\n" \
       "Bare mdev is a kernel hotplug helper. To activate it:\n" \
       "	echo /sbin/mdev >/proc/sys/kernel/hotplug\n" \
	IF_FEATURE_MDEV_CONF( \
       "\n" \
       "It uses /etc/mdev.conf with lines\n" \
       "	[-][ENV=regex;]...DEVNAME UID:GID PERM" \
			IF_FEATURE_MDEV_RENAME(" [>|=PATH]|[!]") \
			IF_FEATURE_MDEV_EXEC(" [@|$|*PROG]") \
       "\n" \
       "where DEVNAME is device name regex, @major,minor[-minor2], or\n" \
       "environment variable regex. A common use of the latter is\n" \
       "to load modules for hotplugged devices:\n" \
       "	$MODALIAS=.* 0:0 660 @modprobe \"$MODALIAS\"\n" \
	) \
       "\n" \
       "If /dev/mdev.seq file exists, mdev will wait for its value\n" \
       "to match $SEQNUM variable. This prevents plug/unplug races.\n" \
       "To activate this feature, create empty /dev/mdev.seq at boot.\n" \
       "\n" \
       "If /dev/mdev.log file exists, debug log will be appended to it." \

#define mesg_trivial_usage \
       "[y|n]" \

#define mesg_full_usage "\n\n" \
       "Control write access to your terminal\n" \
       "	y	Allow write access to your terminal\n" \
       "	n	Disallow write access to your terminal" \

#define mkfs_ext2_trivial_usage \
       "[-Fn] " \
       "[-b BLK_SIZE] " \
       "[-i INODE_RATIO] [-I INODE_SIZE] " \
       "[-m RESERVED_PERCENT] " \
       "[-L LABEL] " \
       "BLOCKDEV [KBYTES]" \

#define mkfs_ext2_full_usage "\n\n" \
       "	-b BLK_SIZE	Block size, bytes" \
     "\n	-F		Force" \
     "\n	-i RATIO	Max number of files is filesystem_size / RATIO" \
     "\n	-I BYTES	Inode size (min 128)" \
     "\n	-L LBL		Volume label" \
     "\n	-m PERCENT	Percent of blocks to reserve for admin" \
     "\n	-n		Dry run" \

#define mkfs_minix_trivial_usage \
       "[-c | -l FILE] [-nXX] [-iXX] BLOCKDEV [KBYTES]" \

#define mkfs_minix_full_usage "\n\n" \
       "Make a MINIX filesystem\n" \
     "\n	-c		Check device for bad blocks" \
     "\n	-n [14|30]	Maximum length of filenames" \
     "\n	-i INODES	Number of inodes for the filesystem" \
     "\n	-l FILE		Read bad blocks list from FILE" \
     "\n	-v		Make version 2 filesystem" \

#define mkfs_reiser_trivial_usage \
       "[-f] [-l LABEL] BLOCKDEV [4K-BLOCKS]" \

#define mkfs_reiser_full_usage "\n\n" \
       "Make a ReiserFS V3 filesystem\n" \
     "\n	-f	Force" \
     "\n	-l LBL	Volume label" \

#define mkfs_vfat_trivial_usage \
       "[-v] [-n LABEL] BLOCKDEV [KBYTES]" \

#define mkfs_vfat_full_usage "\n\n" \
       "Make a FAT32 filesystem\n" \
     "\n	-v	Verbose" \
     "\n	-n LBL	Volume label" \

#define mkswap_trivial_usage \
       "[-L LBL] BLOCKDEV [KBYTES]" \

#define mkswap_full_usage "\n\n" \
       "Prepare BLOCKDEV to be used as swap partition\n" \
     "\n	-L LBL	Label" \

#define more_trivial_usage \
       "[FILE]..." \

#define more_full_usage "\n\n" \
       "View FILE (or stdin) one screenful at a time" \

#define more_example_usage \
       "$ dmesg | more\n" \

#define mount_trivial_usage \
       "[OPTIONS] [-o OPT] DEVICE NODE" \

#define mount_full_usage "\n\n" \
       "Mount a filesystem. Filesystem autodetection requires /proc.\n" \
     "\n	-a		Mount all filesystems in fstab" \
	IF_FEATURE_MOUNT_FAKE( \
	IF_FEATURE_MTAB_SUPPORT( \
     "\n	-f		Update /etc/mtab, but don't mount" \
	) \
	IF_NOT_FEATURE_MTAB_SUPPORT( \
     "\n	-f		Dry run" \
	) \
	) \
	IF_FEATURE_MOUNT_HELPERS( \
     "\n	-i		Don't run mount helper" \
	) \
	IF_FEATURE_MTAB_SUPPORT( \
     "\n	-n		Don't update /etc/mtab" \
	) \
	IF_FEATURE_MOUNT_VERBOSE( \
     "\n	-v		Verbose" \
	) \
     "\n	-r		Read-only mount" \
     "\n	-t FSTYPE[,...]	Filesystem type(s)" \
	IF_FEATURE_MOUNT_OTHERTAB( \
     "\n	-T FILE		Read FILE instead of /etc/fstab" \
	) \
     "\n	-O OPT		Mount only filesystems with option OPT (-a only)" \
     "\n-o OPT:" \
	IF_FEATURE_MOUNT_LOOP( \
     "\n	loop		Ignored (loop devices are autodetected)" \
	) \
	IF_FEATURE_MOUNT_FLAGS( \
     "\n	[a]sync		Writes are [a]synchronous" \
     "\n	[no]atime	Disable/enable updates to inode access times" \
     "\n	[no]diratime	Disable/enable atime updates to directories" \
     "\n	[no]relatime	Disable/enable atime updates relative to modification time" \
     "\n	[no]dev		(Dis)allow use of special device files" \
     "\n	[no]exec	(Dis)allow use of executable files" \
     "\n	[no]suid	(Dis)allow set-user-id-root programs" \
     "\n	[r]shared	Convert [recursively] to a shared subtree" \
     "\n	[r]slave	Convert [recursively] to a slave subtree" \
     "\n	[r]private	Convert [recursively] to a private subtree" \
     "\n	[un]bindable	Make mount point [un]able to be bind mounted" \
     "\n	[r]bind		Bind a file or directory [recursively] to another location" \
     "\n	move		Relocate an existing mount point" \
	) \
     "\n	remount		Remount a mounted filesystem, changing flags" \
     "\n	ro		Same as -r" \
     "\n" \
     "\nThere are filesystem-specific -o flags." \

#define mount_example_usage \
       "$ mount\n" \
       "/dev/hda3 on / type minix (rw)\n" \
       "proc on /proc type proc (rw)\n" \
       "devpts on /dev/pts type devpts (rw)\n" \
       "$ mount /dev/fd0 /mnt -t msdos -o ro\n" \
       "$ mount /tmp/diskimage /opt -t ext2 -o loop\n" \
       "$ mount cd_image.iso mydir\n" \

#define mount_notes_usage \
       "Returns 0 for success, number of failed mounts for -a, or errno for one mount." \

#define mountpoint_trivial_usage \
       "[-q] <[-dn] DIR | -x DEVICE>" \

#define mountpoint_full_usage "\n\n" \
       "Check if the directory is a mountpoint\n" \
     "\n	-q	Quiet" \
     "\n	-d	Print major/minor device number of the filesystem" \
     "\n	-n	Print device name of the filesystem" \
     "\n	-x	Print major/minor device number of the blockdevice" \

#define mountpoint_example_usage \
       "$ mountpoint /proc\n" \
       "/proc is not a mountpoint\n" \
       "$ mountpoint /sys\n" \
       "/sys is a mountpoint\n" \

#define nologin_trivial_usage \
	"" \

#define nologin_full_usage "\n\n" \
	"Politely refuse a login" \

#define nsenter_trivial_usage \
       "[OPTIONS] [PROG [ARGS]]" \

#define nsenter_full_usage "\n" \
     "\n	-t PID		Target process to get namespaces from" \
     "\n	-m[FILE]	Enter mount namespace" \
     "\n	-u[FILE]	Enter UTS namespace (hostname etc)" \
     "\n	-i[FILE]	Enter System V IPC namespace" \
     "\n	-n[FILE]	Enter network namespace" \
     "\n	-p[FILE]	Enter pid namespace" \
     "\n	-U[FILE]	Enter user namespace" \
     "\n	-S UID		Set uid in entered namespace" \
     "\n	-G GID		Set gid in entered namespace" \
	IF_LONG_OPTS( \
     "\n	--preserve-credentials	Don't touch uids or gids" \
	) \
     "\n	-r[DIR]		Set root directory" \
     "\n	-w[DIR]		Set working directory" \
     "\n	-F		Don't fork before exec'ing PROG" \

#define pivot_root_trivial_usage \
       "NEW_ROOT PUT_OLD" \

#define pivot_root_full_usage "\n\n" \
       "Move the current root file system to PUT_OLD and make NEW_ROOT\n" \
       "the new root file system" \

#define rdate_trivial_usage \
       "[-s/-p] HOST" \

#define rdate_full_usage "\n\n" \
       "Set and print time from HOST using RFC 868\n" \
     "\n	-s	Only set system time" \
     "\n	-p	Only print time" \

#define rdev_trivial_usage \
       "" \

#define rdev_full_usage "\n\n" \
       "Print the device node associated with the filesystem mounted at '/'" \

#define rdev_example_usage \
       "$ rdev\n" \
       "/dev/mtdblock9 /\n" \

#define readprofile_trivial_usage \
       "[OPTIONS]" \

#define readprofile_full_usage "\n\n" \
       "	-m mapfile	(Default: /boot/System.map)" \
     "\n	-p profile	(Default: /proc/profile)" \
     "\n	-M NUM		Set the profiling multiplier to NUM" \
     "\n	-i		Print only info about the sampling step" \
     "\n	-v		Verbose" \
     "\n	-a		Print all symbols, even if count is 0" \
     "\n	-b		Print individual histogram-bin counts" \
     "\n	-s		Print individual counters within functions" \
     "\n	-r		Reset all the counters (root only)" \
     "\n	-n		Disable byte order auto-detection" \

#define renice_trivial_usage \
       "[-n] PRIORITY [[-p | -g | -u] ID...]..." \

#define renice_full_usage "\n\n" \
       "Change scheduling priority of a running process\n" \
     "\n	-n	Add PRIORITY to current nice value" \
     "\n		Without -n, nice value is set to PRIORITY" \
     "\n	-p	Process ids (default)" \
     "\n	-g	Process group ids" \
     "\n	-u	Process user names" \

#define rev_trivial_usage \
	"[FILE]..." \

#define rev_full_usage "\n\n" \
	"Reverse lines of FILE" \

#define rtcwake_trivial_usage \
       "[-a | -l | -u] [-d DEV] [-m MODE] [-s SEC | -t TIME]" \

#define rtcwake_full_usage "\n\n" \
       "Enter a system sleep state until specified wakeup time\n" \
	IF_LONG_OPTS( \
     "\n	-a,--auto	Read clock mode from adjtime" \
     "\n	-l,--local	Clock is set to local time" \
     "\n	-u,--utc	Clock is set to UTC time" \
     "\n	-d,--device DEV	Specify the RTC device" \
     "\n	-m,--mode MODE	Set sleep state (default: standby)" \
     "\n	-s,--seconds SEC Set timeout in SEC seconds from now" \
     "\n	-t,--time TIME	Set timeout to TIME seconds from epoch" \
	) \
	IF_NOT_LONG_OPTS( \
     "\n	-a	Read clock mode from adjtime" \
     "\n	-l	Clock is set to local time" \
     "\n	-u	Clock is set to UTC time" \
     "\n	-d DEV	Specify the RTC device" \
     "\n	-m MODE	Set sleep state (default: standby)" \
     "\n	-s SEC	Set timeout in SEC seconds from now" \
     "\n	-t TIME	Set timeout to TIME seconds from epoch" \
	) \

#define script_trivial_usage \
       "[-afq] [-t[FILE]] [-c PROG] [OUTFILE]" \

#define script_full_usage "\n\n" \
       "Default OUTFILE is 'typescript'" \
     "\n" \
     "\n	-a	Append output" \
     "\n	-c PROG	Run PROG, not shell" \
     "\n	-q	Quiet" \
     "\n	-t[FILE] Send timing to stderr or FILE" \

#define scriptreplay_trivial_usage \
       "TIMINGFILE [TYPESCRIPT [DIVISOR]]" \

#define scriptreplay_full_usage "\n\n" \
       "Play back typescripts, using timing information" \

#define setarch_trivial_usage \
       "PERSONALITY [-R] PROG ARGS" \

#define setarch_full_usage "\n\n" \
       "PERSONALITY may be:" \
   "\n""	linux32	Set 32bit uname emulation" \
   "\n""	linux64	Set 64bit uname emulation" \
   "\n" \
   "\n""	-R	Disable address space randomization" \

#define linux32_trivial_usage NOUSAGE_STR \

#define linux32_full_usage "" \

#define linux64_trivial_usage NOUSAGE_STR \

#define linux64_full_usage "" \

#define setpriv_trivial_usage \
	"[OPTIONS] PROG [ARGS]" \

#define setpriv_full_usage "\n\n" \
       "Run PROG with different privilege settings\n" \
	IF_FEATURE_SETPRIV_DUMP( \
     "\n-d,--dump		Show current capabilities" \
	) \
     "\n--nnp,--no-new-privs	Ignore setuid/setgid bits and file capabilities" \
	IF_FEATURE_SETPRIV_CAPABILITIES( \
     "\n--inh-caps CAP,CAP	Set inheritable capabilities" \
     "\n--ambient-caps CAP,CAP	Set ambient capabilities" \
	) \

#define setsid_trivial_usage \
       "[-c] PROG ARGS" \

#define setsid_full_usage "\n\n" \
       "Run PROG in a new session. PROG will have no controlling terminal\n" \
       "and will not be affected by keyboard signals (^C etc).\n" \
     "\n	-c	Set controlling terminal to stdin" \

#define swapon_trivial_usage \
       "[-a] [-e]" IF_FEATURE_SWAPON_DISCARD(" [-d[POL]]") IF_FEATURE_SWAPON_PRI(" [-p PRI]") " [DEVICE]" \

#define swapon_full_usage "\n\n" \
       "Start swapping on DEVICE\n" \
     "\n	-a	Start swapping on all swap devices" \
	IF_FEATURE_SWAPON_DISCARD( \
     "\n	-d[POL]	Discard blocks at swapon (POL=once)," \
     "\n		as freed (POL=pages), or both (POL omitted)" \
	) \
     "\n	-e	Silently skip devices that do not exist" \
	IF_FEATURE_SWAPON_PRI( \
     "\n	-p PRI	Set swap device priority" \
	) \

#define swapoff_trivial_usage \
       "[-a] [DEVICE]" \

#define swapoff_full_usage "\n\n" \
       "Stop swapping on DEVICE\n" \
     "\n	-a	Stop swapping on all swap devices" \

#define switch_root_trivial_usage \
       "[-c CONSOLE_DEV] NEW_ROOT NEW_INIT [ARGS]" \

#define switch_root_full_usage "\n\n" \
       "Free initramfs and switch to another root fs:\n" \
       "chroot to NEW_ROOT, delete all in /, move NEW_ROOT to /,\n" \
       "execute NEW_INIT. PID must be 1. NEW_ROOT must be a mountpoint.\n" \
     "\n	-c DEV	Reopen stdio to DEV after switch" \

#define run_init_trivial_usage \
       "[-d CAP,CAP...] [-n] [-c CONSOLE_DEV] NEW_ROOT NEW_INIT [ARGS]" \

#define run_init_full_usage "\n\n" \
       "Free initramfs and switch to another root fs:\n" \
       "chroot to NEW_ROOT, delete all in /, move NEW_ROOT to /,\n" \
       "execute NEW_INIT. PID must be 1. NEW_ROOT must be a mountpoint.\n" \
     "\n	-c DEV	Reopen stdio to DEV after switch" \
     "\n	-d CAPS	Drop capabilities" \
     "\n	-n	Dry run" \

#define taskset_trivial_usage \
       "[-p] [HEXMASK] PID | PROG ARGS" \

#define taskset_full_usage "\n\n" \
       "Set or get CPU affinity\n" \
     "\n	-p	Operate on an existing PID" \

#define taskset_example_usage \
       "$ taskset 0x7 ./dgemm_test&\n" \
       "$ taskset -p 0x1 $!\n" \
       "pid 4790's current affinity mask: 7\n" \
       "pid 4790's new affinity mask: 1\n" \
       "$ taskset 0x7 /bin/sh -c './taskset -p 0x1 $$'\n" \
       "pid 6671's current affinity mask: 1\n" \
       "pid 6671's new affinity mask: 1\n" \
       "$ taskset -p 1\n" \
       "pid 1's current affinity mask: 3\n" \

#define uevent_trivial_usage \
       "[PROG [ARGS]]" \

#define uevent_full_usage "\n\n" \
       "uevent runs PROG for every netlink notification." \
   "\n""PROG's environment contains data passed from the kernel." \
   "\n""Typical usage (daemon for dynamic device node creation):" \
   "\n""	# uevent mdev & mdev -s" \

#define umount_trivial_usage \
       "[OPTIONS] FILESYSTEM|DIRECTORY" \

#define umount_full_usage "\n\n" \
       "Unmount file systems\n" \
	IF_FEATURE_UMOUNT_ALL( \
     "\n	-a	Unmount all file systems" IF_FEATURE_MTAB_SUPPORT(" in /etc/mtab") \
	) \
	IF_FEATURE_MTAB_SUPPORT( \
     "\n	-n	Don't erase /etc/mtab entries" \
	) \
     "\n	-r	Try to remount devices as read-only if mount is busy" \
     "\n	-l	Lazy umount (detach filesystem)" \
     "\n	-f	Force umount (i.e., unreachable NFS server)" \
	IF_FEATURE_MOUNT_LOOP( \
     "\n	-d	Free loop device if it has been used" \
	) \
     "\n	-t FSTYPE[,...]	Unmount only these filesystem type(s)" \

#define umount_example_usage \
       "$ umount /dev/hdc1\n" \

#define unshare_trivial_usage \
       "[OPTIONS] [PROG [ARGS]]" \

#define unshare_full_usage "\n" \
     "\n	-m,--mount[=FILE]	Unshare mount namespace" \
     "\n	-u,--uts[=FILE]		Unshare UTS namespace (hostname etc.)" \
     "\n	-i,--ipc[=FILE]		Unshare System V IPC namespace" \
     "\n	-n,--net[=FILE]		Unshare network namespace" \
     "\n	-p,--pid[=FILE]		Unshare PID namespace" \
     "\n	-U,--user[=FILE]	Unshare user namespace" \
     "\n	-f,--fork		Fork before execing PROG" \
     "\n	-r,--map-root-user	Map current user to root (implies -U)" \
     "\n	--mount-proc[=DIR]	Mount /proc filesystem first (implies -m)" \
     "\n	--propagation slave|shared|private|unchanged" \
     "\n				Modify mount propagation in mount namespace" \
     "\n	--setgroups allow|deny	Control the setgroups syscall in user namespaces" \

#define wall_trivial_usage \
	"[FILE]" \

#define wall_full_usage "\n\n" \
	"Write content of FILE or stdin to all logged-in users" \

#define wall_sample_usage \
	"echo foo | wall\n" \
	"wall ./mymessage" \

#define fixup_trivial_usage \
       "[-t] [-T] [DEVICE]" \

#define fixup_full_usage "\n\n" \
       "Eject DEVICE or default /dev/cdrom\n" \
     "\n	-t	Close tray" \
     "\n	-T	Open/close tray (toggle)" \

#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 1 \

# define IF_UDHCP_VERBOSE(...) __VA_ARGS__ \

#else \

# define IF_UDHCP_VERBOSE(...) \

#endif \

#define udhcpc6_trivial_usage \
       "[-fbnq"IF_UDHCP_VERBOSE("v")"odR] [-i IFACE] [-r IPv6] [-s PROG] [-p PIDFILE]\n" \
       "	[-x OPT:VAL]... [-O OPT]..." IF_FEATURE_UDHCP_PORT(" [-P N]") \

#define udhcpc6_full_usage "\n" \
     "\n	-i IFACE	Interface to use (default eth0)" \
     "\n	-p FILE		Create pidfile" \
     "\n	-s PROG		Run PROG at DHCP events (default "CONFIG_UDHCPC_DEFAULT_SCRIPT")" \
     "\n	-B		Request broadcast replies" \
     "\n	-t N		Send up to N discover packets" \
     "\n	-T N		Pause between packets (default 3 seconds)" \
     "\n	-A N		Wait N seconds (default 20) after failure" \
     "\n	-f		Run in foreground" \
	USE_FOR_MMU( \
     "\n	-b		Background if lease is not obtained" \
	) \
     "\n	-n		Exit if lease is not obtained" \
     "\n	-q		Exit after obtaining lease" \
     "\n	-R		Release IP on exit" \
     "\n	-S		Log to syslog too" \
	IF_FEATURE_UDHCP_PORT( \
     "\n	-P N		Use port N (default 546)" \
	) \
     "\n	-O OPT		Request option OPT from server (cumulative)" \
     "\n	-o		Don't request any options (unless -O is given)" \
     "\n	-r IPv6		Request this address ('no' to not request any IP)" \
     "\n	-d		Request prefix" \
     "\n	-x OPT:VAL	Include option OPT in sent packets (cumulative)" \
     "\n			Examples of string, numeric, and hex byte opts:" \
     "\n			-x hostname:bbox - option 12" \
     "\n			-x lease:3600 - option 51 (lease time)" \
     "\n			-x 0x3d:0100BEEFC0FFEE - option 61 (client id)" \
     "\n			-x 14:'\"dumpfile\"' - option 14 (shell-quoted)" \
	IF_UDHCP_VERBOSE( \
     "\n	-v		Verbose" \
	) \
     "\nSignals:" \
     "\n	USR1	Renew lease" \
     "\n	USR2	Release lease" \

#if defined CONFIG_UDHCP_DEBUG && CONFIG_UDHCP_DEBUG >= 1 \

# define IF_UDHCP_VERBOSE(...) __VA_ARGS__ \

#else \

# define IF_UDHCP_VERBOSE(...) \

#endif \

#define udhcpc_trivial_usage \
       "[-fbq"IF_UDHCP_VERBOSE("v")"RB]"IF_FEATURE_UDHCPC_ARPING(" [-a[MSEC]]")" [-t N] [-T SEC] [-A SEC/-n]\n" \
       "	[-i IFACE]"IF_FEATURE_UDHCP_PORT(" [-P PORT]")" [-s PROG] [-p PIDFILE]\n" \
       "	[-oC] [-r IP] [-V VENDOR] [-F NAME] [-x OPT:VAL]... [-O OPT]..." \

#define udhcpc_full_usage "\n" \
     "\n	-i IFACE	Interface to use (default eth0)" \
	IF_FEATURE_UDHCP_PORT( \
     "\n	-P PORT		Use PORT (default 68)" \
	) \
     "\n	-s PROG		Run PROG at DHCP events (default "CONFIG_UDHCPC_DEFAULT_SCRIPT")" \
     "\n	-p FILE		Create pidfile" \
     "\n	-B		Request broadcast replies" \
     "\n	-t N		Send up to N discover packets (default 3)" \
     "\n	-T SEC		Pause between packets (default 3)" \
     "\n	-A SEC		Wait if lease is not obtained (default 20)" \
     "\n	-n		Exit if lease is not obtained" \
     "\n	-q		Exit after obtaining lease" \
     "\n	-R		Release IP on exit" \
     "\n	-f		Run in foreground" \
	USE_FOR_MMU( \
     "\n	-b		Background if lease is not obtained" \
	) \
     "\n	-S		Log to syslog too" \
	IF_FEATURE_UDHCPC_ARPING( \
     "\n	-a[MSEC]	Validate offered address with ARP ping" \
	) \
     "\n	-r IP		Request this IP address" \
     "\n	-o		Don't request any options (unless -O is given)" \
     "\n	-O OPT		Request option OPT from server (cumulative)" \
     "\n	-x OPT:VAL	Include option OPT in sent packets (cumulative)" \
     "\n			Examples of string, numeric, and hex byte opts:" \
     "\n			-x hostname:bbox - option 12" \
     "\n			-x lease:3600 - option 51 (lease time)" \
     "\n			-x 0x3d:0100BEEFC0FFEE - option 61 (client id)" \
     "\n			-x 14:'\"dumpfile\"' - option 14 (shell-quoted)" \
     "\n	-F NAME		Ask server to update DNS mapping for NAME" \
     "\n	-V VENDOR	Vendor identifier (default 'udhcp VERSION')" \
     "\n	-C		Don't send MAC as client identifier" \
	IF_UDHCP_VERBOSE( \
     "\n	-v		Verbose" \
	) \
     "\nSignals:" \
     "\n	USR1	Renew lease" \
     "\n	USR2	Release lease" \

#define udhcpd_trivial_usage \
       "[-fS] [-I ADDR]" IF_FEATURE_UDHCP_PORT(" [-P N]") " [CONFFILE]" \

#define udhcpd_full_usage "\n\n" \
       "DHCP server\n" \
     "\n	-f	Run in foreground" \
     "\n	-S	Log to syslog too" \
     "\n	-I ADDR	Local address" \
     "\n	-a MSEC	Timeout for ARP ping (default 2000)" \
	IF_FEATURE_UDHCP_PORT( \
     "\n	-P N	Use port N (default 67)" \
	) \

#define dhcprelay_trivial_usage \
       "CLIENT_IFACE[,CLIENT_IFACE2]... SERVER_IFACE [SERVER_IP]" \

#define dhcprelay_full_usage "\n\n" \
       "Relay DHCP requests between clients and server" \

#define dumpleases_trivial_usage \
       "[-r|-a] [-d] [-f LEASEFILE]" \

#define dumpleases_full_usage "\n\n" \
       "Display DHCP leases granted by udhcpd\n" \
	IF_LONG_OPTS( \
     "\n	-f,--file FILE	Lease file" \
     "\n	-r,--remaining	Show remaining time" \
     "\n	-a,--absolute	Show expiration time" \
     "\n	-d,--decimal	Show time in seconds" \
	) \
	IF_NOT_LONG_OPTS( \
     "\n	-f FILE	Lease file" \
     "\n	-r	Show remaining time" \
     "\n	-a	Show expiration time" \
     "\n	-d	Show time in seconds" \
	) \

#define busybox_notes_usage \
       "Hello world!\n"

#endif
