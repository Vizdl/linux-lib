/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_types.h"
#include "xfs_bit.h"
#include "xfs_log.h"
#include "xfs_inum.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
#include "xfs_dir2.h"
#include "xfs_dmapi.h"
#include "xfs_mount.h"
#include "xfs_bmap_btree.h"
#include "xfs_alloc_btree.h"
#include "xfs_ialloc_btree.h"
#include "xfs_dir2_sf.h"
#include "xfs_attr_sf.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_ialloc.h"
#include "xfs_itable.h"
#include "xfs_error.h"
#include "xfs_btree.h"

STATIC int
xfs_internal_inum(
	xfs_mount_t	*mp,
	xfs_ino_t	ino)
{
	return (ino == mp->m_sb.sb_rbmino || ino == mp->m_sb.sb_rsumino ||
		(xfs_sb_version_hasquota(&mp->m_sb) &&
		 (ino == mp->m_sb.sb_uquotino || ino == mp->m_sb.sb_gquotino)));
}

STATIC int
xfs_bulkstat_one_iget(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	ino,		/* inode number to get data for */
	xfs_daddr_t	bno,		/* starting bno of inode cluster */
	xfs_bstat_t	*buf,		/* return buffer */
	int		*stat)		/* BULKSTAT_RV_... */
{
	xfs_icdinode_t	*dic;	/* dinode core info pointer */
	xfs_inode_t	*ip;		/* incore inode pointer */
	struct inode	*inode;
	int		error;

	error = xfs_iget(mp, NULL, ino,
			 XFS_IGET_BULKSTAT, XFS_ILOCK_SHARED, &ip, bno);
	if (error) {
		*stat = BULKSTAT_RV_NOTHING;
		return error;
	}

	ASSERT(ip != NULL);
	ASSERT(ip->i_imap.im_blkno != 0);

	dic = &ip->i_d;
	inode = VFS_I(ip);

	/* xfs_iget returns the following without needing
	 * further change.
	 */
	buf->bs_nlink = dic->di_nlink;
	buf->bs_projid = dic->di_projid;
	buf->bs_ino = ino;
	buf->bs_mode = dic->di_mode;
	buf->bs_uid = dic->di_uid;
	buf->bs_gid = dic->di_gid;
	buf->bs_size = dic->di_size;

	/*
	 * We need to read the timestamps from the Linux inode because
	 * the VFS keeps writing directly into the inode structure instead
	 * of telling us about the updates.
	 */
	buf->bs_atime.tv_sec = inode->i_atime.tv_sec;
	buf->bs_atime.tv_nsec = inode->i_atime.tv_nsec;
	buf->bs_mtime.tv_sec = inode->i_mtime.tv_sec;
	buf->bs_mtime.tv_nsec = inode->i_mtime.tv_nsec;
	buf->bs_ctime.tv_sec = inode->i_ctime.tv_sec;
	buf->bs_ctime.tv_nsec = inode->i_ctime.tv_nsec;

	buf->bs_xflags = xfs_ip2xflags(ip);
	buf->bs_extsize = dic->di_extsize << mp->m_sb.sb_blocklog;
	buf->bs_extents = dic->di_nextents;
	buf->bs_gen = dic->di_gen;
	memset(buf->bs_pad, 0, sizeof(buf->bs_pad));
	buf->bs_dmevmask = dic->di_dmevmask;
	buf->bs_dmstate = dic->di_dmstate;
	buf->bs_aextents = dic->di_anextents;
	buf->bs_forkoff = XFS_IFORK_BOFF(ip);

	switch (dic->di_format) {
	case XFS_DINODE_FMT_DEV:
		buf->bs_rdev = ip->i_df.if_u2.if_rdev;
		buf->bs_blksize = BLKDEV_IOSIZE;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_LOCAL:
	case XFS_DINODE_FMT_UUID:
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_EXTENTS:
	case XFS_DINODE_FMT_BTREE:
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
		buf->bs_blocks = dic->di_nblocks + ip->i_delayed_blks;
		break;
	}

	xfs_iput(ip, XFS_ILOCK_SHARED);
	return error;
}

