# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the Linux/LoongArch-specific parts of the memory manager.
#

obj-y				+= init.o cache.o tlb.o tlbex.o extable.o \
<<<<<<< HEAD
				   fault.o ioremap.o maccess.o mmap.o pgtable.o page.o
=======
				   fault.o ioremap.o maccess.o mmap.o pgtable.o \
				   page.o pageattr.o
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_KASAN)		+= kasan_init.o

KASAN_SANITIZE_kasan_init.o     := n
