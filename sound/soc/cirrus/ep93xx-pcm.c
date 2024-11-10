// SPDX-License-Identifier: GPL-2.0-only
/*
 * linux/sound/arm/ep93xx-pcm.c - EP93xx ALSA PCM interface
 *
 * Copyright (C) 2006 Lennert Buytenhek <buytenh@wantstofly.org>
 * Copyright (C) 2006 Applied Data Systems
 *
 * Rewritten for the SoC audio subsystem (Based on PXA2xx code):
 *   Copyright (c) 2008 Ryan Mallon
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dmaengine.h>

#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/dmaengine_pcm.h>

<<<<<<< HEAD
#include <linux/platform_data/dma-ep93xx.h>

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#include "ep93xx-pcm.h"

static const struct snd_pcm_hardware ep93xx_pcm_hardware = {
	.info			= (SNDRV_PCM_INFO_MMAP		|
				   SNDRV_PCM_INFO_MMAP_VALID	|
				   SNDRV_PCM_INFO_INTERLEAVED	|
				   SNDRV_PCM_INFO_BLOCK_TRANSFER),
	.buffer_bytes_max	= 131072,
	.period_bytes_min	= 32,
	.period_bytes_max	= 32768,
	.periods_min		= 1,
	.periods_max		= 32,
	.fifo_size		= 32,
};

<<<<<<< HEAD
static bool ep93xx_pcm_dma_filter(struct dma_chan *chan, void *filter_param)
{
	struct ep93xx_dma_data *data = filter_param;

	if (data->direction == ep93xx_dma_chan_direction(chan)) {
		chan->private = data;
		return true;
	}

	return false;
}

static const struct snd_dmaengine_pcm_config ep93xx_dmaengine_pcm_config = {
	.pcm_hardware = &ep93xx_pcm_hardware,
	.compat_filter_fn = ep93xx_pcm_dma_filter,
=======
static const struct snd_dmaengine_pcm_config ep93xx_dmaengine_pcm_config = {
	.pcm_hardware = &ep93xx_pcm_hardware,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	.prealloc_buffer_size = 131072,
};

int devm_ep93xx_pcm_platform_register(struct device *dev)
{
	return devm_snd_dmaengine_pcm_register(dev,
<<<<<<< HEAD
		&ep93xx_dmaengine_pcm_config,
		SND_DMAENGINE_PCM_FLAG_NO_DT |
		SND_DMAENGINE_PCM_FLAG_COMPAT);
=======
		&ep93xx_dmaengine_pcm_config, 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}
EXPORT_SYMBOL_GPL(devm_ep93xx_pcm_platform_register);

MODULE_AUTHOR("Ryan Mallon");
MODULE_DESCRIPTION("EP93xx ALSA PCM interface");
MODULE_LICENSE("GPL");
