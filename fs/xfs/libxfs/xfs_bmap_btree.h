/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2000,2002-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef __XFS_BMAP_BTREE_H__
#define __XFS_BMAP_BTREE_H__

struct xfs_btree_cur;
struct xfs_btree_block;
struct xfs_mount;
struct xfs_inode;
struct xfs_trans;
struct xbtree_ifakeroot;

/*
<<<<<<< HEAD
 * Btree block header size depends on a superblock flag.
 */
#define XFS_BMBT_BLOCK_LEN(mp) \
	(xfs_has_crc(((mp))) ? \
		XFS_BTREE_LBLOCK_CRC_LEN : XFS_BTREE_LBLOCK_LEN)

#define XFS_BMBT_REC_ADDR(mp, block, index) \
	((xfs_bmbt_rec_t *) \
		((char *)(block) + \
		 XFS_BMBT_BLOCK_LEN(mp) + \
		 ((index) - 1) * sizeof(xfs_bmbt_rec_t)))

#define XFS_BMBT_KEY_ADDR(mp, block, index) \
	((xfs_bmbt_key_t *) \
		((char *)(block) + \
		 XFS_BMBT_BLOCK_LEN(mp) + \
		 ((index) - 1) * sizeof(xfs_bmbt_key_t)))

#define XFS_BMBT_PTR_ADDR(mp, block, index, maxrecs) \
	((xfs_bmbt_ptr_t *) \
		((char *)(block) + \
		 XFS_BMBT_BLOCK_LEN(mp) + \
		 (maxrecs) * sizeof(xfs_bmbt_key_t) + \
		 ((index) - 1) * sizeof(xfs_bmbt_ptr_t)))

#define XFS_BMDR_REC_ADDR(block, index) \
	((xfs_bmdr_rec_t *) \
		((char *)(block) + \
		 sizeof(struct xfs_bmdr_block) + \
	         ((index) - 1) * sizeof(xfs_bmdr_rec_t)))

#define XFS_BMDR_KEY_ADDR(block, index) \
	((xfs_bmdr_key_t *) \
		((char *)(block) + \
		 sizeof(struct xfs_bmdr_block) + \
		 ((index) - 1) * sizeof(xfs_bmdr_key_t)))

#define XFS_BMDR_PTR_ADDR(block, index, maxrecs) \
	((xfs_bmdr_ptr_t *) \
		((char *)(block) + \
		 sizeof(struct xfs_bmdr_block) + \
		 (maxrecs) * sizeof(xfs_bmdr_key_t) + \
		 ((index) - 1) * sizeof(xfs_bmdr_ptr_t)))

/*
 * These are to be used when we know the size of the block and
 * we don't have a cursor.
 */
#define XFS_BMAP_BROOT_PTR_ADDR(mp, bb, i, sz) \
	XFS_BMBT_PTR_ADDR(mp, bb, i, xfs_bmbt_maxrecs(mp, sz, 0))

#define XFS_BMAP_BROOT_SPACE_CALC(mp, nrecs) \
	(int)(XFS_BMBT_BLOCK_LEN(mp) + \
	       ((nrecs) * (sizeof(xfs_bmbt_key_t) + sizeof(xfs_bmbt_ptr_t))))

#define XFS_BMAP_BROOT_SPACE(mp, bb) \
	(XFS_BMAP_BROOT_SPACE_CALC(mp, be16_to_cpu((bb)->bb_numrecs)))
#define XFS_BMDR_SPACE_CALC(nrecs) \
	(int)(sizeof(xfs_bmdr_block_t) + \
	       ((nrecs) * (sizeof(xfs_bmbt_key_t) + sizeof(xfs_bmbt_ptr_t))))
#define XFS_BMAP_BMDR_SPACE(bb) \
	(XFS_BMDR_SPACE_CALC(be16_to_cpu((bb)->bb_numrecs)))

/*
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
 * Maximum number of bmap btree levels.
 */
#define XFS_BM_MAXLEVELS(mp,w)		((mp)->m_bm_maxlevels[(w)])

/*
 * Prototypes for xfs_bmap.c to call.
 */
extern void xfs_bmdr_to_bmbt(struct xfs_inode *, xfs_bmdr_block_t *, int,
			struct xfs_btree_block *, int);

void xfs_bmbt_disk_set_all(struct xfs_bmbt_rec *r, struct xfs_bmbt_irec *s);
extern xfs_filblks_t xfs_bmbt_disk_get_blockcount(const struct xfs_bmbt_rec *r);
extern xfs_fileoff_t xfs_bmbt_disk_get_startoff(const struct xfs_bmbt_rec *r);
void xfs_bmbt_disk_get_all(const struct xfs_bmbt_rec *r,
		struct xfs_bmbt_irec *s);

extern void xfs_bmbt_to_bmdr(struct xfs_mount *, struct xfs_btree_block *, int,
			xfs_bmdr_block_t *, int);

extern int xfs_bmbt_get_maxrecs(struct xfs_btree_cur *, int level);
extern int xfs_bmdr_maxrecs(int blocklen, int leaf);
<<<<<<< HEAD
extern int xfs_bmbt_maxrecs(struct xfs_mount *, int blocklen, int leaf);
=======
unsigned int xfs_bmbt_maxrecs(struct xfs_mount *mp, unsigned int blocklen,
		bool leaf);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

extern int xfs_bmbt_change_owner(struct xfs_trans *tp, struct xfs_inode *ip,
				 int whichfork, xfs_ino_t new_owner,
				 struct list_head *buffer_list);

