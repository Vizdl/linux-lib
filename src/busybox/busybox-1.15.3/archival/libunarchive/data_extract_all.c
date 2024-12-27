/* vi: set sw=4 ts=4: */
/*
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"
#include "unarchive.h"

void FAST_FUNC data_extract_all(archive_handle_t *archive_handle)
{
	file_header_t *file_header = archive_handle->file_header;
	int dst_fd;
	int res;

	if (archive_handle->ah_flags & ARCHIVE_CREATE_LEADING_DIRS) {
		char *name = xstrdup(file_header->name);
		bb_make_directory(dirname(name), -1, FILEUTILS_RECUR);
		free(name);
	}

	/* Check if the file already exists */
	if (archive_handle->ah_flags & ARCHIVE_EXTRACT_UNCONDITIONAL) {
		/* Remove the entry if it exists */
		if ((!S_ISDIR(file_header->mode))
		 && (unlink(file_header->name) == -1)
		 && (errno != ENOENT)
		) {
			bb_perror_msg_and_die("cannot remove old file %s",
					file_header->name);
		}
	}
	else if (archive_handle->ah_flags & ARCHIVE_EXTRACT_NEWER) {
		/* Remove the existing entry if its older than the extracted entry */
		struct stat statbuf;
		if (lstat(file_header->name, &statbuf) == -1) {
			if (errno != ENOENT) {
				bb_perror_msg_and_die("cannot stat old file");
			}
		}
		else if (statbuf.st_mtime <= file_header->mtime) {
			if (!(archive_handle->ah_flags & ARCHIVE_EXTRACT_QUIET)) {
				bb_error_msg("%s not created: newer or "
					"same age file exists", file_header->name);
			}
			data_skip(archive_handle);
			return;
		}
		else if ((unlink(file_header->name) == -1) && (errno != EISDIR)) {
			bb_perror_msg_and_die("cannot remove old file %s",
					file_header->name);
		}
	}

	/* Handle hard links separately
	 * We identified hard links as regular files of size 0 with a symlink */
	if (S_ISREG(file_header->mode) && (file_header->link_target)
	 && (file_header->size == 0)
	) {
		/* hard link */
		res = link(file_header->link_target, file_header->name);
		if ((res == -1) && !(archive_handle->ah_flags & ARCHIVE_EXTRACT_QUIET)) {
			bb_perror_msg("cannot create %slink "
					"from %s to %s", "hard",
					file_header->name,
					file_header->link_target);
		}
	} else {
		/* Create the filesystem entry */
		switch (file_header->mode & S_IFMT) {
		case S_IFREG: {
			/* Regular file */
			dst_fd = xopen3(file_header->name, O_WRONLY | O_CREAT | O_EXCL,
							file_header->mode);
			bb_copyfd_exact_size(archive_handle->src_fd, dst_fd, file_header->size);
			close(dst_fd);
			break;
		}
		case S_IFDIR:
			res = mkdir(file_header->name, file_header->mode);
			if ((res == -1)
			 && (errno != EISDIR) /* btw, Linux doesn't return this */
			 && (errno != EEXIST)
			 && !(archive_handle->ah_flags & ARCHIVE_EXTRACT_QUIET)
			) {
				bb_perror_msg("cannot make dir %s", file_header->name);
			}
			break;
		case S_IFLNK:
			/* Symlink */
			res = symlink(file_header->link_target, file_header->name);
			if ((res == -1)
			 && !(archive_handle->ah_flags & ARCHIVE_EXTRACT_QUIET)
			) {
				bb_perror_msg("cannot create %slink "
					"from %s to %s", "sym",
					file_header->name,
					file_header->link_target);
			}
			break;
		case S_IFSOCK:
		case S_IFBLK:
		case S_IFCHR:
		case S_IFIFO:
			res = mknod(file_header->name, file_header->mode, file_header->device);
			if ((res == -1)
			 && !(archive_handle->ah_flags & ARCHIVE_EXTRACT_QUIET)
			) {
				bb_perror_msg("cannot create node %s", file_header->name);
			}
			break;
		default:
			bb_error_msg_and_die("unrecognized file type");
		}
	}

	if (!(archive_handle->ah_flags & ARCHIVE_NOPRESERVE_OWN)) {
#if ENABLE_FEATURE_TAR_UNAME_GNAME
		if (!(archive_handle->ah_flags & ARCHIVE_NUMERIC_OWNER)) {
			uid_t uid = file_header->uid;
			gid_t gid = file_header->gid;

			if (file_header->uname) {
				struct passwd *pwd = getpwnam(file_header->uname);
				if (pwd) uid = pwd->pw_uid;
			}
			if (file_header->gname) {
				struct group *grp = getgrnam(file_header->gname);
				if (grp) gid = grp->gr_gid;
			}
			lchown(file_header->name, uid, gid);
		} else
#endif
			lchown(file_header->name, file_header->uid, file_header->gid);
	}
	if (!S_ISLNK(file_header->mode)) {
		/* uclibc has no lchmod, glibc is even stranger -
		 * it has lchmod which seems to do nothing!
		 * so we use chmod... */
		if (!(archive_handle->ah_flags & ARCHIVE_NOPRESERVE_PERM)) {
			chmod(file_header->name, file_header->mode);
		}
		/* same for utime */
		if (archive_handle->ah_flags & ARCHIVE_PRESERVE_DATE) {
			struct utimbuf t;
			t.actime = t.modtime = file_header->mtime;
			utime(file_header->name, &t);
		}
	}
}
