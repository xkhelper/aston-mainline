/* SPDX-License-Identifier: GPL-2.0 */
/*
<<<<<<< HEAD
 * Define generic no-op hooks for arch_dup_mmap, arch_exit_mmap
 * and arch_unmap to be included in asm-FOO/mmu_context.h for any
 * arch FOO which doesn't need to hook these.
=======
 * Define generic no-op hooks for arch_dup_mmap and arch_exit_mmap
 * to be included in asm-FOO/mmu_context.h for any arch FOO which
 * doesn't need to hook these.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
 */
#ifndef _ASM_GENERIC_MM_HOOKS_H
#define _ASM_GENERIC_MM_HOOKS_H

static inline int arch_dup_mmap(struct mm_struct *oldmm,
				struct mm_struct *mm)
{
	return 0;
}

static inline void arch_exit_mmap(struct mm_struct *mm)
{
}

<<<<<<< HEAD
static inline void arch_unmap(struct mm_struct *mm,
			unsigned long start, unsigned long end)
{
}

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static inline bool arch_vma_access_permitted(struct vm_area_struct *vma,
		bool write, bool execute, bool foreign)
{
	/* by default, allow everything */
	return true;
}
#endif	/* _ASM_GENERIC_MM_HOOKS_H */
