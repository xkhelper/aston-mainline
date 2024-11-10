// SPDX-License-Identifier: GPL-2.0
/*
 * PCIe host controller driver for Freescale i.MX6 SoCs
 *
 * Copyright (C) 2013 Kosagi
 *		https://www.kosagi.com
 *
 * Author: Sean Cross <xobs@kosagi.com>
 */

#include <linux/bitfield.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/mfd/syscon/imx6q-iomuxc-gpr.h>
#include <linux/mfd/syscon/imx7-iomuxc-gpr.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/resource.h>
#include <linux/signal.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/reset.h>
<<<<<<< HEAD
=======
#include <linux/phy/pcie.h>
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#include <linux/phy/phy.h>
#include <linux/pm_domain.h>
#include <linux/pm_runtime.h>

#include "pcie-designware.h"

#define IMX8MQ_GPR_PCIE_REF_USE_PAD		BIT(9)
#define IMX8MQ_GPR_PCIE_CLK_REQ_OVERRIDE_EN	BIT(10)
#define IMX8MQ_GPR_PCIE_CLK_REQ_OVERRIDE	BIT(11)
#define IMX8MQ_GPR_PCIE_VREG_BYPASS		BIT(12)
#define IMX8MQ_GPR12_PCIE2_CTRL_DEVICE_TYPE	GENMASK(11, 8)
#define IMX8MQ_PCIE2_BASE_ADDR			0x33c00000

#define IMX95_PCIE_PHY_GEN_CTRL			0x0
#define IMX95_PCIE_REF_USE_PAD			BIT(17)

#define IMX95_PCIE_SS_RW_REG_0			0xf0
#define IMX95_PCIE_REF_CLKEN			BIT(23)
#define IMX95_PCIE_PHY_CR_PARA_SEL		BIT(9)

#define IMX95_PE0_GEN_CTRL_1			0x1050
#define IMX95_PCIE_DEVICE_TYPE			GENMASK(3, 0)

#define IMX95_PE0_GEN_CTRL_3			0x1058
#define IMX95_PCIE_LTSSM_EN			BIT(0)

<<<<<<< HEAD
#define to_imx6_pcie(x)	dev_get_drvdata((x)->dev)

enum imx6_pcie_variants {
=======
#define to_imx_pcie(x)	dev_get_drvdata((x)->dev)

enum imx_pcie_variants {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	IMX6Q,
	IMX6SX,
	IMX6QP,
	IMX7D,
	IMX8MQ,
	IMX8MM,
	IMX8MP,
<<<<<<< HEAD
=======
	IMX8Q,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	IMX95,
	IMX8MQ_EP,
	IMX8MM_EP,
	IMX8MP_EP,
	IMX95_EP,
};

<<<<<<< HEAD
#define IMX6_PCIE_FLAG_IMX6_PHY			BIT(0)
#define IMX6_PCIE_FLAG_IMX6_SPEED_CHANGE	BIT(1)
#define IMX6_PCIE_FLAG_SUPPORTS_SUSPEND		BIT(2)
#define IMX6_PCIE_FLAG_HAS_PHYDRV			BIT(3)
#define IMX6_PCIE_FLAG_HAS_APP_RESET		BIT(4)
#define IMX6_PCIE_FLAG_HAS_PHY_RESET		BIT(5)
#define IMX6_PCIE_FLAG_HAS_SERDES		BIT(6)
#define IMX6_PCIE_FLAG_SUPPORT_64BIT		BIT(7)

#define imx6_check_flag(pci, val)     (pci->drvdata->flags & val)

#define IMX6_PCIE_MAX_CLKS       6

#define IMX6_PCIE_MAX_INSTANCES			2

struct imx6_pcie;

struct imx6_pcie_drvdata {
	enum imx6_pcie_variants variant;
=======
#define IMX_PCIE_FLAG_IMX_PHY			BIT(0)
#define IMX_PCIE_FLAG_IMX_SPEED_CHANGE		BIT(1)
#define IMX_PCIE_FLAG_SUPPORTS_SUSPEND		BIT(2)
#define IMX_PCIE_FLAG_HAS_PHYDRV		BIT(3)
#define IMX_PCIE_FLAG_HAS_APP_RESET		BIT(4)
#define IMX_PCIE_FLAG_HAS_PHY_RESET		BIT(5)
#define IMX_PCIE_FLAG_HAS_SERDES		BIT(6)
#define IMX_PCIE_FLAG_SUPPORT_64BIT		BIT(7)
#define IMX_PCIE_FLAG_CPU_ADDR_FIXUP		BIT(8)

#define imx_check_flag(pci, val)	(pci->drvdata->flags & val)

#define IMX_PCIE_MAX_CLKS	6
#define IMX_PCIE_MAX_INSTANCES	2

struct imx_pcie;

struct imx_pcie_drvdata {
	enum imx_pcie_variants variant;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	enum dw_pcie_device_mode mode;
	u32 flags;
	int dbi_length;
	const char *gpr;
	const char * const *clk_names;
	const u32 clks_cnt;
	const u32 ltssm_off;
	const u32 ltssm_mask;
<<<<<<< HEAD
	const u32 mode_off[IMX6_PCIE_MAX_INSTANCES];
	const u32 mode_mask[IMX6_PCIE_MAX_INSTANCES];
	const struct pci_epc_features *epc_features;
	int (*init_phy)(struct imx6_pcie *pcie);
};

struct imx6_pcie {
	struct dw_pcie		*pci;
	struct gpio_desc	*reset_gpiod;
	bool			link_is_up;
	struct clk_bulk_data	clks[IMX6_PCIE_MAX_CLKS];
=======
	const u32 mode_off[IMX_PCIE_MAX_INSTANCES];
	const u32 mode_mask[IMX_PCIE_MAX_INSTANCES];
	const struct pci_epc_features *epc_features;
	int (*init_phy)(struct imx_pcie *pcie);
	int (*enable_ref_clk)(struct imx_pcie *pcie, bool enable);
	int (*core_reset)(struct imx_pcie *pcie, bool assert);
};

struct imx_pcie {
	struct dw_pcie		*pci;
	struct gpio_desc	*reset_gpiod;
	bool			link_is_up;
	struct clk_bulk_data	clks[IMX_PCIE_MAX_CLKS];
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct regmap		*iomuxc_gpr;
	u16			msi_ctrl;
	u32			controller_id;
	struct reset_control	*pciephy_reset;
	struct reset_control	*apps_reset;
	struct reset_control	*turnoff_reset;
	u32			tx_deemph_gen1;
	u32			tx_deemph_gen2_3p5db;
	u32			tx_deemph_gen2_6db;
	u32			tx_swing_full;
	u32			tx_swing_low;
	struct regulator	*vpcie;
	struct regulator	*vph;
	void __iomem		*phy_base;

	/* power domain for pcie */
	struct device		*pd_pcie;
	/* power domain for pcie phy */
	struct device		*pd_pcie_phy;
	struct phy		*phy;
<<<<<<< HEAD
	const struct imx6_pcie_drvdata *drvdata;
=======
	const struct imx_pcie_drvdata *drvdata;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
};

/* Parameters for the waiting for PCIe PHY PLL to lock on i.MX7 */
#define PHY_PLL_LOCK_WAIT_USLEEP_MAX	200
#define PHY_PLL_LOCK_WAIT_TIMEOUT	(2000 * PHY_PLL_LOCK_WAIT_USLEEP_MAX)

/* PCIe Port Logic registers (memory-mapped) */
#define PL_OFFSET 0x700

#define PCIE_PHY_CTRL (PL_OFFSET + 0x114)
#define PCIE_PHY_CTRL_DATA(x)		FIELD_PREP(GENMASK(15, 0), (x))
#define PCIE_PHY_CTRL_CAP_ADR		BIT(16)
#define PCIE_PHY_CTRL_CAP_DAT		BIT(17)
#define PCIE_PHY_CTRL_WR		BIT(18)
#define PCIE_PHY_CTRL_RD		BIT(19)

#define PCIE_PHY_STAT (PL_OFFSET + 0x110)
#define PCIE_PHY_STAT_ACK		BIT(16)

/* PHY registers (not memory-mapped) */
#define PCIE_PHY_ATEOVRD			0x10
#define  PCIE_PHY_ATEOVRD_EN			BIT(2)
#define  PCIE_PHY_ATEOVRD_REF_CLKDIV_SHIFT	0
#define  PCIE_PHY_ATEOVRD_REF_CLKDIV_MASK	0x1

#define PCIE_PHY_MPLL_OVRD_IN_LO		0x11
#define  PCIE_PHY_MPLL_MULTIPLIER_SHIFT		2
#define  PCIE_PHY_MPLL_MULTIPLIER_MASK		0x7f
#define  PCIE_PHY_MPLL_MULTIPLIER_OVRD		BIT(9)

#define PCIE_PHY_RX_ASIC_OUT 0x100D
#define PCIE_PHY_RX_ASIC_OUT_VALID	(1 << 0)

/* iMX7 PCIe PHY registers */
#define PCIE_PHY_CMN_REG4		0x14
/* These are probably the bits that *aren't* DCC_FB_EN */
#define PCIE_PHY_CMN_REG4_DCC_FB_EN	0x29

#define PCIE_PHY_CMN_REG15	        0x54
#define PCIE_PHY_CMN_REG15_DLY_4	BIT(2)
#define PCIE_PHY_CMN_REG15_PLL_PD	BIT(5)
#define PCIE_PHY_CMN_REG15_OVRD_PLL_PD	BIT(7)

#define PCIE_PHY_CMN_REG24		0x90
#define PCIE_PHY_CMN_REG24_RX_EQ	BIT(6)
#define PCIE_PHY_CMN_REG24_RX_EQ_SEL	BIT(3)

#define PCIE_PHY_CMN_REG26		0x98
#define PCIE_PHY_CMN_REG26_ATT_MODE	0xBC

#define PHY_RX_OVRD_IN_LO 0x1005
#define PHY_RX_OVRD_IN_LO_RX_DATA_EN		BIT(5)
#define PHY_RX_OVRD_IN_LO_RX_PLL_EN		BIT(3)

<<<<<<< HEAD
static unsigned int imx6_pcie_grp_offset(const struct imx6_pcie *imx6_pcie)
{
	WARN_ON(imx6_pcie->drvdata->variant != IMX8MQ &&
		imx6_pcie->drvdata->variant != IMX8MQ_EP &&
		imx6_pcie->drvdata->variant != IMX8MM &&
		imx6_pcie->drvdata->variant != IMX8MM_EP &&
		imx6_pcie->drvdata->variant != IMX8MP &&
		imx6_pcie->drvdata->variant != IMX8MP_EP);
	return imx6_pcie->controller_id == 1 ? IOMUXC_GPR16 : IOMUXC_GPR14;
}

static int imx95_pcie_init_phy(struct imx6_pcie *imx6_pcie)
{
	regmap_update_bits(imx6_pcie->iomuxc_gpr,
=======
static unsigned int imx_pcie_grp_offset(const struct imx_pcie *imx_pcie)
{
	WARN_ON(imx_pcie->drvdata->variant != IMX8MQ &&
		imx_pcie->drvdata->variant != IMX8MQ_EP &&
		imx_pcie->drvdata->variant != IMX8MM &&
		imx_pcie->drvdata->variant != IMX8MM_EP &&
		imx_pcie->drvdata->variant != IMX8MP &&
		imx_pcie->drvdata->variant != IMX8MP_EP);
	return imx_pcie->controller_id == 1 ? IOMUXC_GPR16 : IOMUXC_GPR14;
}

static int imx95_pcie_init_phy(struct imx_pcie *imx_pcie)
{
	regmap_update_bits(imx_pcie->iomuxc_gpr,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			IMX95_PCIE_SS_RW_REG_0,
			IMX95_PCIE_PHY_CR_PARA_SEL,
			IMX95_PCIE_PHY_CR_PARA_SEL);

<<<<<<< HEAD
	regmap_update_bits(imx6_pcie->iomuxc_gpr,
			   IMX95_PCIE_PHY_GEN_CTRL,
			   IMX95_PCIE_REF_USE_PAD, 0);
	regmap_update_bits(imx6_pcie->iomuxc_gpr,
=======
	regmap_update_bits(imx_pcie->iomuxc_gpr,
			   IMX95_PCIE_PHY_GEN_CTRL,
			   IMX95_PCIE_REF_USE_PAD, 0);
	regmap_update_bits(imx_pcie->iomuxc_gpr,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			   IMX95_PCIE_SS_RW_REG_0,
			   IMX95_PCIE_REF_CLKEN,
			   IMX95_PCIE_REF_CLKEN);

	return 0;
}

<<<<<<< HEAD
static void imx6_pcie_configure_type(struct imx6_pcie *imx6_pcie)
{
	const struct imx6_pcie_drvdata *drvdata = imx6_pcie->drvdata;
=======
static void imx_pcie_configure_type(struct imx_pcie *imx_pcie)
{
	const struct imx_pcie_drvdata *drvdata = imx_pcie->drvdata;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	unsigned int mask, val, mode, id;

	if (drvdata->mode == DW_PCIE_EP_TYPE)
		mode = PCI_EXP_TYPE_ENDPOINT;
	else
		mode = PCI_EXP_TYPE_ROOT_PORT;

<<<<<<< HEAD
	id = imx6_pcie->controller_id;
=======
	id = imx_pcie->controller_id;

	/* If mode_mask is 0, then generic PHY driver is used to set the mode */
	if (!drvdata->mode_mask[0])
		return;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	/* If mode_mask[id] is zero, means each controller have its individual gpr */
	if (!drvdata->mode_mask[id])
		id = 0;

	mask = drvdata->mode_mask[id];
	val = mode << (ffs(mask) - 1);

<<<<<<< HEAD
	regmap_update_bits(imx6_pcie->iomuxc_gpr, drvdata->mode_off[id], mask, val);
}

static int pcie_phy_poll_ack(struct imx6_pcie *imx6_pcie, bool exp_val)
{
	struct dw_pcie *pci = imx6_pcie->pci;
=======
	regmap_update_bits(imx_pcie->iomuxc_gpr, drvdata->mode_off[id], mask, val);
}

static int pcie_phy_poll_ack(struct imx_pcie *imx_pcie, bool exp_val)
{
	struct dw_pcie *pci = imx_pcie->pci;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	bool val;
	u32 max_iterations = 10;
	u32 wait_counter = 0;

	do {
		val = dw_pcie_readl_dbi(pci, PCIE_PHY_STAT) &
			PCIE_PHY_STAT_ACK;
		wait_counter++;

		if (val == exp_val)
			return 0;

		udelay(1);
	} while (wait_counter < max_iterations);

	return -ETIMEDOUT;
}

<<<<<<< HEAD
static int pcie_phy_wait_ack(struct imx6_pcie *imx6_pcie, int addr)
{
	struct dw_pcie *pci = imx6_pcie->pci;
=======
static int pcie_phy_wait_ack(struct imx_pcie *imx_pcie, int addr)
{
	struct dw_pcie *pci = imx_pcie->pci;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	u32 val;
	int ret;

	val = PCIE_PHY_CTRL_DATA(addr);
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, val);

	val |= PCIE_PHY_CTRL_CAP_ADR;
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, val);

<<<<<<< HEAD
	ret = pcie_phy_poll_ack(imx6_pcie, true);
=======
	ret = pcie_phy_poll_ack(imx_pcie, true);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	val = PCIE_PHY_CTRL_DATA(addr);
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, val);

<<<<<<< HEAD
	return pcie_phy_poll_ack(imx6_pcie, false);
}

/* Read from the 16-bit PCIe PHY control registers (not memory-mapped) */
static int pcie_phy_read(struct imx6_pcie *imx6_pcie, int addr, u16 *data)
{
	struct dw_pcie *pci = imx6_pcie->pci;
	u32 phy_ctl;
	int ret;

	ret = pcie_phy_wait_ack(imx6_pcie, addr);
=======
	return pcie_phy_poll_ack(imx_pcie, false);
}

/* Read from the 16-bit PCIe PHY control registers (not memory-mapped) */
static int pcie_phy_read(struct imx_pcie *imx_pcie, int addr, u16 *data)
{
	struct dw_pcie *pci = imx_pcie->pci;
	u32 phy_ctl;
	int ret;

	ret = pcie_phy_wait_ack(imx_pcie, addr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	/* assert Read signal */
	phy_ctl = PCIE_PHY_CTRL_RD;
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, phy_ctl);

<<<<<<< HEAD
	ret = pcie_phy_poll_ack(imx6_pcie, true);
=======
	ret = pcie_phy_poll_ack(imx_pcie, true);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	*data = dw_pcie_readl_dbi(pci, PCIE_PHY_STAT);

	/* deassert Read signal */
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, 0x00);

<<<<<<< HEAD
	return pcie_phy_poll_ack(imx6_pcie, false);
}

static int pcie_phy_write(struct imx6_pcie *imx6_pcie, int addr, u16 data)
{
	struct dw_pcie *pci = imx6_pcie->pci;
=======
	return pcie_phy_poll_ack(imx_pcie, false);
}

static int pcie_phy_write(struct imx_pcie *imx_pcie, int addr, u16 data)
{
	struct dw_pcie *pci = imx_pcie->pci;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	u32 var;
	int ret;

	/* write addr */
	/* cap addr */
<<<<<<< HEAD
	ret = pcie_phy_wait_ack(imx6_pcie, addr);
=======
	ret = pcie_phy_wait_ack(imx_pcie, addr);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	var = PCIE_PHY_CTRL_DATA(data);
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, var);

	/* capture data */
	var |= PCIE_PHY_CTRL_CAP_DAT;
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, var);

<<<<<<< HEAD
	ret = pcie_phy_poll_ack(imx6_pcie, true);
=======
	ret = pcie_phy_poll_ack(imx_pcie, true);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	/* deassert cap data */
	var = PCIE_PHY_CTRL_DATA(data);
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, var);

