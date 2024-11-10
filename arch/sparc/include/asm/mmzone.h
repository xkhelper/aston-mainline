/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _SPARC64_MMZONE_H
#define _SPARC64_MMZONE_H

#ifdef CONFIG_NUMA

#include <linux/cpumask.h>

<<<<<<< HEAD
extern struct pglist_data *node_data[];

#define NODE_DATA(nid)		(node_data[nid])

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
extern int numa_cpu_lookup_table[];
extern cpumask_t numa_cpumask_lookup_table[];

#endif /* CONFIG_NUMA */

#endif /* _SPARC64_MMZONE_H */
