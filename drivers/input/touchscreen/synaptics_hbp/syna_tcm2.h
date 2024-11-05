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

#ifndef _SYNAPTICS_TCM2_DRIVER_H_
#define _SYNAPTICS_TCM2_DRIVER_H_

#define REMOVE_OPLUS_FUNCTION 1
#include "syna_tcm2_platform.h"
#include "tcm/synaptics_touchcom_core_dev.h"
#include "tcm/synaptics_touchcom_func_touch.h"
#include "touchpanel_common.h"

#define PLATFORM_DRIVER_NAME "synaptics_tcm_hbp"

#define TOUCH_INPUT_NAME "touchpanel"
#define TOUCH_INPUT_PHYS_PATH "synaptics_tcm/touch_input"

#define CHAR_DEVICE_NAME "tcm_hbp"
#define CHAR_DEVICE_MODE (0x0600)

#define SIG_DISPLAY_ON  44
#define SIG_DISPLAY_OFF 45
#define SIG_FINGER_DOWN 46

#undef MAX_DEVICE_VERSION_LENGTH
#define MAX_DEVICE_VERSION_LENGTH 256

#define SYNAPTICS_TCM_DRIVER_ID (1 << 0)
#define SYNAPTICS_TCM_DRIVER_VERSION 1
#define SYNAPTICS_TCM_DRIVER_SUBVER "4.0"

#define ABS_TOUCH_COST_TIME_KERNEL  0x21
#define ABS_TOUCH_COST_TIME_ALGO    0x22
#define ABS_TOUCH_COST_TIME_DAEMON  0x23
#define MAX_TOUCH_COST_TIME         1000 * 1000

#define TX_NUM 17
#define RX_NUM 38

#define IRQ_COST_TIME_OVER_5MS 5000
#define IRQ_COST_TIME_OVER_10MS 10000
#define IRQ_COST_TIME_OVER_20MS 20000
#define IRQ_COST_TIME_OVER_50MS 50000

#define FW_UPDATE_COMPLETE_TIMEOUT  msecs_to_jiffies(40*1000)

#define GESTURE_MODE_SWITCH_RETRY_TIMES     5
#define MAX_HEALTH_REPORT_LEN 50

#define HAS_SYSFS_INTERFACE
#define HAS_REFLASH_FEATURE
#define HAS_ROMBOOT_REFLASH_FEATURE
#define HAS_TESTING_FEATURE

#define TYPE_B_PROTOCOL

#define RESET_ON_RESUME

#define ENABLE_WAKEUP_GESTURE 1

#define REPORT_TOUCH_WIDTH

#define STARTUP_REFLASH

#define ENABLE_DISP_NOTIFIER

#define RESUME_EARLY_UNBLANK

#define ENABLE_EXTERNAL_FRAME_PROCESS
#define REPORT_TYPES (256)
#define EFP_ENABLE	(1)
#define EFP_DISABLE (0)

#define TCM_CONNECT_IN_PROBE

#define TOUCH_BIT_CHECK           0x3FF

enum fingerprint_err_type {
	FOD_ENABLE_NO_ERROR = 0,
	FINGERPRINT_AREA_NOT_MATCH = 7,
	ANOTHER_FINGER_ON_NON_FP_ZONE = 8,
	FINGERPRINT_DOWN_BEFORE_FP_ENABLE = 9,
};

enum power_state {
	PWR_OFF = 0,
	PWR_ON,
	LOW_PWR,
	PWR_UNKNOWN,
};

enum sub_power_state {
	SUB_NONE = 0,
	SUB_PWR_RESUMING,
	SUB_PWR_RESUME_DONE,
	SUB_PWR_EARLY_SUSPENDING,
	SUB_PWR_SUSPENDING,
	SUB_PWR_SUSPEND_DONE,
};

enum driver_state {
	IS_PROBE = 0,
	IS_SHUTDOWN,
	IS_REMOVE,
};

enum driver_req_mode {
	DRIVER_REQ_MODE_NONE    = 0,
	DRIVER_REQ_MODE_SUSPEND = 1,
};

enum daemon_states {
	STATE_NONE = 0,
	STATE_START = 1,
	STATE_INIT_DEV = 2,
	STATE_SCREEN_ON = 3,
	STATE_SCREEN_OFF = 4,
	STATE_RUN = 5,
	STATE_TERMINATE,
};

struct tcm_engineer_test_operations {
	int (*auto_test)(struct seq_file *s,  struct device *dev);
};

static inline unsigned int le2_to_uint(const unsigned char *src)
{
	return (unsigned int)src[0] +
	       (unsigned int)src[1] * 0x100;
}

static inline unsigned int le4_to_uint(const unsigned char *src)
{
	return (unsigned int)src[0] +
	       (unsigned int)src[1] * 0x100 +
	       (unsigned int)src[2] * 0x10000 +
	       (unsigned int)src[3] * 0x1000000;
}

struct syna_tcm_test {
	unsigned int num_of_reports;
	unsigned char report_type;
	unsigned int report_index;
	struct tcm_buffer report;
	struct tcm_buffer test_resp;
	struct tcm_buffer test_out;
};
struct syna_tcm {