	/* wait for ack de-assertion */
<<<<<<< HEAD
	ret = pcie_phy_poll_ack(imx6_pcie, false);
=======
	ret = pcie_phy_poll_ack(imx_pcie, false);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	/* assert wr signal */
	var = PCIE_PHY_CTRL_WR;
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, var);

	/* wait for ack */
<<<<<<< HEAD
	ret = pcie_phy_poll_ack(imx6_pcie, true);
=======
	ret = pcie_phy_poll_ack(imx_pcie, true);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	/* deassert wr signal */
	var = PCIE_PHY_CTRL_DATA(data);
	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, var);

	/* wait for ack de-assertion */
<<<<<<< HEAD
	ret = pcie_phy_poll_ack(imx6_pcie, false);
=======
	ret = pcie_phy_poll_ack(imx_pcie, false);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret)
		return ret;

	dw_pcie_writel_dbi(pci, PCIE_PHY_CTRL, 0x0);

	return 0;
}

<<<<<<< HEAD
static int imx8mq_pcie_init_phy(struct imx6_pcie *imx6_pcie)
{
	/* TODO: Currently this code assumes external oscillator is being used */
	regmap_update_bits(imx6_pcie->iomuxc_gpr,
			   imx6_pcie_grp_offset(imx6_pcie),
=======
static int imx8mq_pcie_init_phy(struct imx_pcie *imx_pcie)
{
	/* TODO: Currently this code assumes external oscillator is being used */
	regmap_update_bits(imx_pcie->iomuxc_gpr,
			   imx_pcie_grp_offset(imx_pcie),
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			   IMX8MQ_GPR_PCIE_REF_USE_PAD,
			   IMX8MQ_GPR_PCIE_REF_USE_PAD);
	/*
	 * Regarding the datasheet, the PCIE_VPH is suggested to be 1.8V. If the PCIE_VPH is
	 * supplied by 3.3V, the VREG_BYPASS should be cleared to zero.
	 */
<<<<<<< HEAD
	if (imx6_pcie->vph && regulator_get_voltage(imx6_pcie->vph) > 3000000)
		regmap_update_bits(imx6_pcie->iomuxc_gpr,
				   imx6_pcie_grp_offset(imx6_pcie),
=======
	if (imx_pcie->vph && regulator_get_voltage(imx_pcie->vph) > 3000000)
		regmap_update_bits(imx_pcie->iomuxc_gpr,
				   imx_pcie_grp_offset(imx_pcie),
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
				   IMX8MQ_GPR_PCIE_VREG_BYPASS,
				   0);

	return 0;
}

<<<<<<< HEAD
static int imx7d_pcie_init_phy(struct imx6_pcie *imx6_pcie)
{
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12, IMX7D_GPR12_PCIE_PHY_REFCLK_SEL, 0);
=======
static int imx7d_pcie_init_phy(struct imx_pcie *imx_pcie)
{
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12, IMX7D_GPR12_PCIE_PHY_REFCLK_SEL, 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return 0;
}

<<<<<<< HEAD
static int imx6_pcie_init_phy(struct imx6_pcie *imx6_pcie)
{
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
				   IMX6Q_GPR12_PCIE_CTL_2, 0 << 10);

	/* configure constant input signal to the pcie ctrl and phy */
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
			   IMX6Q_GPR12_LOS_LEVEL, 9 << 4);

	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_DEEMPH_GEN1,
			   imx6_pcie->tx_deemph_gen1 << 0);
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_DEEMPH_GEN2_3P5DB,
			   imx6_pcie->tx_deemph_gen2_3p5db << 6);
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_DEEMPH_GEN2_6DB,
			   imx6_pcie->tx_deemph_gen2_6db << 12);
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_SWING_FULL,
			   imx6_pcie->tx_swing_full << 18);
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_SWING_LOW,
			   imx6_pcie->tx_swing_low << 25);
	return 0;
}

static int imx6sx_pcie_init_phy(struct imx6_pcie *imx6_pcie)
{
	regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
			   IMX6SX_GPR12_PCIE_RX_EQ_MASK, IMX6SX_GPR12_PCIE_RX_EQ_2);

	return imx6_pcie_init_phy(imx6_pcie);
}

static void imx7d_pcie_wait_for_phy_pll_lock(struct imx6_pcie *imx6_pcie)
{
	u32 val;
	struct device *dev = imx6_pcie->pci->dev;

	if (regmap_read_poll_timeout(imx6_pcie->iomuxc_gpr,
=======
static int imx_pcie_init_phy(struct imx_pcie *imx_pcie)
{
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
				   IMX6Q_GPR12_PCIE_CTL_2, 0 << 10);

	/* configure constant input signal to the pcie ctrl and phy */
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
			   IMX6Q_GPR12_LOS_LEVEL, 9 << 4);

	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_DEEMPH_GEN1,
			   imx_pcie->tx_deemph_gen1 << 0);
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_DEEMPH_GEN2_3P5DB,
			   imx_pcie->tx_deemph_gen2_3p5db << 6);
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_DEEMPH_GEN2_6DB,
			   imx_pcie->tx_deemph_gen2_6db << 12);
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_SWING_FULL,
			   imx_pcie->tx_swing_full << 18);
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR8,
			   IMX6Q_GPR8_TX_SWING_LOW,
			   imx_pcie->tx_swing_low << 25);
	return 0;
}

static int imx6sx_pcie_init_phy(struct imx_pcie *imx_pcie)
{
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
			   IMX6SX_GPR12_PCIE_RX_EQ_MASK, IMX6SX_GPR12_PCIE_RX_EQ_2);

	return imx_pcie_init_phy(imx_pcie);
}

static void imx7d_pcie_wait_for_phy_pll_lock(struct imx_pcie *imx_pcie)
{
	u32 val;
	struct device *dev = imx_pcie->pci->dev;

	if (regmap_read_poll_timeout(imx_pcie->iomuxc_gpr,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
				     IOMUXC_GPR22, val,
				     val & IMX7D_GPR22_PCIE_PHY_PLL_LOCKED,
				     PHY_PLL_LOCK_WAIT_USLEEP_MAX,
				     PHY_PLL_LOCK_WAIT_TIMEOUT))
		dev_err(dev, "PCIe PLL lock timeout\n");
}

<<<<<<< HEAD
static int imx6_setup_phy_mpll(struct imx6_pcie *imx6_pcie)
=======
static int imx_setup_phy_mpll(struct imx_pcie *imx_pcie)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	unsigned long phy_rate = 0;
	int mult, div;
	u16 val;
	int i;

<<<<<<< HEAD
	if (!(imx6_pcie->drvdata->flags & IMX6_PCIE_FLAG_IMX6_PHY))
		return 0;

	for (i = 0; i < imx6_pcie->drvdata->clks_cnt; i++)
		if (strncmp(imx6_pcie->clks[i].id, "pcie_phy", 8) == 0)
			phy_rate = clk_get_rate(imx6_pcie->clks[i].clk);
=======
	if (!(imx_pcie->drvdata->flags & IMX_PCIE_FLAG_IMX_PHY))
		return 0;

	for (i = 0; i < imx_pcie->drvdata->clks_cnt; i++)
		if (strncmp(imx_pcie->clks[i].id, "pcie_phy", 8) == 0)
			phy_rate = clk_get_rate(imx_pcie->clks[i].clk);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	switch (phy_rate) {
	case 125000000:
		/*
		 * The default settings of the MPLL are for a 125MHz input
		 * clock, so no need to reconfigure anything in that case.
		 */
		return 0;
	case 100000000:
		mult = 25;
		div = 0;
		break;
	case 200000000:
		mult = 25;
		div = 1;
		break;
	default:
<<<<<<< HEAD
		dev_err(imx6_pcie->pci->dev,
=======
		dev_err(imx_pcie->pci->dev,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			"Unsupported PHY reference clock rate %lu\n", phy_rate);
		return -EINVAL;
	}

<<<<<<< HEAD
	pcie_phy_read(imx6_pcie, PCIE_PHY_MPLL_OVRD_IN_LO, &val);
=======
	pcie_phy_read(imx_pcie, PCIE_PHY_MPLL_OVRD_IN_LO, &val);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	val &= ~(PCIE_PHY_MPLL_MULTIPLIER_MASK <<
		 PCIE_PHY_MPLL_MULTIPLIER_SHIFT);
	val |= mult << PCIE_PHY_MPLL_MULTIPLIER_SHIFT;
	val |= PCIE_PHY_MPLL_MULTIPLIER_OVRD;
<<<<<<< HEAD
	pcie_phy_write(imx6_pcie, PCIE_PHY_MPLL_OVRD_IN_LO, val);

	pcie_phy_read(imx6_pcie, PCIE_PHY_ATEOVRD, &val);
=======
	pcie_phy_write(imx_pcie, PCIE_PHY_MPLL_OVRD_IN_LO, val);

	pcie_phy_read(imx_pcie, PCIE_PHY_ATEOVRD, &val);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	val &= ~(PCIE_PHY_ATEOVRD_REF_CLKDIV_MASK <<
		 PCIE_PHY_ATEOVRD_REF_CLKDIV_SHIFT);
	val |= div << PCIE_PHY_ATEOVRD_REF_CLKDIV_SHIFT;
	val |= PCIE_PHY_ATEOVRD_EN;
<<<<<<< HEAD
	pcie_phy_write(imx6_pcie, PCIE_PHY_ATEOVRD, val);
=======
	pcie_phy_write(imx_pcie, PCIE_PHY_ATEOVRD, val);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return 0;
}

