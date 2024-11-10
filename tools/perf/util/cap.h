/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_CAP_H
#define __PERF_CAP_H

#include <stdbool.h>
<<<<<<< HEAD
#include <linux/capability.h>
#include <linux/compiler.h>

#ifdef HAVE_LIBCAP_SUPPORT

#include <sys/capability.h>

bool perf_cap__capable(cap_value_t cap);

#else

#include <unistd.h>
#include <sys/types.h>

static inline bool perf_cap__capable(int cap __maybe_unused)
{
	return geteuid() == 0;
}

#endif /* HAVE_LIBCAP_SUPPORT */
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

/* For older systems */
#ifndef CAP_SYSLOG
#define CAP_SYSLOG	34
#endif

#ifndef CAP_PERFMON
#define CAP_PERFMON	38
#endif

<<<<<<< HEAD
=======
/* Query if a capability is supported, used_root is set if the fallback root check was used. */
bool perf_cap__capable(int cap, bool *used_root);

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#endif /* __PERF_CAP_H */
