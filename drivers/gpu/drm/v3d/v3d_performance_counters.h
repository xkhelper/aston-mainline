/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2024 Raspberry Pi
 */
#ifndef V3D_PERFORMANCE_COUNTERS_H
#define V3D_PERFORMANCE_COUNTERS_H

/* Holds a description of a given performance counter. The index of performance
 * counter is given by the array on v3d_performance_counter.h
 */
struct v3d_perf_counter_desc {
	/* Category of the counter */
	char category[32];

	/* Name of the counter */
	char name[64];

	/* Description of the counter */
	char description[256];
};

<<<<<<< HEAD

#define V3D_V42_NUM_PERFCOUNTERS (87)
#define V3D_V71_NUM_PERFCOUNTERS (93)

/* Maximum number of performance counters supported by any version of V3D */
#define V3D_MAX_COUNTERS (93)
=======
struct v3d_perfmon_info {
	/*
	 * Different revisions of V3D have different total number of
	 * performance counters.
	 */
	unsigned int max_counters;

	/*
	 * Array of counters valid for the platform.
	 */
	const struct v3d_perf_counter_desc *counters;
};
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#endif
