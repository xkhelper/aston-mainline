.. SPDX-License-Identifier: GPL-2.0

Arch Support
============

Currently, the Rust compiler (``rustc``) uses LLVM for code generation,
which limits the supported architectures that can be targeted. In addition,
support for building the kernel with LLVM/Clang varies (please see
Documentation/kbuild/llvm.rst). This support is needed for ``bindgen``
which uses ``libclang``.

Below is a general summary of architectures that currently work. Level of
support corresponds to ``S`` values in the ``MAINTAINERS`` file.

=============  ================  ==============================================
Architecture   Level of support  Constraints
=============  ================  ==============================================
``arm64``      Maintained        Little Endian only.
``loongarch``  Maintained        \-
<<<<<<< HEAD
``riscv``      Maintained        ``riscv64`` only.
=======
``riscv``      Maintained        ``riscv64`` and LLVM/Clang only.
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
``um``         Maintained        \-
``x86``        Maintained        ``x86_64`` only.
=============  ================  ==============================================

