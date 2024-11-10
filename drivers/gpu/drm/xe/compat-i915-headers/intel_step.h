/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2023 Intel Corporation
 */

#ifndef __INTEL_STEP_H__
#define __INTEL_STEP_H__

<<<<<<< HEAD
#include "xe_device_types.h"
#include "xe_step.h"

#define intel_display_step_name xe_display_step_name

static inline
const char *xe_display_step_name(struct xe_device *xe)
{
	return xe_step_name(xe->info.step.display);
}
=======
#include "xe_step.h"

#define intel_step xe_step
#define intel_step_name xe_step_name
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#endif /* __INTEL_STEP_H__ */
