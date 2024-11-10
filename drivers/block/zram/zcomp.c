// SPDX-License-Identifier: GPL-2.0-or-later
<<<<<<< HEAD
/*
 * Copyright (C) 2014 Sergey Senozhatsky.
 */
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/cpu.h>
#include <linux/crypto.h>
#include <linux/vmalloc.h>

#include "zcomp.h"

<<<<<<< HEAD
static const char * const backends[] = {
#if IS_ENABLED(CONFIG_CRYPTO_LZO)
	"lzo",
	"lzo-rle",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_LZ4)
	"lz4",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_LZ4HC)
	"lz4hc",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_842)
	"842",
#endif
#if IS_ENABLED(CONFIG_CRYPTO_ZSTD)
	"zstd",
#endif
};

static void zcomp_strm_free(struct zcomp_strm *zstrm)
{
	if (!IS_ERR_OR_NULL(zstrm->tfm))
		crypto_free_comp(zstrm->tfm);
	vfree(zstrm->buffer);
	zstrm->tfm = NULL;
	zstrm->buffer = NULL;
}

/*
 * Initialize zcomp_strm structure with ->tfm initialized by backend, and
 * ->buffer. Return a negative value on error.
 */
static int zcomp_strm_init(struct zcomp_strm *zstrm, struct zcomp *comp)
{
	zstrm->tfm = crypto_alloc_comp(comp->name, 0, 0);
=======
#include "backend_lzo.h"
#include "backend_lzorle.h"
#include "backend_lz4.h"
#include "backend_lz4hc.h"
#include "backend_zstd.h"
#include "backend_deflate.h"
#include "backend_842.h"

static const struct zcomp_ops *backends[] = {
#if IS_ENABLED(CONFIG_ZRAM_BACKEND_LZO)
	&backend_lzorle,
	&backend_lzo,
#endif
#if IS_ENABLED(CONFIG_ZRAM_BACKEND_LZ4)
	&backend_lz4,
#endif
#if IS_ENABLED(CONFIG_ZRAM_BACKEND_LZ4HC)
	&backend_lz4hc,
#endif
#if IS_ENABLED(CONFIG_ZRAM_BACKEND_ZSTD)
	&backend_zstd,
#endif
#if IS_ENABLED(CONFIG_ZRAM_BACKEND_DEFLATE)
	&backend_deflate,
#endif
#if IS_ENABLED(CONFIG_ZRAM_BACKEND_842)
	&backend_842,
#endif
	NULL
};

static void zcomp_strm_free(struct zcomp *comp, struct zcomp_strm *zstrm)
{
	comp->ops->destroy_ctx(&zstrm->ctx);
	vfree(zstrm->buffer);
	zstrm->buffer = NULL;
}

static int zcomp_strm_init(struct zcomp *comp, struct zcomp_strm *zstrm)
{
	int ret;

	ret = comp->ops->create_ctx(comp->params, &zstrm->ctx);
	if (ret)
		return ret;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	/*
	 * allocate 2 pages. 1 for compressed data, plus 1 extra for the
	 * case when compressed size is larger than the original one
	 */
	zstrm->buffer = vzalloc(2 * PAGE_SIZE);
<<<<<<< HEAD
	if (IS_ERR_OR_NULL(zstrm->tfm) || !zstrm->buffer) {
		zcomp_strm_free(zstrm);
=======
	if (!zstrm->buffer) {
		zcomp_strm_free(comp, zstrm);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		return -ENOMEM;
	}
	return 0;
}

<<<<<<< HEAD
bool zcomp_available_algorithm(const char *comp)
{
	/*
	 * Crypto does not ignore a trailing new line symbol,
	 * so make sure you don't supply a string containing
	 * one.
	 * This also means that we permit zcomp initialisation
	 * with any compressing algorithm known to crypto api.
	 */
	return crypto_has_comp(comp, 0, 0) == 1;
=======
static const struct zcomp_ops *lookup_backend_ops(const char *comp)
{
	int i = 0;

	while (backends[i]) {
		if (sysfs_streq(comp, backends[i]->name))
			break;
		i++;
	}
	return backends[i];
}

bool zcomp_available_algorithm(const char *comp)
{
	return lookup_backend_ops(comp) != NULL;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

/* show available compressors */
ssize_t zcomp_available_show(const char *comp, char *buf)
{
<<<<<<< HEAD
	bool known_algorithm = false;
	ssize_t sz = 0;
	int i;

	for (i = 0; i < ARRAY_SIZE(backends); i++) {
		if (!strcmp(comp, backends[i])) {
			known_algorithm = true;
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"[%s] ", backends[i]);
		} else {
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"%s ", backends[i]);
		}
	}

	/*
	 * Out-of-tree module known to crypto api or a missing
	 * entry in `backends'.
	 */
	if (!known_algorithm && crypto_has_comp(comp, 0, 0) == 1)
		sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
				"[%s] ", comp);

=======
	ssize_t sz = 0;
	int i;

	for (i = 0; i < ARRAY_SIZE(backends) - 1; i++) {
		if (!strcmp(comp, backends[i]->name)) {
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"[%s] ", backends[i]->name);
		} else {
			sz += scnprintf(buf + sz, PAGE_SIZE - sz - 2,
					"%s ", backends[i]->name);
		}
	}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	sz += scnprintf(buf + sz, PAGE_SIZE - sz, "\n");
	return sz;
}

