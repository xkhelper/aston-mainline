<<<<<<< HEAD
=======
/* SPDX-License-Identifier: 0BSD */

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
/*
 * Wrapper for decompressing XZ-compressed kernel, initramfs, and initrd
 *
 * Author: Lasse Collin <lasse.collin@tukaani.org>
<<<<<<< HEAD
 *
 * This file has been put into the public domain.
 * You can do whatever you want with this file.
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
 */

#ifndef DECOMPRESS_UNXZ_H
#define DECOMPRESS_UNXZ_H

int unxz(unsigned char *in, long in_size,
	 long (*fill)(void *dest, unsigned long size),
	 long (*flush)(void *src, unsigned long size),
	 unsigned char *out, long *in_used,
	 void (*error)(char *x));

#endif
