// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022-2024 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 */

#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
<<<<<<< HEAD
=======
#include <sched.h>
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#include <signal.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/random.h>
#include <sys/syscall.h>
<<<<<<< HEAD
#include <sys/types.h>
#include <linux/random.h>

#include "../kselftest.h"
#include "parse_vdso.h"
=======
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/random.h>
#include <linux/compiler.h>
#include <linux/ptrace.h>

#include "../kselftest.h"
#include "parse_vdso.h"
#include "vdso_config.h"
#include "vdso_call.h"
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#ifndef timespecsub
#define	timespecsub(tsp, usp, vsp)					\
	do {								\
		(vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;		\
		(vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec;	\
		if ((vsp)->tv_nsec < 0) {				\
			(vsp)->tv_sec--;				\
			(vsp)->tv_nsec += 1000000000L;			\
		}							\
	} while (0)
#endif

<<<<<<< HEAD
=======
#define ksft_assert(condition) \
	do { if (!(condition)) ksft_exit_fail_msg("Assertion failed: %s\n", #condition); } while (0)

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static struct {
	pthread_mutex_t lock;
	void **states;
	size_t len, cap;
<<<<<<< HEAD
} grnd_allocator = {
	.lock = PTHREAD_MUTEX_INITIALIZER
};

static struct {
	ssize_t(*fn)(void *, size_t, unsigned long, void *, size_t);
	pthread_key_t key;
	pthread_once_t initialized;
	struct vgetrandom_opaque_params params;
} grnd_ctx = {
	.initialized = PTHREAD_ONCE_INIT
=======
	ssize_t(*fn)(void *, size_t, unsigned long, void *, size_t);
	struct vgetrandom_opaque_params params;
} vgrnd = {
	.lock = PTHREAD_MUTEX_INITIALIZER
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
};

static void *vgetrandom_get_state(void)
{
	void *state = NULL;

<<<<<<< HEAD
	pthread_mutex_lock(&grnd_allocator.lock);
	if (!grnd_allocator.len) {
		size_t page_size = getpagesize();
		size_t new_cap;
		size_t alloc_size, num = sysconf(_SC_NPROCESSORS_ONLN); /* Just a decent heuristic. */
		void *new_block, *new_states;

		alloc_size = (num * grnd_ctx.params.size_of_opaque_state + page_size - 1) & (~(page_size - 1));
		num = (page_size / grnd_ctx.params.size_of_opaque_state) * (alloc_size / page_size);
		new_block = mmap(0, alloc_size, grnd_ctx.params.mmap_prot, grnd_ctx.params.mmap_flags, -1, 0);
		if (new_block == MAP_FAILED)
			goto out;

		new_cap = grnd_allocator.cap + num;
		new_states = reallocarray(grnd_allocator.states, new_cap, sizeof(*grnd_allocator.states));
		if (!new_states)
			goto unmap;
		grnd_allocator.cap = new_cap;
		grnd_allocator.states = new_states;

		for (size_t i = 0; i < num; ++i) {
			if (((uintptr_t)new_block & (page_size - 1)) + grnd_ctx.params.size_of_opaque_state > page_size)
				new_block = (void *)(((uintptr_t)new_block + page_size - 1) & (~(page_size - 1)));
			grnd_allocator.states[i] = new_block;
			new_block += grnd_ctx.params.size_of_opaque_state;
		}
		grnd_allocator.len = num;
=======
	pthread_mutex_lock(&vgrnd.lock);
	if (!vgrnd.len) {
		size_t page_size = getpagesize();
		size_t new_cap;
		size_t alloc_size, num = sysconf(_SC_NPROCESSORS_ONLN); /* Just a decent heuristic. */
		size_t state_size_aligned, cache_line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE) ?: 1;
		void *new_block, *new_states;

		state_size_aligned = (vgrnd.params.size_of_opaque_state + cache_line_size - 1) & (~(cache_line_size - 1));
		alloc_size = (num * state_size_aligned + page_size - 1) & (~(page_size - 1));
		num = (page_size / state_size_aligned) * (alloc_size / page_size);
		new_block = mmap(0, alloc_size, vgrnd.params.mmap_prot, vgrnd.params.mmap_flags, -1, 0);
		if (new_block == MAP_FAILED)
			goto out;

		new_cap = vgrnd.cap + num;
		new_states = reallocarray(vgrnd.states, new_cap, sizeof(*vgrnd.states));
		if (!new_states)
			goto unmap;
		vgrnd.cap = new_cap;
		vgrnd.states = new_states;

		for (size_t i = 0; i < num; ++i) {
			if (((uintptr_t)new_block & (page_size - 1)) + vgrnd.params.size_of_opaque_state > page_size)
				new_block = (void *)(((uintptr_t)new_block + page_size - 1) & (~(page_size - 1)));
			vgrnd.states[i] = new_block;
			new_block += state_size_aligned;
		}
		vgrnd.len = num;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		goto success;

	unmap:
		munmap(new_block, alloc_size);
		goto out;
	}
success:
<<<<<<< HEAD
	state = grnd_allocator.states[--grnd_allocator.len];

out:
	pthread_mutex_unlock(&grnd_allocator.lock);
=======
	state = vgrnd.states[--vgrnd.len];

out:
	pthread_mutex_unlock(&vgrnd.lock);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	return state;
}

static void vgetrandom_put_state(void *state)
{
	if (!state)
		return;
<<<<<<< HEAD
	pthread_mutex_lock(&grnd_allocator.lock);
	grnd_allocator.states[grnd_allocator.len++] = state;
	pthread_mutex_unlock(&grnd_allocator.lock);
=======
	pthread_mutex_lock(&vgrnd.lock);
	vgrnd.states[vgrnd.len++] = state;
	pthread_mutex_unlock(&vgrnd.lock);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static void vgetrandom_init(void)
{
<<<<<<< HEAD
	if (pthread_key_create(&grnd_ctx.key, vgetrandom_put_state) != 0)
		return;
	unsigned long sysinfo_ehdr = getauxval(AT_SYSINFO_EHDR);
	if (!sysinfo_ehdr) {
		printf("AT_SYSINFO_EHDR is not present!\n");
		exit(KSFT_SKIP);
	}
	vdso_init_from_sysinfo_ehdr(sysinfo_ehdr);
	grnd_ctx.fn = (__typeof__(grnd_ctx.fn))vdso_sym("LINUX_2.6", "__vdso_getrandom");
	if (!grnd_ctx.fn) {
		printf("__vdso_getrandom is missing!\n");
		exit(KSFT_FAIL);
	}
	if (grnd_ctx.fn(NULL, 0, 0, &grnd_ctx.params, ~0UL) != 0) {
		printf("failed to fetch vgetrandom params!\n");
		exit(KSFT_FAIL);
	}
=======
	const char *version = versions[VDSO_VERSION];
	const char *name = names[VDSO_NAMES][6];
	unsigned long sysinfo_ehdr = getauxval(AT_SYSINFO_EHDR);
	ssize_t ret;

	if (!sysinfo_ehdr)
		ksft_exit_skip("AT_SYSINFO_EHDR is not present\n");
	vdso_init_from_sysinfo_ehdr(sysinfo_ehdr);
	vgrnd.fn = (__typeof__(vgrnd.fn))vdso_sym(version, name);
	if (!vgrnd.fn)
		ksft_exit_skip("%s@%s symbol is missing from vDSO\n", name, version);
	ret = VDSO_CALL(vgrnd.fn, 5, NULL, 0, 0, &vgrnd.params, ~0UL);
	if (ret == -ENOSYS)
		ksft_exit_skip("CPU does not have runtime support\n");
	else if (ret)
		ksft_exit_fail_msg("Failed to fetch vgetrandom params: %zd\n", ret);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static ssize_t vgetrandom(void *buf, size_t len, unsigned long flags)
{
<<<<<<< HEAD
	void *state;

	pthread_once(&grnd_ctx.initialized, vgetrandom_init);
	state = pthread_getspecific(grnd_ctx.key);
	if (!state) {
		state = vgetrandom_get_state();
		if (pthread_setspecific(grnd_ctx.key, state) != 0) {
			vgetrandom_put_state(state);
			state = NULL;
		}
		if (!state) {
			printf("vgetrandom_get_state failed!\n");
			exit(KSFT_FAIL);
		}
	}
	return grnd_ctx.fn(buf, len, flags, state, grnd_ctx.params.size_of_opaque_state);
=======
	static __thread void *state;

	if (!state) {
		state = vgetrandom_get_state();
		ksft_assert(state);
	}
	return VDSO_CALL(vgrnd.fn, 5, buf, len, flags, state, vgrnd.params.size_of_opaque_state);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

enum { TRIALS = 25000000, THREADS = 256 };

<<<<<<< HEAD
static void *test_vdso_getrandom(void *)
=======
static void *test_vdso_getrandom(void *ctx)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	for (size_t i = 0; i < TRIALS; ++i) {
		unsigned int val;
		ssize_t ret = vgetrandom(&val, sizeof(val), 0);
<<<<<<< HEAD
		assert(ret == sizeof(val));
=======
		ksft_assert(ret == sizeof(val));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return NULL;
}

<<<<<<< HEAD
static void *test_libc_getrandom(void *)
=======
static void *test_libc_getrandom(void *ctx)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	for (size_t i = 0; i < TRIALS; ++i) {
		unsigned int val;
		ssize_t ret = getrandom(&val, sizeof(val), 0);
<<<<<<< HEAD
		assert(ret == sizeof(val));
=======
		ksft_assert(ret == sizeof(val));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return NULL;
}

<<<<<<< HEAD
static void *test_syscall_getrandom(void *)
=======
static void *test_syscall_getrandom(void *ctx)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	for (size_t i = 0; i < TRIALS; ++i) {
		unsigned int val;
		ssize_t ret = syscall(__NR_getrandom, &val, sizeof(val), 0);
<<<<<<< HEAD
		assert(ret == sizeof(val));
=======
		ksft_assert(ret == sizeof(val));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}
	return NULL;
}

static void bench_single(void)
{
	struct timespec start, end, diff;

	clock_gettime(CLOCK_MONOTONIC, &start);
	test_vdso_getrandom(NULL);
	clock_gettime(CLOCK_MONOTONIC, &end);
	timespecsub(&end, &start, &diff);
	printf("   vdso: %u times in %lu.%09lu seconds\n", TRIALS, diff.tv_sec, diff.tv_nsec);

	clock_gettime(CLOCK_MONOTONIC, &start);
	test_libc_getrandom(NULL);
	clock_gettime(CLOCK_MONOTONIC, &end);
	timespecsub(&end, &start, &diff);
	printf("   libc: %u times in %lu.%09lu seconds\n", TRIALS, diff.tv_sec, diff.tv_nsec);

	clock_gettime(CLOCK_MONOTONIC, &start);
	test_syscall_getrandom(NULL);
	clock_gettime(CLOCK_MONOTONIC, &end);
	timespecsub(&end, &start, &diff);
	printf("syscall: %u times in %lu.%09lu seconds\n", TRIALS, diff.tv_sec, diff.tv_nsec);
}

static void bench_multi(void)
{
	struct timespec start, end, diff;
	pthread_t threads[THREADS];

	clock_gettime(CLOCK_MONOTONIC, &start);
	for (size_t i = 0; i < THREADS; ++i)
<<<<<<< HEAD
		assert(pthread_create(&threads[i], NULL, test_vdso_getrandom, NULL) == 0);
=======
		ksft_assert(pthread_create(&threads[i], NULL, test_vdso_getrandom, NULL) == 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	for (size_t i = 0; i < THREADS; ++i)
		pthread_join(threads[i], NULL);
	clock_gettime(CLOCK_MONOTONIC, &end);
	timespecsub(&end, &start, &diff);
	printf("   vdso: %u x %u times in %lu.%09lu seconds\n", TRIALS, THREADS, diff.tv_sec, diff.tv_nsec);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for (size_t i = 0; i < THREADS; ++i)
<<<<<<< HEAD
		assert(pthread_create(&threads[i], NULL, test_libc_getrandom, NULL) == 0);
=======
		ksft_assert(pthread_create(&threads[i], NULL, test_libc_getrandom, NULL) == 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	for (size_t i = 0; i < THREADS; ++i)
		pthread_join(threads[i], NULL);
	clock_gettime(CLOCK_MONOTONIC, &end);
	timespecsub(&end, &start, &diff);
	printf("   libc: %u x %u times in %lu.%09lu seconds\n", TRIALS, THREADS, diff.tv_sec, diff.tv_nsec);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for (size_t i = 0; i < THREADS; ++i)
<<<<<<< HEAD
		assert(pthread_create(&threads[i], NULL, test_syscall_getrandom, NULL) == 0);
=======
		ksft_assert(pthread_create(&threads[i], NULL, test_syscall_getrandom, NULL) == 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	for (size_t i = 0; i < THREADS; ++i)
		pthread_join(threads[i], NULL);
	clock_gettime(CLOCK_MONOTONIC, &end);
	timespecsub(&end, &start, &diff);
	printf("   syscall: %u x %u times in %lu.%09lu seconds\n", TRIALS, THREADS, diff.tv_sec, diff.tv_nsec);
}

static void fill(void)
{
	uint8_t weird_size[323929];
	for (;;)
		vgetrandom(weird_size, sizeof(weird_size), 0);
}

static void kselftest(void)
{
	uint8_t weird_size[1263];
<<<<<<< HEAD

	ksft_print_header();
	ksft_set_plan(1);

	for (size_t i = 0; i < 1000; ++i) {
		ssize_t ret = vgetrandom(weird_size, sizeof(weird_size), 0);
		if (ret != sizeof(weird_size))
			exit(KSFT_FAIL);
	}

	ksft_test_result_pass("getrandom: PASS\n");
	exit(KSFT_PASS);
=======
	pid_t child;

	ksft_print_header();
	ksft_set_plan(2);

	for (size_t i = 0; i < 1000; ++i) {
		ssize_t ret = vgetrandom(weird_size, sizeof(weird_size), 0);
		ksft_assert(ret == sizeof(weird_size));
	}

	ksft_test_result_pass("getrandom: PASS\n");

	unshare(CLONE_NEWUSER);
	ksft_assert(unshare(CLONE_NEWTIME) == 0);
	child = fork();
	ksft_assert(child >= 0);
	if (!child) {
		vgetrandom_init();
		child = getpid();
		ksft_assert(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == 0);
		ksft_assert(kill(child, SIGSTOP) == 0);
		ksft_assert(vgetrandom(weird_size, sizeof(weird_size), 0) == sizeof(weird_size));
		_exit(0);
	}
	for (;;) {
		struct ptrace_syscall_info info = { 0 };
		int status, ret;
		ksft_assert(waitpid(child, &status, 0) >= 0);
		if (WIFEXITED(status)) {
			ksft_assert(WEXITSTATUS(status) == 0);
			break;
		}
		ksft_assert(WIFSTOPPED(status));
		if (WSTOPSIG(status) == SIGSTOP)
			ksft_assert(ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD) == 0);
		else if (WSTOPSIG(status) == (SIGTRAP | 0x80)) {
			ksft_assert(ptrace(PTRACE_GET_SYSCALL_INFO, child, sizeof(info), &info) > 0);
			if (info.op == PTRACE_SYSCALL_INFO_ENTRY && info.entry.nr == __NR_getrandom &&
			    info.entry.args[0] == (uintptr_t)weird_size && info.entry.args[1] == sizeof(weird_size))
				ksft_exit_fail_msg("vgetrandom passed buffer to syscall getrandom unexpectedly\n");
		}
		ksft_assert(ptrace(PTRACE_SYSCALL, child, 0, 0) == 0);
	}

	ksft_test_result_pass("getrandom timens: PASS\n");

	ksft_exit_pass();
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static void usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s [bench-single|bench-multi|fill]\n", argv0);
}

int main(int argc, char *argv[])
{
<<<<<<< HEAD
=======
	vgetrandom_init();

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (argc == 1) {
		kselftest();
		return 0;
	}

	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}
	if (!strcmp(argv[1], "bench-single"))
		bench_single();
	else if (!strcmp(argv[1], "bench-multi"))
		bench_multi();
	else if (!strcmp(argv[1], "fill"))
		fill();
	else {
		usage(argv[0]);
		return 1;
	}
	return 0;
}
