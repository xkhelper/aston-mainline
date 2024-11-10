// SPDX-License-Identifier: GPL-2.0
/*
 * Turris Mox rWTM firmware driver
 *
 * Copyright (C) 2019, 2024 Marek Behún <kabel@kernel.org>
 */

<<<<<<< HEAD
#include <linux/armada-37xx-rwtm-mailbox.h>
#include <linux/completion.h>
#include <linux/debugfs.h>
#include <linux/dma-mapping.h>
#include <linux/hw_random.h>
#include <linux/mailbox_client.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#define DRIVER_NAME		"turris-mox-rwtm"

=======
#include <crypto/sha2.h>
#include <linux/align.h>
#include <linux/armada-37xx-rwtm-mailbox.h>
#include <linux/completion.h>
#include <linux/container_of.h>
#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/hw_random.h>
#include <linux/if_ether.h>
#include <linux/kobject.h>
#include <linux/mailbox_client.h>
#include <linux/minmax.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/sizes.h>
#include <linux/sysfs.h>
#include <linux/types.h>

#define DRIVER_NAME		"turris-mox-rwtm"

#define RWTM_DMA_BUFFER_SIZE	SZ_4K

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
/*
 * The macros and constants below come from Turris Mox's rWTM firmware code.
 * This firmware is open source and it's sources can be found at
 * https://gitlab.labs.nic.cz/turris/mox-boot-builder/tree/master/wtmi.
 */

<<<<<<< HEAD
=======
#define MOX_ECC_NUMBER_WORDS	17
#define MOX_ECC_NUMBER_LEN	(MOX_ECC_NUMBER_WORDS * sizeof(u32))

#define MOX_ECC_SIGNATURE_WORDS	(2 * MOX_ECC_NUMBER_WORDS)

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#define MBOX_STS_SUCCESS	(0 << 30)
#define MBOX_STS_FAIL		(1 << 30)
#define MBOX_STS_BADCMD		(2 << 30)
#define MBOX_STS_ERROR(s)	((s) & (3 << 30))
#define MBOX_STS_VALUE(s)	(((s) >> 10) & 0xfffff)
#define MBOX_STS_CMD(s)		((s) & 0x3ff)

enum mbox_cmd {
	MBOX_CMD_GET_RANDOM	= 1,
	MBOX_CMD_BOARD_INFO	= 2,
	MBOX_CMD_ECDSA_PUB_KEY	= 3,
	MBOX_CMD_HASH		= 4,
	MBOX_CMD_SIGN		= 5,
	MBOX_CMD_VERIFY		= 6,

	MBOX_CMD_OTP_READ	= 7,
	MBOX_CMD_OTP_WRITE	= 8,
};

<<<<<<< HEAD
struct mox_kobject;

struct mox_rwtm {
	struct device *dev;
	struct mbox_client mbox_client;
	struct mbox_chan *mbox;
	struct mox_kobject *kobj;
=======
struct mox_rwtm {
	struct mbox_client mbox_client;
	struct mbox_chan *mbox;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct hwrng hwrng;

	struct armada_37xx_rwtm_rx_msg reply;

	void *buf;
	dma_addr_t buf_phys;

	struct mutex busy;
	struct completion cmd_done;

	/* board information */
<<<<<<< HEAD
	int has_board_info;
	u64 serial_number;
	int board_version, ram_size;
	u8 mac_address1[6], mac_address2[6];

	/* public key burned in eFuse */
	int has_pubkey;
=======
	bool has_board_info;
	u64 serial_number;
	int board_version, ram_size;
	u8 mac_address1[ETH_ALEN], mac_address2[ETH_ALEN];

