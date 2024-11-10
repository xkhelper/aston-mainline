/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_
#ifdef __KERNEL__

<<<<<<< HEAD
=======
#include <linux/cleanup.h>
#include <linux/err.h>

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
struct mnt_namespace;
struct fs_struct;
struct user_namespace;
struct ns_common;

extern struct mnt_namespace *copy_mnt_ns(unsigned long, struct mnt_namespace *,
		struct user_namespace *, struct fs_struct *);
extern void put_mnt_ns(struct mnt_namespace *ns);
<<<<<<< HEAD
=======
DEFINE_FREE(put_mnt_ns, struct mnt_namespace *, if (!IS_ERR_OR_NULL(_T)) put_mnt_ns(_T))
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
extern struct ns_common *from_mnt_ns(struct mnt_namespace *);

extern const struct file_operations proc_mounts_operations;
extern const struct file_operations proc_mountinfo_operations;
extern const struct file_operations proc_mountstats_operations;

#endif
#endif
