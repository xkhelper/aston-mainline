// SPDX-License-Identifier: GPL-2.0
/*
 * Capability utilities
 */

<<<<<<< HEAD
#ifdef HAVE_LIBCAP_SUPPORT

#include "cap.h"
#include <stdbool.h>
#include <sys/capability.h>

bool perf_cap__capable(cap_value_t cap)
{
	cap_flag_value_t val;
	cap_t caps = cap_get_proc();

	if (!caps)
		return false;

	if (cap_get_flag(caps, cap, CAP_EFFECTIVE, &val) != 0)
		val = CAP_CLEAR;

	if (cap_free(caps) != 0)
		return false;

	return val == CAP_SET;
}

#endif  /* HAVE_LIBCAP_SUPPORT */
=======
#include "cap.h"
#include "debug.h"
#include <errno.h>
#include <string.h>
#include <linux/capability.h>
#include <sys/syscall.h>
#include <unistd.h>

#define MAX_LINUX_CAPABILITY_U32S _LINUX_CAPABILITY_U32S_3

bool perf_cap__capable(int cap, bool *used_root)
{
	struct __user_cap_header_struct header = {
		.version = _LINUX_CAPABILITY_VERSION_3,
		.pid = 0,
	};
	struct __user_cap_data_struct data[MAX_LINUX_CAPABILITY_U32S] = {};
	__u32 cap_val;

	*used_root = false;
	while (syscall(SYS_capget, &header, &data[0]) == -1) {
		/* Retry, first attempt has set the header.version correctly. */
		if (errno == EINVAL && header.version != _LINUX_CAPABILITY_VERSION_3 &&
		    header.version == _LINUX_CAPABILITY_VERSION_1)
			continue;

		pr_debug2("capget syscall failed (%s - %d) fall back on root check\n",
			  strerror(errno), errno);
		*used_root = true;
		return geteuid() == 0;
	}

	/* Extract the relevant capability bit. */
	if (cap >= 32) {
		if (header.version == _LINUX_CAPABILITY_VERSION_3) {
			cap_val = data[1].effective;
		} else {
			/* Capability beyond 32 is requested but only 32 are supported. */
			return false;
		}
	} else {
		cap_val = data[0].effective;
	}
	return (cap_val & (1 << (cap & 0x1f))) != 0;
}
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
