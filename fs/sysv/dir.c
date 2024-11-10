// SPDX-License-Identifier: GPL-2.0
/*
 *  linux/fs/sysv/dir.c
 *
 *  minix/dir.c
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  coh/dir.c
 *  Copyright (C) 1993  Pascal Haible, Bruno Haible
 *
 *  sysv/dir.c
 *  Copyright (C) 1993  Bruno Haible
 *
 *  SystemV/Coherent directory handling functions
 */

#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/swap.h>
#include "sysv.h"

static int sysv_readdir(struct file *, struct dir_context *);

const struct file_operations sysv_dir_operations = {
	.llseek		= generic_file_llseek,
	.read		= generic_read_dir,
	.iterate_shared	= sysv_readdir,
	.fsync		= generic_file_fsync,
};

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

static int sysv_handle_dirsync(struct inode *dir)
{
	int err;

	err = filemap_write_and_wait(dir->i_mapping);
	if (!err)
		err = sync_inode_metadata(dir, 1);
	return err;
}

/*
<<<<<<< HEAD
 * Calls to dir_get_page()/unmap_and_put_page() must be nested according to the
 * rules documented in mm/highmem.rst.
 *
 * NOTE: sysv_find_entry() and sysv_dotdot() act as calls to dir_get_page()
 * and must be treated accordingly for nesting purposes.
 */