struct zcomp_strm *zcomp_stream_get(struct zcomp *comp)
{
	local_lock(&comp->stream->lock);
	return this_cpu_ptr(comp->stream);
}

void zcomp_stream_put(struct zcomp *comp)
{
	local_unlock(&comp->stream->lock);
}

<<<<<<< HEAD
int zcomp_compress(struct zcomp_strm *zstrm,
		const void *src, unsigned int *dst_len)
{
	/*
	 * Our dst memory (zstrm->buffer) is always `2 * PAGE_SIZE' sized
	 * because sometimes we can endup having a bigger compressed data
	 * due to various reasons: for example compression algorithms tend
	 * to add some padding to the compressed buffer. Speaking of padding,
	 * comp algorithm `842' pads the compressed length to multiple of 8
	 * and returns -ENOSP when the dst memory is not big enough, which
	 * is not something that ZRAM wants to see. We can handle the
	 * `compressed_size > PAGE_SIZE' case easily in ZRAM, but when we
	 * receive -ERRNO from the compressing backend we can't help it
	 * anymore. To make `842' happy we need to tell the exact size of
	 * the dst buffer, zram_drv will take care of the fact that
	 * compressed buffer is too big.
	 */
	*dst_len = PAGE_SIZE * 2;

	return crypto_comp_compress(zstrm->tfm,
			src, PAGE_SIZE,
			zstrm->buffer, dst_len);
}

