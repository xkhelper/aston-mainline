/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _BCACHEFS_DISK_ACCOUNTING_TYPES_H
#define _BCACHEFS_DISK_ACCOUNTING_TYPES_H

#include "darray.h"

struct accounting_mem_entry {
	struct bpos				pos;
<<<<<<< HEAD
	struct bversion				version;
=======
	struct bversion				bversion;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	unsigned				nr_counters;
	u64 __percpu				*v[2];
};

struct bch_accounting_mem {
	DARRAY(struct accounting_mem_entry)	k;
	bool					gc_running;
};

#endif /* _BCACHEFS_DISK_ACCOUNTING_TYPES_H */
