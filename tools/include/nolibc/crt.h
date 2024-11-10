/* SPDX-License-Identifier: LGPL-2.1 OR MIT */
/*
 * C Run Time support for NOLIBC
 * Copyright (C) 2023 Zhangjin Wu <falcon@tinylab.org>
 */

#ifndef _NOLIBC_CRT_H
#define _NOLIBC_CRT_H

char **environ __attribute__((weak));
const unsigned long *_auxv __attribute__((weak));

static void __stack_chk_init(void);
static void exit(int);

<<<<<<< HEAD
extern void (*const __preinit_array_start[])(void) __attribute__((weak));
extern void (*const __preinit_array_end[])(void) __attribute__((weak));

extern void (*const __init_array_start[])(void) __attribute__((weak));
extern void (*const __init_array_end[])(void) __attribute__((weak));
=======
extern void (*const __preinit_array_start[])(int, char **, char**) __attribute__((weak));
extern void (*const __preinit_array_end[])(int, char **, char**) __attribute__((weak));

extern void (*const __init_array_start[])(int, char **, char**) __attribute__((weak));
extern void (*const __init_array_end[])(int, char **, char**) __attribute__((weak));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

extern void (*const __fini_array_start[])(void) __attribute__((weak));
extern void (*const __fini_array_end[])(void) __attribute__((weak));

<<<<<<< HEAD
__attribute__((weak))
=======
__attribute__((weak,used))
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
void _start_c(long *sp)
{
	long argc;
	char **argv;
	char **envp;
	int exitcode;
<<<<<<< HEAD
	void (* const *func)(void);
=======
	void (* const *ctor_func)(int, char **, char **);
	void (* const *dtor_func)(void);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	const unsigned long *auxv;
	/* silence potential warning: conflicting types for 'main' */
	int _nolibc_main(int, char **, char **) __asm__ ("main");

	/* initialize stack protector */
	__stack_chk_init();

	/*
	 * sp  :    argc          <-- argument count, required by main()
	 * argv:    argv[0]       <-- argument vector, required by main()
	 *          argv[1]
	 *          ...
	 *          argv[argc-1]
	 *          null
	 * environ: environ[0]    <-- environment variables, required by main() and getenv()
	 *          environ[1]
	 *          ...
	 *          null
	 * _auxv:   _auxv[0]      <-- auxiliary vector, required by getauxval()
	 *          _auxv[1]
	 *          ...
	 *          null
	 */

	/* assign argc and argv */
	argc = *sp;
	argv = (void *)(sp + 1);

	/* find environ */
	environ = envp = argv + argc + 1;

	/* find _auxv */
	for (auxv = (void *)envp; *auxv++;)
		;
	_auxv = auxv;

<<<<<<< HEAD
	for (func = __preinit_array_start; func < __preinit_array_end; func++)
		(*func)();
	for (func = __init_array_start; func < __init_array_end; func++)
		(*func)();
=======
	for (ctor_func = __preinit_array_start; ctor_func < __preinit_array_end; ctor_func++)
		(*ctor_func)(argc, argv, envp);
	for (ctor_func = __init_array_start; ctor_func < __init_array_end; ctor_func++)
		(*ctor_func)(argc, argv, envp);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	/* go to application */
	exitcode = _nolibc_main(argc, argv, envp);

<<<<<<< HEAD
	for (func = __fini_array_end; func > __fini_array_start;)
		(*--func)();
=======
	for (dtor_func = __fini_array_end; dtor_func > __fini_array_start;)
		(*--dtor_func)();
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	exit(exitcode);
}

#endif /* _NOLIBC_CRT_H */