static void *dir_get_page(struct inode *dir, unsigned long n, struct page **p)
{
	struct address_space *mapping = dir->i_mapping;
	struct page *page = read_mapping_page(mapping, n, NULL);
	if (IS_ERR(page))
		return ERR_CAST(page);
	*p = page;
	return kmap_local_page(page);
=======
 * Calls to dir_get_folio()/folio_release_kmap() must be nested according to the
 * rules documented in mm/highmem.rst.
 *
 * NOTE: sysv_find_entry() and sysv_dotdot() act as calls to dir_get_folio()
 * and must be treated accordingly for nesting purposes.
 */
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

static int sysv_readdir(struct file *file, struct dir_context *ctx)
{
	unsigned long pos = ctx->pos;
	struct inode *inode = file_inode(file);
	struct super_block *sb = inode->i_sb;
	unsigned long npages = dir_pages(inode);
	unsigned offset;
	unsigned long n;

	ctx->pos = pos = (pos + SYSV_DIRSIZE-1) & ~(SYSV_DIRSIZE-1);
	if (pos >= inode->i_size)
		return 0;

	offset = pos & ~PAGE_MASK;
	n = pos >> PAGE_SHIFT;

	for ( ; n < npages; n++, offset = 0) {
		char *kaddr, *limit;
		struct sysv_dir_entry *de;
<<<<<<< HEAD
		struct page *page;

		kaddr = dir_get_page(inode, n, &page);
=======
		struct folio *folio;

		kaddr = dir_get_folio(inode, n, &folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (IS_ERR(kaddr))
			continue;
		de = (struct sysv_dir_entry *)(kaddr+offset);
		limit = kaddr + PAGE_SIZE - SYSV_DIRSIZE;
		for ( ;(char*)de <= limit; de++, ctx->pos += sizeof(*de)) {
			char *name = de->name;

			if (!de->inode)
				continue;

			if (!dir_emit(ctx, name, strnlen(name,SYSV_NAMELEN),
					fs16_to_cpu(SYSV_SB(sb), de->inode),
					DT_UNKNOWN)) {
<<<<<<< HEAD
				unmap_and_put_page(page, kaddr);
				return 0;
			}
		}
		unmap_and_put_page(page, kaddr);
=======
				folio_release_kmap(folio, kaddr);
				return 0;
			}
		}
		folio_release_kmap(folio, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return 0;
}

/* compare strings: name[0..len-1] (not zero-terminated) and
 * buffer[0..] (filled with zeroes up to buffer[0..maxlen-1])
 */
static inline int namecompare(int len, int maxlen,
	const char * name, const char * buffer)
{
	if (len < maxlen && buffer[len])
		return 0;
	return !memcmp(name, buffer, len);
}

/*
 *	sysv_find_entry()
 *
<<<<<<< HEAD
 * finds an entry in the specified directory with the wanted name. It
 * returns the cache buffer in which the entry was found, and the entry
 * itself (as a parameter - res_dir). It does NOT read the inode of the
 * entry - you'll have to do that yourself if you want to.
 *
 * On Success unmap_and_put_page() should be called on *res_page.
 *
 * sysv_find_entry() acts as a call to dir_get_page() and must be treated
 * accordingly for nesting purposes.
 */
struct sysv_dir_entry *sysv_find_entry(struct dentry *dentry, struct page **res_page)
=======
 * finds an entry in the specified directory with the wanted name.
 * It does NOT read the inode of the
 * entry - you'll have to do that yourself if you want to.
 *
 * On Success folio_release_kmap() should be called on *foliop.
 *
 * sysv_find_entry() acts as a call to dir_get_folio() and must be treated
 * accordingly for nesting purposes.
 */
struct sysv_dir_entry *sysv_find_entry(struct dentry *dentry, struct folio **foliop)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	const char * name = dentry->d_name.name;
	int namelen = dentry->d_name.len;
	struct inode * dir = d_inode(dentry->d_parent);
	unsigned long start, n;
	unsigned long npages = dir_pages(dir);
<<<<<<< HEAD
	struct page *page = NULL;
	struct sysv_dir_entry *de;

	*res_page = NULL;

=======
	struct sysv_dir_entry *de;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	start = SYSV_I(dir)->i_dir_start_lookup;
	if (start >= npages)
		start = 0;
	n = start;

	do {
<<<<<<< HEAD
		char *kaddr = dir_get_page(dir, n, &page);

		if (!IS_ERR(kaddr)) {
			de = (struct sysv_dir_entry *)kaddr;
			kaddr += PAGE_SIZE - SYSV_DIRSIZE;
=======
		char *kaddr = dir_get_folio(dir, n, foliop);

		if (!IS_ERR(kaddr)) {
			de = (struct sysv_dir_entry *)kaddr;
			kaddr += folio_size(*foliop) - SYSV_DIRSIZE;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			for ( ; (char *) de <= kaddr ; de++) {
				if (!de->inode)
					continue;
				if (namecompare(namelen, SYSV_NAMELEN,
							name, de->name))
					goto found;
			}
<<<<<<< HEAD
			unmap_and_put_page(page, kaddr);
=======
			folio_release_kmap(*foliop, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		}

		if (++n >= npages)
			n = 0;
	} while (n != start);

	return NULL;

found:
	SYSV_I(dir)->i_dir_start_lookup = n;
<<<<<<< HEAD
	*res_page = page;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return de;
}

int sysv_add_link(struct dentry *dentry, struct inode *inode)
{
	struct inode *dir = d_inode(dentry->d_parent);
	const char * name = dentry->d_name.name;
	int namelen = dentry->d_name.len;
<<<<<<< HEAD
	struct page *page = NULL;
=======
	struct folio *folio = NULL;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct sysv_dir_entry * de;
	unsigned long npages = dir_pages(dir);
	unsigned long n;
	char *kaddr;
	loff_t pos;
	int err;

	/* We take care of directory expansion in the same loop */
	for (n = 0; n <= npages; n++) {
<<<<<<< HEAD
		kaddr = dir_get_page(dir, n, &page);
=======
		kaddr = dir_get_folio(dir, n, &folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (IS_ERR(kaddr))
			return PTR_ERR(kaddr);
		de = (struct sysv_dir_entry *)kaddr;
		kaddr += PAGE_SIZE - SYSV_DIRSIZE;
		while ((char *)de <= kaddr) {
			if (!de->inode)
				goto got_it;
			err = -EEXIST;
			if (namecompare(namelen, SYSV_NAMELEN, name, de->name)) 
<<<<<<< HEAD
				goto out_page;
			de++;
		}
		unmap_and_put_page(page, kaddr);
=======
				goto out_folio;
			de++;
		}
		folio_release_kmap(folio, kaddr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	BUG();
	return -EINVAL;

got_it:
<<<<<<< HEAD
	pos = page_offset(page) + offset_in_page(de);
	lock_page(page);
	err = sysv_prepare_chunk(page, pos, SYSV_DIRSIZE);
=======
	pos = folio_pos(folio) + offset_in_folio(folio, de);
	folio_lock(folio);
	err = sysv_prepare_chunk(folio, pos, SYSV_DIRSIZE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (err)
		goto out_unlock;
	memcpy (de->name, name, namelen);
	memset (de->name + namelen, 0, SYSV_DIRSIZE - namelen - 2);
	de->inode = cpu_to_fs16(SYSV_SB(inode->i_sb), inode->i_ino);
<<<<<<< HEAD
	dir_commit_chunk(page, pos, SYSV_DIRSIZE);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	err = sysv_handle_dirsync(dir);
out_page:
	unmap_and_put_page(page, kaddr);
	return err;
out_unlock:
	unlock_page(page);
	goto out_page;
}

int sysv_delete_entry(struct sysv_dir_entry *de, struct page *page)
{
	struct inode *inode = page->mapping->host;
	loff_t pos = page_offset(page) + offset_in_page(de);
	int err;

	lock_page(page);
	err = sysv_prepare_chunk(page, pos, SYSV_DIRSIZE);
	if (err) {
		unlock_page(page);
		return err;
	}
	de->inode = 0;
	dir_commit_chunk(page, pos, SYSV_DIRSIZE);
=======
	dir_commit_chunk(folio, pos, SYSV_DIRSIZE);
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	err = sysv_handle_dirsync(dir);
out_folio:
	folio_release_kmap(folio, kaddr);
	return err;
out_unlock:
	folio_unlock(folio);
	goto out_folio;
}

int sysv_delete_entry(struct sysv_dir_entry *de, struct folio *folio)
{
	struct inode *inode = folio->mapping->host;
	loff_t pos = folio_pos(folio) + offset_in_folio(folio, de);
	int err;

	folio_lock(folio);
	err = sysv_prepare_chunk(folio, pos, SYSV_DIRSIZE);
	if (err) {
		folio_unlock(folio);
		return err;
	}
	de->inode = 0;
	dir_commit_chunk(folio, pos, SYSV_DIRSIZE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	inode_set_mtime_to_ts(inode, inode_set_ctime_current(inode));
	mark_inode_dirty(inode);
	return sysv_handle_dirsync(inode);
}

int sysv_make_empty(struct inode *inode, struct inode *dir)
{
<<<<<<< HEAD
	struct page *page = grab_cache_page(inode->i_mapping, 0);
	struct sysv_dir_entry * de;
	char *base;
	int err;

	if (!page)
		return -ENOMEM;
	err = sysv_prepare_chunk(page, 0, 2 * SYSV_DIRSIZE);
	if (err) {
		unlock_page(page);
		goto fail;
	}
	base = kmap_local_page(page);
	memset(base, 0, PAGE_SIZE);

	de = (struct sysv_dir_entry *) base;
=======
	struct folio *folio = filemap_grab_folio(inode->i_mapping, 0);
	struct sysv_dir_entry * de;
	char *kaddr;
	int err;

	if (IS_ERR(folio))
		return PTR_ERR(folio);
	err = sysv_prepare_chunk(folio, 0, 2 * SYSV_DIRSIZE);
	if (err) {
		folio_unlock(folio);
		goto fail;
	}
	kaddr = kmap_local_folio(folio, 0);
	memset(kaddr, 0, folio_size(folio));

	de = (struct sysv_dir_entry *)kaddr;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	de->inode = cpu_to_fs16(SYSV_SB(inode->i_sb), inode->i_ino);
	strcpy(de->name,".");
	de++;
	de->inode = cpu_to_fs16(SYSV_SB(inode->i_sb), dir->i_ino);
	strcpy(de->name,"..");

<<<<<<< HEAD
	kunmap_local(base);
	dir_commit_chunk(page, 0, 2 * SYSV_DIRSIZE);
	err = sysv_handle_dirsync(inode);
fail:
	put_page(page);
=======
	kunmap_local(kaddr);
	dir_commit_chunk(folio, 0, 2 * SYSV_DIRSIZE);
	err = sysv_handle_dirsync(inode);
fail:
	folio_put(folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return err;
}

/*
 * routine to check that the specified directory is empty (for rmdir)
 */
int sysv_empty_dir(struct inode * inode)
{
	struct super_block *sb = inode->i_sb;
<<<<<<< HEAD
	struct page *page = NULL;
=======
	struct folio *folio = NULL;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	unsigned long i, npages = dir_pages(inode);
	char *kaddr;

	for (i = 0; i < npages; i++) {
		struct sysv_dir_entry *de;

<<<<<<< HEAD
		kaddr = dir_get_page(inode, i, &page);
=======
		kaddr = dir_get_folio(inode, i, &folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (IS_ERR(kaddr))
			continue;

		de = (struct sysv_dir_entry *)kaddr;
<<<<<<< HEAD
		kaddr += PAGE_SIZE-SYSV_DIRSIZE;
=======
		kaddr += folio_size(folio) - SYSV_DIRSIZE;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

		for ( ;(char *)de <= kaddr; de++) {
			if (!de->inode)
				continue;
			/* check for . and .. */
			if (de->name[0] != '.')
				goto not_empty;
			if (!de->name[1]) {
				if (de->inode == cpu_to_fs16(SYSV_SB(sb),
							inode->i_ino))
					continue;
				goto not_empty;
			}
			if (de->name[1] != '.' || de->name[2])
				goto not_empty;
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
int sysv_set_link(struct sysv_dir_entry *de, struct page *page,
	struct inode *inode)
{
	struct inode *dir = page->mapping->host;
	loff_t pos = page_offset(page) + offset_in_page(de);
	int err;

	lock_page(page);
	err = sysv_prepare_chunk(page, pos, SYSV_DIRSIZE);
	if (err) {
		unlock_page(page);
		return err;
	}
	de->inode = cpu_to_fs16(SYSV_SB(inode->i_sb), inode->i_ino);
	dir_commit_chunk(page, pos, SYSV_DIRSIZE);
=======
int sysv_set_link(struct sysv_dir_entry *de, struct folio *folio,
		struct inode *inode)
{
	struct inode *dir = folio->mapping->host;
	loff_t pos = folio_pos(folio) + offset_in_folio(folio, de);
	int err;

	folio_lock(folio);
	err = sysv_prepare_chunk(folio, pos, SYSV_DIRSIZE);
	if (err) {
		folio_unlock(folio);
		return err;
	}
	de->inode = cpu_to_fs16(SYSV_SB(inode->i_sb), inode->i_ino);
	dir_commit_chunk(folio, pos, SYSV_DIRSIZE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	inode_set_mtime_to_ts(dir, inode_set_ctime_current(dir));
	mark_inode_dirty(dir);
	return sysv_handle_dirsync(inode);
}

/*
<<<<<<< HEAD
 * Calls to dir_get_page()/unmap_and_put_page() must be nested according to the
 * rules documented in mm/highmem.rst.
 *
 * sysv_dotdot() acts as a call to dir_get_page() and must be treated
 * accordingly for nesting purposes.
 */
struct sysv_dir_entry *sysv_dotdot(struct inode *dir, struct page **p)
{
	struct sysv_dir_entry *de = dir_get_page(dir, 0, p);
=======
 * Calls to dir_get_folio()/folio_release_kmap() must be nested according to the
 * rules documented in mm/highmem.rst.
 *
 * sysv_dotdot() acts as a call to dir_get_folio() and must be treated
 * accordingly for nesting purposes.
 */
struct sysv_dir_entry *sysv_dotdot(struct inode *dir, struct folio **foliop)
{
	struct sysv_dir_entry *de = dir_get_folio(dir, 0, foliop);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if (IS_ERR(de))
		return NULL;
	/* ".." is the second directory entry */
	return de + 1;
}

ino_t sysv_inode_by_name(struct dentry *dentry)
{
<<<<<<< HEAD
	struct page *page;
	struct sysv_dir_entry *de = sysv_find_entry (dentry, &page);
=======
	struct folio *folio;
	struct sysv_dir_entry *de = sysv_find_entry (dentry, &folio);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	ino_t res = 0;
	
	if (de) {
		res = fs16_to_cpu(SYSV_SB(dentry->d_sb), de->inode);
<<<<<<< HEAD
		unmap_and_put_page(page, de);
=======
		folio_release_kmap(folio, de);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return res;
}
