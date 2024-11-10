/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _TOOLS_ASM_ALTERNATIVE_ASM_H
#define _TOOLS_ASM_ALTERNATIVE_ASM_H

<<<<<<< HEAD
=======
#if defined(__s390x__)
#ifdef __ASSEMBLY__
.macro ALTERNATIVE oldinstr, newinstr, feature
	\oldinstr
.endm
#endif
#else

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
/* Just disable it so we can build arch/x86/lib/memcpy_64.S for perf bench: */

#define ALTERNATIVE #

#endif
<<<<<<< HEAD
=======

#endif
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