STATIC void
xfs_bulkstat_one_dinode(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	ino,		/* inode number to get data for */
	xfs_dinode_t	*dic,		/* dinode inode pointer */
	xfs_bstat_t	*buf)		/* return buffer */
{
	/*
	 * The inode format changed when we moved the link count and
	 * made it 32 bits long.  If this is an old format inode,
	 * convert it in memory to look like a new one.  If it gets
	 * flushed to disk we will convert back before flushing or
	 * logging it.  We zero out the new projid field and the old link
	 * count field.  We'll handle clearing the pad field (the remains
	 * of the old uuid field) when we actually convert the inode to
	 * the new format. We don't change the version number so that we
	 * can distinguish this from a real new format inode.
	 */
	if (dic->di_version == 1) {
		buf->bs_nlink = be16_to_cpu(dic->di_onlink);
		buf->bs_projid = 0;
	} else {
		buf->bs_nlink = be32_to_cpu(dic->di_nlink);
		buf->bs_projid = be16_to_cpu(dic->di_projid);
	}

	buf->bs_ino = ino;
	buf->bs_mode = be16_to_cpu(dic->di_mode);
	buf->bs_uid = be32_to_cpu(dic->di_uid);
	buf->bs_gid = be32_to_cpu(dic->di_gid);
	buf->bs_size = be64_to_cpu(dic->di_size);
	buf->bs_atime.tv_sec = be32_to_cpu(dic->di_atime.t_sec);
	buf->bs_atime.tv_nsec = be32_to_cpu(dic->di_atime.t_nsec);
	buf->bs_mtime.tv_sec = be32_to_cpu(dic->di_mtime.t_sec);
	buf->bs_mtime.tv_nsec = be32_to_cpu(dic->di_mtime.t_nsec);
	buf->bs_ctime.tv_sec = be32_to_cpu(dic->di_ctime.t_sec);
	buf->bs_ctime.tv_nsec = be32_to_cpu(dic->di_ctime.t_nsec);
	buf->bs_xflags = xfs_dic2xflags(dic);
	buf->bs_extsize = be32_to_cpu(dic->di_extsize) << mp->m_sb.sb_blocklog;
	buf->bs_extents = be32_to_cpu(dic->di_nextents);
	buf->bs_gen = be32_to_cpu(dic->di_gen);
	memset(buf->bs_pad, 0, sizeof(buf->bs_pad));
	buf->bs_dmevmask = be32_to_cpu(dic->di_dmevmask);
	buf->bs_dmstate = be16_to_cpu(dic->di_dmstate);
	buf->bs_aextents = be16_to_cpu(dic->di_anextents);
	buf->bs_forkoff = XFS_DFORK_BOFF(dic);

	switch (dic->di_format) {
	case XFS_DINODE_FMT_DEV:
		buf->bs_rdev = xfs_dinode_get_rdev(dic);
		buf->bs_blksize = BLKDEV_IOSIZE;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_LOCAL:
	case XFS_DINODE_FMT_UUID:
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
		buf->bs_blocks = 0;
		break;
	case XFS_DINODE_FMT_EXTENTS:
	case XFS_DINODE_FMT_BTREE:
		buf->bs_rdev = 0;
		buf->bs_blksize = mp->m_sb.sb_blocksize;
		buf->bs_blocks = be64_to_cpu(dic->di_nblocks);
		break;
	}
}

/* Return 0 on success or positive error */
STATIC int
xfs_bulkstat_one_fmt(
	void			__user *ubuffer,
	int			ubsize,
	int			*ubused,
	const xfs_bstat_t	*buffer)
{
	if (ubsize < sizeof(*buffer))
		return XFS_ERROR(ENOMEM);
	if (copy_to_user(ubuffer, buffer, sizeof(*buffer)))
		return XFS_ERROR(EFAULT);
	if (ubused)
		*ubused = sizeof(*buffer);
	return 0;
}

/*
 * Return stat information for one inode.
 * Return 0 if ok, else errno.
 */