<<<<<<< HEAD
static void imx6_pcie_reset_phy(struct imx6_pcie *imx6_pcie)
{
	u16 tmp;

	if (!(imx6_pcie->drvdata->flags & IMX6_PCIE_FLAG_IMX6_PHY))
		return;

	pcie_phy_read(imx6_pcie, PHY_RX_OVRD_IN_LO, &tmp);
	tmp |= (PHY_RX_OVRD_IN_LO_RX_DATA_EN |
		PHY_RX_OVRD_IN_LO_RX_PLL_EN);
	pcie_phy_write(imx6_pcie, PHY_RX_OVRD_IN_LO, tmp);

	usleep_range(2000, 3000);

	pcie_phy_read(imx6_pcie, PHY_RX_OVRD_IN_LO, &tmp);
	tmp &= ~(PHY_RX_OVRD_IN_LO_RX_DATA_EN |
		  PHY_RX_OVRD_IN_LO_RX_PLL_EN);
	pcie_phy_write(imx6_pcie, PHY_RX_OVRD_IN_LO, tmp);
=======
static void imx_pcie_reset_phy(struct imx_pcie *imx_pcie)
{
	u16 tmp;

	if (!(imx_pcie->drvdata->flags & IMX_PCIE_FLAG_IMX_PHY))
		return;

	pcie_phy_read(imx_pcie, PHY_RX_OVRD_IN_LO, &tmp);
	tmp |= (PHY_RX_OVRD_IN_LO_RX_DATA_EN |
		PHY_RX_OVRD_IN_LO_RX_PLL_EN);
	pcie_phy_write(imx_pcie, PHY_RX_OVRD_IN_LO, tmp);

	usleep_range(2000, 3000);

	pcie_phy_read(imx_pcie, PHY_RX_OVRD_IN_LO, &tmp);
	tmp &= ~(PHY_RX_OVRD_IN_LO_RX_DATA_EN |
		  PHY_RX_OVRD_IN_LO_RX_PLL_EN);
	pcie_phy_write(imx_pcie, PHY_RX_OVRD_IN_LO, tmp);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

#ifdef CONFIG_ARM
/*  Added for PCI abort handling */
static int imx6q_pcie_abort_handler(unsigned long addr,
		unsigned int fsr, struct pt_regs *regs)
{
	unsigned long pc = instruction_pointer(regs);
	unsigned long instr = *(unsigned long *)pc;
	int reg = (instr >> 12) & 15;

	/*
	 * If the instruction being executed was a read,
	 * make it look like it read all-ones.
	 */
	if ((instr & 0x0c100000) == 0x04100000) {
		unsigned long val;

		if (instr & 0x00400000)
			val = 255;
		else
			val = -1;

		regs->uregs[reg] = val;
		regs->ARM_pc += 4;
		return 0;
	}

	if ((instr & 0x0e100090) == 0x00100090) {
		regs->uregs[reg] = -1;
		regs->ARM_pc += 4;
		return 0;
	}

	return 1;
}
#endif

<<<<<<< HEAD
static int imx6_pcie_attach_pd(struct device *dev)
{
	struct imx6_pcie *imx6_pcie = dev_get_drvdata(dev);
=======
static int imx_pcie_attach_pd(struct device *dev)
{
	struct imx_pcie *imx_pcie = dev_get_drvdata(dev);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct device_link *link;

	/* Do nothing when in a single power domain */
	if (dev->pm_domain)
		return 0;

<<<<<<< HEAD
	imx6_pcie->pd_pcie = dev_pm_domain_attach_by_name(dev, "pcie");
	if (IS_ERR(imx6_pcie->pd_pcie))
		return PTR_ERR(imx6_pcie->pd_pcie);
	/* Do nothing when power domain missing */
	if (!imx6_pcie->pd_pcie)
		return 0;
	link = device_link_add(dev, imx6_pcie->pd_pcie,
=======
	imx_pcie->pd_pcie = dev_pm_domain_attach_by_name(dev, "pcie");
	if (IS_ERR(imx_pcie->pd_pcie))
		return PTR_ERR(imx_pcie->pd_pcie);
	/* Do nothing when power domain missing */
	if (!imx_pcie->pd_pcie)
		return 0;
	link = device_link_add(dev, imx_pcie->pd_pcie,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			DL_FLAG_STATELESS |
			DL_FLAG_PM_RUNTIME |
			DL_FLAG_RPM_ACTIVE);
	if (!link) {
		dev_err(dev, "Failed to add device_link to pcie pd.\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	imx6_pcie->pd_pcie_phy = dev_pm_domain_attach_by_name(dev, "pcie_phy");
	if (IS_ERR(imx6_pcie->pd_pcie_phy))
		return PTR_ERR(imx6_pcie->pd_pcie_phy);

	link = device_link_add(dev, imx6_pcie->pd_pcie_phy,
=======
	imx_pcie->pd_pcie_phy = dev_pm_domain_attach_by_name(dev, "pcie_phy");
	if (IS_ERR(imx_pcie->pd_pcie_phy))
		return PTR_ERR(imx_pcie->pd_pcie_phy);

	link = device_link_add(dev, imx_pcie->pd_pcie_phy,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			DL_FLAG_STATELESS |
			DL_FLAG_PM_RUNTIME |
			DL_FLAG_RPM_ACTIVE);
	if (!link) {
		dev_err(dev, "Failed to add device_link to pcie_phy pd.\n");
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
static int imx6_pcie_enable_ref_clk(struct imx6_pcie *imx6_pcie)
{
	unsigned int offset;
	int ret = 0;

	switch (imx6_pcie->drvdata->variant) {
	case IMX6SX:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
				   IMX6SX_GPR12_PCIE_TEST_POWERDOWN, 0);
		break;
	case IMX6QP:
	case IMX6Q:
		/* power up core phy and enable ref clock */
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				   IMX6Q_GPR1_PCIE_TEST_PD, 0 << 18);
=======
static int imx6sx_pcie_enable_ref_clk(struct imx_pcie *imx_pcie, bool enable)
{
	if (enable)
		regmap_clear_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
				  IMX6SX_GPR12_PCIE_TEST_POWERDOWN);

	return 0;
}

static int imx6q_pcie_enable_ref_clk(struct imx_pcie *imx_pcie, bool enable)
{
	if (enable) {
		/* power up core phy and enable ref clock */
		regmap_clear_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR1, IMX6Q_GPR1_PCIE_TEST_PD);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		/*
		 * the async reset input need ref clock to sync internally,
		 * when the ref clock comes after reset, internal synced
		 * reset time is too short, cannot meet the requirement.
		 * add one ~10us delay here.
		 */
		usleep_range(10, 100);
<<<<<<< HEAD
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				   IMX6Q_GPR1_PCIE_REF_CLK_EN, 1 << 16);
		break;
	case IMX7D:
	case IMX95:
	case IMX95_EP:
		break;
	case IMX8MM:
	case IMX8MM_EP:
	case IMX8MQ:
	case IMX8MQ_EP:
	case IMX8MP:
	case IMX8MP_EP:
		offset = imx6_pcie_grp_offset(imx6_pcie);
		/*
		 * Set the over ride low and enabled
		 * make sure that REF_CLK is turned on.
		 */
		regmap_update_bits(imx6_pcie->iomuxc_gpr, offset,
				   IMX8MQ_GPR_PCIE_CLK_REQ_OVERRIDE,
				   0);
		regmap_update_bits(imx6_pcie->iomuxc_gpr, offset,
				   IMX8MQ_GPR_PCIE_CLK_REQ_OVERRIDE_EN,
				   IMX8MQ_GPR_PCIE_CLK_REQ_OVERRIDE_EN);
		break;
	}

	return ret;
}

static void imx6_pcie_disable_ref_clk(struct imx6_pcie *imx6_pcie)
{
	switch (imx6_pcie->drvdata->variant) {
	case IMX6QP:
	case IMX6Q:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				IMX6Q_GPR1_PCIE_REF_CLK_EN, 0);
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				IMX6Q_GPR1_PCIE_TEST_PD,
				IMX6Q_GPR1_PCIE_TEST_PD);
		break;
	case IMX7D:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
				   IMX7D_GPR12_PCIE_PHY_REFCLK_SEL,
				   IMX7D_GPR12_PCIE_PHY_REFCLK_SEL);
		break;
	default:
		break;
	}
}

static int imx6_pcie_clk_enable(struct imx6_pcie *imx6_pcie)
{
	struct dw_pcie *pci = imx6_pcie->pci;
	struct device *dev = pci->dev;
	int ret;

	ret = clk_bulk_prepare_enable(imx6_pcie->drvdata->clks_cnt, imx6_pcie->clks);
	if (ret)
		return ret;

	ret = imx6_pcie_enable_ref_clk(imx6_pcie);
	if (ret) {
		dev_err(dev, "unable to enable pcie ref clock\n");
		goto err_ref_clk;
=======
		regmap_set_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR1, IMX6Q_GPR1_PCIE_REF_CLK_EN);
	} else {
		regmap_clear_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR1, IMX6Q_GPR1_PCIE_REF_CLK_EN);
		regmap_set_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR1, IMX6Q_GPR1_PCIE_TEST_PD);
	}

	return 0;
}

static int imx8mm_pcie_enable_ref_clk(struct imx_pcie *imx_pcie, bool enable)
{
	int offset = imx_pcie_grp_offset(imx_pcie);

	if (enable) {
		regmap_clear_bits(imx_pcie->iomuxc_gpr, offset, IMX8MQ_GPR_PCIE_CLK_REQ_OVERRIDE);
		regmap_set_bits(imx_pcie->iomuxc_gpr, offset, IMX8MQ_GPR_PCIE_CLK_REQ_OVERRIDE_EN);
	}

	return 0;
}

static int imx7d_pcie_enable_ref_clk(struct imx_pcie *imx_pcie, bool enable)
{
	if (!enable)
		regmap_set_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
				IMX7D_GPR12_PCIE_PHY_REFCLK_SEL);
	return 0;
}

static int imx_pcie_clk_enable(struct imx_pcie *imx_pcie)
{
	struct dw_pcie *pci = imx_pcie->pci;
	struct device *dev = pci->dev;
	int ret;

	ret = clk_bulk_prepare_enable(imx_pcie->drvdata->clks_cnt, imx_pcie->clks);
	if (ret)
		return ret;

	if (imx_pcie->drvdata->enable_ref_clk) {
		ret = imx_pcie->drvdata->enable_ref_clk(imx_pcie, true);
		if (ret) {
			dev_err(dev, "Failed to enable PCIe REFCLK\n");
			goto err_ref_clk;
		}
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}

	/* allow the clocks to stabilize */
	usleep_range(200, 500);
	return 0;

err_ref_clk:
<<<<<<< HEAD
	clk_bulk_disable_unprepare(imx6_pcie->drvdata->clks_cnt, imx6_pcie->clks);
=======
	clk_bulk_disable_unprepare(imx_pcie->drvdata->clks_cnt, imx_pcie->clks);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return ret;
}

<<<<<<< HEAD
static void imx6_pcie_clk_disable(struct imx6_pcie *imx6_pcie)
{
	imx6_pcie_disable_ref_clk(imx6_pcie);
	clk_bulk_disable_unprepare(imx6_pcie->drvdata->clks_cnt, imx6_pcie->clks);
}

static void imx6_pcie_assert_core_reset(struct imx6_pcie *imx6_pcie)
{
	reset_control_assert(imx6_pcie->pciephy_reset);
	reset_control_assert(imx6_pcie->apps_reset);

	switch (imx6_pcie->drvdata->variant) {
	case IMX6SX:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
				   IMX6SX_GPR12_PCIE_TEST_POWERDOWN,
				   IMX6SX_GPR12_PCIE_TEST_POWERDOWN);
		/* Force PCIe PHY reset */
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR5,
				   IMX6SX_GPR5_PCIE_BTNRST_RESET,
				   IMX6SX_GPR5_PCIE_BTNRST_RESET);
		break;
	case IMX6QP:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				   IMX6Q_GPR1_PCIE_SW_RST,
				   IMX6Q_GPR1_PCIE_SW_RST);
		break;
	case IMX6Q:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				   IMX6Q_GPR1_PCIE_TEST_PD, 1 << 18);
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				   IMX6Q_GPR1_PCIE_REF_CLK_EN, 0 << 16);
		break;
	default:
		break;
	}

	/* Some boards don't have PCIe reset GPIO. */
	gpiod_set_value_cansleep(imx6_pcie->reset_gpiod, 1);
}

