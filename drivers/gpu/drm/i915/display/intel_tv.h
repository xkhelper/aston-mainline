/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __INTEL_TV_H__
#define __INTEL_TV_H__

<<<<<<< HEAD
struct drm_i915_private;

#ifdef I915
void intel_tv_init(struct drm_i915_private *dev_priv);
#else
static inline void intel_tv_init(struct drm_i915_private *dev_priv)
=======
struct intel_display;

#ifdef I915
void intel_tv_init(struct intel_display *display);
#else
static inline void intel_tv_init(struct intel_display *display)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
}
#endif

#endif /* __INTEL_TV_H__ */
