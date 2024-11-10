// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/minix/dir.c
 *
 *  Copyright (C) 1991, 1992 Linus Torvalds
 *
 *  minix directory handling functions
 *
 *  Updated to filesystem version 3 by Daniel Aragones
 */

#include "minix.h"
#include <linux/buffer_head.h>
#include <linux/highmem.h>
#include <linux/swap.h>

typedef struct minix_dir_entry minix_dirent;
typedef struct minix3_dir_entry minix3_dirent;

static int minix_readdir(struct file *, struct dir_context *);

const struct file_operations minix_dir_operations = {
	.llseek		= generic_file_llseek,
	.read		= generic_read_dir,
	.iterate_shared	= minix_readdir,
	.fsync		= generic_file_fsync,
};

/*
 * Return the offset into page `page_nr' of the last valid
 * byte in that page, plus one.
 */
static unsigned
minix_last_byte(struct inode *inode, unsigned long page_nr)
{
	unsigned last_byte = PAGE_SIZE;

	if (page_nr == (inode->i_size >> PAGE_SHIFT))
		last_byte = inode->i_size & (PAGE_SIZE - 1);
	return last_byte;
}

<<<<<<< HEAD
static void dir_commit_chunk(struct page *page, loff_t pos, unsigned len)
{
	struct address_space *mapping = page->mapping;
	struct inode *dir = mapping->host;

	block_write_end(NULL, mapping, pos, len, len, page, NULL);
=======
static void dir_commit_chunk(struct folio *folio, loff_t pos, unsigned len)
{
	struct address_space *mapping = folio->mapping;
	struct inode *dir = mapping->host;

	block_write_end(NULL, mapping, pos, len, len, folio, NULL);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if (pos+len > dir->i_size) {
		i_size_write(dir, pos+len);
		mark_inode_dirty(dir);
	}
<<<<<<< HEAD
	unlock_page(page);
=======
	folio_unlock(folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static int minix_handle_dirsync(struct inode *dir)
{
	int err;

	err = filemap_write_and_wait(dir->i_mapping);
	if (!err)
		err = sync_inode_metadata(dir, 1);
	return err;
}

<<<<<<< HEAD
static void *dir_get_page(struct inode *dir, unsigned long n, struct page **p)
{
	struct address_space *mapping = dir->i_mapping;
	struct page *page = read_mapping_page(mapping, n, NULL);
	if (IS_ERR(page))
		return ERR_CAST(page);
	*p = page;
	return kmap_local_page(page);
=======
static void *dir_get_folio(struct inode *dir, unsigned long n,
		struct folio **foliop)
{
	struct folio *folio = read_mapping_folio(dir->i_mapping, n, NULL);

	if (IS_ERR(folio))
		return ERR_CAST(folio);
	*foliop = folio;
	return kmap_local_folio(folio, 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static inline void *minix_next_entry(void *de, struct minix_sb_info *sbi)
{
	return (void*)((char*)de + sbi->s_dirsize);
}

static int minix_readdir(struct file *file, struct dir_context *ctx)
{
	struct inode *inode = file_inode(file);
	struct super_block *sb = inode->i_sb;
	struct minix_sb_info *sbi = minix_sb(sb);
	unsigned chunk_size = sbi->s_dirsize;
	unsigned long npages = dir_pages(inode);
	unsigned long pos = ctx->pos;
	unsigned offset;
	unsigned long n;

	ctx->pos = pos = ALIGN(pos, chunk_size);
	if (pos >= inode->i_size)
		return 0;

	offset = pos & ~PAGE_MASK;
	n = pos >> PAGE_SHIFT;

	for ( ; n < npages; n++, offset = 0) {
		char *p, *kaddr, *limit;
<<<<<<< HEAD
		struct page *page;

		kaddr = dir_get_page(inode, n, &page);
=======
		struct folio *folio;

		kaddr = dir_get_folio(inode, n, &folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (IS_ERR(kaddr))
			continue;
		p = kaddr+offset;
		limit = kaddr + minix_last_byte(inode, n) - chunk_size;
		for ( ; p <= limit; p = minix_next_entry(p, sbi)) {
			const char *name;
			__u32 inumber;
			if (sbi->s_version == MINIX_V3) {
				minix3_dirent *de3 = (minix3_dirent *)p;
				name = de3->name;
				inumber = de3->inode;
	 		} else {
				minix_dirent *de = (minix_dirent *)p;
				name = de->name;
				inumber = de->inode;
			}
			if (inumber) {
				unsigned l = strnlen(name, sbi->s_namelen);
				if (!dir_emit(ctx, name, l,
					      inumber, DT_UNKNOWN)) {
<<<<<<< HEAD
					unmap_and_put_page(page, p);
=======
					folio_release_kmap(folio, p);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
					return 0;
				}
			}
			ctx->pos += chunk_size;
		}
<<<<<<< HEAD
		unmap_and_put_page(page, kaddr);
=======
		folio_release_kmap(folio, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return 0;
}

static inline int namecompare(int len, int maxlen,
	const char * name, const char * buffer)
{
	if (len < maxlen && buffer[len])
		return 0;
	return !memcmp(name, buffer, len);
}

/*
 *	minix_find_entry()
 *
<<<<<<< HEAD
 * finds an entry in the specified directory with the wanted name. It
 * returns the cache buffer in which the entry was found, and the entry
 * itself (as a parameter - res_dir). It does NOT read the inode of the
 * entry - you'll have to do that yourself if you want to.
 */
minix_dirent *minix_find_entry(struct dentry *dentry, struct page **res_page)
=======
 * finds an entry in the specified directory with the wanted name.
 * It does NOT read the inode of the
 * entry - you'll have to do that yourself if you want to.
 * 
 * On Success folio_release_kmap() should be called on *foliop.
 */
minix_dirent *minix_find_entry(struct dentry *dentry, struct folio **foliop)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	const char * name = dentry->d_name.name;
	int namelen = dentry->d_name.len;
	struct inode * dir = d_inode(dentry->d_parent);
	struct super_block * sb = dir->i_sb;
	struct minix_sb_info * sbi = minix_sb(sb);
	unsigned long n;
	unsigned long npages = dir_pages(dir);
<<<<<<< HEAD
	struct page *page = NULL;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	char *p;

	char *namx;
	__u32 inumber;
<<<<<<< HEAD
	*res_page = NULL;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	for (n = 0; n < npages; n++) {
		char *kaddr, *limit;

<<<<<<< HEAD
		kaddr = dir_get_page(dir, n, &page);
=======
		kaddr = dir_get_folio(dir, n, foliop);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (IS_ERR(kaddr))
			continue;

		limit = kaddr + minix_last_byte(dir, n) - sbi->s_dirsize;
		for (p = kaddr; p <= limit; p = minix_next_entry(p, sbi)) {
			if (sbi->s_version == MINIX_V3) {
				minix3_dirent *de3 = (minix3_dirent *)p;
				namx = de3->name;
				inumber = de3->inode;
 			} else {
				minix_dirent *de = (minix_dirent *)p;
				namx = de->name;
				inumber = de->inode;
			}
			if (!inumber)
				continue;
			if (namecompare(namelen, sbi->s_namelen, name, namx))
				goto found;
		}
<<<<<<< HEAD
		unmap_and_put_page(page, kaddr);
=======
		folio_release_kmap(*foliop, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return NULL;

found:
<<<<<<< HEAD
	*res_page = page;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return (minix_dirent *)p;
}

int minix_add_link(struct dentry *dentry, struct inode *inode)
{
	struct inode *dir = d_inode(dentry->d_parent);
	const char * name = dentry->d_name.name;
	int namelen = dentry->d_name.len;
	struct super_block * sb = dir->i_sb;
	struct minix_sb_info * sbi = minix_sb(sb);
<<<<<<< HEAD
	struct page *page = NULL;
=======
	struct folio *folio = NULL;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	unsigned long npages = dir_pages(dir);
	unsigned long n;
	char *kaddr, *p;
	minix_dirent *de;
	minix3_dirent *de3;
	loff_t pos;
	int err;
	char *namx = NULL;
	__u32 inumber;

	/*
	 * We take care of directory expansion in the same loop
	 * This code plays outside i_size, so it locks the page
	 * to protect that region.
	 */
	for (n = 0; n <= npages; n++) {
		char *limit, *dir_end;

<<<<<<< HEAD
		kaddr = dir_get_page(dir, n, &page);
		if (IS_ERR(kaddr))
			return PTR_ERR(kaddr);
		lock_page(page);
=======
		kaddr = dir_get_folio(dir, n, &folio);
		if (IS_ERR(kaddr))
			return PTR_ERR(kaddr);
		folio_lock(folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		dir_end = kaddr + minix_last_byte(dir, n);
		limit = kaddr + PAGE_SIZE - sbi->s_dirsize;
		for (p = kaddr; p <= limit; p = minix_next_entry(p, sbi)) {
			de = (minix_dirent *)p;
			de3 = (minix3_dirent *)p;
			if (sbi->s_version == MINIX_V3) {
				namx = de3->name;
				inumber = de3->inode;
		 	} else {
  				namx = de->name;
				inumber = de->inode;
			}
			if (p == dir_end) {
				/* We hit i_size */
				if (sbi->s_version == MINIX_V3)
					de3->inode = 0;
		 		else
					de->inode = 0;
				goto got_it;
			}
			if (!inumber)
				goto got_it;
			err = -EEXIST;
			if (namecompare(namelen, sbi->s_namelen, name, namx))
				goto out_unlock;
		}
<<<<<<< HEAD
		unlock_page(page);
		unmap_and_put_page(page, kaddr);
=======
		folio_unlock(folio);
		folio_release_kmap(folio, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	BUG();
	return -EINVAL;

got_it:
<<<<<<< HEAD
	pos = page_offset(page) + offset_in_page(p);
	err = minix_prepare_chunk(page, pos, sbi->s_dirsize);
=======
	pos = folio_pos(folio) + offset_in_folio(folio, p);
	err = minix_prepare_chunk(folio, pos, sbi->s_dirsize);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (err)
		goto out_unlock;
	memcpy (namx, name, namelen);
	if (sbi->s_version == MINIX_V3) {
		memset (namx + namelen, 0, sbi->s_dirsize - namelen - 4);
		de3->inode = inode->i_ino;
	} else {
		memset (namx + namelen, 0, sbi->s_dirsize - namelen - 2);
		de->inode = inode->i_ino;
	}
<<<<<<< HEAD
	dir_commit_chunk(page, pos, sbi->s_dirsize);
=======
	dir_commit_chunk(folio, pos, sbi->s_dirsize);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	err = minix_handle_dirsync(dir);
out_put:
<<<<<<< HEAD
	unmap_and_put_page(page, kaddr);
	return err;
out_unlock:
	unlock_page(page);
	goto out_put;
}

int minix_delete_entry(struct minix_dir_entry *de, struct page *page)
{
	struct inode *inode = page->mapping->host;
	loff_t pos = page_offset(page) + offset_in_page(de);
=======
	folio_release_kmap(folio, kaddr);
	return err;
out_unlock:
	folio_unlock(folio);
	goto out_put;
}

int minix_delete_entry(struct minix_dir_entry *de, struct folio *folio)
{
	struct inode *inode = folio->mapping->host;
	loff_t pos = folio_pos(folio) + offset_in_folio(folio, de);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct minix_sb_info *sbi = minix_sb(inode->i_sb);
	unsigned len = sbi->s_dirsize;
	int err;

<<<<<<< HEAD
	lock_page(page);
	err = minix_prepare_chunk(page, pos, len);
	if (err) {
		unlock_page(page);
=======
	folio_lock(folio);
	err = minix_prepare_chunk(folio, pos, len);
	if (err) {
		folio_unlock(folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		return err;
	}
	if (sbi->s_version == MINIX_V3)
		((minix3_dirent *)de)->inode = 0;
	else
		de->inode = 0;
<<<<<<< HEAD
	dir_commit_chunk(page, pos, len);
=======
	dir_commit_chunk(folio, pos, len);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	inode_set_mtime_to_ts(inode, inode_set_ctime_current(inode));
	mark_inode_dirty(inode);
	return minix_handle_dirsync(inode);
}

int minix_make_empty(struct inode *inode, struct inode *dir)
{
<<<<<<< HEAD
	struct page *page = grab_cache_page(inode->i_mapping, 0);
=======
	struct folio *folio = filemap_grab_folio(inode->i_mapping, 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct minix_sb_info *sbi = minix_sb(inode->i_sb);
	char *kaddr;
	int err;

<<<<<<< HEAD
	if (!page)
		return -ENOMEM;
	err = minix_prepare_chunk(page, 0, 2 * sbi->s_dirsize);
	if (err) {
		unlock_page(page);
		goto fail;
	}

	kaddr = kmap_local_page(page);
	memset(kaddr, 0, PAGE_SIZE);
=======
	if (IS_ERR(folio))
		return PTR_ERR(folio);
	err = minix_prepare_chunk(folio, 0, 2 * sbi->s_dirsize);
	if (err) {
		folio_unlock(folio);
		goto fail;
	}

	kaddr = kmap_local_folio(folio, 0);
	memset(kaddr, 0, folio_size(folio));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if (sbi->s_version == MINIX_V3) {
		minix3_dirent *de3 = (minix3_dirent *)kaddr;

		de3->inode = inode->i_ino;
		strcpy(de3->name, ".");
		de3 = minix_next_entry(de3, sbi);
		de3->inode = dir->i_ino;
		strcpy(de3->name, "..");
	} else {
		minix_dirent *de = (minix_dirent *)kaddr;

		de->inode = inode->i_ino;
		strcpy(de->name, ".");
		de = minix_next_entry(de, sbi);
		de->inode = dir->i_ino;
		strcpy(de->name, "..");
	}
	kunmap_local(kaddr);

<<<<<<< HEAD
	dir_commit_chunk(page, 0, 2 * sbi->s_dirsize);
	err = minix_handle_dirsync(inode);
fail:
	put_page(page);
=======
	dir_commit_chunk(folio, 0, 2 * sbi->s_dirsize);
	err = minix_handle_dirsync(inode);
fail:
	folio_put(folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return err;
}

/*
 * routine to check that the specified directory is empty (for rmdir)
 */
int minix_empty_dir(struct inode * inode)
{
<<<<<<< HEAD
	struct page *page = NULL;
=======
	struct folio *folio = NULL;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	unsigned long i, npages = dir_pages(inode);
	struct minix_sb_info *sbi = minix_sb(inode->i_sb);
	char *name, *kaddr;
	__u32 inumber;

	for (i = 0; i < npages; i++) {
		char *p, *limit;

<<<<<<< HEAD
		kaddr = dir_get_page(inode, i, &page);
=======
		kaddr = dir_get_folio(inode, i, &folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (IS_ERR(kaddr))
			continue;

		limit = kaddr + minix_last_byte(inode, i) - sbi->s_dirsize;
		for (p = kaddr; p <= limit; p = minix_next_entry(p, sbi)) {
			if (sbi->s_version == MINIX_V3) {
				minix3_dirent *de3 = (minix3_dirent *)p;
				name = de3->name;
				inumber = de3->inode;
			} else {
				minix_dirent *de = (minix_dirent *)p;
				name = de->name;
				inumber = de->inode;
			}

			if (inumber != 0) {
				/* check for . and .. */
				if (name[0] != '.')
					goto not_empty;
				if (!name[1]) {
					if (inumber != inode->i_ino)
						goto not_empty;
				} else if (name[1] != '.')
					goto not_empty;
				else if (name[2])
					goto not_empty;
			}
		}
<<<<<<< HEAD
		unmap_and_put_page(page, kaddr);
=======
		folio_release_kmap(folio, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return 1;

not_empty:
<<<<<<< HEAD
	unmap_and_put_page(page, kaddr);
=======
	folio_release_kmap(folio, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return 0;
}

/* Releases the page */
<<<<<<< HEAD
int minix_set_link(struct minix_dir_entry *de, struct page *page,
		struct inode *inode)
{
	struct inode *dir = page->mapping->host;
	struct minix_sb_info *sbi = minix_sb(dir->i_sb);
	loff_t pos = page_offset(page) + offset_in_page(de);
	int err;

	lock_page(page);
	err = minix_prepare_chunk(page, pos, sbi->s_dirsize);
	if (err) {
		unlock_page(page);
=======
int minix_set_link(struct minix_dir_entry *de, struct folio *folio,
		struct inode *inode)
{
	struct inode *dir = folio->mapping->host;
	struct minix_sb_info *sbi = minix_sb(dir->i_sb);
	loff_t pos = folio_pos(folio) + offset_in_folio(folio, de);
	int err;

	folio_lock(folio);
	err = minix_prepare_chunk(folio, pos, sbi->s_dirsize);
	if (err) {
		folio_unlock(folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		return err;
	}
	if (sbi->s_version == MINIX_V3)
		((minix3_dirent *)de)->inode = inode->i_ino;
	else
		de->inode = inode->i_ino;
<<<<<<< HEAD
	dir_commit_chunk(page, pos, sbi->s_dirsize);
=======
	dir_commit_chunk(folio, pos, sbi->s_dirsize);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	return minix_handle_dirsync(dir);
}

<<<<<<< HEAD
struct minix_dir_entry * minix_dotdot (struct inode *dir, struct page **p)
{
	struct minix_sb_info *sbi = minix_sb(dir->i_sb);
	struct minix_dir_entry *de = dir_get_page(dir, 0, p);
=======
struct minix_dir_entry *minix_dotdot(struct inode *dir, struct folio **foliop)
{
	struct minix_sb_info *sbi = minix_sb(dir->i_sb);
	struct minix_dir_entry *de = dir_get_folio(dir, 0, foliop);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if (!IS_ERR(de))
		return minix_next_entry(de, sbi);
	return NULL;
}

ino_t minix_inode_by_name(struct dentry *dentry)
{
<<<<<<< HEAD
	struct page *page;
	struct minix_dir_entry *de = minix_find_entry(dentry, &page);
	ino_t res = 0;

	if (de) {
		struct address_space *mapping = page->mapping;
		struct inode *inode = mapping->host;
=======
	struct folio *folio;
	struct minix_dir_entry *de = minix_find_entry(dentry, &folio);
	ino_t res = 0;

	if (de) {
		struct inode *inode = folio->mapping->host;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		struct minix_sb_info *sbi = minix_sb(inode->i_sb);

		if (sbi->s_version == MINIX_V3)
			res = ((minix3_dirent *) de)->inode;
		else
			res = de->inode;
<<<<<<< HEAD
		unmap_and_put_page(page, de);
=======
		folio_release_kmap(folio, de);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return res;
}