static int imx6_pcie_deassert_core_reset(struct imx6_pcie *imx6_pcie)
{
	struct dw_pcie *pci = imx6_pcie->pci;
	struct device *dev = pci->dev;

	reset_control_deassert(imx6_pcie->pciephy_reset);

	switch (imx6_pcie->drvdata->variant) {
	case IMX7D:
		/* Workaround for ERR010728, failure of PCI-e PLL VCO to
		 * oscillate, especially when cold.  This turns off "Duty-cycle
		 * Corrector" and other mysterious undocumented things.
		 */
		if (likely(imx6_pcie->phy_base)) {
			/* De-assert DCC_FB_EN */
			writel(PCIE_PHY_CMN_REG4_DCC_FB_EN,
			       imx6_pcie->phy_base + PCIE_PHY_CMN_REG4);
			/* Assert RX_EQS and RX_EQS_SEL */
			writel(PCIE_PHY_CMN_REG24_RX_EQ_SEL
				| PCIE_PHY_CMN_REG24_RX_EQ,
			       imx6_pcie->phy_base + PCIE_PHY_CMN_REG24);
			/* Assert ATT_MODE */
			writel(PCIE_PHY_CMN_REG26_ATT_MODE,
			       imx6_pcie->phy_base + PCIE_PHY_CMN_REG26);
		} else {
			dev_warn(dev, "Unable to apply ERR010728 workaround. DT missing fsl,imx7d-pcie-phy phandle ?\n");
		}

		imx7d_pcie_wait_for_phy_pll_lock(imx6_pcie);
		break;
	case IMX6SX:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR5,
				   IMX6SX_GPR5_PCIE_BTNRST_RESET, 0);
		break;
	case IMX6QP:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR1,
				   IMX6Q_GPR1_PCIE_SW_RST, 0);

		usleep_range(200, 500);
		break;
	default:
		break;
	}

	/* Some boards don't have PCIe reset GPIO. */
	if (imx6_pcie->reset_gpiod) {
		msleep(100);
		gpiod_set_value_cansleep(imx6_pcie->reset_gpiod, 0);
=======
static void imx_pcie_clk_disable(struct imx_pcie *imx_pcie)
{
	if (imx_pcie->drvdata->enable_ref_clk)
		imx_pcie->drvdata->enable_ref_clk(imx_pcie, false);
	clk_bulk_disable_unprepare(imx_pcie->drvdata->clks_cnt, imx_pcie->clks);
}

static int imx6sx_pcie_core_reset(struct imx_pcie *imx_pcie, bool assert)
{
	if (assert)
		regmap_set_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
				IMX6SX_GPR12_PCIE_TEST_POWERDOWN);

	/* Force PCIe PHY reset */
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR5, IMX6SX_GPR5_PCIE_BTNRST_RESET,
			   assert ? IMX6SX_GPR5_PCIE_BTNRST_RESET : 0);
	return 0;
}

static int imx6qp_pcie_core_reset(struct imx_pcie *imx_pcie, bool assert)
{
	regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR1, IMX6Q_GPR1_PCIE_SW_RST,
			   assert ? IMX6Q_GPR1_PCIE_SW_RST : 0);
	if (!assert)
		usleep_range(200, 500);

	return 0;
}

static int imx6q_pcie_core_reset(struct imx_pcie *imx_pcie, bool assert)
{
	if (!assert)
		return 0;

	regmap_set_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR1, IMX6Q_GPR1_PCIE_TEST_PD);
	regmap_set_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR1, IMX6Q_GPR1_PCIE_REF_CLK_EN);

	return 0;
}

static int imx7d_pcie_core_reset(struct imx_pcie *imx_pcie, bool assert)
{
	struct dw_pcie *pci = imx_pcie->pci;
	struct device *dev = pci->dev;

	if (assert)
		return 0;

	/*
	 * Workaround for ERR010728 (IMX7DS_2N09P, Rev. 1.1, 4/2023):
	 *
	 * PCIe: PLL may fail to lock under corner conditions.
	 *
	 * Initial VCO oscillation may fail under corner conditions such as
	 * cold temperature which will cause the PCIe PLL fail to lock in the
	 * initialization phase.
	 *
	 * The Duty-cycle Corrector calibration must be disabled.
	 *
	 * 1. De-assert the G_RST signal by clearing
	 *    SRC_PCIEPHY_RCR[PCIEPHY_G_RST].
	 * 2. De-assert DCC_FB_EN by writing data “0x29” to the register
	 *    address 0x306d0014 (PCIE_PHY_CMN_REG4).
	 * 3. Assert RX_EQS, RX_EQ_SEL by writing data “0x48” to the register
	 *    address 0x306d0090 (PCIE_PHY_CMN_REG24).
	 * 4. Assert ATT_MODE by writing data “0xbc” to the register
	 *    address 0x306d0098 (PCIE_PHY_CMN_REG26).
	 * 5. De-assert the CMN_RST signal by clearing register bit
	 *    SRC_PCIEPHY_RCR[PCIEPHY_BTN]
	 */

	if (likely(imx_pcie->phy_base)) {
		/* De-assert DCC_FB_EN */
		writel(PCIE_PHY_CMN_REG4_DCC_FB_EN, imx_pcie->phy_base + PCIE_PHY_CMN_REG4);
		/* Assert RX_EQS and RX_EQS_SEL */
		writel(PCIE_PHY_CMN_REG24_RX_EQ_SEL | PCIE_PHY_CMN_REG24_RX_EQ,
		       imx_pcie->phy_base + PCIE_PHY_CMN_REG24);
		/* Assert ATT_MODE */
		writel(PCIE_PHY_CMN_REG26_ATT_MODE, imx_pcie->phy_base + PCIE_PHY_CMN_REG26);
	} else {
		dev_warn(dev, "Unable to apply ERR010728 workaround. DT missing fsl,imx7d-pcie-phy phandle ?\n");
	}
	imx7d_pcie_wait_for_phy_pll_lock(imx_pcie);
	return 0;
}

static void imx_pcie_assert_core_reset(struct imx_pcie *imx_pcie)
{
	reset_control_assert(imx_pcie->pciephy_reset);
	reset_control_assert(imx_pcie->apps_reset);

	if (imx_pcie->drvdata->core_reset)
		imx_pcie->drvdata->core_reset(imx_pcie, true);

	/* Some boards don't have PCIe reset GPIO. */
	gpiod_set_value_cansleep(imx_pcie->reset_gpiod, 1);
}

static int imx_pcie_deassert_core_reset(struct imx_pcie *imx_pcie)
{
	reset_control_deassert(imx_pcie->pciephy_reset);

	if (imx_pcie->drvdata->core_reset)
		imx_pcie->drvdata->core_reset(imx_pcie, false);

	/* Some boards don't have PCIe reset GPIO. */
	if (imx_pcie->reset_gpiod) {
		msleep(100);
		gpiod_set_value_cansleep(imx_pcie->reset_gpiod, 0);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		/* Wait for 100ms after PERST# deassertion (PCIe r5.0, 6.6.1) */
		msleep(100);
	}

	return 0;
}

<<<<<<< HEAD
static int imx6_pcie_wait_for_speed_change(struct imx6_pcie *imx6_pcie)
{
	struct dw_pcie *pci = imx6_pcie->pci;
=======
static int imx_pcie_wait_for_speed_change(struct imx_pcie *imx_pcie)
{
	struct dw_pcie *pci = imx_pcie->pci;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct device *dev = pci->dev;
	u32 tmp;
	unsigned int retries;

	for (retries = 0; retries < 200; retries++) {
		tmp = dw_pcie_readl_dbi(pci, PCIE_LINK_WIDTH_SPEED_CONTROL);
		/* Test if the speed change finished. */
		if (!(tmp & PORT_LOGIC_SPEED_CHANGE))
			return 0;
		usleep_range(100, 1000);
	}

	dev_err(dev, "Speed change timeout\n");
	return -ETIMEDOUT;
}

<<<<<<< HEAD
static void imx6_pcie_ltssm_enable(struct device *dev)
{
	struct imx6_pcie *imx6_pcie = dev_get_drvdata(dev);
	const struct imx6_pcie_drvdata *drvdata = imx6_pcie->drvdata;

	if (drvdata->ltssm_mask)
		regmap_update_bits(imx6_pcie->iomuxc_gpr, drvdata->ltssm_off, drvdata->ltssm_mask,
				   drvdata->ltssm_mask);

	reset_control_deassert(imx6_pcie->apps_reset);
}

static void imx6_pcie_ltssm_disable(struct device *dev)
{
	struct imx6_pcie *imx6_pcie = dev_get_drvdata(dev);
	const struct imx6_pcie_drvdata *drvdata = imx6_pcie->drvdata;

	if (drvdata->ltssm_mask)
		regmap_update_bits(imx6_pcie->iomuxc_gpr, drvdata->ltssm_off,
				   drvdata->ltssm_mask, 0);

	reset_control_assert(imx6_pcie->apps_reset);
}

static int imx6_pcie_start_link(struct dw_pcie *pci)
{
	struct imx6_pcie *imx6_pcie = to_imx6_pcie(pci);
=======
static void imx_pcie_ltssm_enable(struct device *dev)
{
	struct imx_pcie *imx_pcie = dev_get_drvdata(dev);
	const struct imx_pcie_drvdata *drvdata = imx_pcie->drvdata;
	u8 offset = dw_pcie_find_capability(imx_pcie->pci, PCI_CAP_ID_EXP);
	u32 tmp;

	tmp = dw_pcie_readl_dbi(imx_pcie->pci, offset + PCI_EXP_LNKCAP);
	phy_set_speed(imx_pcie->phy, FIELD_GET(PCI_EXP_LNKCAP_SLS, tmp));
	if (drvdata->ltssm_mask)
		regmap_update_bits(imx_pcie->iomuxc_gpr, drvdata->ltssm_off, drvdata->ltssm_mask,
				   drvdata->ltssm_mask);

	reset_control_deassert(imx_pcie->apps_reset);
}

static void imx_pcie_ltssm_disable(struct device *dev)
{
	struct imx_pcie *imx_pcie = dev_get_drvdata(dev);
	const struct imx_pcie_drvdata *drvdata = imx_pcie->drvdata;

	phy_set_speed(imx_pcie->phy, 0);
	if (drvdata->ltssm_mask)
		regmap_update_bits(imx_pcie->iomuxc_gpr, drvdata->ltssm_off,
				   drvdata->ltssm_mask, 0);

	reset_control_assert(imx_pcie->apps_reset);
}

static int imx_pcie_start_link(struct dw_pcie *pci)
{
	struct imx_pcie *imx_pcie = to_imx_pcie(pci);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct device *dev = pci->dev;
	u8 offset = dw_pcie_find_capability(pci, PCI_CAP_ID_EXP);
	u32 tmp;
	int ret;

	/*
	 * Force Gen1 operation when starting the link.  In case the link is
	 * started in Gen2 mode, there is a possibility the devices on the
	 * bus will not be detected at all.  This happens with PCIe switches.
	 */
	dw_pcie_dbi_ro_wr_en(pci);
	tmp = dw_pcie_readl_dbi(pci, offset + PCI_EXP_LNKCAP);
	tmp &= ~PCI_EXP_LNKCAP_SLS;
	tmp |= PCI_EXP_LNKCAP_SLS_2_5GB;
	dw_pcie_writel_dbi(pci, offset + PCI_EXP_LNKCAP, tmp);
	dw_pcie_dbi_ro_wr_dis(pci);

	/* Start LTSSM. */
<<<<<<< HEAD
	imx6_pcie_ltssm_enable(dev);
=======
	imx_pcie_ltssm_enable(dev);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	ret = dw_pcie_wait_for_link(pci);
	if (ret)
		goto err_reset_phy;

<<<<<<< HEAD
	if (pci->link_gen > 1) {
=======
	if (pci->max_link_speed > 1) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		/* Allow faster modes after the link is up */
		dw_pcie_dbi_ro_wr_en(pci);
		tmp = dw_pcie_readl_dbi(pci, offset + PCI_EXP_LNKCAP);
		tmp &= ~PCI_EXP_LNKCAP_SLS;
<<<<<<< HEAD
		tmp |= pci->link_gen;
=======
		tmp |= pci->max_link_speed;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		dw_pcie_writel_dbi(pci, offset + PCI_EXP_LNKCAP, tmp);

		/*
		 * Start Directed Speed Change so the best possible
		 * speed both link partners support can be negotiated.
		 */
		tmp = dw_pcie_readl_dbi(pci, PCIE_LINK_WIDTH_SPEED_CONTROL);
		tmp |= PORT_LOGIC_SPEED_CHANGE;
		dw_pcie_writel_dbi(pci, PCIE_LINK_WIDTH_SPEED_CONTROL, tmp);
		dw_pcie_dbi_ro_wr_dis(pci);

<<<<<<< HEAD
		if (imx6_pcie->drvdata->flags &
		    IMX6_PCIE_FLAG_IMX6_SPEED_CHANGE) {
=======
		if (imx_pcie->drvdata->flags &
		    IMX_PCIE_FLAG_IMX_SPEED_CHANGE) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			/*
			 * On i.MX7, DIRECT_SPEED_CHANGE behaves differently
			 * from i.MX6 family when no link speed transition
			 * occurs and we go Gen1 -> yep, Gen1. The difference
			 * is that, in such case, it will not be cleared by HW
			 * which will cause the following code to report false
			 * failure.
			 */

<<<<<<< HEAD
			ret = imx6_pcie_wait_for_speed_change(imx6_pcie);
=======
			ret = imx_pcie_wait_for_speed_change(imx_pcie);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			if (ret) {
				dev_err(dev, "Failed to bring link up!\n");
				goto err_reset_phy;
			}
		}

		/* Make sure link training is finished as well! */
		ret = dw_pcie_wait_for_link(pci);
		if (ret)
			goto err_reset_phy;
	} else {
		dev_info(dev, "Link: Only Gen1 is enabled\n");
	}

<<<<<<< HEAD
	imx6_pcie->link_is_up = true;
=======
	imx_pcie->link_is_up = true;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	tmp = dw_pcie_readw_dbi(pci, offset + PCI_EXP_LNKSTA);
	dev_info(dev, "Link up, Gen%i\n", tmp & PCI_EXP_LNKSTA_CLS);
	return 0;

err_reset_phy:
<<<<<<< HEAD
	imx6_pcie->link_is_up = false;
	dev_dbg(dev, "PHY DEBUG_R0=0x%08x DEBUG_R1=0x%08x\n",
		dw_pcie_readl_dbi(pci, PCIE_PORT_DEBUG0),
		dw_pcie_readl_dbi(pci, PCIE_PORT_DEBUG1));
	imx6_pcie_reset_phy(imx6_pcie);
	return 0;
}

static void imx6_pcie_stop_link(struct dw_pcie *pci)
=======
	imx_pcie->link_is_up = false;
	dev_dbg(dev, "PHY DEBUG_R0=0x%08x DEBUG_R1=0x%08x\n",
		dw_pcie_readl_dbi(pci, PCIE_PORT_DEBUG0),
		dw_pcie_readl_dbi(pci, PCIE_PORT_DEBUG1));
	imx_pcie_reset_phy(imx_pcie);
	return 0;
}

static void imx_pcie_stop_link(struct dw_pcie *pci)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	struct device *dev = pci->dev;

	/* Turn off PCIe LTSSM */
<<<<<<< HEAD
	imx6_pcie_ltssm_disable(dev);
}

