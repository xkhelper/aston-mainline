// SPDX-License-Identifier: GPL-2.0
#include <linux/bcd.h>
#include <linux/export.h>

unsigned _bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}
EXPORT_SYMBOL(_bcd2bin);

unsigned char _bin2bcd(unsigned val)
{
<<<<<<< HEAD
	return ((val / 10) << 4) + val % 10;
=======
	const unsigned int t = (val * 103) >> 10;

	return (t << 4) | (val - t * 10);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}
EXPORT_SYMBOL(_bin2bcd);
