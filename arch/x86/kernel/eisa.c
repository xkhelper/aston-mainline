// SPDX-License-Identifier: GPL-2.0-only
/*
 * EISA specific code
 */
#include <linux/cc_platform.h>
#include <linux/ioport.h>
#include <linux/eisa.h>
#include <linux/io.h>

#include <xen/xen.h>

static __init int eisa_bus_probe(void)
{
<<<<<<< HEAD
	void __iomem *p;
=======
	u32 *p;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if ((xen_pv_domain() && !xen_initial_domain()) || cc_platform_has(CC_ATTR_GUEST_SEV_SNP))
		return 0;

<<<<<<< HEAD
	p = ioremap(0x0FFFD9, 4);
	if (p && readl(p) == 'E' + ('I' << 8) + ('S' << 16) + ('A' << 24))
		EISA_bus = 1;
	iounmap(p);
=======
	p = memremap(0x0FFFD9, 4, MEMREMAP_WB);
	if (p && *p == 'E' + ('I' << 8) + ('S' << 16) + ('A' << 24))
		EISA_bus = 1;
	memunmap(p);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return 0;
}
subsys_initcall(eisa_bus_probe);
