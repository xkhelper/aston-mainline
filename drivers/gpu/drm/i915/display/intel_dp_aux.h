/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2020-2021 Intel Corporation
 */

#ifndef __INTEL_DP_AUX_H__
#define __INTEL_DP_AUX_H__

#include <linux/types.h>

enum aux_ch;
<<<<<<< HEAD
struct drm_i915_private;
=======
struct intel_display;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
struct intel_dp;
struct intel_encoder;

void intel_dp_aux_fini(struct intel_dp *intel_dp);
void intel_dp_aux_init(struct intel_dp *intel_dp);

enum aux_ch intel_dp_aux_ch(struct intel_encoder *encoder);

<<<<<<< HEAD
void intel_dp_aux_irq_handler(struct drm_i915_private *i915);
=======
void intel_dp_aux_irq_handler(struct intel_display *display);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
u32 intel_dp_aux_pack(const u8 *src, int src_bytes);
int intel_dp_aux_fw_sync_len(struct intel_dp *intel_dp);

#endif /* __INTEL_DP_AUX_H__ */