	int tp_index;
	struct tcm_dev *tcm_dev;
	struct syna_tcm_test *test_hcd;
	struct completion report_complete;
	struct platform_device *pdev;
	struct tcm_touch_data_blob tp_data;
	syna_pal_mutex_t tp_event_mutex;
	syna_pal_mutex_t extif_mutex;
	struct mutex		mutex;
	unsigned char prev_obj_status[MAX_NUM_OBJECTS];
	struct tcm_buffer event_data;
	struct syna_hw_interface *hw_if;
	struct hw_resource hw_res;
	pid_t isr_pid;
	bool irq_wake;
	int irq_cost_time;
	struct cdev char_dev;
	dev_t char_dev_num;
	int char_dev_ref_count;
	struct class *device_class;
	struct device *device;
	struct kobject *sysfs_dir;
	struct proc_dir_entry *prEntry_tp;
	struct proc_dir_entry *prEntry_debug_tp;
	struct input_dev *input_dev;
	struct input_params {
		unsigned int max_x;
		unsigned int max_y;
		unsigned int max_objects;
	} input_dev_params;
	int tx_num;
	int rx_num;
	int dts_max_x;
	int dts_max_y;
	unsigned int firmware_max_x;
	unsigned int firmware_max_y;
	struct panel_info panel_data;
	char *fw_name_fae;
	char algo_version[MAX_DEVICE_VERSION_LENGTH];
	int firmware_update_type;
	struct completion fw_complete;
	struct delayed_work reflash_work;
	struct workqueue_struct *reflash_workqueue;
	pid_t proc_pid;
	struct task_struct *proc_task;
	int probe_done;
	int pwr_state;
	int sub_pwr_state;
	bool slept_in_early_suspend;
	bool lpwg_enabled;
	bool is_attn_redirecting;
	unsigned char fb_ready;
	bool is_connected;
	bool has_custom_tp_config;
	bool helper_enabled;
	bool startup_reflash_enabled;
	bool rst_on_resume_enabled;
	bool hbp_enabled;
	int daemon_state;
	int primary_timestamp_enabled;
	int driver_current_state;
	unsigned int waiting_frame;
	unsigned int wait_for_ioctl_operation;
	unsigned int use_short_frame_waiting;
	bool snr_read_support;
	bool freq_hop_simulate_support;
	int frame_over_cnt_report_en;
	unsigned short gesture_type;
	unsigned short touch_and_hold;
	bool is_fp_down;
	struct fp_underscreen_info fp_info;
	bool fp_active;
	struct notifier_block fb_notifier;
	int aging_test;
	int aging_mode;
	struct aging_test_proc_operations  *aging_test_ops;
	struct debug_info_proc_operations *debug_info_ops;
	void *chip_data;
	struct com_api_data com_api_data;
	struct com_test_data com_test_data;
	struct tcm_engineer_test_operations   *engineer_ops;
	bool in_test_process;
	bool health_monitor_support;
	struct monitor_data monitor_data;
	bool exception_upload_support;
	struct exception_data exception_data;
	unsigned int fifo_remaining_frame;
	struct list_head frame_fifo_queue;
	wait_queue_head_t wait_frame;
	unsigned char report_to_queue[REPORT_TYPES];
	struct work_struct speed_up_work;
	struct workqueue_struct *speedup_resume_wq;
	bool bus_ready;
	wait_queue_head_t wait;
	void *userspace_app_info;
	int (*dev_connect)(struct syna_tcm *tcm);
	int (*dev_disconnect)(struct syna_tcm *tcm);
	int (*dev_set_up_app_fw)(struct syna_tcm *tcm);
	int (*dev_resume)(struct device *dev);
	int (*dev_suspend)(struct device *dev);
};

int syna_dev_disable_hbp_mode(struct syna_tcm *tcm);
void syna_dev_update_lpwg_status(struct syna_tcm *tcm);
void syna_send_signal(struct syna_tcm *tcm, int signal_num);
int syna_dev_enable_lowpwr_gesture(struct syna_tcm *tcm, bool en);
#ifdef HAS_SYSFS_INTERFACE

int syna_cdev_create_sysfs(struct syna_tcm *ptcm,
		struct platform_device *pdev);

void syna_cdev_remove_sysfs(struct syna_tcm *ptcm);

void syna_cdev_redirect_attn(struct syna_tcm *ptcm);

#ifdef ENABLE_EXTERNAL_FRAME_PROCESS
void syna_cdev_update_report_queue(struct syna_tcm *tcm,
		unsigned char code, struct tcm_buffer *pevent_data);
void syna_cdev_update_power_state_report_queue(struct syna_tcm *tcm, bool wakeup);
#endif

#endif



#define INIT_BUFFER(buffer, is_clone) \
	mutex_init(&buffer.buf_mutex); \

#define LOCK_BUFFER(buffer) \
	mutex_lock(&buffer.buf_mutex)

#define UNLOCK_BUFFER(buffer) \
	mutex_unlock(&buffer.buf_mutex)

#define RELEASE_BUFFER(buffer) \
	do { \
        if (buffer.clone == false) { \
			kfree(buffer.buf); \
			buffer.buf_size = 0; \
			buffer.data_length = 0; \
        } \
	} while (0)

struct syna_tcm_report {
	unsigned char id;
	struct tcm_buffer buffer;
};

static inline int syna_tcm_alloc_mem(struct tcm_buffer *buffer,
					 unsigned int size)
{
	if (size > buffer->buf_size) {
		if (buffer->buf != NULL) {
			kfree(buffer->buf);
		}
		buffer->buf = kzalloc(size, GFP_KERNEL);

		if (!(buffer->buf)) {
			buffer->buf_size = 0;
			buffer->data_length = 0;
			return -ENOMEM;
		}
		buffer->buf_size = size;
	}

	memset(buffer->buf, 0, buffer->buf_size);
	buffer->data_length = 0;

	return 0;
}

static inline int tp_memcpy(void *dest, unsigned int dest_size,
			    void *src, unsigned int src_size,
			    unsigned int count)
{
	if (dest == NULL || src == NULL) {
		return -EINVAL;
	}

	if (count > dest_size || count > src_size) {
		return -EINVAL;
	}

	memcpy((void *)dest, (void *)src, count);

	return 0;
}

#endif /* end of _SYNAPTICS_TCM2_DRIVER_H_ */

