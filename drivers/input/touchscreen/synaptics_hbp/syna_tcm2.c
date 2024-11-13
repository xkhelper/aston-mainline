// SPDX-License-Identifier: GPL-2.0
/*
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

#include "syna_tcm2.h"
#include "syna_tcm2_platform.h"
#include "tcm/synaptics_touchcom_core_dev.h"
#include "tcm/synaptics_touchcom_func_base.h"
#include "tcm/synaptics_touchcom_func_touch.h"
#include "tcm/synaptics_touchcom_func_reflash.h"
#include "../touchpanel_notify/touchpanel_event_notify.h"
#include <linux/sched/signal.h>
#include <linux/msm_drm_notify.h>
#include <linux/wait.h>

static DECLARE_WAIT_QUEUE_HEAD(state_waiter);

extern struct platform_device *syna_spi_device;

#define STARTUP_REFLASH_DELAY_TIME_MS (200)
#define RESET_ON_RESUME_DELAY_MS (100)

#define POWER_ALIVE_AT_SUSPEND
static void syna_delta_read(struct seq_file *s, void *chip_data);
static void syna_baseline_read(struct seq_file *s, void *chip_data);
static void syna_main_register(struct seq_file *s, void *chip_data);
static void syna_reserve_read(struct seq_file *s, void *chip_data);
static void syna_tcm_test_report(struct syna_tcm *tcm_info, u32 code);

void syna_dev_update_lpwg_status(struct syna_tcm *tcm)
{
	tcm->lpwg_enabled = (tcm->gesture_type || tcm->touch_and_hold || tcm->fp_active) ? true : false;
	return;
}

int syna_dev_enable_lowpwr_gesture(struct syna_tcm *tcm, bool en)
{
	int retval = 0;
	int retry = GESTURE_MODE_SWITCH_RETRY_TIMES;
	char *report = NULL;
	unsigned short config = 0;
	struct syna_hw_attn_data *attn = &tcm->hw_if->bdata_attn;

	if (!tcm->lpwg_enabled || attn->irq_id == 0)
		return 0;

	if (en) {
		if (!tcm->irq_wake) {
			enable_irq_wake(attn->irq_id);
			tcm->irq_wake = true;
		}
		retval = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_ENABLE_WAKEUP_GESTURE_MODE, 1, RESP_IN_ATTN);
		if (retval < 0) {
			LOGE("Fail to enable wakeup gesture via DC command\n");
			return retval;
		}
	} else {
		if (tcm->irq_wake) {
			disable_irq_wake(attn->irq_id);
			tcm->irq_wake = false;
		}
		retval = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_ENABLE_WAKEUP_GESTURE_MODE, 0, RESP_IN_ATTN);
		if (retval < 0) {
			LOGE("Fail to disable wakeup gesture via DC command\n");
			return retval;
		}
	}
	retval = syna_tcm_get_dynamic_config(tcm->tcm_dev, DC_ENABLE_WAKEUP_GESTURE_MODE, &config, 0);
	if (retval < 0) {
		LOGE("fail to read back gesture mode\n");
		return retval;
	}
	LOGI("read back gesture mode is %d\n", config);
	while (config != !!en && retry > 0) {
		retry--;
		LOGE("Detected: Failed to %s gesture mode, retry %d\n", en ? "enter" : "exit", GESTURE_MODE_SWITCH_RETRY_TIMES - retry);
		retval = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_ENABLE_WAKEUP_GESTURE_MODE, !!en, RESP_IN_ATTN);
		if (retval < 0) {
			LOGE("fail to re-write gesture mode\n");
			return retval;
		} else {
			LOGI("re-write gesture mode to %d\n", !!en);
		}
		retval = syna_tcm_get_dynamic_config(tcm->tcm_dev, DC_ENABLE_WAKEUP_GESTURE_MODE, &config, 0);
		if (retval < 0) {
			LOGE("fail to read back gesture mode\n");
			return retval;
		}
		LOGI("read back gesture mode is %d\n", config);
	}
	LOGI("set wakeup gesture(0x09) mode to %d\n", en);
	if (retry < GESTURE_MODE_SWITCH_RETRY_TIMES) {
		report = devm_kzalloc(&tcm->pdev->dev, MAX_HEALTH_REPORT_LEN, GFP_KERNEL);
		if (report) {
			snprintf(report, MAX_HEALTH_REPORT_LEN, "gesture_mode_%s_retry_%d_times", en ? "enter" : "exit", GESTURE_MODE_SWITCH_RETRY_TIMES - retry);
			devm_kfree(&tcm->pdev->dev, report);
		}
		if (config != !!en && retry == 0) {
			LOGE("Detected: Failed to %s gesture mode over retry times!!\n", en ? "enter" : "exit");
		}
	}
	return retval;
}

static int syna_dev_set_gesture_type(struct syna_tcm *tcm, unsigned short value)
{
	int retval = 0;
	retval = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_GESTURE_TYPE_ENABLE, value, RESP_IN_ATTN);
	if (retval < 0) {
		LOGE("Fail to set gesture type\n");
	}
	return retval;
}


static int syna_dev_set_fingerprint_enable(struct syna_tcm *tcm, unsigned short value)
{
	int retval = 0;
	retval = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_TOUCH_AND_HOLD, value, RESP_IN_ATTN);
	if (retval < 0) {
		LOGE("Fail to set gesture type\n");
	}
	return retval;
}

int syna_dev_disable_hbp_mode(struct syna_tcm *tcm)
{
	int retval = 0;
	retval = syna_tcm_enable_report(tcm->tcm_dev, REPORT_HBP_ACTIVE_FRAME, false);
	if (retval < 0) {
		LOGE("Fail to disalbe HBP Active Frame report\n");
		return retval;
	}
	retval = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_CONTROL_LBP_HBP, 0x02, RESP_IN_ATTN);
	if (retval < 0) {
		LOGE("Fail to disable HBP mode via DC command\n");
		return retval;
	}
	LOGI("Disable all Report and response to report_to_queue\n");
	syna_pal_mem_set(tcm->report_to_queue, EFP_DISABLE, REPORT_TYPES);
	tcm->hbp_enabled = false;
	return retval;
}

static int syna_dev_disable_lbp_mode(struct syna_tcm *tcm)
{
	int retval = 0;
	retval = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_CONTROL_LBP_HBP, 0x01, RESP_IN_ATTN);
	if (retval < 0) {
		LOGE("Fail to disable LBP mode via DC command\n");
	}
	return retval;
}

static void syna_dev_free_input_events(struct syna_tcm *tcm)
{
	struct input_dev *input_dev = tcm->input_dev;
	unsigned int idx;
	if (input_dev == NULL)
		return;
	syna_pal_mutex_lock(&tcm->tp_event_mutex);
	for (idx = 0; idx < MAX_NUM_OBJECTS; idx++) {
		input_mt_slot(input_dev, idx);
		input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 0);
	}
	input_report_key(input_dev, BTN_TOUCH, 0);
	input_report_key(input_dev, BTN_TOOL_FINGER, 0);
	input_sync(input_dev);
	syna_pal_mutex_unlock(&tcm->tp_event_mutex);

}

static void syna_dev_report_input_events(struct syna_tcm *tcm)
{
	unsigned int idx;
	unsigned int x;
	unsigned int y;
	int wx;
	int wy;
	unsigned int status;
	unsigned int touch_count;
	struct input_dev *input_dev = tcm->input_dev;
	unsigned int max_objects = tcm->tcm_dev->max_objects;
	struct tcm_touch_data_blob *touch_data;
	struct tcm_objects_data_blob *object_data;
	if (input_dev == NULL)
		return;
	syna_pal_mutex_lock(&tcm->tp_event_mutex);
	touch_data = &tcm->tp_data;
	object_data = &tcm->tp_data.object_data[0];

#ifdef ENABLE_WAKEUP_GESTURE
	if (tcm->pwr_state == LOW_PWR) {
		if (touch_data->gesture_id) {
			LOGD("Gesture detected, id:%d\n", touch_data->gesture_id);
			input_report_key(input_dev, KEY_POWER, 1);
			input_sync(input_dev);
			input_report_key(input_dev, KEY_POWER, 0);
			input_sync(input_dev);
		}
	}
#endif
	if (tcm->pwr_state == LOW_PWR)
		goto exit;
	touch_count = 0;
	for (idx = 0; idx < max_objects; idx++) {
		if (tcm->prev_obj_status[idx] == LIFT &&
				object_data[idx].status == LIFT)
			status = NOP;
		else
			status = object_data[idx].status;

		switch (status) {
		case LIFT:
			input_mt_slot(input_dev, idx);
			input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 0);
			break;
		case FINGER:
		case GLOVED_OBJECT:
			x = object_data[idx].x_pos;
			y = object_data[idx].y_pos;
			wx = object_data[idx].x_width;
			wy = object_data[idx].y_width;
			input_mt_slot(input_dev, idx);
			input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 1);
			input_report_key(input_dev, BTN_TOUCH, 1);
			input_report_key(input_dev, BTN_TOOL_FINGER, 1);
			if (tcm->dts_max_x && tcm->dts_max_y) {
				input_report_abs(input_dev, ABS_MT_POSITION_X, x * tcm->dts_max_x / tcm->firmware_max_x);
				input_report_abs(input_dev, ABS_MT_POSITION_Y, y * tcm->dts_max_y / tcm->firmware_max_y);
			} else {
				input_report_abs(input_dev, ABS_MT_POSITION_X, x);
				input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
			}
			input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, MAX(wx, wy));
			input_report_abs(input_dev, ABS_MT_TOUCH_MINOR, MIN(wx, wy));
			LOGD("Finger %d: x = %d, y = %d\n", idx, x, y);
			touch_count++;
			break;
		default:
			break;
		}
		tcm->prev_obj_status[idx] = object_data[idx].status;
	}
	if (touch_count == 0) {
		input_report_key(input_dev, BTN_TOUCH, 0);
		input_report_key(input_dev, BTN_TOOL_FINGER, 0);
	}
	input_sync(input_dev);
exit:
	syna_pal_mutex_unlock(&tcm->tp_event_mutex);
}

static int syna_dev_create_input_device(struct syna_tcm *tcm)
{
	int retval = 0;
	struct tcm_dev *tcm_dev = tcm->tcm_dev;
	struct input_dev *input_dev = NULL;
	LOGI("%s is called.\n", __func__);
	input_dev = input_allocate_device();
	if (input_dev == NULL) {
		LOGE("Fail to allocate input device\n");
		return -ENODEV;
	}
	input_dev->name = TOUCH_INPUT_NAME;
	input_dev->phys = TOUCH_INPUT_PHYS_PATH;
	input_dev->id.product = SYNAPTICS_TCM_DRIVER_ID;
	input_dev->id.version = SYNAPTICS_TCM_DRIVER_VERSION;
	input_dev->dev.parent = tcm->pdev->dev.parent;
	input_set_drvdata(input_dev, tcm);
	set_bit(EV_SYN, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);
	set_bit(EV_ABS, input_dev->evbit);
	set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	set_bit(ABS_MT_TOOL_TYPE, input_dev->absbit);
	set_bit(BTN_TOUCH, input_dev->keybit);
	set_bit(BTN_TOOL_FINGER, input_dev->keybit);
#ifdef INPUT_PROP_DIRECT
	set_bit(INPUT_PROP_DIRECT, input_dev->propbit);
#endif
	set_bit(KEY_SLEEP, input_dev->keybit);
#ifdef ENABLE_WAKEUP_GESTURE
	set_bit(KEY_POWER, input_dev->keybit);
	input_set_capability(input_dev, EV_KEY, KEY_POWER);
#endif
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, tcm_dev->max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, tcm_dev->max_y, 0, 0);
	input_mt_init_slots(input_dev, tcm_dev->max_objects, INPUT_MT_DIRECT);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MINOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_PROFILE, 0, 1, 0, 0);
	tcm->input_dev_params.max_x = tcm_dev->max_x;
	tcm->input_dev_params.max_y = tcm_dev->max_y;
	tcm->input_dev_params.max_objects = tcm_dev->max_objects;

	retval = input_register_device(input_dev);
	if (retval < 0) {
		LOGE("Fail to register input device\n");
		input_free_device(input_dev);
		input_dev = NULL;
		return retval;
	}

	tcm->input_dev = input_dev;

	return 0;
}

static void syna_dev_release_input_device(struct syna_tcm *tcm)
{
	if (!tcm->input_dev)
		return;
	input_unregister_device(tcm->input_dev);
	tcm->input_dev = NULL;
}

static int syna_dev_check_input_params(struct syna_tcm *tcm)
{
	struct tcm_dev *tcm_dev = tcm->tcm_dev;
	if ((tcm->input_dev_params.max_x != tcm_dev->max_x) || (tcm->input_dev_params.max_y != tcm_dev->max_y) || (tcm->input_dev_params.max_objects != tcm_dev->max_objects))
		return 1;
	return 0;
}

static int syna_dev_set_up_input_device(struct syna_tcm *tcm)
{
	int retval = 0;
	if (IS_NOT_APP_FW_MODE(tcm->tcm_dev->dev_mode)) {
		LOGI("Application firmware not running, current mode: %02x\n",
			tcm->tcm_dev->dev_mode);
		return 0;
	}
	syna_dev_free_input_events(tcm);
	syna_pal_mutex_lock(&tcm->tp_event_mutex);
	retval = syna_dev_check_input_params(tcm);
	if (retval == 0) {
		LOGI("Failed to check input params, exit.\n");
		goto exit;
	}
	if (tcm->input_dev != NULL)
		syna_dev_release_input_device(tcm);
	retval = syna_dev_create_input_device(tcm);
	if (retval < 0) {
		LOGE("Fail to create input device\n");
		goto exit;
	}

exit:
	syna_pal_mutex_unlock(&tcm->tp_event_mutex);
	return retval;
}

static bool monitor_irq_bus_ready(struct syna_tcm *tcm)
{
	struct monitor_data *moni = NULL;

	moni = &tcm->monitor_data;

	if (false == tcm->bus_ready) {
		moni->irq_need_dev_resume_all_count++;
		moni->irq_bus_not_ready_count++;
		return false;
	} else {
		if (moni->irq_bus_not_ready_count > moni->irq_need_dev_resume_max_count) {
			moni->irq_need_dev_resume_max_count = moni->irq_bus_not_ready_count;
		}
		moni->irq_bus_not_ready_count = 0;
		return true;
	}
	return true;
}

static irqreturn_t syna_dev_isr(int irq, void *data)
{
	int retval;
	unsigned char code = 0;
	ktime_t irq_cost_timer;
	struct syna_tcm *tcm = data;
	struct syna_hw_attn_data *attn = &tcm->hw_if->bdata_attn;

	irq_cost_timer = ktime_get();

	if (unlikely(gpio_get_value(attn->irq_gpio) != attn->irq_on_state))
		goto exit;

	tcm->isr_pid = current->pid;

	if (tcm->bus_ready == false) {
		wait_event_interruptible_timeout(tcm->wait,
						 tcm->bus_ready,
						 msecs_to_jiffies(30));
	}

	if (false == monitor_irq_bus_ready(tcm)) {
		goto exit;
	}

	retval = syna_tcm_get_event_data(tcm->tcm_dev,
			&code,
			&tcm->event_data);
	if (retval < 0) {
		LOGE("Fail to get event data\n");
		goto exit;
	}

	if (code == REPORT_DELTA || code == REPORT_RAW || code == REPORT_DEBUG) {
		syna_tcm_test_report(tcm, code);
		goto exit;
	}

	if (code == REPORT_TOUCH) {
		retval = syna_tcm_parse_touch_report(tcm->tcm_dev,
				tcm->event_data.buf,
				tcm->event_data.data_length,
				&tcm->tp_data);
		if (retval < 0) {
			LOGE("Fail to parse touch report\n");
			goto exit;
		}
		syna_dev_report_input_events(tcm);
	}

exit:
	tcm->irq_cost_time = ktime_to_us(ktime_get()) - ktime_to_us(irq_cost_timer);

	return IRQ_HANDLED;
}

static int syna_dev_request_irq(struct syna_tcm *tcm)
{
	int retval;
	struct syna_hw_attn_data *attn = &tcm->hw_if->bdata_attn;

	if (attn->irq_gpio < 0) {
		LOGE("Invalid IRQ GPIO\n");
		retval = -EINVAL;
		goto exit;
	}

	attn->irq_id = gpio_to_irq(attn->irq_gpio);
	retval = request_threaded_irq(attn->irq_id,
			NULL,
			syna_dev_isr,
			attn->irq_flags | IRQF_ONESHOT,
			PLATFORM_DRIVER_NAME,
			tcm);
	if (retval < 0) {
		LOGE("Fail to request threaded irq\n");
		goto exit;
	}

	attn->irq_enabled = true;
	tcm->bus_ready = true;

	LOGI("Interrupt handler registered\n");

exit:
	return retval;
}

static void syna_dev_release_irq(struct syna_tcm *tcm)
{
	struct syna_hw_attn_data *attn = &tcm->hw_if->bdata_attn;

	if (attn->irq_id <= 0)
		return;

	if (tcm->hw_if->ops_enable_irq)
		tcm->hw_if->ops_enable_irq(tcm->hw_if, false);

	free_irq(attn->irq_id, tcm);

	attn->irq_id = 0;
	attn->irq_enabled = false;

	LOGI("Interrupt handler released\n");
}

static int syna_dev_set_up_app_fw(struct syna_tcm *tcm)
{
	int retval = 0;
	struct tcm_dev *tcm_dev = tcm->tcm_dev;

	if (IS_NOT_APP_FW_MODE(tcm_dev->dev_mode)) {
		LOGN("Application firmware not running, current mode: %02x\n",
			tcm_dev->dev_mode);
		return -EINVAL;
	}

	retval = syna_tcm_get_app_info(tcm_dev, &tcm_dev->app_info);
	if (retval < 0) {
		LOGE("Fail to get application info\n");
		return -EIO;
	}
	if (tcm->dts_max_x && tcm->dts_max_y) {
		tcm->firmware_max_x = tcm_dev->max_x;
		tcm->firmware_max_y = tcm_dev->max_y;
		tcm_dev->max_x = tcm->dts_max_x;
		tcm_dev->max_y = tcm->dts_max_y;
	}
	retval = syna_tcm_preserve_touch_report_config(tcm_dev);
	if (retval < 0) {
		LOGE("Fail to preserve touch report config\n");
		return -EIO;
	}

	return retval;
}

static void syna_dev_reflash_startup_work(struct work_struct *work)
{
	int retval;
	struct delayed_work *delayed_work;
	struct syna_tcm *tcm;
	struct tcm_dev *tcm_dev;
	struct syna_hw_interface *hw_if;
	const struct firmware *fw_entry = NULL;
	const unsigned char *fw_image = NULL;
	unsigned int fw_image_size;
	struct syna_hw_attn_data *attn;

	delayed_work = container_of(work, struct delayed_work, work);
	tcm = container_of(delayed_work, struct syna_tcm, reflash_work);

	tcm_dev = tcm->tcm_dev;
	hw_if = tcm->hw_if;
	attn = &hw_if->bdata_attn;

	syna_pal_mutex_lock(&tcm->extif_mutex);

	if (tcm->firmware_update_type == 1) {
		if (tcm->fw_name_fae) {
			retval = request_firmware(&fw_entry,
				   tcm->fw_name_fae,
				   tcm->pdev->dev.parent);
		} else {
			LOGE("fw_name_fae is NULL\n");
			retval = -1;
		}
	} else {
		if (tcm->panel_data.fw_name) {
			retval = request_firmware_select(&fw_entry,
				   tcm->panel_data.fw_name,
				   tcm->pdev->dev.parent);
		} else {
			LOGE("panel_data.fw_name is NULL\n");
			retval = -1;
		}
	}
	if (retval < 0) {
		LOGE("Fail to request %s\n", (tcm->firmware_update_type == 1) ?
				   tcm->fw_name_fae : tcm->panel_data.fw_name);
		complete(&tcm->fw_complete);
		syna_pal_mutex_unlock(&tcm->extif_mutex);
		return;
	}

	fw_image = fw_entry->data;
	fw_image_size = fw_entry->size;

	LOGD("Firmware image size = %d\n", fw_image_size);

	pm_stay_awake(&tcm->pdev->dev);

	syna_pal_mutex_lock(&hw_if->bdata_rst.reset_en_mutex);
	retval = syna_tcm_do_fw_update(tcm_dev,
			fw_image,
			fw_image_size,
			RESP_IN_ATTN,
			(tcm->firmware_update_type == 1));
	if (retval < 0) {
		LOGE("Fail to do reflash\n");
		syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);
		goto exit;
	}

	retval = syna_dev_set_up_app_fw(tcm);
	if (retval < 0) {
		LOGE("Fail to set up app fw after fw update\n");
		syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);
		goto exit;
	}

	if (!tcm->input_dev || !tcm->char_dev_ref_count) {
		retval = syna_dev_set_up_input_device(tcm);
		if (retval < 0) {
			LOGE("Fail to register input device\n");
			syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);
			goto exit;
		}
	}
	syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);

	LOGI("Do reset after fw update and msleep\n");
	msleep(RST_ENABLE_IRQ_DELAY_MS);
	if (hw_if->ops_hw_reset) {
		if (attn->irq_enabled) {
			LOGI("Disable IRQ.\n");
			hw_if->ops_enable_irq(hw_if, false);
			hw_if->ops_hw_reset(hw_if);
			hw_if->ops_enable_irq(hw_if, true);
		} else {
			LOGI("IRQ already disable.\n");
			hw_if->ops_hw_reset(hw_if);
		}
	} else {
		retval = syna_tcm_reset(tcm->tcm_dev);
		if (retval < 0) {
			LOGE("Fail to do sw reset\n");
			goto exit;
		}
	}
exit:
	fw_image = NULL;

	release_firmware(fw_entry);
	fw_entry = NULL;

	pm_relax(&tcm->pdev->dev);

	complete(&tcm->fw_complete);

	syna_pal_mutex_unlock(&tcm->extif_mutex);
}

static void syna_dev_fw_update_in_bl(struct syna_tcm *tcm)
{
	int retval;
	struct tcm_dev *tcm_dev;
	struct syna_hw_interface *hw_if;
	const struct firmware *fw_entry = NULL;
	const unsigned char *fw_image = NULL;
	unsigned int fw_image_size;
	int locked = 0;

	tcm_dev = tcm->tcm_dev;
	hw_if = tcm->hw_if;

	locked = syna_pal_mutex_trylock(&tcm->extif_mutex);
	if (!locked) {
		LOGE("extif_mutex has been acquired, lock failed\n");
	}

	if (tcm->panel_data.fw_name) {
		retval = request_firmware_select(&fw_entry,
			   tcm->panel_data.fw_name,
			   tcm->pdev->dev.parent);
	} else {
		LOGE("panel_data.fw_name is NULL\n");
		retval = -1;
	}

	if (retval < 0) {
		LOGE("Fail to request %s\n", (tcm->firmware_update_type == 1) ?
				   tcm->fw_name_fae : tcm->panel_data.fw_name);
		if (locked) {
			syna_pal_mutex_unlock(&tcm->extif_mutex);
		}
		return;
	}

	fw_image = fw_entry->data;
	fw_image_size = fw_entry->size;

	LOGD("Firmware image size = %d\n", fw_image_size);

	pm_stay_awake(&tcm->pdev->dev);

	syna_pal_mutex_lock(&hw_if->bdata_rst.reset_en_mutex);
	retval = syna_tcm_do_fw_update(tcm_dev,
			fw_image,
			fw_image_size,
			RESP_IN_ATTN,
			(tcm->firmware_update_type == 1));
	if (retval < 0) {
		LOGE("Fail to do reflash\n");
		syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);
		goto exit;
	}

	retval = syna_dev_set_up_app_fw(tcm);
	if (retval < 0) {
		LOGE("Fail to set up app fw after fw update\n");
		syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);
		goto exit;
	}

	if (!tcm->input_dev || !tcm->char_dev_ref_count) {
		retval = syna_dev_set_up_input_device(tcm);
		if (retval < 0) {
			LOGE("Fail to register input device\n");
			syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);
			goto exit;
		}
	}
	syna_pal_mutex_unlock(&hw_if->bdata_rst.reset_en_mutex);
exit:
	fw_image = NULL;

	release_firmware(fw_entry);
	fw_entry = NULL;

	pm_relax(&tcm->pdev->dev);
	if (locked) {
		syna_pal_mutex_unlock(&tcm->extif_mutex);
	}
}
static int syna_dev_enter_normal_sensing(struct syna_tcm *tcm)
{
	int retval = 0;

	if (!tcm)
		return -EINVAL;

	retval = syna_tcm_sleep(tcm->tcm_dev, false);
	if (retval < 0) {
		LOGE("Fail to exit deep sleep\n");
		return retval;
	}

	if (tcm->lpwg_enabled) {
		retval = syna_dev_enable_lowpwr_gesture(tcm, false);
		if (retval < 0) {
			LOGE("Fail to disable low power gesture mode\n");
			return retval;
		}
	}
	LOGI("low power gesture mode disabled\n");

	return 0;
}
static int syna_dev_enter_lowpwr_sensing(struct syna_tcm *tcm)
{
	int retval = 0;

	if (!tcm)
		return -EINVAL;

	if (tcm->lpwg_enabled) {
		retval = syna_dev_set_gesture_type(tcm, tcm->gesture_type);
		if (retval < 0) {
			LOGE("Fail to set gesture type\n");
			return retval;
		}

		retval = syna_dev_enable_lowpwr_gesture(tcm, true);
		if (retval < 0) {
			LOGE("Fail to enable low power gesture mode\n");
			return retval;
		}
		LOGI("low power gesture mode enabled\n");
	} else {
		if (!tcm->slept_in_early_suspend) {
			retval = syna_tcm_sleep(tcm->tcm_dev, true);
			if (retval < 0) {
				LOGE("Fail to enter deep sleep\n");
				return retval;
			}
		}
	}

	return 0;
}

void syna_send_signal(struct syna_tcm *tcm, int signal_num)
{
	if(tcm->proc_task != NULL && tcm->char_dev_ref_count) {
		LOGI("Sending signal[%d] to app\n", signal_num);
		if (send_sig(signal_num, tcm->proc_task, 0) < 0) {
			LOGE("Unable to send signal\n");
		}
	}
}

static int syna_dev_resume(struct device *dev)
{
	struct syna_tcm *tcm = dev_get_drvdata(dev);
	queue_work(tcm->speedup_resume_wq, &tcm->speed_up_work);
	return 0;
}

static void syna_speedup_resume(struct work_struct *work)
{
	struct syna_tcm *tcm = container_of(work, struct syna_tcm,
				     speed_up_work);
	int retval;
	struct syna_hw_interface *hw_if = tcm->hw_if;
	bool irq_enabled = true;

	LOGI("%s is called\n", __func__);

	if (tcm->pwr_state == PWR_ON) {
		LOGI("pwr_state is already in PWR_ON, exit.\n");
		tcm->sub_pwr_state = SUB_PWR_RESUME_DONE;
		return;
	}
	mutex_lock(&tcm->mutex);

	if (IS_REMOVE == tcm->driver_current_state) {
		LOGE("%s:driver is remove!!\n", __func__);
		mutex_unlock(&tcm->mutex);
		return;
	};

	tcm->sub_pwr_state = SUB_PWR_RESUMING;

	LOGI("Prepare to resume device\n");

	irq_enabled = (!hw_if->bdata_attn.irq_enabled);

	if (irq_enabled && (hw_if->ops_enable_irq))
		hw_if->ops_enable_irq(hw_if, true);

	if ((!tcm->is_fp_down)||(tcm->pwr_state == PWR_UNKNOWN)) {
		if (tcm->probe_done && tcm->tcm_dev->firmware_mode_count % FWUPDATE_BL_MAX == 0 && tcm->tcm_dev->firmware_mode_count) {
			LOGE("bootloader trigger fw update\n");
			tcm->tcm_dev->firmware_mode_count = 0;
			syna_dev_fw_update_in_bl(tcm);
		}
		LOGI("Do reset on resume\n");

		if (hw_if->ops_hw_reset) {
			hw_if->ops_hw_reset(hw_if);
		} else {
			retval = syna_tcm_reset(tcm->tcm_dev);
			if (retval < 0) {
				LOGE("Fail to do sw reset\n");
				goto exit;
			}
		}
	} else {
		LOGI("is_fp_down, ignore hw reset\n");
		retval = syna_dev_enter_normal_sensing(tcm);
		if (retval < 0) {
			LOGE("Fail to enter normal power mode\n");
			goto exit;
		}
		LOGI("Exit power saved mode\n");
	}
	if (tcm->char_dev_ref_count) {
		retval = syna_dev_disable_lbp_mode(tcm);
		if (retval < 0) {
			LOGE("Fail to disable lbp mode\n");
		}
	}
	if(tcm->pwr_state == PWR_UNKNOWN)
	{
		tcm->pwr_state = PWR_ON;
	}
	tcm->pwr_state = PWR_ON;

	LOGI("Prepare to set up application firmware\n");

	retval = syna_dev_set_up_app_fw(tcm);
	if (retval < 0) {
		LOGE("Fail to set up app firmware on resume\n");
		goto exit;
	}

	retval = 0;

	LOGI("Device resumed (pwr_state:%d)\n", tcm->pwr_state);

	syna_send_signal(tcm, SIG_DISPLAY_ON);

exit:
	tcm->sub_pwr_state = SUB_PWR_RESUME_DONE;
	tcm->slept_in_early_suspend = false;

	mutex_unlock(&tcm->mutex);
}
static int syna_dev_suspend(struct device *dev)
{
#ifdef POWER_ALIVE_AT_SUSPEND
	int retval;
#endif
	struct syna_tcm *tcm = dev_get_drvdata(dev);
	struct syna_hw_interface *hw_if = tcm->hw_if;
	struct touchpanel_event event_data;
	bool irq_disabled = true;

	if (tcm->pwr_state != PWR_ON)
		return 0;
	mutex_lock(&tcm->mutex);

	if (IS_REMOVE == tcm->driver_current_state) {
		LOGE("%s:driver is remove!!\n", __func__);
		mutex_unlock(&tcm->mutex);
		return -EINVAL;
	};

	tcm->sub_pwr_state = SUB_PWR_SUSPENDING;
	LOGI("[TP]touchpanel: tp_suspend: start.\n");

	retval = syna_dev_disable_hbp_mode(tcm);
	if (retval < 0) {
		LOGE("Fail to disable hbp mode\n");
	}

	syna_dev_free_input_events(tcm);

#ifdef POWER_ALIVE_AT_SUSPEND
	retval = syna_dev_enter_lowpwr_sensing(tcm);
	if (retval < 0) {
		tcm->pwr_state = PWR_UNKNOWN;
		LOGE("Fail to enter suspended power mode, tcm->pwr_state: %d.\n", tcm->pwr_state);
		mutex_unlock(&tcm->mutex);
		return retval;
	}
	tcm->pwr_state = LOW_PWR;
	LOGI("Enter power saved mode\n");
#else
	tcm->pwr_state = PWR_OFF;
#endif
	memset(&event_data, 0, sizeof(struct touchpanel_event));
	touchpanel_event_call_notifier(EVENT_ACTION_FOR_FINGPRINT,
		   (void *)&event_data);
	LOGI("[compensate]Report UP event to fingerprint notifier\n");
	irq_disabled = (!tcm->lpwg_enabled);

	if (irq_disabled && (hw_if->ops_enable_irq))
		hw_if->ops_enable_irq(hw_if, false);

	LOGI("Device suspended (pwr_state:%d)\n", tcm->pwr_state);

	tcm->sub_pwr_state = SUB_PWR_SUSPEND_DONE;
	mutex_unlock(&tcm->mutex);

	return 0;
}

#if defined(ENABLE_DISP_NOTIFIER)
static int syna_dev_early_suspend(struct device *dev)
{
	int retval;
	struct syna_tcm *tcm = dev_get_drvdata(dev);

	if (tcm->pwr_state != PWR_ON || tcm->sub_pwr_state > SUB_PWR_RESUME_DONE)
		return 0;

	if (tcm->is_connected && tcm->daemon_state != STATE_RUN) {
		LOGE("daemon state in %d, wait for exit...\n", tcm->daemon_state);
		wait_event_interruptible_timeout(state_waiter,
				             (tcm->daemon_state == STATE_RUN),
				             msecs_to_jiffies(200));
		if (tcm->daemon_state != STATE_RUN) {
			LOGE("wait daemon state %d exit timeout...\n", tcm->daemon_state);
		}
	}
	mutex_lock(&tcm->mutex);

	if (IS_REMOVE == tcm->driver_current_state) {
		LOGE("%s:driver is remove!!\n", __func__);
		mutex_unlock(&tcm->mutex);
		return -EINVAL;
	};

	tcm->touch_and_hold = 1;
	syna_dev_update_lpwg_status(tcm);
	syna_dev_set_fingerprint_enable(tcm, tcm->touch_and_hold);

	tcm->sub_pwr_state = SUB_PWR_EARLY_SUSPENDING;
	LOGI("Prepare to early suspend device\n");

	syna_send_signal(tcm, SIG_DISPLAY_OFF);

	if (!tcm->lpwg_enabled) {
		retval = syna_tcm_sleep(tcm->tcm_dev, true);
		if (retval < 0) {
			LOGE("Fail to enter deep sleep\n");
			mutex_unlock(&tcm->mutex);
			return retval;
		}
	}

	tcm->slept_in_early_suspend = true;

	mutex_unlock(&tcm->mutex);

	if (tcm->fp_active) {
		syna_dev_suspend(dev);
		tcm->fb_ready = 0;
	}

	return 0;
}
#if IS_ENABLED(CONFIG_QCOM_PANEL_EVENT_NOTIFIER)
static void ts_panel_notifier_callback(enum panel_event_notifier_tag tag,
		 struct panel_event_notification *notification, void *client_data)
{
	int time = 0;
	struct syna_tcm *tcm = client_data;

	if (!notification) {
		LOGE("Invalid notification\n");
		return;
	}
	if (!tcm || IS_REMOVE == tcm->driver_current_state) {
		LOGE("%s: driver is remove!!\n", __func__);
		return;
	};
	syna_dev_update_lpwg_status(tcm);

	if (notification->notif_type <= DRM_PANEL_EVENT_FOR_TOUCH) {
		LOGI("Notification type:%d, early_trigger:%d",
				notification->notif_type,
				notification->notif_data.early_trigger);
	}

	switch (notification->notif_type) {
	case DRM_PANEL_EVENT_UNBLANK:
		if (notification->notif_data.early_trigger) {

			syna_dev_resume(&tcm->pdev->dev);
			tcm->fb_ready++;
		}
		break;
	case DRM_PANEL_EVENT_BLANK:
		if (notification->notif_data.early_trigger) {
			while (ATOMIC_GET(tcm->tcm_dev->firmware_flashing)) {
				syna_pal_sleep_ms(500);
				time += 500;
				if (time >= 5000) {
					LOGE("Timed out waiting for reflashing\n");
					ATOMIC_SET(tcm->tcm_dev->firmware_flashing, 0);
					return;
				}
			}
			if (tcm->speedup_resume_wq) {
				flush_workqueue(tcm->speedup_resume_wq);
			}
			syna_dev_early_suspend(&tcm->pdev->dev);
		} else if (!tcm->fp_active) {
			syna_dev_suspend(&tcm->pdev->dev);
			tcm->fb_ready = 0;
		}
		break;
	case DRM_PANEL_EVENT_BLANK_LP:
		LOGI("received lp event\n");
		if (!notification->notif_data.early_trigger) {
			syna_dev_suspend(&tcm->pdev->dev);
		}
		break;
	case DRM_PANEL_EVENT_FPS_CHANGE:
		LOGI("shashank:Received fps change old fps:%d new fps:%d\n",
				notification->notif_data.old_fps,
				notification->notif_data.new_fps);
		break;
	case DRM_PANEL_EVENT_FOR_TOUCH:
		break;
	default:
		if (notification->notif_type <= DRM_PANEL_EVENT_FOR_TOUCH) {
			LOGI("notification serviced :%d\n",
				   notification->notif_type);
		}
		break;
	}
}

#else
static int fb_notifier_callback(struct notifier_block *self, unsigned long event, void *data)
{
	int time = 0;
	int retval;
	int *blank;
	struct msm_drm_notifier *evdata = data;

	struct syna_tcm *tcm = container_of(self, struct syna_tcm, fb_notifier);

	if (!tcm || IS_REMOVE == tcm->driver_current_state) {
		LOGE("%s: driver is remove!!\n", __func__);
		return 0;
	};
	if (event != MSM_DRM_EARLY_EVENT_BLANK && event != MSM_DRM_EVENT_BLANK
	    && event != MSM_DRM_EVENT_FOR_TOUCH)
		return 0;

	if (evdata && evdata->data && tcm) {
		blank = evdata->data;
		LOGE("%s: event = %ld, blank = %d\n", __func__, event, *blank);
		syna_dev_update_lpwg_status(tcm);
		if (*blank == MSM_DRM_BLANK_POWERDOWN) { 
			if (event == MSM_DRM_EARLY_EVENT_BLANK) { 
				while (ATOMIC_GET(tcm->tcm_dev->firmware_flashing)) {
					syna_pal_sleep_ms(500);
					time += 500;
					if (time >= 5000) {
						LOGE("Timed out waiting for reflashing\n");
						ATOMIC_SET(tcm->tcm_dev->firmware_flashing, 0);
						return -EIO;
					}
				}
				if (tcm->speedup_resume_wq) {
					flush_workqueue(tcm->speedup_resume_wq);
				}

				syna_dev_early_suspend(&tcm->pdev->dev);
			} else if (event == MSM_DRM_EVENT_BLANK) {
				syna_dev_suspend(&tcm->pdev->dev);
				tcm->fb_ready = 0;
			}
		} else if (*blank == MSM_DRM_BLANK_UNBLANK) { 
			if (event == MSM_DRM_EARLY_EVENT_BLANK) {  
				retval = syna_dev_resume(&tcm->pdev->dev);
				tcm->fb_ready++;
			}
		}
	}

	return 0;
}
#endif
#endif

static int syna_dev_disconnect(struct syna_tcm *tcm)
{
	struct syna_hw_interface *hw_if = tcm->hw_if;
	int ret = 0;

	if (tcm->is_connected == false) {
		LOGI("%s already disconnected\n", PLATFORM_DRIVER_NAME);
		return 0;
	}

	cancel_delayed_work_sync(&tcm->reflash_work);
	flush_workqueue(tcm->reflash_workqueue);
	destroy_workqueue(tcm->reflash_workqueue);

	if (hw_if->bdata_attn.irq_id)
		syna_dev_release_irq(tcm);

	if (hw_if->bdata_rst.reset_gpio >= 0) {
		ret = gpio_direction_output(hw_if->bdata_rst.reset_gpio, 0);
		if (ret) {
			LOGI("failed to set the reset_gpio to 0.\n");
			return -EINVAL;
		}
		LOGI("set reset_gpio(%d) to 0\n", hw_if->bdata_rst.reset_gpio);
	}

	syna_dev_release_input_device(tcm);

	tcm->input_dev_params.max_x = 0;
	tcm->input_dev_params.max_y = 0;
	tcm->input_dev_params.max_objects = 0;

	if (hw_if->ops_power_on)
		hw_if->ops_power_on(hw_if, false);

	tcm->pwr_state = PWR_OFF;
	tcm->is_connected = false;

	LOGI("%s device disconnected\n", PLATFORM_DRIVER_NAME);

	return 0;
}

static int syna_dev_connect(struct syna_tcm *tcm)
{
	int retval;
	struct syna_hw_interface *hw_if = tcm->hw_if;
	struct syna_hw_bus_data *bus = &hw_if->bdata_io;
	struct tcm_dev *tcm_dev = tcm->tcm_dev;

	if (!tcm_dev) {
		LOGE("Invalid tcm_dev\n");
		return -EINVAL;
	}

	if (tcm->is_connected) {
		LOGI("%s already connected\n", PLATFORM_DRIVER_NAME);
		return 0;
	}

	if (hw_if->ops_power_on) {
		retval = hw_if->ops_power_on(hw_if, true);
		if (retval < 0)
			return -ENODEV;
	}

	if (hw_if->ops_hw_reset)
		hw_if->ops_hw_reset(hw_if);

	retval = syna_tcm_detect_device(tcm->tcm_dev);
	if (retval < 0) {
		LOGE("Fail to detect the device\n");
		goto err_detect_dev;
	}

	switch (retval) {
	case MODE_APPLICATION_FIRMWARE:
		retval = syna_dev_set_up_app_fw(tcm);
		if (retval < 0) {
			LOGE("Fail to set up application firmware\n");
			LOGI("Switch device to bootloader mode instead\n");
			syna_tcm_switch_fw_mode(tcm_dev,
					MODE_BOOTLOADER,
					FW_MODE_SWITCH_DELAY_MS);
		} else {
			retval = syna_dev_set_up_input_device(tcm);
			if (retval < 0) {
				LOGE("Fail to set up input device\n");
				goto err_setup_input_dev;
			}
			LOGI("Success to set up input device\n");
		}

		break;
	default:
		LOGN("Application firmware not running, current mode: %02x\n",
			retval);
		break;
	}

	retval = syna_dev_request_irq(tcm);
	if (retval < 0) {
		LOGE("Fail to request the interrupt line\n");
		goto err_request_irq;
	}
	tcm->reflash_workqueue =
			create_singlethread_workqueue("syna_reflash");
	INIT_DELAYED_WORK(&tcm->reflash_work, syna_dev_reflash_startup_work);

	tcm->pwr_state = PWR_ON;
	tcm->is_connected = true;
	tcm->bus_ready = true;

	LOGI("%s device connected\n", PLATFORM_DRIVER_NAME);

	LOGI("TCM packrat: %d\n", tcm->tcm_dev->packrat_number);
	LOGI("Config: lpwg_mode(%s), custom_tp_config(%s) helper_work(%s)\n",
		(tcm->lpwg_enabled) ? "yes" : "no",
		(tcm->has_custom_tp_config) ? "yes" : "no",
		(tcm->helper_enabled) ? "yes" : "no");
	LOGI("Config: startup_reflash(%s), hw_reset(%s), rst_on_resume(%s)\n",
		(tcm->startup_reflash_enabled) ? "yes" : "no",
		(hw_if->ops_hw_reset) ? "yes" : "no",
		(tcm->rst_on_resume_enabled) ? "yes" : "no");

	if (tcm->tcm_dev->max_wr_size != bus->wr_chunk_size)
		bus->wr_chunk_size = tcm->tcm_dev->max_wr_size;
	if (tcm->tcm_dev->max_rd_size != bus->rd_chunk_size)
		bus->rd_chunk_size = tcm->tcm_dev->max_rd_size;

	LOGI("Config: write_chunk(%d), read_chunk(%d)\n",
		bus->wr_chunk_size, bus->rd_chunk_size);

	return 0;

err_request_irq:
	syna_dev_release_input_device(tcm);

err_setup_input_dev:
err_detect_dev:
	if (hw_if->ops_power_on)
		hw_if->ops_power_on(hw_if, false);

	return retval;
}

static struct debug_info_proc_operations syna_debug_proc_ops = {
	.delta_read    = syna_delta_read,
	.baseline_read = syna_baseline_read,
	.baseline_blackscreen_read = syna_baseline_read,
	.main_register_read = syna_main_register,
	.reserve_read  = syna_reserve_read,
};

static void syna_start_aging_test(void *chip_data)
{
	int ret = -1;
	struct syna_tcm *tcm = (struct syna_tcm *)chip_data;

	ret = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_DISABLE_DOZE, 1, RESP_IN_ATTN);
	if (ret < 0) {
	}
}

static void syna_finish_aging_test(void *chip_data)
{
	int ret = -1;
	struct syna_tcm *tcm = (struct syna_tcm *)chip_data;

	ret = syna_tcm_set_dynamic_config(tcm->tcm_dev, DC_DISABLE_DOZE, 0, RESP_IN_ATTN);
	if (ret < 0) {
	}
}

static struct aging_test_proc_operations aging_test_proc_ops = {
	.start_aging_test = syna_start_aging_test,
	.finish_aging_test = syna_finish_aging_test,
};

static struct device_node* is_support_child_node(struct device *dev, struct syna_tcm *tcm)
{
	char panel_node[60] = {0};
	struct device_node *child_node = NULL;

	snprintf(panel_node, sizeof(panel_node), "%s_PANEL%d", tcm->panel_data.chip_name[0], tcm->panel_data.tp_type);

	child_node = of_get_child_by_name(dev->of_node, panel_node);
	if (!child_node) {
	}

	return child_node;
}

static void init_panel_config(struct device *dev, struct syna_tcm *tcm)
{
	int rc = 0;
	int tx_rx_num[2];
	struct device_node *child_node = NULL;

	child_node = is_support_child_node(dev, tcm);
	if (!child_node) {
		return;
	}

	rc = of_property_read_u32_array(child_node, "touchpanel,tx-rx-num", tx_rx_num, 2);

	if (rc) {
		tcm->tx_num = TX_NUM;
		tcm->rx_num = RX_NUM;
	} else {
		tcm->tx_num = tx_rx_num[0];
		tcm->rx_num = tx_rx_num[1];
	}
}

static int init_chip_dts(struct device *dev, void *chip_data)
{
	int rc = 0;
	struct device_node *np;
	struct syna_tcm *tcm = (struct syna_tcm *)chip_data;
	int tx_rx_num[2], panel_coords[2];
	int i = 0;
	np = dev->of_node;

	of_property_read_u32(np, "project_id", &tcm->panel_data.project_id);
	tcm->panel_data.project_num = of_property_count_u32_elems(np, "platform_support_project");
	if (tcm->panel_data.project_num > 0) {
		rc = of_property_read_u32_array(np, "platform_support_project", tcm->panel_data.platform_support_project, tcm->panel_data.project_num);

		if (rc) {
			return -1;
		}

		rc = of_property_read_u32_array(np, "platform_support_project_dir", tcm->panel_data.platform_support_project_dir, tcm->panel_data.project_num);

		if (rc) {
			return -1;
		}

	}

	tcm->panel_data.chip_num = 1;
	for (i = 0; i < tcm->panel_data.chip_num; i++) {
		of_property_read_string_index(np, "chip-name", i, (const char **)&tcm->panel_data.chip_name[i]);
	}

	tcm->panel_data.panel_num = of_property_count_u32_elems(np, "panel_type");
	if (tcm->panel_data.panel_num > 0) {
		rc = of_property_read_u32_array(np, "panel_type", tcm->panel_data.panel_type, tcm->panel_data.panel_num);
		if (rc) {
			return -1;
		}
	}

	for (i = 0; i < tcm->panel_data.panel_num; i++) {
		rc = of_property_read_string_index(np, "platform_support_project_commandline", i, (const char **)&tcm->panel_data.platform_support_commandline[i]);

		if (rc) {
			return -1;
		}

		of_property_read_string_index(np, "firmware_name", i, (const char **)&tcm->panel_data.firmware_name[i]);

	}

	tcm->panel_data.tp_type = TP_UNKNOWN;

	rc = of_property_read_u32_array(np, "touchpanel,tx-rx-num", tx_rx_num, 2);

	if (rc) {
		tcm->tx_num = TX_NUM;
		tcm->rx_num = RX_NUM;

	} else {
		tcm->tx_num = tx_rx_num[0];
		tcm->rx_num = tx_rx_num[1];
	}


	rc = of_property_read_u32_array(np, "touchpanel,panel-coords", panel_coords, 2);

	if (rc) {
		tcm->dts_max_x = 0;
		tcm->dts_max_y = 0;

	} else {
		tcm->dts_max_x = panel_coords[0];
		tcm->dts_max_y = panel_coords[1];
	}

	init_panel_config(dev, tcm);

	return 0;
}

static int tp_paneldata_init(struct syna_tcm *pdata)
{
	struct syna_tcm *tcm = pdata;
	int ret = -1;
	char *fw_name_tmp = NULL;	

	if (!tcm) {
		return ret;
	}

	tcm->panel_data.fw_name = devm_kzalloc(&tcm->pdev->dev, MAX_FW_NAME_LENGTH, GFP_KERNEL);

	if (tcm->panel_data.fw_name == NULL) {
		ret = -ENOMEM;
		return ret;
	}

	if (fw_name_tmp) {
		devm_kfree(&tcm->pdev->dev, fw_name_tmp);
	}
	return 0;
}

static int syna_dev_probe(struct platform_device *pdev)
{
	int retval;
	int ret = 0;
	struct syna_tcm *tcm = NULL;
	struct tcm_dev *tcm_dev = NULL;
	struct syna_hw_interface *hw_if = NULL;
	struct device * syna_spi_pdev;

	LOGI("%s is called.\n", __func__);
	hw_if = pdev->dev.platform_data;
	if (!hw_if) {
		LOGE("Fail to find hardware configuration\n");
		return -EINVAL;
	}

	tcm = syna_pal_mem_alloc(1, sizeof(struct syna_tcm));
	if (!tcm) {
		LOGE("Fail to create the instance of syna_tcm\n");
		return -ENOMEM;
	}

	tcm->test_hcd = (struct syna_tcm_test *)devm_kzalloc(&pdev->dev, sizeof(struct syna_tcm_test), GFP_KERNEL);
	if (!tcm->test_hcd) {
		syna_pal_mem_free((void *)tcm);
		LOGE("Fail to alloc tcm->test_hcd mem\n");
		return -ENOMEM;
	}

	INIT_BUFFER(tcm->test_hcd->report, false);
	INIT_BUFFER(tcm->test_hcd->test_resp, false);
	INIT_BUFFER(tcm->test_hcd->test_out, false);
	retval = syna_tcm_allocate_device(&tcm_dev, hw_if, RESP_IN_POLLING);
	if ((retval < 0) || (!tcm_dev)) {
		LOGE("Fail to allocate TouchCom device handle\n");
		goto err_allocate_cdev;
	}

	tcm->monitor_data.health_monitor_support = false;

	tcm->exception_upload_support = true;
	tcm->exception_data.exception_upload_support = true;
	tcm->exception_data.chip_data = tcm;

	tcm->tcm_dev = tcm_dev;
	tcm->pdev = pdev;
	tcm->hw_if = hw_if;
	tcm_dev->monitor_data = &tcm->monitor_data;
	tcm->frame_over_cnt_report_en = 1;

	syna_spi_pdev = syna_spi_device->dev.parent;

	retval = init_chip_dts(syna_spi_pdev, tcm);

	if (retval < 0) {
		goto err_manufacture_info;
	}

	retval = tp_paneldata_init(tcm);

	if (retval < 0) {
		goto err_manufacture_info;
	}

	syna_tcm_buf_init(&tcm->event_data);

	syna_pal_mutex_alloc(&tcm->tp_event_mutex);

	mutex_init(&tcm->mutex);
	init_completion(&tcm->fw_complete);
	init_waitqueue_head(&tcm->wait);

	tcm->has_custom_tp_config = false;
	tcm->startup_reflash_enabled = true;
	tcm->rst_on_resume_enabled = true;
	tcm->helper_enabled = false;
	tcm->lpwg_enabled = false;
#ifdef ENABLE_WAKEUP_GESTURE
	tcm->gesture_type = 0x0000;
	tcm->touch_and_hold = 0;
	syna_dev_update_lpwg_status(tcm);
#endif
	tcm->irq_wake = false;

	tcm->is_connected = false;
	tcm->pwr_state = PWR_OFF;

	tcm->dev_connect = syna_dev_connect;
	tcm->dev_disconnect = syna_dev_disconnect;
	tcm->dev_set_up_app_fw = syna_dev_set_up_app_fw;
	tcm->dev_resume = syna_dev_resume;
	tcm->dev_suspend = syna_dev_suspend;

	tcm->engineer_ops = NULL;
	tcm->com_test_data.chip_test_ops = NULL;

	tcm->debug_info_ops = &syna_debug_proc_ops;
	tcm->aging_test_ops = &aging_test_proc_ops;

	tcm->userspace_app_info = NULL;

	tcm->wait_for_ioctl_operation = 0;
	tcm->waiting_frame = 0;
	tcm->use_short_frame_waiting = 0;
	tcm->primary_timestamp_enabled = 1;

	platform_set_drvdata(pdev, tcm);

	device_init_wakeup(&pdev->dev, 1);
	init_completion(&tcm->report_complete);
	
	retval = tcm->dev_connect(tcm);
	if (retval < 0) {
		LOGE("Fail to connect to the device\n");
		mutex_destroy(&tcm->mutex);
		syna_pal_mutex_free(&tcm->tp_event_mutex);
		goto err_connect;
	}

	tcm->fb_notifier.notifier_call = fb_notifier_callback;
	ret = msm_drm_register_client(&tcm->fb_notifier);

	if (ret) {
		LOGE("Unable to register fb_notifier: %d\n", ret);
		goto err_create_cdev;
	}

	tcm->speedup_resume_wq = create_singlethread_workqueue("sp_resume0");
	INIT_WORK(&tcm->speed_up_work, syna_speedup_resume);

	syna_tcm_enable_predict_reading(tcm->tcm_dev, true);

	tcm->probe_done = 1;
	LOGI("TouchComm driver, %s v%d.%s installed\n",
		PLATFORM_DRIVER_NAME,
		SYNAPTICS_TCM_DRIVER_VERSION,
		SYNAPTICS_TCM_DRIVER_SUBVER);

	return 0;
	tcm->dev_disconnect(tcm);
err_create_cdev:
err_manufacture_info:
err_connect:
	syna_tcm_buf_release(&tcm->event_data);
	mutex_destroy(&tcm->mutex);
	syna_pal_mutex_free(&tcm->tp_event_mutex);
err_allocate_cdev:
	syna_pal_mem_free((void *)tcm);

	return retval;
}

static void syna_dev_remove(struct platform_device *pdev)
{
	struct syna_tcm *tcm = platform_get_drvdata(pdev);
	syna_send_signal(tcm, SIGKILL);
	syna_pal_sleep_ms(25);
	tcm->driver_current_state = IS_REMOVE;
	mutex_lock(&tcm->mutex);
    tcm->dev_disconnect(tcm);
	if (tcm->userspace_app_info != NULL) 
		syna_pal_mem_free(tcm->userspace_app_info);
	mutex_unlock(&tcm->mutex);
	if (tcm->fb_notifier.notifier_call) {
		msm_drm_unregister_client(&tcm->fb_notifier);
	}
	syna_tcm_buf_release(&tcm->event_data);
	syna_pal_mutex_free(&tcm->tp_event_mutex);
	syna_tcm_remove_device(tcm->tcm_dev);
	syna_pal_mem_free((void *)tcm);
}

static void syna_dev_shutdown(struct platform_device *pdev)
{
	syna_dev_remove(pdev);
}

enum dynamic_config_id {
	DC_UNKNOWN_1 = 0x00,
	DC_NO_DOZE,
	DC_DISABLE_NOISE_MITIGATION_1,
	DC_INHIBIT_FREQUENCY_SHIFT,
	DC_REQUESTED_FREQUENCY,
	DC_DISABLE_HSYNC_1,
	DC_REZERO_ON_EXIT_DEEP_SLEEP_1,
	DC_CHARGER_CONNECTED,
	DC_NO_BASELINE_RELAXATION,
	DC_IN_WAKEUP_GESTURE_MODE,
	DC_STIMULUS_FINGERS,
	DC_GRIP_SUPPRESSION_ENABLED,
	DC_ENABLE_THICK_GLOVE_1,
	DC_ENABLE_GLOVE_1,
	DC_PS_STATUS = 0xC1,
	DC_DISABLE_ESD = 0xC2,
	DC_FREQUENCE_HOPPING = 0xD2,
	DC_TOUCH_HOLD = 0xD4,
	DC_ERROR_PRIORITY = 0xD5,
	DC_NOISE_LENGTH = 0xD6,
	DC_GRIP_CONDTION_ZONE = 0xD8,
	DC_GRIP_SPECIAL_ZONE_X = 0xD9,
	DC_GRIP_SPECIAL_ZONE_Y = 0xDA,
	DC_GRIP_SPECIAL_ZONE_L = 0xDB,
	DC_GRIP_ROATE_TO_HORIZONTAL_LEVEL = 0xDC,
	DC_DARK_ZONE_ENABLE = 0xDD,
	DC_GRIP_ENABLED = 0xDE,
	DC_GRIP_DARK_ZONE_X = 0xDF,
	DC_GRIP_DARK_ZONE_Y = 0xE0,
	DC_GRIP_ABS_DARK_X = 0xE1,
	DC_GRIP_ABS_DARK_Y = 0xE2,
	DC_GRIP_ABS_DARK_U = 0xE3,
	DC_GRIP_ABS_DARK_V = 0xE4,
	DC_GRIP_ABS_DARK_SEL = 0xE5,
	DC_SET_REPORT_FRE = 0xE6,
	DC_GESTURE_MASK = 0xFE,
	DC_LOW_TEMP_ENABLE = 0xFD,
};

static void syna_main_register(struct seq_file *s, void *chip_data)
{
	return;
}

static void syna_tcm_format_print(struct seq_file *s,
				  struct syna_tcm *tcm_info, char *buffer)
{
	unsigned int row, col;
	unsigned int rows, cols;
	unsigned int cnt = 0;
	short *pdata_16;
	struct syna_tcm_test *test_hcd = tcm_info->test_hcd;

	rows = le2_to_uint(tcm_info->tcm_dev->app_info.num_of_image_rows);
	cols = le2_to_uint(tcm_info->tcm_dev->app_info.num_of_image_cols);


	if (buffer == NULL) {
		pdata_16 = (short *)&test_hcd->report.buf[0];
	} else {
		pdata_16 = (short *)buffer;
	}

	for (row = 0; row < rows; row++) {
		seq_printf(s, "[%02d] ", row);
		for (col = 0; col < cols; col++) {
			seq_printf(s, "%5d ", *pdata_16);
			pdata_16++;
		}
		seq_printf(s, "\n");
	}

	if (test_hcd->report.data_length == rows * cols * 2 + (rows + cols) * 2) {
		for (cnt = 0; cnt < rows + cols; cnt++) {
			seq_printf(s, "%5d ", *pdata_16);
			pdata_16++;
		}
	}

	seq_printf(s, "\n");

	return;
}

static void syna_tcm_test_report(struct syna_tcm *tcm_info, u32 code)
{
	int retval;
	unsigned int offset, report_size;
	struct syna_tcm_test *test_hcd = tcm_info->test_hcd;

	if (code != test_hcd->report_type) {
		return;
	}

	report_size = tcm_info->event_data.data_length;
	LOCK_BUFFER(test_hcd->report);

	if (test_hcd->report_index == 0) {
		retval = syna_tcm_alloc_mem(&test_hcd->report, report_size * test_hcd->num_of_reports);
		if (retval < 0) {
			UNLOCK_BUFFER(test_hcd->report);
			return;
		}
	}

	if (test_hcd->report_index < test_hcd->num_of_reports) {
		offset = report_size * test_hcd->report_index;
		retval = tp_memcpy(test_hcd->report.buf + offset,
			test_hcd->report.buf_size - offset,
			tcm_info->event_data.buf,
			tcm_info->event_data.buf_size,
			tcm_info->event_data.data_length);
		if (retval < 0) {
			UNLOCK_BUFFER(test_hcd->report);
			return;
		}

		test_hcd->report_index++;
		test_hcd->report.data_length += report_size;
	}

	UNLOCK_BUFFER(test_hcd->report);

	if (test_hcd->report_index == test_hcd->num_of_reports) {
		complete(&tcm_info->report_complete);
	}

	return;
}

#define REPORT_TIMEOUT_MS       1000

static int syna_tcm_collect_reports(struct syna_tcm *tcm_info, enum tcm_report_type report_type, unsigned int num_of_reports) {
	int retval;
	bool completed = false;
	struct syna_tcm_test *test_hcd = tcm_info->test_hcd;
	unsigned char out[2] = {0};
	unsigned char resp_code;
	unsigned int timeout;
	test_hcd->report_index = 0;
	test_hcd->report_type = report_type;
	test_hcd->num_of_reports = num_of_reports;
	reinit_completion(&tcm_info->report_complete);
	out[0] = test_hcd->report_type;
	retval = tcm_info->tcm_dev->write_message(tcm_info->tcm_dev, CMD_ENABLE_REPORT, out, 1, &resp_code, tcm_info->tcm_dev->msg_data.default_resp_reading);
	if (retval < 0) {
		completed = false;
		return retval;
	}
	timeout = REPORT_TIMEOUT_MS * num_of_reports;
	retval = wait_for_completion_timeout(&tcm_info->report_complete, msecs_to_jiffies(timeout));
	if (retval != 0) {
		completed = true;
	}
	out[0] = test_hcd->report_type;
	retval = tcm_info->tcm_dev->write_message(tcm_info->tcm_dev, CMD_DISABLE_REPORT, out, 1, &resp_code, tcm_info->tcm_dev->msg_data.default_resp_reading);
	if (!completed) {
		retval = -EIO;
	}
	return retval;
}

static void syna_delta_read(struct seq_file *s, void *chip_data) {
	int retval;
	struct syna_tcm *tcm_info = (struct syna_tcm *)chip_data;
	syna_tcm_set_dynamic_config(tcm_info->tcm_dev, DC_NO_DOZE, 1, 0);
	msleep(20); 
	retval = syna_tcm_collect_reports(tcm_info, REPORT_DELTA, 1);
	if (retval < 0) {
		seq_printf(s, "Failed to read delta data\n");
		return;
	}
	syna_tcm_format_print(s, tcm_info, NULL);
	syna_tcm_set_dynamic_config(tcm_info->tcm_dev, DC_NO_DOZE, 0, 0);
	return;
}


static void syna_baseline_read(struct seq_file *s, void *chip_data) {
	int retval;
	struct syna_tcm *tcm_info = (struct syna_tcm *)chip_data;
	syna_tcm_set_dynamic_config(tcm_info->tcm_dev, DC_NO_DOZE, 1, 0);
	msleep(20);
	retval = syna_tcm_collect_reports(tcm_info, REPORT_RAW, 1);
	if (retval < 0) {
		seq_printf(s, "Failed to read baseline data\n");
		return;
	}
	syna_tcm_format_print(s, tcm_info, NULL);
	syna_tcm_set_dynamic_config(tcm_info->tcm_dev, DC_NO_DOZE, 0, 0);
	return;
}

static void syna_reserve_read(struct seq_file *s, void *chip_data) {
	int retval;
	struct syna_tcm *tcm_info = (struct syna_tcm *)chip_data;
	syna_tcm_set_dynamic_config(tcm_info->tcm_dev, DC_NO_DOZE, 1, 0);
	msleep(20); 
	retval = syna_tcm_collect_reports(tcm_info, REPORT_DEBUG, 1);
	if (retval < 0) {
		seq_printf(s, "Failed to read delta data\n");
		return;
	}
	syna_tcm_format_print(s, tcm_info, NULL);
	syna_tcm_set_dynamic_config(tcm_info->tcm_dev, DC_NO_DOZE, 0, 0);
	return;
}

static int syna_spi_suspend(struct device *dev) {
	struct syna_tcm *tcm = dev_get_drvdata(dev);
	struct syna_hw_attn_data *attn;
	if (!tcm || !tcm->hw_if) {
		return 0;
	}
	attn = &tcm->hw_if->bdata_attn;
	if (attn->irq_id == 0)
		return 0;
	tcm->bus_ready = false;
	if (tcm->lpwg_enabled && !tcm->irq_wake) {
		enable_irq_wake(attn->irq_id);
		tcm->irq_wake = true;
		return 0;
	}
	if (tcm->hw_if->ops_enable_irq)
		tcm->hw_if->ops_enable_irq(tcm->hw_if, false);
	return 0;
}

static int syna_spi_resume(struct device *dev) {
	struct syna_tcm *tcm = dev_get_drvdata(dev);
	struct syna_hw_attn_data *attn;

	if (!tcm || !tcm->hw_if) {
		return 0;
	}
	attn = &tcm->hw_if->bdata_attn;
	if (attn->irq_id == 0)
		return 0;
	if (tcm->lpwg_enabled && tcm->irq_wake) {
		disable_irq_wake(attn->irq_id);
		tcm->irq_wake = false;
	}
	if (tcm->hw_if->ops_enable_irq)
		tcm->hw_if->ops_enable_irq(tcm->hw_if, true);
	tcm->bus_ready = true;
	if (tcm->lpwg_enabled) {
		wake_up_interruptible(&tcm->wait);
	}
	return 0;
}

static const struct dev_pm_ops syna_dev_pm_ops = {
	.suspend = syna_spi_suspend,
	.resume = syna_spi_resume,
};

static struct platform_driver syna_dev_driver = {
	.driver = {
		.name = PLATFORM_DRIVER_NAME,
		.owner = THIS_MODULE,
		.pm = &syna_dev_pm_ops,
	},
	.probe = syna_dev_probe,
	.remove = syna_dev_remove,
	.shutdown = syna_dev_shutdown,
};

static int __init syna_dev_module_init(void) {
	int retval;
	retval = syna_hw_interface_init();
	if (retval < 0)
		return retval;
	return platform_driver_register(&syna_dev_driver);
}

static void __exit syna_dev_module_exit(void) {
	platform_driver_unregister(&syna_dev_driver);
	syna_hw_interface_exit();
}

module_init(syna_dev_module_init);
module_exit(syna_dev_module_exit);

MODULE_AUTHOR("Synaptics, Inc.");
MODULE_DESCRIPTION("Synaptics TCM Touch Driver");
MODULE_LICENSE("GPL v2");