int zcomp_decompress(struct zcomp_strm *zstrm,
		const void *src, unsigned int src_len, void *dst)
{
	unsigned int dst_len = PAGE_SIZE;

	return crypto_comp_decompress(zstrm->tfm,
			src, src_len,
			dst, &dst_len);
=======
int zcomp_compress(struct zcomp *comp, struct zcomp_strm *zstrm,
		   const void *src, unsigned int *dst_len)
{
	struct zcomp_req req = {
		.src = src,
		.dst = zstrm->buffer,
		.src_len = PAGE_SIZE,
		.dst_len = 2 * PAGE_SIZE,
	};
	int ret;

	ret = comp->ops->compress(comp->params, &zstrm->ctx, &req);
	if (!ret)
		*dst_len = req.dst_len;
	return ret;
}

int zcomp_decompress(struct zcomp *comp, struct zcomp_strm *zstrm,
		     const void *src, unsigned int src_len, void *dst)
{
	struct zcomp_req req = {
		.src = src,
		.dst = dst,
		.src_len = src_len,
		.dst_len = PAGE_SIZE,
	};

	return comp->ops->decompress(comp->params, &zstrm->ctx, &req);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

int zcomp_cpu_up_prepare(unsigned int cpu, struct hlist_node *node)
{
	struct zcomp *comp = hlist_entry(node, struct zcomp, node);
	struct zcomp_strm *zstrm;
	int ret;

	zstrm = per_cpu_ptr(comp->stream, cpu);
	local_lock_init(&zstrm->lock);

<<<<<<< HEAD
	ret = zcomp_strm_init(zstrm, comp);
=======
	ret = zcomp_strm_init(comp, zstrm);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		pr_err("Can't allocate a compression stream\n");
	return ret;
}

int zcomp_cpu_dead(unsigned int cpu, struct hlist_node *node)
{
	struct zcomp *comp = hlist_entry(node, struct zcomp, node);
	struct zcomp_strm *zstrm;

	zstrm = per_cpu_ptr(comp->stream, cpu);
<<<<<<< HEAD
	zcomp_strm_free(zstrm);
	return 0;
}

static int zcomp_init(struct zcomp *comp)
=======
	zcomp_strm_free(comp, zstrm);
	return 0;
}

static int zcomp_init(struct zcomp *comp, struct zcomp_params *params)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	int ret;

	comp->stream = alloc_percpu(struct zcomp_strm);
	if (!comp->stream)
		return -ENOMEM;

<<<<<<< HEAD
	ret = cpuhp_state_add_instance(CPUHP_ZCOMP_PREPARE, &comp->node);
	if (ret < 0)
		goto cleanup;
	return 0;

cleanup:
=======
	comp->params = params;
	ret = comp->ops->setup_params(comp->params);
	if (ret)
		goto cleanup;

	ret = cpuhp_state_add_instance(CPUHP_ZCOMP_PREPARE, &comp->node);
	if (ret < 0)
		goto cleanup;

	return 0;

cleanup:
	comp->ops->release_params(comp->params);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	free_percpu(comp->stream);
	return ret;
}

void zcomp_destroy(struct zcomp *comp)
{
	cpuhp_state_remove_instance(CPUHP_ZCOMP_PREPARE, &comp->node);
<<<<<<< HEAD
=======
	comp->ops->release_params(comp->params);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	free_percpu(comp->stream);
	kfree(comp);
}

<<<<<<< HEAD
/*
 * search available compressors for requested algorithm.
 * allocate new zcomp and initialize it. return compressing
 * backend pointer or ERR_PTR if things went bad. ERR_PTR(-EINVAL)
 * if requested algorithm is not supported, ERR_PTR(-ENOMEM) in
 * case of allocation error, or any other error potentially
 * returned by zcomp_init().
 */
struct zcomp *zcomp_create(const char *alg)
=======
struct zcomp *zcomp_create(const char *alg, struct zcomp_params *params)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	struct zcomp *comp;
	int error;

	/*
<<<<<<< HEAD
	 * Crypto API will execute /sbin/modprobe if the compression module
	 * is not loaded yet. We must do it here, otherwise we are about to
	 * call /sbin/modprobe under CPU hot-plug lock.
	 */
	if (!zcomp_available_algorithm(alg))
		return ERR_PTR(-EINVAL);
=======
	 * The backends array has a sentinel NULL value, so the minimum
	 * size is 1. In order to be valid the array, apart from the
	 * sentinel NULL element, should have at least one compression
	 * backend selected.
	 */
	BUILD_BUG_ON(ARRAY_SIZE(backends) <= 1);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	comp = kzalloc(sizeof(struct zcomp), GFP_KERNEL);
	if (!comp)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	comp->name = alg;
	error = zcomp_init(comp);
=======
	comp->ops = lookup_backend_ops(alg);
	if (!comp->ops) {
		kfree(comp);
		return ERR_PTR(-EINVAL);
	}

	error = zcomp_init(comp, params);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (error) {
		kfree(comp);
		return ERR_PTR(error);
	}
	return comp;
}
