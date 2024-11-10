// SPDX-License-Identifier: GPL-2.0
/*
 * QNX6 file system, Linux implementation.
 *
 * Version : 1.0.0
 *
 * History :
 *
 * 01-02-2012 by Kai Bankett (chaosman@ontika.net) : first release.
 * 16-02-2012 pagemap extension by Al Viro
 *
 */

#include "qnx6.h"

struct dentry *qnx6_lookup(struct inode *dir, struct dentry *dentry,
				unsigned int flags)
{
	unsigned ino;
<<<<<<< HEAD
	struct page *page;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct inode *foundinode = NULL;
	const char *name = dentry->d_name.name;
	int len = dentry->d_name.len;

	if (len > QNX6_LONG_NAME_MAX)
		return ERR_PTR(-ENAMETOOLONG);

<<<<<<< HEAD
	ino = qnx6_find_entry(len, dir, name, &page);
	if (ino) {
		foundinode = qnx6_iget(dir->i_sb, ino);
		qnx6_put_page(page);
=======
	ino = qnx6_find_ino(len, dir, name);
	if (ino) {
		foundinode = qnx6_iget(dir->i_sb, ino);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (IS_ERR(foundinode))
			pr_debug("lookup->iget ->  error %ld\n",
				 PTR_ERR(foundinode));
	} else {
		pr_debug("%s(): not found %s\n", __func__, name);
	}
	return d_splice_alias(foundinode, dentry);
}