extern struct xfs_btree_cur *xfs_bmbt_init_cursor(struct xfs_mount *,
		struct xfs_trans *, struct xfs_inode *, int);
void xfs_bmbt_commit_staged_btree(struct xfs_btree_cur *cur,
		struct xfs_trans *tp, int whichfork);

extern unsigned long long xfs_bmbt_calc_size(struct xfs_mount *mp,
		unsigned long long len);

unsigned int xfs_bmbt_maxlevels_ondisk(void);

int __init xfs_bmbt_init_cur_cache(void);
void xfs_bmbt_destroy_cur_cache(void);

void xfs_bmbt_init_block(struct xfs_inode *ip, struct xfs_btree_block *buf,
		struct xfs_buf *bp, __u16 level, __u16 numrecs);

<<<<<<< HEAD
=======
/*
 * Btree block header size depends on a superblock flag.
 */
static inline size_t
xfs_bmbt_block_len(struct xfs_mount *mp)
{
	return xfs_has_crc(mp) ?
			XFS_BTREE_LBLOCK_CRC_LEN : XFS_BTREE_LBLOCK_LEN;
}

/* Addresses of key, pointers, and records within an incore bmbt block. */

static inline struct xfs_bmbt_rec *
xfs_bmbt_rec_addr(
	struct xfs_mount	*mp,
	struct xfs_btree_block	*block,
	unsigned int		index)
{
	return (struct xfs_bmbt_rec *)
		((char *)block + xfs_bmbt_block_len(mp) +
		 (index - 1) * sizeof(struct xfs_bmbt_rec));
}

static inline struct xfs_bmbt_key *
xfs_bmbt_key_addr(
	struct xfs_mount	*mp,
	struct xfs_btree_block	*block,
	unsigned int		index)
{
	return (struct xfs_bmbt_key *)
		((char *)block + xfs_bmbt_block_len(mp) +
		 (index - 1) * sizeof(struct xfs_bmbt_key *));
}

static inline xfs_bmbt_ptr_t *
xfs_bmbt_ptr_addr(
	struct xfs_mount	*mp,
	struct xfs_btree_block	*block,
	unsigned int		index,
	unsigned int		maxrecs)
{
	return (xfs_bmbt_ptr_t *)
		((char *)block + xfs_bmbt_block_len(mp) +
		 maxrecs * sizeof(struct xfs_bmbt_key) +
		 (index - 1) * sizeof(xfs_bmbt_ptr_t));
}

/* Addresses of key, pointers, and records within an ondisk bmbt block. */

static inline struct xfs_bmbt_rec *
xfs_bmdr_rec_addr(
	struct xfs_bmdr_block	*block,
	unsigned int		index)
{
	return (struct xfs_bmbt_rec *)
		((char *)(block + 1) +
		 (index - 1) * sizeof(struct xfs_bmbt_rec));
}

static inline struct xfs_bmbt_key *
xfs_bmdr_key_addr(
	struct xfs_bmdr_block	*block,
	unsigned int		index)
{
	return (struct xfs_bmbt_key *)
		((char *)(block + 1) +
		 (index - 1) * sizeof(struct xfs_bmbt_key));
}

static inline xfs_bmbt_ptr_t *
xfs_bmdr_ptr_addr(
	struct xfs_bmdr_block	*block,
	unsigned int		index,
	unsigned int		maxrecs)
{
	return (xfs_bmbt_ptr_t *)
		((char *)(block + 1) +
		 maxrecs * sizeof(struct xfs_bmbt_key) +
		 (index - 1) * sizeof(xfs_bmbt_ptr_t));
}

/*
 * Address of pointers within the incore btree root.
 *
 * These are to be used when we know the size of the block and
 * we don't have a cursor.
 */
static inline xfs_bmbt_ptr_t *
xfs_bmap_broot_ptr_addr(
	struct xfs_mount	*mp,
	struct xfs_btree_block	*bb,
	unsigned int		i,
	unsigned int		sz)
{
	return xfs_bmbt_ptr_addr(mp, bb, i, xfs_bmbt_maxrecs(mp, sz, false));
}

/*
 * Compute the space required for the incore btree root containing the given
 * number of records.
 */
static inline size_t
xfs_bmap_broot_space_calc(
	struct xfs_mount	*mp,
	unsigned int		nrecs)
{
	return xfs_bmbt_block_len(mp) +
	       (nrecs * (sizeof(struct xfs_bmbt_key) + sizeof(xfs_bmbt_ptr_t)));
}

/*
 * Compute the space required for the incore btree root given the ondisk
 * btree root block.
 */
static inline size_t
xfs_bmap_broot_space(
	struct xfs_mount	*mp,
	struct xfs_bmdr_block	*bb)
{
	return xfs_bmap_broot_space_calc(mp, be16_to_cpu(bb->bb_numrecs));
}

/* Compute the space required for the ondisk root block. */
static inline size_t
xfs_bmdr_space_calc(unsigned int nrecs)
{
	return sizeof(struct xfs_bmdr_block) +
	       (nrecs * (sizeof(struct xfs_bmbt_key) + sizeof(xfs_bmbt_ptr_t)));
}

/*
 * Compute the space required for the ondisk root block given an incore root
 * block.
 */
static inline size_t
xfs_bmap_bmdr_space(struct xfs_btree_block *bb)
{
	return xfs_bmdr_space_calc(be16_to_cpu(bb->bb_numrecs));
}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#endif	/* __XFS_BMAP_BTREE_H__ */