static int imx6_pcie_host_init(struct dw_pcie_rp *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct device *dev = pci->dev;
	struct imx6_pcie *imx6_pcie = to_imx6_pcie(pci);
	int ret;

	if (imx6_pcie->vpcie) {
		ret = regulator_enable(imx6_pcie->vpcie);
=======
	imx_pcie_ltssm_disable(dev);
}

static int imx_pcie_host_init(struct dw_pcie_rp *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct device *dev = pci->dev;
	struct imx_pcie *imx_pcie = to_imx_pcie(pci);
	int ret;

	if (imx_pcie->vpcie) {
		ret = regulator_enable(imx_pcie->vpcie);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (ret) {
			dev_err(dev, "failed to enable vpcie regulator: %d\n",
				ret);
			return ret;
		}
	}

<<<<<<< HEAD
	imx6_pcie_assert_core_reset(imx6_pcie);

	if (imx6_pcie->drvdata->init_phy)
		imx6_pcie->drvdata->init_phy(imx6_pcie);

	imx6_pcie_configure_type(imx6_pcie);

	ret = imx6_pcie_clk_enable(imx6_pcie);
=======
	imx_pcie_assert_core_reset(imx_pcie);

	if (imx_pcie->drvdata->init_phy)
		imx_pcie->drvdata->init_phy(imx_pcie);

	imx_pcie_configure_type(imx_pcie);

	ret = imx_pcie_clk_enable(imx_pcie);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret) {
		dev_err(dev, "unable to enable pcie clocks: %d\n", ret);
		goto err_reg_disable;
	}

<<<<<<< HEAD
	if (imx6_pcie->phy) {
		ret = phy_init(imx6_pcie->phy);
=======
	if (imx_pcie->phy) {
		ret = phy_init(imx_pcie->phy);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (ret) {
			dev_err(dev, "pcie PHY power up failed\n");
			goto err_clk_disable;
		}
<<<<<<< HEAD
	}

	if (imx6_pcie->phy) {
		ret = phy_power_on(imx6_pcie->phy);
		if (ret) {
			dev_err(dev, "waiting for PHY ready timeout!\n");
			goto err_phy_off;
		}
	}

	ret = imx6_pcie_deassert_core_reset(imx6_pcie);
=======

		ret = phy_set_mode_ext(imx_pcie->phy, PHY_MODE_PCIE, PHY_MODE_PCIE_RC);
		if (ret) {
			dev_err(dev, "unable to set PCIe PHY mode\n");
			goto err_phy_exit;
		}

		ret = phy_power_on(imx_pcie->phy);
		if (ret) {
			dev_err(dev, "waiting for PHY ready timeout!\n");
			goto err_phy_exit;
		}
	}

	ret = imx_pcie_deassert_core_reset(imx_pcie);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (ret < 0) {
		dev_err(dev, "pcie deassert core reset failed: %d\n", ret);
		goto err_phy_off;
	}

<<<<<<< HEAD
	imx6_setup_phy_mpll(imx6_pcie);
=======
	imx_setup_phy_mpll(imx_pcie);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return 0;

err_phy_off:
<<<<<<< HEAD
	if (imx6_pcie->phy)
		phy_exit(imx6_pcie->phy);
err_clk_disable:
	imx6_pcie_clk_disable(imx6_pcie);
err_reg_disable:
	if (imx6_pcie->vpcie)
		regulator_disable(imx6_pcie->vpcie);
	return ret;
}

static void imx6_pcie_host_exit(struct dw_pcie_rp *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct imx6_pcie *imx6_pcie = to_imx6_pcie(pci);

	if (imx6_pcie->phy) {
		if (phy_power_off(imx6_pcie->phy))
			dev_err(pci->dev, "unable to power off PHY\n");
		phy_exit(imx6_pcie->phy);
	}
	imx6_pcie_clk_disable(imx6_pcie);

	if (imx6_pcie->vpcie)
		regulator_disable(imx6_pcie->vpcie);
}

static const struct dw_pcie_host_ops imx6_pcie_host_ops = {
	.init = imx6_pcie_host_init,
	.deinit = imx6_pcie_host_exit,
};

static const struct dw_pcie_ops dw_pcie_ops = {
	.start_link = imx6_pcie_start_link,
	.stop_link = imx6_pcie_stop_link,
};

static void imx6_pcie_ep_init(struct dw_pcie_ep *ep)
=======
	phy_power_off(imx_pcie->phy);
err_phy_exit:
	phy_exit(imx_pcie->phy);
err_clk_disable:
	imx_pcie_clk_disable(imx_pcie);
err_reg_disable:
	if (imx_pcie->vpcie)
		regulator_disable(imx_pcie->vpcie);
	return ret;
}

static void imx_pcie_host_exit(struct dw_pcie_rp *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct imx_pcie *imx_pcie = to_imx_pcie(pci);

	if (imx_pcie->phy) {
		if (phy_power_off(imx_pcie->phy))
			dev_err(pci->dev, "unable to power off PHY\n");
		phy_exit(imx_pcie->phy);
	}
	imx_pcie_clk_disable(imx_pcie);

	if (imx_pcie->vpcie)
		regulator_disable(imx_pcie->vpcie);
}

static u64 imx_pcie_cpu_addr_fixup(struct dw_pcie *pcie, u64 cpu_addr)
{
	struct imx_pcie *imx_pcie = to_imx_pcie(pcie);
	struct dw_pcie_rp *pp = &pcie->pp;
	struct resource_entry *entry;

	if (!(imx_pcie->drvdata->flags & IMX_PCIE_FLAG_CPU_ADDR_FIXUP))
		return cpu_addr;

	entry = resource_list_first_type(&pp->bridge->windows, IORESOURCE_MEM);
	if (!entry)
		return cpu_addr;

	return cpu_addr - entry->offset;
}

static const struct dw_pcie_host_ops imx_pcie_host_ops = {
	.init = imx_pcie_host_init,
	.deinit = imx_pcie_host_exit,
};

static const struct dw_pcie_ops dw_pcie_ops = {
	.start_link = imx_pcie_start_link,
	.stop_link = imx_pcie_stop_link,
	.cpu_addr_fixup = imx_pcie_cpu_addr_fixup,
};

static void imx_pcie_ep_init(struct dw_pcie_ep *ep)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	enum pci_barno bar;
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);

	for (bar = BAR_0; bar <= BAR_5; bar++)
		dw_pcie_ep_reset_bar(pci, bar);
}

<<<<<<< HEAD
static int imx6_pcie_ep_raise_irq(struct dw_pcie_ep *ep, u8 func_no,
=======
static int imx_pcie_ep_raise_irq(struct dw_pcie_ep *ep, u8 func_no,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
				  unsigned int type, u16 interrupt_num)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);

	switch (type) {
	case PCI_IRQ_INTX:
		return dw_pcie_ep_raise_intx_irq(ep, func_no);
	case PCI_IRQ_MSI:
		return dw_pcie_ep_raise_msi_irq(ep, func_no, interrupt_num);
	case PCI_IRQ_MSIX:
		return dw_pcie_ep_raise_msix_irq(ep, func_no, interrupt_num);
	default:
		dev_err(pci->dev, "UNKNOWN IRQ type\n");
		return -EINVAL;
	}

	return 0;
}

static const struct pci_epc_features imx8m_pcie_epc_features = {
	.linkup_notifier = false,
	.msi_capable = true,
	.msix_capable = false,
	.bar[BAR_1] = { .type = BAR_RESERVED, },
	.bar[BAR_3] = { .type = BAR_RESERVED, },
	.align = SZ_64K,
};

/*
 * BAR#	| Default BAR enable	| Default BAR Type	| Default BAR Size	| BAR Sizing Scheme
 * ================================================================================================
 * BAR0	| Enable		| 64-bit		| 1 MB			| Programmable Size
 * BAR1	| Disable		| 32-bit		| 64 KB			| Fixed Size
 *        BAR1 should be disabled if BAR0 is 64bit.
 * BAR2	| Enable		| 32-bit		| 1 MB			| Programmable Size
 * BAR3	| Enable		| 32-bit		| 64 KB			| Programmable Size
 * BAR4	| Enable		| 32-bit		| 1M			| Programmable Size
 * BAR5	| Enable		| 32-bit		| 64 KB			| Programmable Size
 */
static const struct pci_epc_features imx95_pcie_epc_features = {
	.msi_capable = true,
	.bar[BAR_1] = { .type = BAR_FIXED, .fixed_size = SZ_64K, },
	.align = SZ_4K,
};

static const struct pci_epc_features*
<<<<<<< HEAD
imx6_pcie_ep_get_features(struct dw_pcie_ep *ep)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);
	struct imx6_pcie *imx6_pcie = to_imx6_pcie(pci);

	return imx6_pcie->drvdata->epc_features;
}

static const struct dw_pcie_ep_ops pcie_ep_ops = {
	.init = imx6_pcie_ep_init,
	.raise_irq = imx6_pcie_ep_raise_irq,
	.get_features = imx6_pcie_ep_get_features,
};

static int imx6_add_pcie_ep(struct imx6_pcie *imx6_pcie,
=======
imx_pcie_ep_get_features(struct dw_pcie_ep *ep)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);
	struct imx_pcie *imx_pcie = to_imx_pcie(pci);

	return imx_pcie->drvdata->epc_features;
}

static const struct dw_pcie_ep_ops pcie_ep_ops = {
	.init = imx_pcie_ep_init,
	.raise_irq = imx_pcie_ep_raise_irq,
	.get_features = imx_pcie_ep_get_features,
};

static int imx_add_pcie_ep(struct imx_pcie *imx_pcie,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			   struct platform_device *pdev)
{
	int ret;
	unsigned int pcie_dbi2_offset;
	struct dw_pcie_ep *ep;
<<<<<<< HEAD
	struct dw_pcie *pci = imx6_pcie->pci;
	struct dw_pcie_rp *pp = &pci->pp;
	struct device *dev = pci->dev;

	imx6_pcie_host_init(pp);
	ep = &pci->ep;
	ep->ops = &pcie_ep_ops;

	switch (imx6_pcie->drvdata->variant) {
=======
	struct dw_pcie *pci = imx_pcie->pci;
	struct dw_pcie_rp *pp = &pci->pp;
	struct device *dev = pci->dev;

	imx_pcie_host_init(pp);
	ep = &pci->ep;
	ep->ops = &pcie_ep_ops;

	switch (imx_pcie->drvdata->variant) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	case IMX8MQ_EP:
	case IMX8MM_EP:
	case IMX8MP_EP:
		pcie_dbi2_offset = SZ_1M;
		break;
	default:
		pcie_dbi2_offset = SZ_4K;
		break;
	}

	pci->dbi_base2 = pci->dbi_base + pcie_dbi2_offset;

	/*
	 * FIXME: Ideally, dbi2 base address should come from DT. But since only IMX95 is defining
	 * "dbi2" in DT, "dbi_base2" is set to NULL here for that platform alone so that the DWC
	 * core code can fetch that from DT. But once all platform DTs were fixed, this and the
	 * above "dbi_base2" setting should be removed.
	 */
	if (device_property_match_string(dev, "reg-names", "dbi2") >= 0)
		pci->dbi_base2 = NULL;

<<<<<<< HEAD
	if (imx6_check_flag(imx6_pcie, IMX6_PCIE_FLAG_SUPPORT_64BIT))
		dma_set_mask_and_coherent(dev, DMA_BIT_MASK(64));

=======
	if (imx_check_flag(imx_pcie, IMX_PCIE_FLAG_SUPPORT_64BIT))
		dma_set_mask_and_coherent(dev, DMA_BIT_MASK(64));

	ep->page_size = imx_pcie->drvdata->epc_features->align;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	ret = dw_pcie_ep_init(ep);
	if (ret) {
		dev_err(dev, "failed to initialize endpoint\n");
		return ret;
	}

	ret = dw_pcie_ep_init_registers(ep);
	if (ret) {
		dev_err(dev, "Failed to initialize DWC endpoint registers\n");
		dw_pcie_ep_deinit(ep);
		return ret;
	}

	pci_epc_init_notify(ep->epc);

	/* Start LTSSM. */
<<<<<<< HEAD
	imx6_pcie_ltssm_enable(dev);
=======
	imx_pcie_ltssm_enable(dev);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return 0;
}

