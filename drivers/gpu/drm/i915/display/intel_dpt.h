/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2021 Intel Corporation
 */

#ifndef __INTEL_DPT_H__
#define __INTEL_DPT_H__

<<<<<<< HEAD
=======
#include <linux/types.h>

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
struct drm_i915_private;

struct i915_address_space;
struct i915_vma;
struct intel_framebuffer;

void intel_dpt_destroy(struct i915_address_space *vm);
struct i915_vma *intel_dpt_pin_to_ggtt(struct i915_address_space *vm,
				       unsigned int alignment);
void intel_dpt_unpin_from_ggtt(struct i915_address_space *vm);
void intel_dpt_suspend(struct drm_i915_private *i915);
void intel_dpt_resume(struct drm_i915_private *i915);
struct i915_address_space *
intel_dpt_create(struct intel_framebuffer *fb);
<<<<<<< HEAD
=======
u64 intel_dpt_offset(struct i915_vma *dpt_vma);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#endif /* __INTEL_DPT_H__ */
