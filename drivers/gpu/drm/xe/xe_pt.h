/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2022 Intel Corporation
 */
#ifndef _XE_PT_H_
#define _XE_PT_H_

#include <linux/types.h>

#include "xe_pt_types.h"

struct dma_fence;
struct xe_bo;
struct xe_device;
struct xe_exec_queue;
struct xe_sync_entry;
struct xe_tile;
struct xe_vm;
struct xe_vma;
<<<<<<< HEAD
=======
struct xe_vma_ops;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

/* Largest huge pte is currently 1GiB. May become device dependent. */
#define MAX_HUGEPTE_LEVEL 2

#define xe_pt_write(xe, map, idx, data) \
	xe_map_wr(xe, map, (idx) * sizeof(u64), u64, data)

unsigned int xe_pt_shift(unsigned int level);

struct xe_pt *xe_pt_create(struct xe_vm *vm, struct xe_tile *tile,
			   unsigned int level);

void xe_pt_populate_empty(struct xe_tile *tile, struct xe_vm *vm,
			  struct xe_pt *pt);

void xe_pt_destroy(struct xe_pt *pt, u32 flags, struct llist_head *deferred);

<<<<<<< HEAD
struct dma_fence *
__xe_pt_bind_vma(struct xe_tile *tile, struct xe_vma *vma, struct xe_exec_queue *q,
		 struct xe_sync_entry *syncs, u32 num_syncs,
		 bool rebind);

struct dma_fence *
__xe_pt_unbind_vma(struct xe_tile *tile, struct xe_vma *vma, struct xe_exec_queue *q,
		   struct xe_sync_entry *syncs, u32 num_syncs);
=======
int xe_pt_update_ops_prepare(struct xe_tile *tile, struct xe_vma_ops *vops);
struct dma_fence *xe_pt_update_ops_run(struct xe_tile *tile,
				       struct xe_vma_ops *vops);
void xe_pt_update_ops_fini(struct xe_tile *tile, struct xe_vma_ops *vops);
void xe_pt_update_ops_abort(struct xe_tile *tile, struct xe_vma_ops *vops);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

bool xe_pt_zap_ptes(struct xe_tile *tile, struct xe_vma *vma);

#endif
