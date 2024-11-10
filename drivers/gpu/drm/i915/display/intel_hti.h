/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2022 Intel Corporation
 */

#ifndef __INTEL_HTI_H__
#define __INTEL_HTI_H__

#include <linux/types.h>

<<<<<<< HEAD
struct drm_i915_private;
enum phy;

void intel_hti_init(struct drm_i915_private *i915);
bool intel_hti_uses_phy(struct drm_i915_private *i915, enum phy phy);
u32 intel_hti_dpll_mask(struct drm_i915_private *i915);
=======
struct intel_display;
enum phy;

void intel_hti_init(struct intel_display *display);
bool intel_hti_uses_phy(struct intel_display *display, enum phy phy);
u32 intel_hti_dpll_mask(struct intel_display *display);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#endif /* __INTEL_HTI_H__ */
