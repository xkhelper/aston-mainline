// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2008 Red Hat.  All rights reserved.
 */

#include "ctree.h"
#include "orphan.h"

int btrfs_insert_orphan_item(struct btrfs_trans_handle *trans,
			     struct btrfs_root *root, u64 offset)
{
<<<<<<< HEAD
	struct btrfs_path *path;
	struct btrfs_key key;
	int ret = 0;
=======
	BTRFS_PATH_AUTO_FREE(path);
	struct btrfs_key key;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	key.objectid = BTRFS_ORPHAN_OBJECTID;
	key.type = BTRFS_ORPHAN_ITEM_KEY;
	key.offset = offset;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

<<<<<<< HEAD
	ret = btrfs_insert_empty_item(trans, root, path, &key, 0);

	btrfs_free_path(path);
	return ret;
=======
	return btrfs_insert_empty_item(trans, root, path, &key, 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

int btrfs_del_orphan_item(struct btrfs_trans_handle *trans,
			  struct btrfs_root *root, u64 offset)
{
<<<<<<< HEAD
	struct btrfs_path *path;
=======
	BTRFS_PATH_AUTO_FREE(path);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct btrfs_key key;
	int ret = 0;

	key.objectid = BTRFS_ORPHAN_OBJECTID;
	key.type = BTRFS_ORPHAN_ITEM_KEY;
	key.offset = offset;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	ret = btrfs_search_slot(trans, root, &key, path, -1, 1);
	if (ret < 0)
<<<<<<< HEAD
		goto out;
	if (ret) { /* JDM: Really? */
		ret = -ENOENT;
		goto out;
	}

	ret = btrfs_del_item(trans, root, path);

out:
	btrfs_free_path(path);
	return ret;
=======
		return ret;
	if (ret)
		return -ENOENT;

	return btrfs_del_item(trans, root, path);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}
