/* vi: set sw=4 ts=4: */
/*
 * xreadlink.c - safe implementation of readlink.
 * Returns a NULL on failure...
 *
 * Licensed under GPLv2, see file LICENSE in this tarball for details.
 */

#include "libbb.h"

/*
 * NOTE: This function returns a malloced char* that you will have to free
 * yourself.
 */
char* FAST_FUNC xmalloc_readlink(const char *path)
{
	enum { GROWBY = 80 }; /* how large we will grow strings by */

	char *buf = NULL;
	int bufsize = 0, readsize = 0;

	do {
		bufsize += GROWBY;
		buf = xrealloc(buf, bufsize);
		readsize = readlink(path, buf, bufsize);
		if (readsize == -1) {
			free(buf);
			return NULL;
		}
	} while (bufsize < readsize + 1);

	buf[readsize] = '\0';

	return buf;
}

/*
 * This routine is not the same as realpath(), which
 * canonicalizes the given path completely. This routine only
 * follows trailing symlinks until a real file is reached and
 * returns its name. If the path ends in a dangling link or if
 * the target doesn't exist, the path is returned in any case.
 * Intermediate symlinks in the path are not expanded -- only
 * those at the tail.
 * A malloced char* is returned, which must be freed by the caller.
 */
char* FAST_FUNC xmalloc_follow_symlinks(const char *path)
{
	char *buf;
	char *lpc;
	char *linkpath;
	int bufsize;
	int looping = MAXSYMLINKS + 1;

	buf = xstrdup(path);
	goto jump_in;

	while (1) {
		linkpath = xmalloc_readlink(buf);
		if (!linkpath) {
			/* not a symlink, or doesn't exist */
			if (errno == EINVAL || errno == ENOENT)
				return buf;
			goto free_buf_ret_null;
		}

		if (!--looping) {
			free(linkpath);
 free_buf_ret_null:
			free(buf);
			return NULL;
		}

		if (*linkpath != '/') {
			bufsize += strlen(linkpath);
			buf = xrealloc(buf, bufsize);
			lpc = bb_get_last_path_component_strip(buf);
			strcpy(lpc, linkpath);
			free(linkpath);
		} else {
			free(buf);
			buf = linkpath;
 jump_in:
			bufsize = strlen(buf) + 1;
		}
	}
}

char* FAST_FUNC xmalloc_readlink_or_warn(const char *path)
{
	char *buf = xmalloc_readlink(path);
	if (!buf) {
		/* EINVAL => "file: Invalid argument" => puzzled user */
		const char *errmsg = "not a symlink";
		int err = errno;
		if (err != EINVAL)
			errmsg = strerror(err);
		bb_error_msg("%s: cannot read link: %s", path, errmsg);
	}
	return buf;
}

/* UNUSED */
#if 0
char* FAST_FUNC xmalloc_realpath(const char *path)
{
#if defined(__GLIBC__) && !defined(__UCLIBC__)
	/* glibc provides a non-standard extension */
	return realpath(path, NULL);
#else
	char buf[PATH_MAX+1];

	/* on error returns NULL (xstrdup(NULL) ==NULL) */
	return xstrdup(realpath(path, buf));
#endif
}
#endif
