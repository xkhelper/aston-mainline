/* SPDX-License-Identifier: GPL-2.0
 *
 * Synaptics TouchCom touchscreen driver
 *
 * Copyright (C) 2017-2020 Synaptics Incorporated. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND SYNAPTICS
 * EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
 * AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHTS.
 * IN NO EVENT SHALL SYNAPTICS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED
 * AND BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF COMPETENT JURISDICTION DOES
 * NOT PERMIT THE DISCLAIMER OF DIRECT DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS'
 * TOTAL CUMULATIVE LIABILITY TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S.
 * DOLLARS.
 */
 
#ifndef _SYNAPTICS_TCM2_C_RUNTIME_H_
#define _SYNAPTICS_TCM2_C_RUNTIME_H_

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/regulator/consumer.h>
#include <linux/platform_device.h>
#include <linux/input/mt.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/crc32.h>
#include <linux/firmware.h>
#ifdef CONFIG_DRM_PANEL
#include <drm/drm_panel.h>
#elif CONFIG_FB
#include <linux/fb.h>
#include <linux/notifier.h>
#endif
#include <linux/fs.h>
#include <linux/moduleparam.h>
 
#define LOGD(log, ...) \
	pr_debug("[  debug] %s: " log, __func__, ##__VA_ARGS__)
#define LOGI(log, ...) \
	pr_info("[   info] %s: " log, __func__, ##__VA_ARGS__)
#define LOGN(log, ...) \
	pr_notice("[   info] %s: " log, __func__, ##__VA_ARGS__)
#define LOGW(log, ...) \
	pr_warn("[warning] %s: " log, __func__, ##__VA_ARGS__)
#define LOGE(log, ...) \
	pr_err("[  error] %s: " log, __func__, ##__VA_ARGS__)


#define _EIO        (-EIO)
#define _ENOMEM     (-ENOMEM)
#define _EINVAL     (-EINVAL)
#define _ENODEV     (-ENODEV)
#define _ETIMEDOUT  (-ETIMEDOUT)

#define GET_BIT(var, pos) \
	(((var) & (1 << (pos))) >> (pos))


typedef atomic_t syna_pal_atomic_t;

#define ATOMIC_SET(atomic, value) \
	atomic_set(&atomic, value)

#define ATOMIC_GET(atomic) \
	atomic_read(&atomic)

static inline unsigned int syna_pal_le2_to_uint(const unsigned char *src)
{
	return (unsigned int)src[0] +
		(unsigned int)src[1] * 0x100;
}

static inline unsigned int syna_pal_le4_to_uint(const unsigned char *src)
{
	return (unsigned int)src[0] +
		(unsigned int)src[1] * 0x100 +
		(unsigned int)src[2] * 0x10000 +
		(unsigned int)src[3] * 0x1000000;
}

static inline unsigned int syna_pal_ceil_div(unsigned int dividend,
		unsigned int divisor)
{
	return (dividend + divisor - 1) / divisor;
}

static inline void *syna_pal_mem_alloc(unsigned int num, unsigned int size)
{
	if ((int)(num * size) <= 0) {
		LOGE("Invalid parameter\n");
		return NULL;
	}

	return kcalloc(num, size, GFP_KERNEL);
}

static inline void syna_pal_mem_free(void *ptr)
{
	if (ptr)
		kfree(ptr);
}

static inline void syna_pal_mem_set(void *ptr, int c, unsigned int n)
{
	memset(ptr, c, n);
}

static inline int syna_pal_mem_cpy(void *dest, unsigned int dest_size,
		const void *src, unsigned int src_size, unsigned int num)
{
	if (dest == NULL || src == NULL)
		return -1;

	if (num > dest_size || num > src_size) {
		LOGE("Invalid size. src:%d, dest:%d, num:%d\n",
			src_size, dest_size, num);
		return -1;
	}

	memcpy((void *)dest, (const void *)src, num);

	return 0;
}

typedef struct mutex syna_pal_mutex_t;

static inline int syna_pal_mutex_alloc(syna_pal_mutex_t *ptr)
{
	mutex_init((struct mutex *)ptr);
	return 0;
}

static inline void syna_pal_mutex_free(syna_pal_mutex_t *ptr) { }

static inline int syna_pal_mutex_trylock(syna_pal_mutex_t *ptr)
{
	return mutex_trylock((struct mutex *)ptr);
}

static inline void syna_pal_mutex_lock(syna_pal_mutex_t *ptr)
{
	mutex_lock((struct mutex *)ptr);
}

static inline void syna_pal_mutex_unlock(syna_pal_mutex_t *ptr)
{
	mutex_unlock((struct mutex *)ptr);
}

typedef struct completion syna_pal_completion_t;

static inline int syna_pal_completion_alloc(syna_pal_completion_t *ptr)
{
	init_completion((struct completion *)ptr);
	return 0;
}

static inline void syna_pal_completion_free(syna_pal_completion_t *ptr) { }

static inline void syna_pal_completion_complete(syna_pal_completion_t *ptr)
{
	complete((struct completion *)ptr);
}

static inline void syna_pal_completion_reset(syna_pal_completion_t *ptr)
{
	reinit_completion((struct completion *)ptr);
}

static inline int syna_pal_completion_wait_for(syna_pal_completion_t *ptr,
		unsigned int timeout_ms)
{
	int retval;

	retval = wait_for_completion_timeout((struct completion *)ptr,
			msecs_to_jiffies(timeout_ms));
	if (retval == 0)
		return -1;

	return 0;
}

static inline void syna_pal_sleep_ms(int time_ms)
{
	msleep(time_ms);
}

static inline void syna_pal_sleep_us(int time_us_min, int time_us_max)
{
	usleep_range(time_us_min, time_us_max);
}

static inline void syna_pal_busy_delay_ms(int time_ms)
{
	mdelay(time_ms);
}

static inline unsigned int syna_pal_str_len(const char *str)
{
	return (unsigned int)strlen(str);
}

static inline int syna_pal_str_cpy(char *dest, unsigned int dest_size,
		const char *src, unsigned int src_size, unsigned int num)
{
	if (dest == NULL || src == NULL)
		return -1;

	if (num > dest_size || num > src_size) {
		LOGE("Invalid size. src:%d, dest:%d, num:%d\n",
			src_size, dest_size, num);
		return -1;
	}

	strncpy(dest, src, num);

	return 0;
}

static inline int syna_pal_str_cmp(const char *str1, const char *str2,
		unsigned int num)
{
	return strncmp(str1, str2, num);
}

static inline unsigned int syna_pal_hex_to_uint(char *str, int length)
{
	unsigned int result = 0;
	char *ptr = NULL;

	for (ptr = str; ptr != str + length; ++ptr) {
		result <<= 4;
		if (*ptr >= 'A')
			result += *ptr - 'A' + 10;
		else
			result += *ptr - '0';
	}

	return result;
}

static inline unsigned int syna_pal_crc32(unsigned int seed,
		const char *data, unsigned int len)
{
	return crc32(seed, data, len);
}

#endif /* end of _SYNAPTICS_TCM2_C_RUNTIME_H_ */