int		   	    		/* error status */
xfs_bulkstat_one_int(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	ino,		/* inode number to get data for */
	void		__user *buffer,	/* buffer to place output in */
	int		ubsize,		/* size of buffer */
	bulkstat_one_fmt_pf formatter,	/* formatter, copy to user */
	xfs_daddr_t	bno,		/* starting bno of inode cluster */
	int		*ubused,	/* bytes used by me */
	void		*dibuff,	/* on-disk inode buffer */
	int		*stat)		/* BULKSTAT_RV_... */
{
	xfs_bstat_t	*buf;		/* return buffer */
	int		error = 0;	/* error value */
	xfs_dinode_t	*dip;		/* dinode inode pointer */

	dip = (xfs_dinode_t *)dibuff;
	*stat = BULKSTAT_RV_NOTHING;

	if (!buffer || xfs_internal_inum(mp, ino))
		return XFS_ERROR(EINVAL);

	buf = kmem_alloc(sizeof(*buf), KM_SLEEP);

	if (dip == NULL) {
		/* We're not being passed a pointer to a dinode.  This happens
		 * if BULKSTAT_FG_IGET is selected.  Do the iget.
		 */
		error = xfs_bulkstat_one_iget(mp, ino, bno, buf, stat);
		if (error)
			goto out_free;
	} else {
		xfs_bulkstat_one_dinode(mp, ino, dip, buf);
	}

	error = formatter(buffer, ubsize, ubused, buf);
	if (error)
		goto out_free;

	*stat = BULKSTAT_RV_DIDONE;

 out_free:
	kmem_free(buf);
	return error;
}

int
xfs_bulkstat_one(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	ino,		/* inode number to get data for */
	void		__user *buffer,	/* buffer to place output in */
	int		ubsize,		/* size of buffer */
	void		*private_data,	/* my private data */
	xfs_daddr_t	bno,		/* starting bno of inode cluster */
	int		*ubused,	/* bytes used by me */
	void		*dibuff,	/* on-disk inode buffer */
	int		*stat)		/* BULKSTAT_RV_... */
{
	return xfs_bulkstat_one_int(mp, ino, buffer, ubsize,
				    xfs_bulkstat_one_fmt, bno,
				    ubused, dibuff, stat);
}

/*
 * Test to see whether we can use the ondisk inode directly, based
 * on the given bulkstat flags, filling in dipp accordingly.
 * Returns zero if the inode is dodgey.
 */
STATIC int
xfs_bulkstat_use_dinode(
	xfs_mount_t	*mp,
	int		flags,
	xfs_buf_t	*bp,
	int		clustidx,
	xfs_dinode_t	**dipp)
{
	xfs_dinode_t	*dip;
	unsigned int	aformat;

	*dipp = NULL;
	if (!bp || (flags & BULKSTAT_FG_IGET))
		return 1;
	dip = (xfs_dinode_t *)
			xfs_buf_offset(bp, clustidx << mp->m_sb.sb_inodelog);
	/*
	 * Check the buffer containing the on-disk inode for di_mode == 0.
	 * This is to prevent xfs_bulkstat from picking up just reclaimed
	 * inodes that have their in-core state initialized but not flushed
	 * to disk yet. This is a temporary hack that would require a proper
	 * fix in the future.
	 */
	if (be16_to_cpu(dip->di_magic) != XFS_DINODE_MAGIC ||
	    !XFS_DINODE_GOOD_VERSION(dip->di_version) ||
	    !dip->di_mode)
		return 0;
	if (flags & BULKSTAT_FG_QUICK) {
		*dipp = dip;
		return 1;
	}
	/* BULKSTAT_FG_INLINE: if attr fork is local, or not there, use it */
	aformat = dip->di_aformat;
	if ((XFS_DFORK_Q(dip) == 0) ||
	    (aformat == XFS_DINODE_FMT_LOCAL) ||
	    (aformat == XFS_DINODE_FMT_EXTENTS && !dip->di_anextents)) {
		*dipp = dip;
		return 1;
	}
	return 1;
}

#define XFS_BULKSTAT_UBLEFT(ubleft)	((ubleft) >= statstruct_size)

/*
 * Return stat information in bulk (by-inode) for the filesystem.
 */