<<<<<<< HEAD
static void imx6_pcie_pm_turnoff(struct imx6_pcie *imx6_pcie)
{
	struct device *dev = imx6_pcie->pci->dev;

	/* Some variants have a turnoff reset in DT */
	if (imx6_pcie->turnoff_reset) {
		reset_control_assert(imx6_pcie->turnoff_reset);
		reset_control_deassert(imx6_pcie->turnoff_reset);
=======
static void imx_pcie_pm_turnoff(struct imx_pcie *imx_pcie)
{
	struct device *dev = imx_pcie->pci->dev;

	/* Some variants have a turnoff reset in DT */
	if (imx_pcie->turnoff_reset) {
		reset_control_assert(imx_pcie->turnoff_reset);
		reset_control_deassert(imx_pcie->turnoff_reset);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		goto pm_turnoff_sleep;
	}

	/* Others poke directly at IOMUXC registers */
<<<<<<< HEAD
	switch (imx6_pcie->drvdata->variant) {
	case IMX6SX:
	case IMX6QP:
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
				IMX6SX_GPR12_PCIE_PM_TURN_OFF,
				IMX6SX_GPR12_PCIE_PM_TURN_OFF);
		regmap_update_bits(imx6_pcie->iomuxc_gpr, IOMUXC_GPR12,
=======
	switch (imx_pcie->drvdata->variant) {
	case IMX6SX:
	case IMX6QP:
		regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
				IMX6SX_GPR12_PCIE_PM_TURN_OFF,
				IMX6SX_GPR12_PCIE_PM_TURN_OFF);
		regmap_update_bits(imx_pcie->iomuxc_gpr, IOMUXC_GPR12,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
				IMX6SX_GPR12_PCIE_PM_TURN_OFF, 0);
		break;
	default:
		dev_err(dev, "PME_Turn_Off not implemented\n");
		return;
	}

	/*
	 * Components with an upstream port must respond to
	 * PME_Turn_Off with PME_TO_Ack but we can't check.
	 *
	 * The standard recommends a 1-10ms timeout after which to
	 * proceed anyway as if acks were received.
	 */
pm_turnoff_sleep:
	usleep_range(1000, 10000);
}

<<<<<<< HEAD
static void imx6_pcie_msi_save_restore(struct imx6_pcie *imx6_pcie, bool save)
{
	u8 offset;
	u16 val;
	struct dw_pcie *pci = imx6_pcie->pci;
=======
static void imx_pcie_msi_save_restore(struct imx_pcie *imx_pcie, bool save)
{
	u8 offset;
	u16 val;
	struct dw_pcie *pci = imx_pcie->pci;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	if (pci_msi_enabled()) {
		offset = dw_pcie_find_capability(pci, PCI_CAP_ID_MSI);
		if (save) {
			val = dw_pcie_readw_dbi(pci, offset + PCI_MSI_FLAGS);
<<<<<<< HEAD
			imx6_pcie->msi_ctrl = val;
		} else {
			dw_pcie_dbi_ro_wr_en(pci);
			val = imx6_pcie->msi_ctrl;
=======
			imx_pcie->msi_ctrl = val;
		} else {
			dw_pcie_dbi_ro_wr_en(pci);
			val = imx_pcie->msi_ctrl;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			dw_pcie_writew_dbi(pci, offset + PCI_MSI_FLAGS, val);
			dw_pcie_dbi_ro_wr_dis(pci);
		}
	}
}

<<<<<<< HEAD
static int imx6_pcie_suspend_noirq(struct device *dev)
{
	struct imx6_pcie *imx6_pcie = dev_get_drvdata(dev);
	struct dw_pcie_rp *pp = &imx6_pcie->pci->pp;

	if (!(imx6_pcie->drvdata->flags & IMX6_PCIE_FLAG_SUPPORTS_SUSPEND))
		return 0;

	imx6_pcie_msi_save_restore(imx6_pcie, true);
	imx6_pcie_pm_turnoff(imx6_pcie);
	imx6_pcie_stop_link(imx6_pcie->pci);
	imx6_pcie_host_exit(pp);
=======
static int imx_pcie_suspend_noirq(struct device *dev)
{
	struct imx_pcie *imx_pcie = dev_get_drvdata(dev);
	struct dw_pcie_rp *pp = &imx_pcie->pci->pp;

	if (!(imx_pcie->drvdata->flags & IMX_PCIE_FLAG_SUPPORTS_SUSPEND))
		return 0;

	imx_pcie_msi_save_restore(imx_pcie, true);
	imx_pcie_pm_turnoff(imx_pcie);
	imx_pcie_stop_link(imx_pcie->pci);
	imx_pcie_host_exit(pp);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return 0;
}

<<<<<<< HEAD
static int imx6_pcie_resume_noirq(struct device *dev)
{
	int ret;
	struct imx6_pcie *imx6_pcie = dev_get_drvdata(dev);
	struct dw_pcie_rp *pp = &imx6_pcie->pci->pp;

	if (!(imx6_pcie->drvdata->flags & IMX6_PCIE_FLAG_SUPPORTS_SUSPEND))
		return 0;

	ret = imx6_pcie_host_init(pp);
	if (ret)
		return ret;
	imx6_pcie_msi_save_restore(imx6_pcie, false);
	dw_pcie_setup_rc(pp);

	if (imx6_pcie->link_is_up)
		imx6_pcie_start_link(imx6_pcie->pci);
=======
static int imx_pcie_resume_noirq(struct device *dev)
{
	int ret;
	struct imx_pcie *imx_pcie = dev_get_drvdata(dev);
	struct dw_pcie_rp *pp = &imx_pcie->pci->pp;

	if (!(imx_pcie->drvdata->flags & IMX_PCIE_FLAG_SUPPORTS_SUSPEND))
		return 0;

	ret = imx_pcie_host_init(pp);
	if (ret)
		return ret;
	imx_pcie_msi_save_restore(imx_pcie, false);
	dw_pcie_setup_rc(pp);

	if (imx_pcie->link_is_up)
		imx_pcie_start_link(imx_pcie->pci);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	return 0;
}

<<<<<<< HEAD
static const struct dev_pm_ops imx6_pcie_pm_ops = {
	NOIRQ_SYSTEM_SLEEP_PM_OPS(imx6_pcie_suspend_noirq,
				  imx6_pcie_resume_noirq)
};

static int imx6_pcie_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct dw_pcie *pci;
	struct imx6_pcie *imx6_pcie;
=======
static const struct dev_pm_ops imx_pcie_pm_ops = {
	NOIRQ_SYSTEM_SLEEP_PM_OPS(imx_pcie_suspend_noirq,
				  imx_pcie_resume_noirq)
};

static int imx_pcie_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct dw_pcie *pci;
	struct imx_pcie *imx_pcie;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	struct device_node *np;
	struct resource *dbi_base;
	struct device_node *node = dev->of_node;
	int ret;
	u16 val;
	int i;

<<<<<<< HEAD
	imx6_pcie = devm_kzalloc(dev, sizeof(*imx6_pcie), GFP_KERNEL);
	if (!imx6_pcie)
=======
	imx_pcie = devm_kzalloc(dev, sizeof(*imx_pcie), GFP_KERNEL);
	if (!imx_pcie)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		return -ENOMEM;

	pci = devm_kzalloc(dev, sizeof(*pci), GFP_KERNEL);
	if (!pci)
		return -ENOMEM;

	pci->dev = dev;
	pci->ops = &dw_pcie_ops;
<<<<<<< HEAD
	pci->pp.ops = &imx6_pcie_host_ops;

	imx6_pcie->pci = pci;
	imx6_pcie->drvdata = of_device_get_match_data(dev);
=======
	pci->pp.ops = &imx_pcie_host_ops;

	imx_pcie->pci = pci;
	imx_pcie->drvdata = of_device_get_match_data(dev);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	/* Find the PHY if one is defined, only imx7d uses it */
	np = of_parse_phandle(node, "fsl,imx7d-pcie-phy", 0);
	if (np) {
		struct resource res;

		ret = of_address_to_resource(np, 0, &res);
		if (ret) {
			dev_err(dev, "Unable to map PCIe PHY\n");
			return ret;
		}
<<<<<<< HEAD
		imx6_pcie->phy_base = devm_ioremap_resource(dev, &res);
		if (IS_ERR(imx6_pcie->phy_base))
			return PTR_ERR(imx6_pcie->phy_base);
=======
		imx_pcie->phy_base = devm_ioremap_resource(dev, &res);
		if (IS_ERR(imx_pcie->phy_base))
			return PTR_ERR(imx_pcie->phy_base);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}

	pci->dbi_base = devm_platform_get_and_ioremap_resource(pdev, 0, &dbi_base);
	if (IS_ERR(pci->dbi_base))
		return PTR_ERR(pci->dbi_base);

	/* Fetch GPIOs */
<<<<<<< HEAD
	imx6_pcie->reset_gpiod = devm_gpiod_get_optional(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(imx6_pcie->reset_gpiod))
		return dev_err_probe(dev, PTR_ERR(imx6_pcie->reset_gpiod),
				     "unable to get reset gpio\n");
	gpiod_set_consumer_name(imx6_pcie->reset_gpiod, "PCIe reset");

	if (imx6_pcie->drvdata->clks_cnt >= IMX6_PCIE_MAX_CLKS)
		return dev_err_probe(dev, -ENOMEM, "clks_cnt is too big\n");

	for (i = 0; i < imx6_pcie->drvdata->clks_cnt; i++)
		imx6_pcie->clks[i].id = imx6_pcie->drvdata->clk_names[i];

	/* Fetch clocks */
	ret = devm_clk_bulk_get(dev, imx6_pcie->drvdata->clks_cnt, imx6_pcie->clks);
	if (ret)
		return ret;

	if (imx6_check_flag(imx6_pcie, IMX6_PCIE_FLAG_HAS_PHYDRV)) {
		imx6_pcie->phy = devm_phy_get(dev, "pcie-phy");
		if (IS_ERR(imx6_pcie->phy))
			return dev_err_probe(dev, PTR_ERR(imx6_pcie->phy),
					     "failed to get pcie phy\n");
	}

	if (imx6_check_flag(imx6_pcie, IMX6_PCIE_FLAG_HAS_APP_RESET)) {
		imx6_pcie->apps_reset = devm_reset_control_get_exclusive(dev, "apps");
		if (IS_ERR(imx6_pcie->apps_reset))
			return dev_err_probe(dev, PTR_ERR(imx6_pcie->apps_reset),
					     "failed to get pcie apps reset control\n");
	}

	if (imx6_check_flag(imx6_pcie, IMX6_PCIE_FLAG_HAS_PHY_RESET)) {
		imx6_pcie->pciephy_reset = devm_reset_control_get_exclusive(dev, "pciephy");
		if (IS_ERR(imx6_pcie->pciephy_reset))
			return dev_err_probe(dev, PTR_ERR(imx6_pcie->pciephy_reset),
					     "Failed to get PCIEPHY reset control\n");
	}

	switch (imx6_pcie->drvdata->variant) {
=======
	imx_pcie->reset_gpiod = devm_gpiod_get_optional(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(imx_pcie->reset_gpiod))
		return dev_err_probe(dev, PTR_ERR(imx_pcie->reset_gpiod),
				     "unable to get reset gpio\n");
	gpiod_set_consumer_name(imx_pcie->reset_gpiod, "PCIe reset");

	if (imx_pcie->drvdata->clks_cnt >= IMX_PCIE_MAX_CLKS)
		return dev_err_probe(dev, -ENOMEM, "clks_cnt is too big\n");

	for (i = 0; i < imx_pcie->drvdata->clks_cnt; i++)
		imx_pcie->clks[i].id = imx_pcie->drvdata->clk_names[i];

	/* Fetch clocks */
	ret = devm_clk_bulk_get(dev, imx_pcie->drvdata->clks_cnt, imx_pcie->clks);
	if (ret)
		return ret;

	if (imx_check_flag(imx_pcie, IMX_PCIE_FLAG_HAS_PHYDRV)) {
		imx_pcie->phy = devm_phy_get(dev, "pcie-phy");
		if (IS_ERR(imx_pcie->phy))
			return dev_err_probe(dev, PTR_ERR(imx_pcie->phy),
					     "failed to get pcie phy\n");
	}

	if (imx_check_flag(imx_pcie, IMX_PCIE_FLAG_HAS_APP_RESET)) {
		imx_pcie->apps_reset = devm_reset_control_get_exclusive(dev, "apps");
		if (IS_ERR(imx_pcie->apps_reset))
			return dev_err_probe(dev, PTR_ERR(imx_pcie->apps_reset),
					     "failed to get pcie apps reset control\n");
	}

	if (imx_check_flag(imx_pcie, IMX_PCIE_FLAG_HAS_PHY_RESET)) {
		imx_pcie->pciephy_reset = devm_reset_control_get_exclusive(dev, "pciephy");
		if (IS_ERR(imx_pcie->pciephy_reset))
			return dev_err_probe(dev, PTR_ERR(imx_pcie->pciephy_reset),
					     "Failed to get PCIEPHY reset control\n");
	}

	switch (imx_pcie->drvdata->variant) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	case IMX8MQ:
	case IMX8MQ_EP:
	case IMX7D:
		if (dbi_base->start == IMX8MQ_PCIE2_BASE_ADDR)
<<<<<<< HEAD
			imx6_pcie->controller_id = 1;
=======
			imx_pcie->controller_id = 1;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		break;
	default:
		break;
	}

	/* Grab turnoff reset */
<<<<<<< HEAD
	imx6_pcie->turnoff_reset = devm_reset_control_get_optional_exclusive(dev, "turnoff");
	if (IS_ERR(imx6_pcie->turnoff_reset)) {
		dev_err(dev, "Failed to get TURNOFF reset control\n");
		return PTR_ERR(imx6_pcie->turnoff_reset);
	}

	if (imx6_pcie->drvdata->gpr) {
	/* Grab GPR config register range */
		imx6_pcie->iomuxc_gpr =
			 syscon_regmap_lookup_by_compatible(imx6_pcie->drvdata->gpr);
		if (IS_ERR(imx6_pcie->iomuxc_gpr))
			return dev_err_probe(dev, PTR_ERR(imx6_pcie->iomuxc_gpr),
					     "unable to find iomuxc registers\n");
	}

	if (imx6_check_flag(imx6_pcie, IMX6_PCIE_FLAG_HAS_SERDES)) {
=======
	imx_pcie->turnoff_reset = devm_reset_control_get_optional_exclusive(dev, "turnoff");
	if (IS_ERR(imx_pcie->turnoff_reset)) {
		dev_err(dev, "Failed to get TURNOFF reset control\n");
		return PTR_ERR(imx_pcie->turnoff_reset);
	}

	if (imx_pcie->drvdata->gpr) {
	/* Grab GPR config register range */
		imx_pcie->iomuxc_gpr =
			 syscon_regmap_lookup_by_compatible(imx_pcie->drvdata->gpr);
		if (IS_ERR(imx_pcie->iomuxc_gpr))
			return dev_err_probe(dev, PTR_ERR(imx_pcie->iomuxc_gpr),
					     "unable to find iomuxc registers\n");
	}

	if (imx_check_flag(imx_pcie, IMX_PCIE_FLAG_HAS_SERDES)) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		void __iomem *off = devm_platform_ioremap_resource_byname(pdev, "app");

		if (IS_ERR(off))
			return dev_err_probe(dev, PTR_ERR(off),
					     "unable to find serdes registers\n");

		static const struct regmap_config regmap_config = {
			.reg_bits = 32,
			.val_bits = 32,
			.reg_stride = 4,
		};

<<<<<<< HEAD
		imx6_pcie->iomuxc_gpr = devm_regmap_init_mmio(dev, off, &regmap_config);
		if (IS_ERR(imx6_pcie->iomuxc_gpr))
			return dev_err_probe(dev, PTR_ERR(imx6_pcie->iomuxc_gpr),
=======
		imx_pcie->iomuxc_gpr = devm_regmap_init_mmio(dev, off, &regmap_config);
		if (IS_ERR(imx_pcie->iomuxc_gpr))
			return dev_err_probe(dev, PTR_ERR(imx_pcie->iomuxc_gpr),
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
					     "unable to find iomuxc registers\n");
	}

	/* Grab PCIe PHY Tx Settings */
	if (of_property_read_u32(node, "fsl,tx-deemph-gen1",
<<<<<<< HEAD
				 &imx6_pcie->tx_deemph_gen1))
		imx6_pcie->tx_deemph_gen1 = 0;

	if (of_property_read_u32(node, "fsl,tx-deemph-gen2-3p5db",
				 &imx6_pcie->tx_deemph_gen2_3p5db))
		imx6_pcie->tx_deemph_gen2_3p5db = 0;

	if (of_property_read_u32(node, "fsl,tx-deemph-gen2-6db",
				 &imx6_pcie->tx_deemph_gen2_6db))
		imx6_pcie->tx_deemph_gen2_6db = 20;

	if (of_property_read_u32(node, "fsl,tx-swing-full",
				 &imx6_pcie->tx_swing_full))
		imx6_pcie->tx_swing_full = 127;

	if (of_property_read_u32(node, "fsl,tx-swing-low",
				 &imx6_pcie->tx_swing_low))
		imx6_pcie->tx_swing_low = 127;

	/* Limit link speed */
	pci->link_gen = 1;
	of_property_read_u32(node, "fsl,max-link-speed", &pci->link_gen);

	imx6_pcie->vpcie = devm_regulator_get_optional(&pdev->dev, "vpcie");
	if (IS_ERR(imx6_pcie->vpcie)) {
		if (PTR_ERR(imx6_pcie->vpcie) != -ENODEV)
			return PTR_ERR(imx6_pcie->vpcie);
		imx6_pcie->vpcie = NULL;
	}

	imx6_pcie->vph = devm_regulator_get_optional(&pdev->dev, "vph");
	if (IS_ERR(imx6_pcie->vph)) {
		if (PTR_ERR(imx6_pcie->vph) != -ENODEV)
			return PTR_ERR(imx6_pcie->vph);
		imx6_pcie->vph = NULL;
	}

	platform_set_drvdata(pdev, imx6_pcie);

	ret = imx6_pcie_attach_pd(dev);
	if (ret)
		return ret;

	if (imx6_pcie->drvdata->mode == DW_PCIE_EP_TYPE) {
		ret = imx6_add_pcie_ep(imx6_pcie, pdev);
=======
				 &imx_pcie->tx_deemph_gen1))
		imx_pcie->tx_deemph_gen1 = 0;

	if (of_property_read_u32(node, "fsl,tx-deemph-gen2-3p5db",
				 &imx_pcie->tx_deemph_gen2_3p5db))
		imx_pcie->tx_deemph_gen2_3p5db = 0;

	if (of_property_read_u32(node, "fsl,tx-deemph-gen2-6db",
				 &imx_pcie->tx_deemph_gen2_6db))
		imx_pcie->tx_deemph_gen2_6db = 20;

	if (of_property_read_u32(node, "fsl,tx-swing-full",
				 &imx_pcie->tx_swing_full))
		imx_pcie->tx_swing_full = 127;

	if (of_property_read_u32(node, "fsl,tx-swing-low",
				 &imx_pcie->tx_swing_low))
		imx_pcie->tx_swing_low = 127;

	/* Limit link speed */
	pci->max_link_speed = 1;
	of_property_read_u32(node, "fsl,max-link-speed", &pci->max_link_speed);

	imx_pcie->vpcie = devm_regulator_get_optional(&pdev->dev, "vpcie");
	if (IS_ERR(imx_pcie->vpcie)) {
		if (PTR_ERR(imx_pcie->vpcie) != -ENODEV)
			return PTR_ERR(imx_pcie->vpcie);
		imx_pcie->vpcie = NULL;
	}

	imx_pcie->vph = devm_regulator_get_optional(&pdev->dev, "vph");
	if (IS_ERR(imx_pcie->vph)) {
		if (PTR_ERR(imx_pcie->vph) != -ENODEV)
			return PTR_ERR(imx_pcie->vph);
		imx_pcie->vph = NULL;
	}

	platform_set_drvdata(pdev, imx_pcie);

	ret = imx_pcie_attach_pd(dev);
	if (ret)
		return ret;

	if (imx_pcie->drvdata->mode == DW_PCIE_EP_TYPE) {
		ret = imx_add_pcie_ep(imx_pcie, pdev);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (ret < 0)
			return ret;
	} else {
		ret = dw_pcie_host_init(&pci->pp);
		if (ret < 0)
			return ret;

		if (pci_msi_enabled()) {
			u8 offset = dw_pcie_find_capability(pci, PCI_CAP_ID_MSI);

			val = dw_pcie_readw_dbi(pci, offset + PCI_MSI_FLAGS);
			val |= PCI_MSI_FLAGS_ENABLE;
			dw_pcie_writew_dbi(pci, offset + PCI_MSI_FLAGS, val);
		}
	}

	return 0;
}

<<<<<<< HEAD
static void imx6_pcie_shutdown(struct platform_device *pdev)
{
	struct imx6_pcie *imx6_pcie = platform_get_drvdata(pdev);

	/* bring down link, so bootloader gets clean state in case of reboot */
	imx6_pcie_assert_core_reset(imx6_pcie);
=======
static void imx_pcie_shutdown(struct platform_device *pdev)
{
	struct imx_pcie *imx_pcie = platform_get_drvdata(pdev);

	/* bring down link, so bootloader gets clean state in case of reboot */
	imx_pcie_assert_core_reset(imx_pcie);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static const char * const imx6q_clks[] = {"pcie_bus", "pcie", "pcie_phy"};
static const char * const imx8mm_clks[] = {"pcie_bus", "pcie", "pcie_aux"};
static const char * const imx8mq_clks[] = {"pcie_bus", "pcie", "pcie_phy", "pcie_aux"};
static const char * const imx6sx_clks[] = {"pcie_bus", "pcie", "pcie_phy", "pcie_inbound_axi"};
<<<<<<< HEAD

static const struct imx6_pcie_drvdata drvdata[] = {
	[IMX6Q] = {
		.variant = IMX6Q,
		.flags = IMX6_PCIE_FLAG_IMX6_PHY |
			 IMX6_PCIE_FLAG_IMX6_SPEED_CHANGE,
=======
static const char * const imx8q_clks[] = {"mstr", "slv", "dbi"};

static const struct imx_pcie_drvdata drvdata[] = {
	[IMX6Q] = {
		.variant = IMX6Q,
		.flags = IMX_PCIE_FLAG_IMX_PHY |
			 IMX_PCIE_FLAG_IMX_SPEED_CHANGE,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.dbi_length = 0x200,
		.gpr = "fsl,imx6q-iomuxc-gpr",
		.clk_names = imx6q_clks,
		.clks_cnt = ARRAY_SIZE(imx6q_clks),
		.ltssm_off = IOMUXC_GPR12,
		.ltssm_mask = IMX6Q_GPR12_PCIE_CTL_2,
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
<<<<<<< HEAD
		.init_phy = imx6_pcie_init_phy,
	},
	[IMX6SX] = {
		.variant = IMX6SX,
		.flags = IMX6_PCIE_FLAG_IMX6_PHY |
			 IMX6_PCIE_FLAG_IMX6_SPEED_CHANGE |
			 IMX6_PCIE_FLAG_SUPPORTS_SUSPEND,
=======
		.init_phy = imx_pcie_init_phy,
		.enable_ref_clk = imx6q_pcie_enable_ref_clk,
		.core_reset = imx6q_pcie_core_reset,
	},
	[IMX6SX] = {
		.variant = IMX6SX,
		.flags = IMX_PCIE_FLAG_IMX_PHY |
			 IMX_PCIE_FLAG_IMX_SPEED_CHANGE |
			 IMX_PCIE_FLAG_SUPPORTS_SUSPEND,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.gpr = "fsl,imx6q-iomuxc-gpr",
		.clk_names = imx6sx_clks,
		.clks_cnt = ARRAY_SIZE(imx6sx_clks),
		.ltssm_off = IOMUXC_GPR12,
		.ltssm_mask = IMX6Q_GPR12_PCIE_CTL_2,
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
		.init_phy = imx6sx_pcie_init_phy,
<<<<<<< HEAD
	},
	[IMX6QP] = {
		.variant = IMX6QP,
		.flags = IMX6_PCIE_FLAG_IMX6_PHY |
			 IMX6_PCIE_FLAG_IMX6_SPEED_CHANGE |
			 IMX6_PCIE_FLAG_SUPPORTS_SUSPEND,
=======
		.enable_ref_clk = imx6sx_pcie_enable_ref_clk,
		.core_reset = imx6sx_pcie_core_reset,
	},
	[IMX6QP] = {
		.variant = IMX6QP,
		.flags = IMX_PCIE_FLAG_IMX_PHY |
			 IMX_PCIE_FLAG_IMX_SPEED_CHANGE |
			 IMX_PCIE_FLAG_SUPPORTS_SUSPEND,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.dbi_length = 0x200,
		.gpr = "fsl,imx6q-iomuxc-gpr",
		.clk_names = imx6q_clks,
		.clks_cnt = ARRAY_SIZE(imx6q_clks),
		.ltssm_off = IOMUXC_GPR12,
		.ltssm_mask = IMX6Q_GPR12_PCIE_CTL_2,
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
<<<<<<< HEAD
		.init_phy = imx6_pcie_init_phy,
	},
	[IMX7D] = {
		.variant = IMX7D,
		.flags = IMX6_PCIE_FLAG_SUPPORTS_SUSPEND |
			 IMX6_PCIE_FLAG_HAS_APP_RESET |
			 IMX6_PCIE_FLAG_HAS_PHY_RESET,
=======
		.init_phy = imx_pcie_init_phy,
		.enable_ref_clk = imx6q_pcie_enable_ref_clk,
		.core_reset = imx6qp_pcie_core_reset,
	},
	[IMX7D] = {
		.variant = IMX7D,
		.flags = IMX_PCIE_FLAG_SUPPORTS_SUSPEND |
			 IMX_PCIE_FLAG_HAS_APP_RESET |
			 IMX_PCIE_FLAG_HAS_PHY_RESET,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.gpr = "fsl,imx7d-iomuxc-gpr",
		.clk_names = imx6q_clks,
		.clks_cnt = ARRAY_SIZE(imx6q_clks),
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
		.init_phy = imx7d_pcie_init_phy,
<<<<<<< HEAD
	},
	[IMX8MQ] = {
		.variant = IMX8MQ,
		.flags = IMX6_PCIE_FLAG_HAS_APP_RESET |
			 IMX6_PCIE_FLAG_HAS_PHY_RESET,
=======
		.enable_ref_clk = imx7d_pcie_enable_ref_clk,
		.core_reset = imx7d_pcie_core_reset,
	},
	[IMX8MQ] = {
		.variant = IMX8MQ,
		.flags = IMX_PCIE_FLAG_HAS_APP_RESET |
			 IMX_PCIE_FLAG_HAS_PHY_RESET,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.gpr = "fsl,imx8mq-iomuxc-gpr",
		.clk_names = imx8mq_clks,
		.clks_cnt = ARRAY_SIZE(imx8mq_clks),
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
		.mode_off[1] = IOMUXC_GPR12,
		.mode_mask[1] = IMX8MQ_GPR12_PCIE2_CTRL_DEVICE_TYPE,
		.init_phy = imx8mq_pcie_init_phy,
<<<<<<< HEAD
	},
	[IMX8MM] = {
		.variant = IMX8MM,
		.flags = IMX6_PCIE_FLAG_SUPPORTS_SUSPEND |
			 IMX6_PCIE_FLAG_HAS_PHYDRV |
			 IMX6_PCIE_FLAG_HAS_APP_RESET,
=======
		.enable_ref_clk = imx8mm_pcie_enable_ref_clk,
	},
	[IMX8MM] = {
		.variant = IMX8MM,
		.flags = IMX_PCIE_FLAG_SUPPORTS_SUSPEND |
			 IMX_PCIE_FLAG_HAS_PHYDRV |
			 IMX_PCIE_FLAG_HAS_APP_RESET,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.gpr = "fsl,imx8mm-iomuxc-gpr",
		.clk_names = imx8mm_clks,
		.clks_cnt = ARRAY_SIZE(imx8mm_clks),
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
<<<<<<< HEAD
	},
	[IMX8MP] = {
		.variant = IMX8MP,
		.flags = IMX6_PCIE_FLAG_SUPPORTS_SUSPEND |
			 IMX6_PCIE_FLAG_HAS_PHYDRV |
			 IMX6_PCIE_FLAG_HAS_APP_RESET,
=======
		.enable_ref_clk = imx8mm_pcie_enable_ref_clk,
	},
	[IMX8MP] = {
		.variant = IMX8MP,
		.flags = IMX_PCIE_FLAG_SUPPORTS_SUSPEND |
			 IMX_PCIE_FLAG_HAS_PHYDRV |
			 IMX_PCIE_FLAG_HAS_APP_RESET,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.gpr = "fsl,imx8mp-iomuxc-gpr",
		.clk_names = imx8mm_clks,
		.clks_cnt = ARRAY_SIZE(imx8mm_clks),
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
<<<<<<< HEAD
	},
	[IMX95] = {
		.variant = IMX95,
		.flags = IMX6_PCIE_FLAG_HAS_SERDES,
=======
		.enable_ref_clk = imx8mm_pcie_enable_ref_clk,
	},
	[IMX8Q] = {
		.variant = IMX8Q,
		.flags = IMX_PCIE_FLAG_HAS_PHYDRV |
			 IMX_PCIE_FLAG_CPU_ADDR_FIXUP,
		.clk_names = imx8q_clks,
		.clks_cnt = ARRAY_SIZE(imx8q_clks),
	},
	[IMX95] = {
		.variant = IMX95,
		.flags = IMX_PCIE_FLAG_HAS_SERDES,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.clk_names = imx8mq_clks,
		.clks_cnt = ARRAY_SIZE(imx8mq_clks),
		.ltssm_off = IMX95_PE0_GEN_CTRL_3,
		.ltssm_mask = IMX95_PCIE_LTSSM_EN,
		.mode_off[0]  = IMX95_PE0_GEN_CTRL_1,
		.mode_mask[0] = IMX95_PCIE_DEVICE_TYPE,
		.init_phy = imx95_pcie_init_phy,
	},
	[IMX8MQ_EP] = {
		.variant = IMX8MQ_EP,
<<<<<<< HEAD
		.flags = IMX6_PCIE_FLAG_HAS_APP_RESET |
			 IMX6_PCIE_FLAG_HAS_PHY_RESET,
=======
		.flags = IMX_PCIE_FLAG_HAS_APP_RESET |
			 IMX_PCIE_FLAG_HAS_PHY_RESET,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.mode = DW_PCIE_EP_TYPE,
		.gpr = "fsl,imx8mq-iomuxc-gpr",
		.clk_names = imx8mq_clks,
		.clks_cnt = ARRAY_SIZE(imx8mq_clks),
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
		.mode_off[1] = IOMUXC_GPR12,
		.mode_mask[1] = IMX8MQ_GPR12_PCIE2_CTRL_DEVICE_TYPE,
		.epc_features = &imx8m_pcie_epc_features,
		.init_phy = imx8mq_pcie_init_phy,
<<<<<<< HEAD
	},
	[IMX8MM_EP] = {
		.variant = IMX8MM_EP,
		.flags = IMX6_PCIE_FLAG_HAS_PHYDRV,
=======
		.enable_ref_clk = imx8mm_pcie_enable_ref_clk,
	},
	[IMX8MM_EP] = {
		.variant = IMX8MM_EP,
		.flags = IMX_PCIE_FLAG_HAS_APP_RESET |
			 IMX_PCIE_FLAG_HAS_PHYDRV,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.mode = DW_PCIE_EP_TYPE,
		.gpr = "fsl,imx8mm-iomuxc-gpr",
		.clk_names = imx8mm_clks,
		.clks_cnt = ARRAY_SIZE(imx8mm_clks),
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
		.epc_features = &imx8m_pcie_epc_features,
<<<<<<< HEAD
	},
	[IMX8MP_EP] = {
		.variant = IMX8MP_EP,
		.flags = IMX6_PCIE_FLAG_HAS_PHYDRV,
=======
		.enable_ref_clk = imx8mm_pcie_enable_ref_clk,
	},
	[IMX8MP_EP] = {
		.variant = IMX8MP_EP,
		.flags = IMX_PCIE_FLAG_HAS_APP_RESET |
			 IMX_PCIE_FLAG_HAS_PHYDRV,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.mode = DW_PCIE_EP_TYPE,
		.gpr = "fsl,imx8mp-iomuxc-gpr",
		.clk_names = imx8mm_clks,
		.clks_cnt = ARRAY_SIZE(imx8mm_clks),
		.mode_off[0] = IOMUXC_GPR12,
		.mode_mask[0] = IMX6Q_GPR12_DEVICE_TYPE,
		.epc_features = &imx8m_pcie_epc_features,
<<<<<<< HEAD
	},
	[IMX95_EP] = {
		.variant = IMX95_EP,
		.flags = IMX6_PCIE_FLAG_HAS_SERDES |
			 IMX6_PCIE_FLAG_SUPPORT_64BIT,
=======
		.enable_ref_clk = imx8mm_pcie_enable_ref_clk,
	},
	[IMX95_EP] = {
		.variant = IMX95_EP,
		.flags = IMX_PCIE_FLAG_HAS_SERDES |
			 IMX_PCIE_FLAG_SUPPORT_64BIT,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		.clk_names = imx8mq_clks,
		.clks_cnt = ARRAY_SIZE(imx8mq_clks),
		.ltssm_off = IMX95_PE0_GEN_CTRL_3,
		.ltssm_mask = IMX95_PCIE_LTSSM_EN,
		.mode_off[0]  = IMX95_PE0_GEN_CTRL_1,
		.mode_mask[0] = IMX95_PCIE_DEVICE_TYPE,
		.init_phy = imx95_pcie_init_phy,
		.epc_features = &imx95_pcie_epc_features,
		.mode = DW_PCIE_EP_TYPE,
	},
};

<<<<<<< HEAD
static const struct of_device_id imx6_pcie_of_match[] = {
=======
static const struct of_device_id imx_pcie_of_match[] = {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	{ .compatible = "fsl,imx6q-pcie",  .data = &drvdata[IMX6Q],  },
	{ .compatible = "fsl,imx6sx-pcie", .data = &drvdata[IMX6SX], },
	{ .compatible = "fsl,imx6qp-pcie", .data = &drvdata[IMX6QP], },
	{ .compatible = "fsl,imx7d-pcie",  .data = &drvdata[IMX7D],  },
	{ .compatible = "fsl,imx8mq-pcie", .data = &drvdata[IMX8MQ], },
	{ .compatible = "fsl,imx8mm-pcie", .data = &drvdata[IMX8MM], },
	{ .compatible = "fsl,imx8mp-pcie", .data = &drvdata[IMX8MP], },
<<<<<<< HEAD
=======
	{ .compatible = "fsl,imx8q-pcie", .data = &drvdata[IMX8Q], },
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	{ .compatible = "fsl,imx95-pcie", .data = &drvdata[IMX95], },
	{ .compatible = "fsl,imx8mq-pcie-ep", .data = &drvdata[IMX8MQ_EP], },
	{ .compatible = "fsl,imx8mm-pcie-ep", .data = &drvdata[IMX8MM_EP], },
	{ .compatible = "fsl,imx8mp-pcie-ep", .data = &drvdata[IMX8MP_EP], },
	{ .compatible = "fsl,imx95-pcie-ep", .data = &drvdata[IMX95_EP], },
	{},
};

<<<<<<< HEAD
static struct platform_driver imx6_pcie_driver = {
	.driver = {
		.name	= "imx6q-pcie",
		.of_match_table = imx6_pcie_of_match,
		.suppress_bind_attrs = true,
		.pm = &imx6_pcie_pm_ops,
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
	},
	.probe    = imx6_pcie_probe,
	.shutdown = imx6_pcie_shutdown,
};

static void imx6_pcie_quirk(struct pci_dev *dev)
=======
static struct platform_driver imx_pcie_driver = {
	.driver = {
		.name	= "imx6q-pcie",
		.of_match_table = imx_pcie_of_match,
		.suppress_bind_attrs = true,
		.pm = &imx_pcie_pm_ops,
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
	},
	.probe    = imx_pcie_probe,
	.shutdown = imx_pcie_shutdown,
};

static void imx_pcie_quirk(struct pci_dev *dev)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
	struct pci_bus *bus = dev->bus;
	struct dw_pcie_rp *pp = bus->sysdata;

	/* Bus parent is the PCI bridge, its parent is this platform driver */
	if (!bus->dev.parent || !bus->dev.parent->parent)
		return;

	/* Make sure we only quirk devices associated with this driver */
<<<<<<< HEAD
	if (bus->dev.parent->parent->driver != &imx6_pcie_driver.driver)
=======
	if (bus->dev.parent->parent->driver != &imx_pcie_driver.driver)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		return;

	if (pci_is_root_bus(bus)) {
		struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
<<<<<<< HEAD
		struct imx6_pcie *imx6_pcie = to_imx6_pcie(pci);
=======
		struct imx_pcie *imx_pcie = to_imx_pcie(pci);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

		/*
		 * Limit config length to avoid the kernel reading beyond
		 * the register set and causing an abort on i.MX 6Quad
		 */
<<<<<<< HEAD
		if (imx6_pcie->drvdata->dbi_length) {
			dev->cfg_size = imx6_pcie->drvdata->dbi_length;
=======
		if (imx_pcie->drvdata->dbi_length) {
			dev->cfg_size = imx_pcie->drvdata->dbi_length;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			dev_info(&dev->dev, "Limiting cfg_size to %d\n",
					dev->cfg_size);
		}
	}
}
DECLARE_PCI_FIXUP_CLASS_HEADER(PCI_VENDOR_ID_SYNOPSYS, 0xabcd,
<<<<<<< HEAD
			PCI_CLASS_BRIDGE_PCI, 8, imx6_pcie_quirk);

static int __init imx6_pcie_init(void)
=======
			PCI_CLASS_BRIDGE_PCI, 8, imx_pcie_quirk);

static int __init imx_pcie_init(void)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
{
#ifdef CONFIG_ARM
	struct device_node *np;

<<<<<<< HEAD
	np = of_find_matching_node(NULL, imx6_pcie_of_match);
=======
	np = of_find_matching_node(NULL, imx_pcie_of_match);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	if (!np)
		return -ENODEV;
	of_node_put(np);

	/*
	 * Since probe() can be deferred we need to make sure that
	 * hook_fault_code is not called after __init memory is freed
	 * by kernel and since imx6q_pcie_abort_handler() is a no-op,
	 * we can install the handler here without risking it
	 * accessing some uninitialized driver state.
	 */
	hook_fault_code(8, imx6q_pcie_abort_handler, SIGBUS, 0,
			"external abort on non-linefetch");
#endif

<<<<<<< HEAD
	return platform_driver_register(&imx6_pcie_driver);
}
device_initcall(imx6_pcie_init);
=======
	return platform_driver_register(&imx_pcie_driver);
}
device_initcall(imx_pcie_init);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