	/* public key burned in eFuse */
	bool has_pubkey;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	u8 pubkey[135];

#ifdef CONFIG_DEBUG_FS
	/*
	 * Signature process. This is currently done via debugfs, because it
	 * does not conform to the sysfs standard "one file per attribute".
	 * It should be rewritten via crypto API once akcipher API is available
	 * from userspace.
	 */
<<<<<<< HEAD
	struct dentry *debugfs_root;
	u32 last_sig[34];
	int last_sig_done;
#endif
};

struct mox_kobject {
	struct kobject kobj;
	struct mox_rwtm *rwtm;
};

static inline struct kobject *rwtm_to_kobj(struct mox_rwtm *rwtm)
{
	return &rwtm->kobj->kobj;
}

static inline struct mox_rwtm *to_rwtm(struct kobject *kobj)
{
	return container_of(kobj, struct mox_kobject, kobj)->rwtm;
}

static void mox_kobj_release(struct kobject *kobj)
{
	kfree(to_rwtm(kobj)->kobj);
}

static const struct kobj_type mox_kobj_ktype = {
	.release	= mox_kobj_release,
	.sysfs_ops	= &kobj_sysfs_ops,
};

static int mox_kobj_create(struct mox_rwtm *rwtm)
{
	rwtm->kobj = kzalloc(sizeof(*rwtm->kobj), GFP_KERNEL);
	if (!rwtm->kobj)
		return -ENOMEM;

	kobject_init(rwtm_to_kobj(rwtm), &mox_kobj_ktype);
	if (kobject_add(rwtm_to_kobj(rwtm), firmware_kobj, "turris-mox-rwtm")) {
		kobject_put(rwtm_to_kobj(rwtm));
		return -ENXIO;
	}

	rwtm->kobj->rwtm = rwtm;

	return 0;
=======
	u32 last_sig[MOX_ECC_SIGNATURE_WORDS];
	bool last_sig_done;
#endif
};

static inline struct device *rwtm_dev(struct mox_rwtm *rwtm)
{
	return rwtm->mbox_client.dev;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

#define MOX_ATTR_RO(name, format, cat)				\
static ssize_t							\
<<<<<<< HEAD
name##_show(struct kobject *kobj, struct kobj_attribute *a,	\
	    char *buf)						\
{								\
	struct mox_rwtm *rwtm = to_rwtm(kobj);	\
	if (!rwtm->has_##cat)					\
		return -ENODATA;				\
	return sprintf(buf, format, rwtm->name);		\
}								\
static struct kobj_attribute mox_attr_##name = __ATTR_RO(name)
=======
name##_show(struct device *dev, struct device_attribute *a,	\
	    char *buf)						\
{								\
	struct mox_rwtm *rwtm = dev_get_drvdata(dev);		\
	if (!rwtm->has_##cat)					\
		return -ENODATA;				\
	return sysfs_emit(buf, format, rwtm->name);		\
}								\
static DEVICE_ATTR_RO(name)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

MOX_ATTR_RO(serial_number, "%016llX\n", board_info);
MOX_ATTR_RO(board_version, "%i\n", board_info);
MOX_ATTR_RO(ram_size, "%i\n", board_info);
MOX_ATTR_RO(mac_address1, "%pM\n", board_info);
MOX_ATTR_RO(mac_address2, "%pM\n", board_info);
MOX_ATTR_RO(pubkey, "%s\n", pubkey);

<<<<<<< HEAD
=======
static struct attribute *turris_mox_rwtm_attrs[] = {
	&dev_attr_serial_number.attr,
	&dev_attr_board_version.attr,
	&dev_attr_ram_size.attr,
	&dev_attr_mac_address1.attr,
	&dev_attr_mac_address2.attr,
	&dev_attr_pubkey.attr,
	NULL
};
ATTRIBUTE_GROUPS(turris_mox_rwtm);

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static int mox_get_status(enum mbox_cmd cmd, u32 retval)
{
	if (MBOX_STS_CMD(retval) != cmd)
		return -EIO;
	else if (MBOX_STS_ERROR(retval) == MBOX_STS_FAIL)
		return -(int)MBOX_STS_VALUE(retval);
	else if (MBOX_STS_ERROR(retval) == MBOX_STS_BADCMD)
<<<<<<< HEAD
		return -ENOSYS;
=======
		return -EOPNOTSUPP;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	else if (MBOX_STS_ERROR(retval) != MBOX_STS_SUCCESS)
		return -EIO;
	else
		return MBOX_STS_VALUE(retval);
}

<<<<<<< HEAD
static const struct attribute *mox_rwtm_attrs[] = {
	&mox_attr_serial_number.attr,
	&mox_attr_board_version.attr,
	&mox_attr_ram_size.attr,
	&mox_attr_mac_address1.attr,
	&mox_attr_mac_address2.attr,
	&mox_attr_pubkey.attr,
	NULL
};

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static void mox_rwtm_rx_callback(struct mbox_client *cl, void *data)
{
	struct mox_rwtm *rwtm = dev_get_drvdata(cl->dev);
	struct armada_37xx_rwtm_rx_msg *msg = data;

	if (completion_done(&rwtm->cmd_done))
		return;

	rwtm->reply = *msg;
	complete(&rwtm->cmd_done);
}

<<<<<<< HEAD
=======
static int mox_rwtm_exec(struct mox_rwtm *rwtm, enum mbox_cmd cmd,
			 struct armada_37xx_rwtm_tx_msg *msg,
			 bool interruptible)
{
	struct armada_37xx_rwtm_tx_msg _msg = {};
	int ret;

	if (!msg)
		msg = &_msg;

	msg->command = cmd;

	ret = mbox_send_message(rwtm->mbox, msg);
	if (ret < 0)
		return ret;

	if (interruptible) {
		ret = wait_for_completion_interruptible(&rwtm->cmd_done);
		if (ret < 0)
			return ret;
	} else {
		if (!wait_for_completion_timeout(&rwtm->cmd_done, HZ / 2))
			return -ETIMEDOUT;
	}

	return mox_get_status(cmd, rwtm->reply.retval);
}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static void reply_to_mac_addr(u8 *mac, u32 t1, u32 t2)
{
	mac[0] = t1 >> 8;
	mac[1] = t1;
	mac[2] = t2 >> 24;
	mac[3] = t2 >> 16;
	mac[4] = t2 >> 8;
	mac[5] = t2;
}

static int mox_get_board_info(struct mox_rwtm *rwtm)
{
<<<<<<< HEAD
	struct armada_37xx_rwtm_tx_msg msg;
	struct armada_37xx_rwtm_rx_msg *reply = &rwtm->reply;
	int ret;

	msg.command = MBOX_CMD_BOARD_INFO;
	ret = mbox_send_message(rwtm->mbox, &msg);
	if (ret < 0)
		return ret;

	if (!wait_for_completion_timeout(&rwtm->cmd_done, HZ / 2))
		return -ETIMEDOUT;

	ret = mox_get_status(MBOX_CMD_BOARD_INFO, reply->retval);
	if (ret == -ENODATA) {
		dev_warn(rwtm->dev,
			 "Board does not have manufacturing information burned!\n");
	} else if (ret == -ENOSYS) {
		dev_notice(rwtm->dev,
=======
	struct device *dev = rwtm_dev(rwtm);
	struct armada_37xx_rwtm_rx_msg *reply = &rwtm->reply;
	int ret;

	ret = mox_rwtm_exec(rwtm, MBOX_CMD_BOARD_INFO, NULL, false);
	if (ret == -ENODATA) {
		dev_warn(dev,
			 "Board does not have manufacturing information burned!\n");
	} else if (ret == -EOPNOTSUPP) {
		dev_notice(dev,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			   "Firmware does not support the BOARD_INFO command\n");
	} else if (ret < 0) {
		return ret;
	} else {
		rwtm->serial_number = reply->status[1];
		rwtm->serial_number <<= 32;
		rwtm->serial_number |= reply->status[0];
		rwtm->board_version = reply->status[2];
		rwtm->ram_size = reply->status[3];
		reply_to_mac_addr(rwtm->mac_address1, reply->status[4],
				  reply->status[5]);
		reply_to_mac_addr(rwtm->mac_address2, reply->status[6],
				  reply->status[7]);
<<<<<<< HEAD
		rwtm->has_board_info = 1;
=======
		rwtm->has_board_info = true;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

		pr_info("Turris Mox serial number %016llX\n",
			rwtm->serial_number);
		pr_info("           board version %i\n", rwtm->board_version);
		pr_info("           burned RAM size %i MiB\n", rwtm->ram_size);
	}

<<<<<<< HEAD
	msg.command = MBOX_CMD_ECDSA_PUB_KEY;
	ret = mbox_send_message(rwtm->mbox, &msg);
	if (ret < 0)
		return ret;

	if (!wait_for_completion_timeout(&rwtm->cmd_done, HZ / 2))
		return -ETIMEDOUT;

	ret = mox_get_status(MBOX_CMD_ECDSA_PUB_KEY, reply->retval);
	if (ret == -ENODATA) {
		dev_warn(rwtm->dev, "Board has no public key burned!\n");
	} else if (ret == -ENOSYS) {
		dev_notice(rwtm->dev,
=======
	ret = mox_rwtm_exec(rwtm, MBOX_CMD_ECDSA_PUB_KEY, NULL, false);
	if (ret == -ENODATA) {
		dev_warn(dev, "Board has no public key burned!\n");
	} else if (ret == -EOPNOTSUPP) {
		dev_notice(dev,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			   "Firmware does not support the ECDSA_PUB_KEY command\n");
	} else if (ret < 0) {
		return ret;
	} else {
		u32 *s = reply->status;

<<<<<<< HEAD
		rwtm->has_pubkey = 1;
=======
		rwtm->has_pubkey = true;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		sprintf(rwtm->pubkey,
			"%06x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x",
			ret, s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7],
			s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15]);
	}

	return 0;
}

static int check_get_random_support(struct mox_rwtm *rwtm)
{
<<<<<<< HEAD
	struct armada_37xx_rwtm_tx_msg msg;
	int ret;

	msg.command = MBOX_CMD_GET_RANDOM;
	msg.args[0] = 1;
	msg.args[1] = rwtm->buf_phys;
	msg.args[2] = 4;

	ret = mbox_send_message(rwtm->mbox, &msg);
	if (ret < 0)
		return ret;

	if (!wait_for_completion_timeout(&rwtm->cmd_done, HZ / 2))
		return -ETIMEDOUT;

	return mox_get_status(MBOX_CMD_GET_RANDOM, rwtm->reply.retval);
=======
	struct armada_37xx_rwtm_tx_msg msg = {
		.args = { 1, rwtm->buf_phys, 4 },
	};

	return mox_rwtm_exec(rwtm, MBOX_CMD_GET_RANDOM, &msg, false);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static int mox_hwrng_read(struct hwrng *rng, void *data, size_t max, bool wait)
{
<<<<<<< HEAD
	struct mox_rwtm *rwtm = (struct mox_rwtm *) rng->priv;
	struct armada_37xx_rwtm_tx_msg msg;
	int ret;

	if (max > 4096)
		max = 4096;

	msg.command = MBOX_CMD_GET_RANDOM;
	msg.args[0] = 1;
	msg.args[1] = rwtm->buf_phys;
	msg.args[2] = (max + 3) & ~3;
=======
	struct mox_rwtm *rwtm = container_of(rng, struct mox_rwtm, hwrng);
	struct armada_37xx_rwtm_tx_msg msg = {
		.args = { 1, rwtm->buf_phys, ALIGN(max, 4) },
	};
	int ret;

	max = min(max, RWTM_DMA_BUFFER_SIZE);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if (!wait) {
		if (!mutex_trylock(&rwtm->busy))
			return -EBUSY;
	} else {
		mutex_lock(&rwtm->busy);
	}

<<<<<<< HEAD
	ret = mbox_send_message(rwtm->mbox, &msg);
	if (ret < 0)
		goto unlock_mutex;

	ret = wait_for_completion_interruptible(&rwtm->cmd_done);
	if (ret < 0)
		goto unlock_mutex;

	ret = mox_get_status(MBOX_CMD_GET_RANDOM, rwtm->reply.retval);
=======
	ret = mox_rwtm_exec(rwtm, MBOX_CMD_GET_RANDOM, &msg, true);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret < 0)
		goto unlock_mutex;

	memcpy(data, rwtm->buf, max);
	ret = max;

unlock_mutex:
	mutex_unlock(&rwtm->busy);
	return ret;
}

#ifdef CONFIG_DEBUG_FS
static int rwtm_debug_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;

	return nonseekable_open(inode, file);
}

static ssize_t do_sign_read(struct file *file, char __user *buf, size_t len,
			    loff_t *ppos)
{
	struct mox_rwtm *rwtm = file->private_data;
	ssize_t ret;

<<<<<<< HEAD
	/* only allow one read, of 136 bytes, from position 0 */
	if (*ppos != 0)
		return 0;

	if (len < 136)
=======
	/* only allow one read, of whole signature, from position 0 */
	if (*ppos != 0)
		return 0;

	if (len < sizeof(rwtm->last_sig))
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		return -EINVAL;

	if (!rwtm->last_sig_done)
		return -ENODATA;

<<<<<<< HEAD
	/* 2 arrays of 17 32-bit words are 136 bytes */
	ret = simple_read_from_buffer(buf, len, ppos, rwtm->last_sig, 136);
	rwtm->last_sig_done = 0;
=======
	ret = simple_read_from_buffer(buf, len, ppos, rwtm->last_sig,
				      sizeof(rwtm->last_sig));
	rwtm->last_sig_done = false;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return ret;
}

static ssize_t do_sign_write(struct file *file, const char __user *buf,
			     size_t len, loff_t *ppos)
{
	struct mox_rwtm *rwtm = file->private_data;
<<<<<<< HEAD
	struct armada_37xx_rwtm_rx_msg *reply = &rwtm->reply;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct armada_37xx_rwtm_tx_msg msg;
	loff_t dummy = 0;
	ssize_t ret;

<<<<<<< HEAD
	/* the input is a SHA-512 hash, so exactly 64 bytes have to be read */
	if (len != 64)
=======
	if (len != SHA512_DIGEST_SIZE)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		return -EINVAL;

	/* if last result is not zero user has not read that information yet */
	if (rwtm->last_sig_done)
		return -EBUSY;

	if (!mutex_trylock(&rwtm->busy))
		return -EBUSY;

	/*
	 * Here we have to send:
	 *   1. Address of the input to sign.
	 *      The input is an array of 17 32-bit words, the first (most
	 *      significat) is 0, the rest 16 words are copied from the SHA-512
	 *      hash given by the user and converted from BE to LE.
	 *   2. Address of the buffer where ECDSA signature value R shall be
	 *      stored by the rWTM firmware.
	 *   3. Address of the buffer where ECDSA signature value S shall be
	 *      stored by the rWTM firmware.
	 */
<<<<<<< HEAD
	memset(rwtm->buf, 0, 4);
	ret = simple_write_to_buffer(rwtm->buf + 4, 64, &dummy, buf, len);
	if (ret < 0)
		goto unlock_mutex;
	be32_to_cpu_array(rwtm->buf, rwtm->buf, 17);

	msg.command = MBOX_CMD_SIGN;
	msg.args[0] = 1;
	msg.args[1] = rwtm->buf_phys;
	msg.args[2] = rwtm->buf_phys + 68;
	msg.args[3] = rwtm->buf_phys + 2 * 68;
	ret = mbox_send_message(rwtm->mbox, &msg);
	if (ret < 0)
		goto unlock_mutex;

	ret = wait_for_completion_interruptible(&rwtm->cmd_done);
	if (ret < 0)
		goto unlock_mutex;

	ret = MBOX_STS_VALUE(reply->retval);
	if (MBOX_STS_ERROR(reply->retval) != MBOX_STS_SUCCESS)
		goto unlock_mutex;

=======
	memset(rwtm->buf, 0, sizeof(u32));
	ret = simple_write_to_buffer(rwtm->buf + sizeof(u32),
				     SHA512_DIGEST_SIZE, &dummy, buf, len);
	if (ret < 0)
		goto unlock_mutex;
	be32_to_cpu_array(rwtm->buf, rwtm->buf, MOX_ECC_NUMBER_WORDS);

	msg.args[0] = 1;
	msg.args[1] = rwtm->buf_phys;
	msg.args[2] = rwtm->buf_phys + MOX_ECC_NUMBER_LEN;
	msg.args[3] = rwtm->buf_phys + 2 * MOX_ECC_NUMBER_LEN;

	ret = mox_rwtm_exec(rwtm, MBOX_CMD_SIGN, &msg, true);
	if (ret < 0)
		goto unlock_mutex;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	/*
	 * Here we read the R and S values of the ECDSA signature
	 * computed by the rWTM firmware and convert their words from
	 * LE to BE.
	 */
<<<<<<< HEAD
	memcpy(rwtm->last_sig, rwtm->buf + 68, 136);
	cpu_to_be32_array(rwtm->last_sig, rwtm->last_sig, 34);
	rwtm->last_sig_done = 1;
=======
	memcpy(rwtm->last_sig, rwtm->buf + MOX_ECC_NUMBER_LEN,
	       sizeof(rwtm->last_sig));
	cpu_to_be32_array(rwtm->last_sig, rwtm->last_sig,
			  MOX_ECC_SIGNATURE_WORDS);
	rwtm->last_sig_done = true;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	mutex_unlock(&rwtm->busy);
	return len;
unlock_mutex:
	mutex_unlock(&rwtm->busy);
	return ret;
}

static const struct file_operations do_sign_fops = {
	.owner	= THIS_MODULE,
	.open	= rwtm_debug_open,
	.read	= do_sign_read,
	.write	= do_sign_write,
<<<<<<< HEAD
	.llseek	= no_llseek,
};

static int rwtm_register_debugfs(struct mox_rwtm *rwtm)
{
	struct dentry *root, *entry;

	root = debugfs_create_dir("turris-mox-rwtm", NULL);

	if (IS_ERR(root))
		return PTR_ERR(root);

	entry = debugfs_create_file_unsafe("do_sign", 0600, root, rwtm,
					   &do_sign_fops);
	if (IS_ERR(entry))
		goto err_remove;

	rwtm->debugfs_root = root;

	return 0;
err_remove:
	debugfs_remove_recursive(root);
	return PTR_ERR(entry);
}

static void rwtm_unregister_debugfs(struct mox_rwtm *rwtm)
{
	debugfs_remove_recursive(rwtm->debugfs_root);
}
#else
static inline int rwtm_register_debugfs(struct mox_rwtm *rwtm)
{
	return 0;
}

static inline void rwtm_unregister_debugfs(struct mox_rwtm *rwtm)
=======
};

static void rwtm_debugfs_release(void *root)
{
	debugfs_remove_recursive(root);
}

static void rwtm_register_debugfs(struct mox_rwtm *rwtm)
{
	struct dentry *root;

	root = debugfs_create_dir("turris-mox-rwtm", NULL);

	debugfs_create_file_unsafe("do_sign", 0600, root, rwtm, &do_sign_fops);

	devm_add_action_or_reset(rwtm_dev(rwtm), rwtm_debugfs_release, root);
}
#else
static inline void rwtm_register_debugfs(struct mox_rwtm *rwtm)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
}
#endif

<<<<<<< HEAD
=======
static void rwtm_devm_mbox_release(void *mbox)
{
	mbox_free_channel(mbox);
}

static void rwtm_firmware_symlink_drop(void *parent)
{
	sysfs_remove_link(parent, DRIVER_NAME);
}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static int turris_mox_rwtm_probe(struct platform_device *pdev)
{
	struct mox_rwtm *rwtm;
	struct device *dev = &pdev->dev;
	int ret;

	rwtm = devm_kzalloc(dev, sizeof(*rwtm), GFP_KERNEL);
	if (!rwtm)
		return -ENOMEM;

<<<<<<< HEAD
	rwtm->dev = dev;
	rwtm->buf = dmam_alloc_coherent(dev, PAGE_SIZE, &rwtm->buf_phys,
					GFP_KERNEL);
	if (!rwtm->buf)
		return -ENOMEM;

	ret = mox_kobj_create(rwtm);
	if (ret < 0) {
		dev_err(dev, "Cannot create turris-mox-rwtm kobject!\n");
		return ret;
	}

	ret = sysfs_create_files(rwtm_to_kobj(rwtm), mox_rwtm_attrs);
	if (ret < 0) {
		dev_err(dev, "Cannot create sysfs files!\n");
		goto put_kobj;
	}

	platform_set_drvdata(pdev, rwtm);

	mutex_init(&rwtm->busy);
=======
	rwtm->buf = dmam_alloc_coherent(dev, RWTM_DMA_BUFFER_SIZE,
					&rwtm->buf_phys, GFP_KERNEL);
	if (!rwtm->buf)
		return -ENOMEM;

	platform_set_drvdata(pdev, rwtm);

	ret = devm_mutex_init(dev, &rwtm->busy);
	if (ret)
		return ret;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	init_completion(&rwtm->cmd_done);

	rwtm->mbox_client.dev = dev;
	rwtm->mbox_client.rx_callback = mox_rwtm_rx_callback;

	rwtm->mbox = mbox_request_channel(&rwtm->mbox_client, 0);
<<<<<<< HEAD
	if (IS_ERR(rwtm->mbox)) {
		ret = PTR_ERR(rwtm->mbox);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "Cannot request mailbox channel: %i\n",
				ret);
		goto remove_files;
	}
=======
	if (IS_ERR(rwtm->mbox))
		return dev_err_probe(dev, PTR_ERR(rwtm->mbox),
				     "Cannot request mailbox channel!\n");

