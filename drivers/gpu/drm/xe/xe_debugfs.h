/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2022 Intel Corporation
 */

#ifndef _XE_DEBUGFS_H_
#define _XE_DEBUGFS_H_

struct xe_device;

<<<<<<< HEAD
void xe_debugfs_register(struct xe_device *xe);
=======
#ifdef CONFIG_DEBUG_FS
void xe_debugfs_register(struct xe_device *xe);
#else
static inline void xe_debugfs_register(struct xe_device *xe) { }
#endif
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#endif
