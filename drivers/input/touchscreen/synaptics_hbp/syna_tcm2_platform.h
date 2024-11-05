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

#ifndef _SYNAPTICS_TCM2_PLATFORM_H_
#define _SYNAPTICS_TCM2_PLATFORM_H_
#include "syna_tcm2_runtime.h"
#include <linux/err.h>
#define RD_CHUNK_SIZE (0)
#define WR_CHUNK_SIZE (1024)
enum power_supply {
	PSU_REGULATOR = 0,
	PSU_GPIO,
};

struct syna_hw_bus_data {
	unsigned char type;
	unsigned int rd_chunk_size;
	unsigned int wr_chunk_size;
	unsigned int frequency_hz;
	unsigned int i2c_addr;
	unsigned int spi_mode;
	unsigned int spi_byte_delay_us;
	unsigned int spi_block_delay_us;
	syna_pal_mutex_t io_mutex;
	int switch_gpio;
	int switch_state;
	struct pinctrl          *pinctrl;
	struct pinctrl_state    *pin_spi_mode_active;
	struct pinctrl_state    *pin_spi_mode_suspend;
	struct pinctrl_state    *pin_cs_high;
	struct pinctrl_state    *pin_cs_low;
};

struct syna_hw_attn_data {
	int irq_gpio;
	int irq_on_state;
	unsigned long irq_flags;
	int irq_id;
	bool irq_enabled;
	syna_pal_mutex_t irq_en_mutex;
};

struct syna_hw_rst_data {
	int reset_gpio;
	int reset_on_state;
	unsigned int reset_delay_ms;
	unsigned int reset_active_ms;
	syna_pal_mutex_t reset_en_mutex;
};

struct syna_hw_pwr_data {
	int psu;
	int vdd_gpio;
	int avdd_gpio;
	int power_on_state;
	unsigned int power_on_delay_ms;
	unsigned int vdd;
	unsigned int vled;
	unsigned int vio;
	unsigned int vddtx;
	const char *vdd_reg_name;
	void *vdd_reg_dev;
	const char *avdd_reg_name;
	void *avdd_reg_dev;
};

struct syna_hw_interface {
	void *pdev;
	struct syna_hw_bus_data bdata_io;
	struct syna_hw_attn_data bdata_attn;
	struct syna_hw_rst_data bdata_rst;
	struct syna_hw_pwr_data bdata_pwr;
	int (*ops_power_on)(struct syna_hw_interface *hw_if, bool en);
	void (*ops_hw_reset)(struct syna_hw_interface *hw_if);
	int (*ops_bus_setup)(struct syna_hw_interface *hw_if, struct syna_hw_bus_data *config);
	int (*ops_read_data)(struct syna_hw_interface *hw_if, unsigned char *rd_data, unsigned int rd_len);
	int (*ops_write_data)(struct syna_hw_interface *hw_if, unsigned char *wr_data, unsigned int wr_len);
	int (*ops_enable_irq)(struct syna_hw_interface *hw_if, bool en);
	int (*ops_wait_irq)(struct syna_hw_interface *hw_if, unsigned int timeout_ms);

};

int syna_hw_interface_init(void);
void syna_hw_interface_exit(void);


#endif /* end of _SYNAPTICS_TCM2_PLATFORM_H_ */