	ret = devm_add_action_or_reset(dev, rwtm_devm_mbox_release, rwtm->mbox);
	if (ret)
		return ret;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	ret = mox_get_board_info(rwtm);
	if (ret < 0)
		dev_warn(dev, "Cannot read board information: %i\n", ret);

	ret = check_get_random_support(rwtm);
	if (ret < 0) {
		dev_notice(dev,
			   "Firmware does not support the GET_RANDOM command\n");
<<<<<<< HEAD
		goto free_channel;
=======
		return ret;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}

	rwtm->hwrng.name = DRIVER_NAME "_hwrng";
	rwtm->hwrng.read = mox_hwrng_read;
<<<<<<< HEAD
	rwtm->hwrng.priv = (unsigned long) rwtm;

	ret = devm_hwrng_register(dev, &rwtm->hwrng);
	if (ret < 0) {
		dev_err(dev, "Cannot register HWRNG: %i\n", ret);
		goto free_channel;
	}

	ret = rwtm_register_debugfs(rwtm);
	if (ret < 0) {
		dev_err(dev, "Failed creating debugfs entries: %i\n", ret);
		goto free_channel;
	}

	dev_info(dev, "HWRNG successfully registered\n");

	return 0;

free_channel:
	mbox_free_channel(rwtm->mbox);
remove_files:
	sysfs_remove_files(rwtm_to_kobj(rwtm), mox_rwtm_attrs);
put_kobj:
	kobject_put(rwtm_to_kobj(rwtm));
	return ret;
}

