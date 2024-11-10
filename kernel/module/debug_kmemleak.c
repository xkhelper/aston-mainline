// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Module kmemleak support
 *
 * Copyright (C) 2009 Catalin Marinas
 */

#include <linux/module.h>
#include <linux/kmemleak.h>
#include "internal.h"

void kmemleak_load_module(const struct module *mod,
			  const struct load_info *info)
{
<<<<<<< HEAD
	unsigned int i;

	/* only scan the sections containing data */
	kmemleak_scan_area(mod, sizeof(struct module), GFP_KERNEL);

	for (i = 1; i < info->hdr->e_shnum; i++) {
		/* Scan all writable sections that's not executable */
		if (!(info->sechdrs[i].sh_flags & SHF_ALLOC) ||
		    !(info->sechdrs[i].sh_flags & SHF_WRITE) ||
		    (info->sechdrs[i].sh_flags & SHF_EXECINSTR))
			continue;

		kmemleak_scan_area((void *)info->sechdrs[i].sh_addr,
				   info->sechdrs[i].sh_size, GFP_KERNEL);
=======
	/* only scan writable, non-executable sections */
	for_each_mod_mem_type(type) {
		if (type != MOD_DATA && type != MOD_INIT_DATA)
			kmemleak_no_scan(mod->mem[type].base);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
}
