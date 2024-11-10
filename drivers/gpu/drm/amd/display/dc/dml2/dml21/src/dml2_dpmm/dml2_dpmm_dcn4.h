// SPDX-License-Identifier: MIT
//
// Copyright 2024 Advanced Micro Devices, Inc.

<<<<<<< HEAD

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#ifndef __DML2_DPMM_DCN4_H__
#define __DML2_DPMM_DCN4_H__

#include "dml2_internal_shared_types.h"

bool dpmm_dcn3_map_mode_to_soc_dpm(struct dml2_dpmm_map_mode_to_soc_dpm_params_in_out *in_out);
bool dpmm_dcn4_map_mode_to_soc_dpm(struct dml2_dpmm_map_mode_to_soc_dpm_params_in_out *in_out);
bool dpmm_dcn4_map_watermarks(struct dml2_dpmm_map_watermarks_params_in_out *in_out);

bool dpmm_dcn4_unit_test(void);

#endif
