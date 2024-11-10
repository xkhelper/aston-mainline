#ifndef _TOOLS_INCLUDE_LINUX_LINKAGE_H
#define _TOOLS_INCLUDE_LINUX_LINKAGE_H

<<<<<<< HEAD
=======
#include <linux/export.h>

#define SYM_FUNC_START(x) .globl x; x:
#define SYM_FUNC_END(x)
#define SYM_DATA_START(x) .globl x; x:
#define SYM_DATA_START_LOCAL(x) x:
#define SYM_DATA_END(x)

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#endif /* _TOOLS_INCLUDE_LINUX_LINKAGE_H */