int					/* error status */
xfs_bulkstat(
	xfs_mount_t		*mp,	/* mount point for filesystem */
	xfs_ino_t		*lastinop, /* last inode returned */
	int			*ubcountp, /* size of buffer/count returned */
	bulkstat_one_pf		formatter, /* func that'd fill a single buf */
	void			*private_data,/* private data for formatter */
	size_t			statstruct_size, /* sizeof struct filling */
	char			__user *ubuffer, /* buffer with inode stats */
	int			flags,	/* defined in xfs_itable.h */
	int			*done)	/* 1 if there are more stats to get */
{
	xfs_agblock_t		agbno=0;/* allocation group block number */
	xfs_buf_t		*agbp;	/* agi header buffer */
	xfs_agi_t		*agi;	/* agi header data */
	xfs_agino_t		agino;	/* inode # in allocation group */
	xfs_agnumber_t		agno;	/* allocation group number */
	xfs_daddr_t		bno;	/* inode cluster start daddr */
	int			chunkidx; /* current index into inode chunk */
	int			clustidx; /* current index into inode cluster */
	xfs_btree_cur_t		*cur;	/* btree cursor for ialloc btree */
	int			end_of_ag; /* set if we've seen the ag end */
	int			error;	/* error code */
	int                     fmterror;/* bulkstat formatter result */
	int			i;	/* loop index */
	int			icount;	/* count of inodes good in irbuf */
	size_t			irbsize; /* size of irec buffer in bytes */
	xfs_ino_t		ino;	/* inode number (filesystem) */
	xfs_inobt_rec_incore_t	*irbp;	/* current irec buffer pointer */
	xfs_inobt_rec_incore_t	*irbuf;	/* start of irec buffer */
	xfs_inobt_rec_incore_t	*irbufend; /* end of good irec buffer entries */
	xfs_ino_t		lastino; /* last inode number returned */
	int			nbcluster; /* # of blocks in a cluster */
	int			nicluster; /* # of inodes in a cluster */
	int			nimask;	/* mask for inode clusters */
	int			nirbuf;	/* size of irbuf */
	int			rval;	/* return value error code */
	int			tmp;	/* result value from btree calls */
	int			ubcount; /* size of user's buffer */
	int			ubleft;	/* bytes left in user's buffer */
	char			__user *ubufp;	/* pointer into user's buffer */
	int			ubelem;	/* spaces used in user's buffer */
	int			ubused;	/* bytes used by formatter */
	xfs_buf_t		*bp;	/* ptr to on-disk inode cluster buf */
	xfs_dinode_t		*dip;	/* ptr into bp for specific inode */

	/*
	 * Get the last inode value, see if there's nothing to do.
	 */
	ino = (xfs_ino_t)*lastinop;
	lastino = ino;
	dip = NULL;
	agno = XFS_INO_TO_AGNO(mp, ino);
	agino = XFS_INO_TO_AGINO(mp, ino);
	if (agno >= mp->m_sb.sb_agcount ||
	    ino != XFS_AGINO_TO_INO(mp, agno, agino)) {
		*done = 1;
		*ubcountp = 0;
		return 0;
	}
	if (!ubcountp || *ubcountp <= 0) {
		return EINVAL;
	}
	ubcount = *ubcountp; /* statstruct's */
	ubleft = ubcount * statstruct_size; /* bytes */
	*ubcountp = ubelem = 0;
	*done = 0;
	fmterror = 0;
	ubufp = ubuffer;
	nicluster = mp->m_sb.sb_blocksize >= XFS_INODE_CLUSTER_SIZE(mp) ?
		mp->m_sb.sb_inopblock :
		(XFS_INODE_CLUSTER_SIZE(mp) >> mp->m_sb.sb_inodelog);
	nimask = ~(nicluster - 1);
	nbcluster = nicluster >> mp->m_sb.sb_inopblog;
	irbuf = kmem_zalloc_greedy(&irbsize, PAGE_SIZE, PAGE_SIZE * 4);
	if (!irbuf)
		return ENOMEM;

	nirbuf = irbsize / sizeof(*irbuf);

	/*
	 * Loop over the allocation groups, starting from the last
	 * inode returned; 0 means start of the allocation group.
	 */
	rval = 0;
	while (XFS_BULKSTAT_UBLEFT(ubleft) && agno < mp->m_sb.sb_agcount) {
		cond_resched();
		bp = NULL;
		error = xfs_ialloc_read_agi(mp, NULL, agno, &agbp);
		if (error) {
			/*
			 * Skip this allocation group and go to the next one.
			 */
			agno++;
			agino = 0;
			continue;
		}
		agi = XFS_BUF_TO_AGI(agbp);
		/*
		 * Allocate and initialize a btree cursor for ialloc btree.
		 */
		cur = xfs_inobt_init_cursor(mp, NULL, agbp, agno);
		irbp = irbuf;
		irbufend = irbuf + nirbuf;
		end_of_ag = 0;
		/*
		 * If we're returning in the middle of an allocation group,
		 * we need to get the remainder of the chunk we're in.
		 */
		if (agino > 0) {
			xfs_inobt_rec_incore_t r;

			/*
			 * Lookup the inode chunk that this inode lives in.
			 */
			error = xfs_inobt_lookup(cur, agino, XFS_LOOKUP_LE,
						 &tmp);
			if (!error &&	/* no I/O error */
			    tmp &&	/* lookup succeeded */
					/* got the record, should always work */
			    !(error = xfs_inobt_get_rec(cur, &r, &i)) &&
			    i == 1 &&
					/* this is the right chunk */
			    agino < r.ir_startino + XFS_INODES_PER_CHUNK &&
					/* lastino was not last in chunk */
			    (chunkidx = agino - r.ir_startino + 1) <
				    XFS_INODES_PER_CHUNK &&
					/* there are some left allocated */
			    xfs_inobt_maskn(chunkidx,
				    XFS_INODES_PER_CHUNK - chunkidx) &
				    ~r.ir_free) {
				/*
				 * Grab the chunk record.  Mark all the
				 * uninteresting inodes (because they're
				 * before our start point) free.
				 */
				for (i = 0; i < chunkidx; i++) {
					if (XFS_INOBT_MASK(i) & ~r.ir_free)
						r.ir_freecount++;
				}
				r.ir_free |= xfs_inobt_maskn(0, chunkidx);
				irbp->ir_startino = r.ir_startino;
				irbp->ir_freecount = r.ir_freecount;
				irbp->ir_free = r.ir_free;
				irbp++;
				agino = r.ir_startino + XFS_INODES_PER_CHUNK;
				icount = XFS_INODES_PER_CHUNK - r.ir_freecount;
			} else {
				/*
				 * If any of those tests failed, bump the
				 * inode number (just in case).
				 */
				agino++;
				icount = 0;
			}
			/*
			 * In any case, increment to the next record.
			 */
			if (!error)
				error = xfs_btree_increment(cur, 0, &tmp);
		} else {
			/*
			 * Start of ag.  Lookup the first inode chunk.
			 */
			error = xfs_inobt_lookup(cur, 0, XFS_LOOKUP_GE, &tmp);
			icount = 0;
		}
		/*
		 * Loop through inode btree records in this ag,
		 * until we run out of inodes or space in the buffer.
		 */
		while (irbp < irbufend && icount < ubcount) {
			xfs_inobt_rec_incore_t r;

			/*
			 * Loop as long as we're unable to read the
			 * inode btree.
			 */
			while (error) {
				agino += XFS_INODES_PER_CHUNK;
				if (XFS_AGINO_TO_AGBNO(mp, agino) >=
						be32_to_cpu(agi->agi_length))
					break;
				error = xfs_inobt_lookup(cur, agino,
							 XFS_LOOKUP_GE, &tmp);
				cond_resched();
			}
			/*
			 * If ran off the end of the ag either with an error,
			 * or the normal way, set end and stop collecting.
			 */
			if (error) {
				end_of_ag = 1;
				break;
			}

			error = xfs_inobt_get_rec(cur, &r, &i);
			if (error || i == 0) {
				end_of_ag = 1;
				break;
			}

			/*
			 * If this chunk has any allocated inodes, save it.
			 * Also start read-ahead now for this chunk.
			 */
			if (r.ir_freecount < XFS_INODES_PER_CHUNK) {
				/*
				 * Loop over all clusters in the next chunk.
				 * Do a readahead if there are any allocated
				 * inodes in that cluster.
				 */
				agbno = XFS_AGINO_TO_AGBNO(mp, r.ir_startino);
				for (chunkidx = 0;
				     chunkidx < XFS_INODES_PER_CHUNK;
				     chunkidx += nicluster,
				     agbno += nbcluster) {
					if (xfs_inobt_maskn(chunkidx, nicluster)
							& ~r.ir_free)
						xfs_btree_reada_bufs(mp, agno,
							agbno, nbcluster);
				}
				irbp->ir_startino = r.ir_startino;
				irbp->ir_freecount = r.ir_freecount;
				irbp->ir_free = r.ir_free;
				irbp++;
				icount += XFS_INODES_PER_CHUNK - r.ir_freecount;
			}
			/*
			 * Set agino to after this chunk and bump the cursor.
			 */
			agino = r.ir_startino + XFS_INODES_PER_CHUNK;
			error = xfs_btree_increment(cur, 0, &tmp);
			cond_resched();
		}
		/*
		 * Drop the btree buffers and the agi buffer.
		 * We can't hold any of the locks these represent
		 * when calling iget.
		 */
		xfs_btree_del_cursor(cur, XFS_BTREE_NOERROR);
		xfs_buf_relse(agbp);
		/*
		 * Now format all the good inodes into the user's buffer.
		 */
		irbufend = irbp;
		for (irbp = irbuf;
		     irbp < irbufend && XFS_BULKSTAT_UBLEFT(ubleft); irbp++) {
			/*
			 * Now process this chunk of inodes.
			 */
			for (agino = irbp->ir_startino, chunkidx = clustidx = 0;
			     XFS_BULKSTAT_UBLEFT(ubleft) &&
				irbp->ir_freecount < XFS_INODES_PER_CHUNK;
			     chunkidx++, clustidx++, agino++) {
				ASSERT(chunkidx < XFS_INODES_PER_CHUNK);
				/*
				 * Recompute agbno if this is the
				 * first inode of the cluster.
				 *
				 * Careful with clustidx.   There can be
				 * multiple clusters per chunk, a single
				 * cluster per chunk or a cluster that has
				 * inodes represented from several different
				 * chunks (if blocksize is large).
				 *
				 * Because of this, the starting clustidx is
				 * initialized to zero in this loop but must
				 * later be reset after reading in the cluster
				 * buffer.
				 */
				if ((chunkidx & (nicluster - 1)) == 0) {
					agbno = XFS_AGINO_TO_AGBNO(mp,
							irbp->ir_startino) +
						((chunkidx & nimask) >>
						 mp->m_sb.sb_inopblog);

					if (flags & (BULKSTAT_FG_QUICK |
						     BULKSTAT_FG_INLINE)) {
						int offset;

						ino = XFS_AGINO_TO_INO(mp, agno,
								       agino);
						bno = XFS_AGB_TO_DADDR(mp, agno,
								       agbno);

						/*
						 * Get the inode cluster buffer
						 */
						if (bp)
							xfs_buf_relse(bp);

						error = xfs_inotobp(mp, NULL, ino, &dip,
								    &bp, &offset,
								    XFS_IGET_BULKSTAT);

						if (!error)
							clustidx = offset / mp->m_sb.sb_inodesize;
						if (XFS_TEST_ERROR(error != 0,
								   mp, XFS_ERRTAG_BULKSTAT_READ_CHUNK,
								   XFS_RANDOM_BULKSTAT_READ_CHUNK)) {
							bp = NULL;
							ubleft = 0;
							rval = error;
							break;
						}
					}
				}
				ino = XFS_AGINO_TO_INO(mp, agno, agino);
				bno = XFS_AGB_TO_DADDR(mp, agno, agbno);
				/*
				 * Skip if this inode is free.
				 */
				if (XFS_INOBT_MASK(chunkidx) & irbp->ir_free) {
					lastino = ino;
					continue;
				}
				/*
				 * Count used inodes as free so we can tell
				 * when the chunk is used up.
				 */
				irbp->ir_freecount++;
				if (!xfs_bulkstat_use_dinode(mp, flags, bp,
							     clustidx, &dip)) {
					lastino = ino;
					continue;
				}
				/*
				 * If we need to do an iget, cannot hold bp.
				 * Drop it, until starting the next cluster.
				 */
				if ((flags & BULKSTAT_FG_INLINE) && !dip) {
					if (bp)
						xfs_buf_relse(bp);
					bp = NULL;
				}

				/*
				 * Get the inode and fill in a single buffer.
				 * BULKSTAT_FG_QUICK uses dip to fill it in.
				 * BULKSTAT_FG_IGET uses igets.
				 * BULKSTAT_FG_INLINE uses dip if we have an
				 * inline attr fork, else igets.
				 * See: xfs_bulkstat_one & xfs_dm_bulkstat_one.
				 * This is also used to count inodes/blks, etc
				 * in xfs_qm_quotacheck.
				 */
				ubused = statstruct_size;
				error = formatter(mp, ino, ubufp,
						ubleft, private_data,
						bno, &ubused, dip, &fmterror);
				if (fmterror == BULKSTAT_RV_NOTHING) {
					if (error && error != ENOENT &&
						error != EINVAL) {
						ubleft = 0;
						rval = error;
						break;
					}
					lastino = ino;
					continue;
				}
				if (fmterror == BULKSTAT_RV_GIVEUP) {
					ubleft = 0;
					ASSERT(error);
					rval = error;
					break;
				}
				if (ubufp)
					ubufp += ubused;
				ubleft -= ubused;
				ubelem++;
				lastino = ino;
			}

			cond_resched();
		}

		if (bp)
			xfs_buf_relse(bp);

		/*
		 * Set up for the next loop iteration.
		 */
		if (XFS_BULKSTAT_UBLEFT(ubleft)) {
			if (end_of_ag) {
				agno++;
				agino = 0;
			} else
				agino = XFS_INO_TO_AGINO(mp, lastino);
		} else
			break;
	}
	/*
	 * Done, we're either out of filesystem or space to put the data.
	 */
	kmem_free_large(irbuf);
	*ubcountp = ubelem;
	/*
	 * Found some inodes, return them now and return the error next time.
	 */
	if (ubelem)
		rval = 0;
	if (agno >= mp->m_sb.sb_agcount) {
		/*
		 * If we ran out of filesystem, mark lastino as off
		 * the end of the filesystem, so the next call
		 * will return immediately.
		 */
		*lastinop = (xfs_ino_t)XFS_AGINO_TO_INO(mp, agno, 0);
		*done = 1;
	} else
		*lastinop = (xfs_ino_t)lastino;

	return rval;
}

