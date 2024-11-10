/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _BCACHEFS_BTREE_KEY_CACHE_TYPES_H
#define _BCACHEFS_BTREE_KEY_CACHE_TYPES_H

<<<<<<< HEAD
struct btree_key_cache_freelist {
	struct bkey_cached	*objs[16];
	unsigned		nr;
};

struct btree_key_cache {
	struct mutex		lock;
	struct rhashtable	table;
	bool			table_init_done;

	struct list_head	freed_pcpu;
	size_t			nr_freed_pcpu;
	struct list_head	freed_nonpcpu;
	size_t			nr_freed_nonpcpu;

	struct shrinker		*shrink;
	unsigned		shrink_iter;
	struct btree_key_cache_freelist __percpu *pcpu_freed;

	atomic_long_t		nr_freed;
=======
#include "rcu_pending.h"

struct btree_key_cache {
	struct rhashtable	table;
	bool			table_init_done;

	struct shrinker		*shrink;
	unsigned		shrink_iter;

	/* 0: non pcpu reader locks, 1: pcpu reader locks */
	struct rcu_pending	pending[2];
	size_t __percpu		*nr_pending;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	atomic_long_t		nr_keys;
	atomic_long_t		nr_dirty;

	/* shrinker stats */
	unsigned long		requested_to_free;
	unsigned long		freed;
<<<<<<< HEAD
	unsigned long		moved_to_freelist;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	unsigned long		skipped_dirty;
	unsigned long		skipped_accessed;
	unsigned long		skipped_lock_fail;
};

struct bkey_cached_key {
	u32			btree_id;
	struct bpos		pos;
} __packed __aligned(4);

#endif /* _BCACHEFS_BTREE_KEY_CACHE_TYPES_H */
