#!/bin/sh
# SPDX-License-Identifier: (GPL-2.0 OR BSD-3-Clause)

# Kselftest framework requirement - SKIP code is 4.
ksft_skip=4

[ -e /dev/tpmrm0 ] || exit $ksft_skip

<<<<<<< HEAD
python3 -m unittest -v tpm2_tests.SpaceTest
=======
python3 -m unittest -v tpm2_tests.SpaceTest 2>&1
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
