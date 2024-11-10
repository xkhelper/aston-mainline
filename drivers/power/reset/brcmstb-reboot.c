// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2013 Broadcom Corporation

#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/notifier.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/reboot.h>
#include <linux/regmap.h>
#include <linux/smp.h>
#include <linux/mfd/syscon.h>

<<<<<<< HEAD
#define RESET_SOURCE_ENABLE_REG 1
#define SW_MASTER_RESET_REG 2

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static struct regmap *regmap;
static u32 rst_src_en;
static u32 sw_mstr_rst;

struct reset_reg_mask {
	u32 rst_src_en_mask;
	u32 sw_mstr_rst_mask;
};

static const struct reset_reg_mask *reset_masks;

<<<<<<< HEAD
static int brcmstb_restart_handler(struct notifier_block *this,
				   unsigned long mode, void *cmd)
=======
static int brcmstb_restart_handler(struct sys_off_data *data)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	int rc;
	u32 tmp;

	rc = regmap_write(regmap, rst_src_en, reset_masks->rst_src_en_mask);
	if (rc) {
		pr_err("failed to write rst_src_en (%d)\n", rc);
		return NOTIFY_DONE;
	}

	rc = regmap_read(regmap, rst_src_en, &tmp);
	if (rc) {
		pr_err("failed to read rst_src_en (%d)\n", rc);
		return NOTIFY_DONE;
	}

	rc = regmap_write(regmap, sw_mstr_rst, reset_masks->sw_mstr_rst_mask);
	if (rc) {
		pr_err("failed to write sw_mstr_rst (%d)\n", rc);
		return NOTIFY_DONE;
	}

	rc = regmap_read(regmap, sw_mstr_rst, &tmp);
	if (rc) {
		pr_err("failed to read sw_mstr_rst (%d)\n", rc);
		return NOTIFY_DONE;
	}

<<<<<<< HEAD
	while (1)
		;

	return NOTIFY_DONE;
}

static struct notifier_block brcmstb_restart_nb = {
	.notifier_call = brcmstb_restart_handler,
	.priority = 128,
};

=======
	return NOTIFY_DONE;
}

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static const struct reset_reg_mask reset_bits_40nm = {
	.rst_src_en_mask = BIT(0),
	.sw_mstr_rst_mask = BIT(0),
};

static const struct reset_reg_mask reset_bits_65nm = {
	.rst_src_en_mask = BIT(3),
	.sw_mstr_rst_mask = BIT(31),
};

<<<<<<< HEAD
static const struct of_device_id of_match[] = {
	{ .compatible = "brcm,brcmstb-reboot", .data = &reset_bits_40nm },
	{ .compatible = "brcm,bcm7038-reboot", .data = &reset_bits_65nm },
	{},
};

=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static int brcmstb_reboot_probe(struct platform_device *pdev)
{
	int rc;
	struct device_node *np = pdev->dev.of_node;
<<<<<<< HEAD
	const struct of_device_id *of_id;

	of_id = of_match_node(of_match, np);
	if (!of_id) {
		pr_err("failed to look up compatible string\n");
		return -EINVAL;
	}
	reset_masks = of_id->data;

	regmap = syscon_regmap_lookup_by_phandle(np, "syscon");
=======
	unsigned int args[2];

	reset_masks = device_get_match_data(&pdev->dev);
	if (!reset_masks) {
		pr_err("failed to get match data\n");
		return -EINVAL;
	}

	regmap = syscon_regmap_lookup_by_phandle_args(np, "syscon", ARRAY_SIZE(args), args);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (IS_ERR(regmap)) {
		pr_err("failed to get syscon phandle\n");
		return -EINVAL;
	}
<<<<<<< HEAD

	rc = of_property_read_u32_index(np, "syscon", RESET_SOURCE_ENABLE_REG,
					&rst_src_en);
	if (rc) {
		pr_err("can't get rst_src_en offset (%d)\n", rc);
		return -EINVAL;
	}

	rc = of_property_read_u32_index(np, "syscon", SW_MASTER_RESET_REG,
					&sw_mstr_rst);
	if (rc) {
		pr_err("can't get sw_mstr_rst offset (%d)\n", rc);
		return -EINVAL;
	}

	rc = register_restart_handler(&brcmstb_restart_nb);
=======
	rst_src_en = args[0];
	sw_mstr_rst = args[1];

	rc = devm_register_sys_off_handler(&pdev->dev, SYS_OFF_MODE_RESTART,
					   128, brcmstb_restart_handler, NULL);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (rc)
		dev_err(&pdev->dev,
			"cannot register restart handler (err=%d)\n", rc);

	return rc;
}

<<<<<<< HEAD
=======
static const struct of_device_id of_match[] = {
	{ .compatible = "brcm,brcmstb-reboot", .data = &reset_bits_40nm },
	{ .compatible = "brcm,bcm7038-reboot", .data = &reset_bits_65nm },
	{},
};

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
static struct platform_driver brcmstb_reboot_driver = {
	.probe = brcmstb_reboot_probe,
	.driver = {
		.name = "brcmstb-reboot",
		.of_match_table = of_match,
	},
};

static int __init brcmstb_reboot_init(void)
{
<<<<<<< HEAD
	return platform_driver_probe(&brcmstb_reboot_driver,
					brcmstb_reboot_probe);
=======
	return platform_driver_register(&brcmstb_reboot_driver);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}
subsys_initcall(brcmstb_reboot_init);