/*
 * Return stat information in bulk (by-inode) for the filesystem.
 * Special case for non-sequential one inode bulkstat.
 */
int					/* error status */
xfs_bulkstat_single(
	xfs_mount_t		*mp,	/* mount point for filesystem */
	xfs_ino_t		*lastinop, /* inode to return */
	char			__user *buffer, /* buffer with inode stats */
	int			*done)	/* 1 if there are more stats to get */
{
	int			count;	/* count value for bulkstat call */
	int			error;	/* return value */
	xfs_ino_t		ino;	/* filesystem inode number */
	int			res;	/* result from bs1 */

	/*
	 * note that requesting valid inode numbers which are not allocated
	 * to inodes will most likely cause xfs_itobp to generate warning
	 * messages about bad magic numbers. This is ok. The fact that
	 * the inode isn't actually an inode is handled by the
	 * error check below. Done this way to make the usual case faster
	 * at the expense of the error case.
	 */

	ino = (xfs_ino_t)*lastinop;
	error = xfs_bulkstat_one(mp, ino, buffer, sizeof(xfs_bstat_t),
				 NULL, 0, NULL, NULL, &res);
	if (error) {
		/*
		 * Special case way failed, do it the "long" way
		 * to see if that works.
		 */
		(*lastinop)--;
		count = 1;
		if (xfs_bulkstat(mp, lastinop, &count, xfs_bulkstat_one,
				NULL, sizeof(xfs_bstat_t), buffer,
				BULKSTAT_FG_IGET, done))
			return error;
		if (count == 0 || (xfs_ino_t)*lastinop != ino)
			return error == EFSCORRUPTED ?
				XFS_ERROR(EINVAL) : error;
		else
			return 0;
	}
	*done = 0;
	return 0;
}

