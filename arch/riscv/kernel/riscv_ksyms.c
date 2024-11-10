// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2017 Zihao Yu
 */

#include <linux/export.h>
#include <linux/uaccess.h>

/*
 * Assembly functions that may be used (directly or indirectly) by modules
 */
EXPORT_SYMBOL(memset);
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);
<<<<<<< HEAD
EXPORT_SYMBOL(strcmp);
EXPORT_SYMBOL(strlen);
EXPORT_SYMBOL(strncmp);
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
EXPORT_SYMBOL(__memset);
EXPORT_SYMBOL(__memcpy);
EXPORT_SYMBOL(__memmove);
