// SPDX-License-Identifier: GPL-2.0
/* Copyright (c) 2023 Meta Platforms, Inc. and affiliates. */

#include <test_progs.h>
#include "nested_trust_failure.skel.h"
#include "nested_trust_success.skel.h"
<<<<<<< HEAD
=======
#include "nested_acquire.skel.h"
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

void test_nested_trust(void)
{
	RUN_TESTS(nested_trust_success);
	RUN_TESTS(nested_trust_failure);
<<<<<<< HEAD
=======

	if (env.has_testmod)
		RUN_TESTS(nested_acquire);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}
