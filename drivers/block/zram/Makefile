# SPDX-License-Identifier: GPL-2.0-only
<<<<<<< HEAD
zram-y	:=	zcomp.o zram_drv.o

=======

zram-y	:=	zcomp.o zram_drv.o

zram-$(CONFIG_ZRAM_BACKEND_LZO)		+= backend_lzorle.o backend_lzo.o
zram-$(CONFIG_ZRAM_BACKEND_LZ4)		+= backend_lz4.o
zram-$(CONFIG_ZRAM_BACKEND_LZ4HC)	+= backend_lz4hc.o
zram-$(CONFIG_ZRAM_BACKEND_ZSTD)	+= backend_zstd.o
zram-$(CONFIG_ZRAM_BACKEND_DEFLATE)	+= backend_deflate.o
zram-$(CONFIG_ZRAM_BACKEND_842)		+= backend_842.o

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
obj-$(CONFIG_ZRAM)	+=	zram.o