static void turris_mox_rwtm_remove(struct platform_device *pdev)
{
	struct mox_rwtm *rwtm = platform_get_drvdata(pdev);

	rwtm_unregister_debugfs(rwtm);
	sysfs_remove_files(rwtm_to_kobj(rwtm), mox_rwtm_attrs);
	kobject_put(rwtm_to_kobj(rwtm));
	mbox_free_channel(rwtm->mbox);
=======

	ret = devm_hwrng_register(dev, &rwtm->hwrng);
	if (ret)
		return dev_err_probe(dev, ret, "Cannot register HWRNG!\n");

	rwtm_register_debugfs(rwtm);

	dev_info(dev, "HWRNG successfully registered\n");

	/*
	 * For sysfs ABI compatibility, create symlink
	 * /sys/firmware/turris-mox-rwtm to this device's sysfs directory.
	 */
	ret = sysfs_create_link(firmware_kobj, &dev->kobj, DRIVER_NAME);
	if (!ret)
		devm_add_action_or_reset(dev, rwtm_firmware_symlink_drop,
					 firmware_kobj);

	return 0;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static const struct of_device_id turris_mox_rwtm_match[] = {
	{ .compatible = "cznic,turris-mox-rwtm", },
	{ .compatible = "marvell,armada-3700-rwtm-firmware", },
	{ },
};

MODULE_DEVICE_TABLE(of, turris_mox_rwtm_match);

static struct platform_driver turris_mox_rwtm_driver = {
	.probe	= turris_mox_rwtm_probe,
<<<<<<< HEAD
	.remove_new = turris_mox_rwtm_remove,
	.driver	= {
		.name		= DRIVER_NAME,
		.of_match_table	= turris_mox_rwtm_match,
=======
	.driver	= {
		.name		= DRIVER_NAME,
		.of_match_table	= turris_mox_rwtm_match,
		.dev_groups	= turris_mox_rwtm_groups,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
};
module_platform_driver(turris_mox_rwtm_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Turris Mox rWTM firmware driver");
MODULE_AUTHOR("Marek Behun <kabel@kernel.org>");
