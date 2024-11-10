/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2023-2024 Intel Corporation
 */

#ifndef _XE_GT_SRIOV_PF_TYPES_H_
#define _XE_GT_SRIOV_PF_TYPES_H_

#include <linux/types.h>

#include "xe_gt_sriov_pf_config_types.h"
<<<<<<< HEAD
=======
#include "xe_gt_sriov_pf_control_types.h"
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#include "xe_gt_sriov_pf_monitor_types.h"
#include "xe_gt_sriov_pf_policy_types.h"
#include "xe_gt_sriov_pf_service_types.h"

/**
 * struct xe_gt_sriov_metadata - GT level per-VF metadata.
 */
struct xe_gt_sriov_metadata {
	/** @config: per-VF provisioning data. */
	struct xe_gt_sriov_config config;

	/** @monitor: per-VF monitoring data. */
	struct xe_gt_sriov_monitor monitor;

<<<<<<< HEAD
=======
	/** @control: per-VF control data. */
	struct xe_gt_sriov_control_state control;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	/** @version: negotiated VF/PF ABI version */
	struct xe_gt_sriov_pf_service_version version;
};

/**
 * struct xe_gt_sriov_pf - GT level PF virtualization data.
 * @service: service data.
<<<<<<< HEAD
=======
 * @control: control data.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
 * @policy: policy data.
 * @spare: PF-only provisioning configuration.
 * @vfs: metadata for all VFs.
 */
struct xe_gt_sriov_pf {
	struct xe_gt_sriov_pf_service service;
<<<<<<< HEAD
=======
	struct xe_gt_sriov_pf_control control;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct xe_gt_sriov_pf_policy policy;
	struct xe_gt_sriov_spare_config spare;
	struct xe_gt_sriov_metadata *vfs;
};

#endif
