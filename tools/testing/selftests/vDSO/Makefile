# SPDX-License-Identifier: GPL-2.0
<<<<<<< HEAD
uname_M := $(shell uname -m 2>/dev/null || echo not)
ARCH ?= $(shell echo $(uname_M) | sed -e s/i.86/x86/ -e s/x86_64/x86/)
SODIUM := $(shell pkg-config --libs libsodium 2>/dev/null)
=======
include ../../../scripts/Makefile.arch
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

TEST_GEN_PROGS := vdso_test_gettimeofday
TEST_GEN_PROGS += vdso_test_getcpu
TEST_GEN_PROGS += vdso_test_abi
TEST_GEN_PROGS += vdso_test_clock_getres
ifeq ($(ARCH),$(filter $(ARCH),x86 x86_64))
TEST_GEN_PROGS += vdso_standalone_test_x86
endif
TEST_GEN_PROGS += vdso_test_correctness
<<<<<<< HEAD
ifeq ($(uname_M),x86_64)
TEST_GEN_PROGS += vdso_test_getrandom
ifneq ($(SODIUM),)
TEST_GEN_PROGS += vdso_test_chacha
endif
endif

CFLAGS := -std=gnu99
=======
TEST_GEN_PROGS += vdso_test_getrandom
TEST_GEN_PROGS += vdso_test_chacha

CFLAGS := -std=gnu99 -O2
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

ifeq ($(CONFIG_X86_32),y)
LDLIBS += -lgcc_s
endif

include ../lib.mk
$(OUTPUT)/vdso_test_gettimeofday: parse_vdso.c vdso_test_gettimeofday.c
$(OUTPUT)/vdso_test_getcpu: parse_vdso.c vdso_test_getcpu.c
$(OUTPUT)/vdso_test_abi: parse_vdso.c vdso_test_abi.c
$(OUTPUT)/vdso_test_clock_getres: vdso_test_clock_getres.c

$(OUTPUT)/vdso_standalone_test_x86: vdso_standalone_test_x86.c parse_vdso.c
$(OUTPUT)/vdso_standalone_test_x86: CFLAGS +=-nostdlib -fno-asynchronous-unwind-tables -fno-stack-protector

$(OUTPUT)/vdso_test_correctness: vdso_test_correctness.c
$(OUTPUT)/vdso_test_correctness: LDFLAGS += -ldl

$(OUTPUT)/vdso_test_getrandom: parse_vdso.c
$(OUTPUT)/vdso_test_getrandom: CFLAGS += -isystem $(top_srcdir)/tools/include \
<<<<<<< HEAD
                                         -isystem $(top_srcdir)/include/uapi

$(OUTPUT)/vdso_test_chacha: $(top_srcdir)/arch/$(ARCH)/entry/vdso/vgetrandom-chacha.S
$(OUTPUT)/vdso_test_chacha: CFLAGS += -idirafter $(top_srcdir)/tools/include \
                                      -isystem $(top_srcdir)/arch/$(ARCH)/include \
                                      -isystem $(top_srcdir)/include \
                                      -D__ASSEMBLY__ -DBULID_VDSO -DCONFIG_FUNCTION_ALIGNMENT=0 \
                                      -Wa,--noexecstack $(SODIUM)
=======
                                         $(KHDR_INCLUDES) \
                                         -isystem $(top_srcdir)/include/uapi

$(OUTPUT)/vdso_test_chacha: vgetrandom-chacha.S
$(OUTPUT)/vdso_test_chacha: CFLAGS += -idirafter $(top_srcdir)/tools/include \
                                      -idirafter $(top_srcdir)/tools/include/generated \
                                      -idirafter $(top_srcdir)/arch/$(SRCARCH)/include \
                                      -idirafter $(top_srcdir)/include \
                                      -Wa,--noexecstack
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