int
xfs_inumbers_fmt(
	void			__user *ubuffer, /* buffer to write to */
	const xfs_inogrp_t	*buffer,	/* buffer to read from */
	long			count,		/* # of elements to read */
	long			*written)	/* # of bytes written */
{
	if (copy_to_user(ubuffer, buffer, count * sizeof(*buffer)))
		return -EFAULT;
	*written = count * sizeof(*buffer);
	return 0;
}

/*
 * Return inode number table for the filesystem.
 */
int					/* error status */
xfs_inumbers(
	xfs_mount_t	*mp,		/* mount point for filesystem */
	xfs_ino_t	*lastino,	/* last inode returned */
	int		*count,		/* size of buffer/count returned */
	void		__user *ubuffer,/* buffer with inode descriptions */
	inumbers_fmt_pf	formatter)
{
	xfs_buf_t	*agbp;
	xfs_agino_t	agino;
	xfs_agnumber_t	agno;
	int		bcount;
	xfs_inogrp_t	*buffer;
	int		bufidx;
	xfs_btree_cur_t	*cur;
	int		error;
	xfs_inobt_rec_incore_t r;
	int		i;
	xfs_ino_t	ino;
	int		left;
	int		tmp;

	ino = (xfs_ino_t)*lastino;
	agno = XFS_INO_TO_AGNO(mp, ino);
	agino = XFS_INO_TO_AGINO(mp, ino);
	left = *count;
	*count = 0;
	bcount = MIN(left, (int)(PAGE_SIZE / sizeof(*buffer)));
	buffer = kmem_alloc(bcount * sizeof(*buffer), KM_SLEEP);
	error = bufidx = 0;
	cur = NULL;
	agbp = NULL;
	while (left > 0 && agno < mp->m_sb.sb_agcount) {
		if (agbp == NULL) {
			error = xfs_ialloc_read_agi(mp, NULL, agno, &agbp);
			if (error) {
				/*
				 * If we can't read the AGI of this ag,
				 * then just skip to the next one.
				 */
				ASSERT(cur == NULL);
				agbp = NULL;
				agno++;
				agino = 0;
				continue;
			}
			cur = xfs_inobt_init_cursor(mp, NULL, agbp, agno);
			error = xfs_inobt_lookup(cur, agino, XFS_LOOKUP_GE,
						 &tmp);
			if (error) {
				xfs_btree_del_cursor(cur, XFS_BTREE_ERROR);
				cur = NULL;
				xfs_buf_relse(agbp);
				agbp = NULL;
				/*
				 * Move up the last inode in the current
				 * chunk.  The lookup_ge will always get
				 * us the first inode in the next chunk.
				 */
				agino += XFS_INODES_PER_CHUNK - 1;
				continue;
			}
		}
		error = xfs_inobt_get_rec(cur, &r, &i);
		if (error || i == 0) {
			xfs_buf_relse(agbp);
			agbp = NULL;
			xfs_btree_del_cursor(cur, XFS_BTREE_NOERROR);
			cur = NULL;
			agno++;
			agino = 0;
			continue;
		}
		agino = r.ir_startino + XFS_INODES_PER_CHUNK - 1;
		buffer[bufidx].xi_startino =
			XFS_AGINO_TO_INO(mp, agno, r.ir_startino);
		buffer[bufidx].xi_alloccount =
			XFS_INODES_PER_CHUNK - r.ir_freecount;
		buffer[bufidx].xi_allocmask = ~r.ir_free;
		bufidx++;
		left--;
		if (bufidx == bcount) {
			long written;
			if (formatter(ubuffer, buffer, bufidx, &written)) {
				error = XFS_ERROR(EFAULT);
				break;
			}
			ubuffer += written;
			*count += bufidx;
			bufidx = 0;
		}
		if (left) {
			error = xfs_btree_increment(cur, 0, &tmp);
			if (error) {
				xfs_btree_del_cursor(cur, XFS_BTREE_ERROR);
				cur = NULL;
				xfs_buf_relse(agbp);
				agbp = NULL;
				/*
				 * The agino value has already been bumped.
				 * Just try to skip up to it.
				 */
				agino += XFS_INODES_PER_CHUNK;
				continue;
			}
		}
	}
	if (!error) {
		if (bufidx) {
			long written;
			if (formatter(ubuffer, buffer, bufidx, &written))
				error = XFS_ERROR(EFAULT);
			else
				*count += bufidx;
		}
		*lastino = XFS_AGINO_TO_INO(mp, agno, agino);
	}
	kmem_free(buffer);
	if (cur)
		xfs_btree_del_cursor(cur, (error ? XFS_BTREE_ERROR :
					   XFS_BTREE_NOERROR));
	if (agbp)
		xfs_buf_relse(agbp);
	return error;
}
