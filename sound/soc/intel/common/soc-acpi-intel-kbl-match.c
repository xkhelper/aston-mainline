// SPDX-License-Identifier: GPL-2.0-only
/*
 * soc-acpi-intel-kbl-match.c - tables and support for KBL ACPI enumeration.
 *
 * Copyright (c) 2018, Intel Corporation.
 *
 */

#include <sound/soc-acpi.h>
#include <sound/soc-acpi-intel-match.h>
<<<<<<< HEAD
#include "../skylake/skl.h"

static struct skl_machine_pdata skl_dmic_data;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

static const struct snd_soc_acpi_codecs kbl_codecs = {
	.num_codecs = 1,
	.codecs = {"10508825"}
};

static const struct snd_soc_acpi_codecs kbl_poppy_codecs = {
	.num_codecs = 1,
	.codecs = {"10EC5663"}
};

static const struct snd_soc_acpi_codecs kbl_5663_5514_codecs = {
	.num_codecs = 2,
	.codecs = {"10EC5663", "10EC5514"}
};

static const struct snd_soc_acpi_codecs kbl_7219_98357_codecs = {
	.num_codecs = 1,
	.codecs = {"MX98357A"}
};

static const struct snd_soc_acpi_codecs kbl_7219_98927_codecs = {
	.num_codecs = 1,
	.codecs = {"MX98927"}
};

static const struct snd_soc_acpi_codecs kbl_7219_98373_codecs = {
	.num_codecs = 1,
	.codecs = {"MX98373"}
};

struct snd_soc_acpi_mach snd_soc_acpi_intel_kbl_machines[] = {
	{
		.id = "INT343A",
		.drv_name = "kbl_alc286s_i2s",
		.fw_filename = "intel/dsp_fw_kbl.bin",
	},
	{
		.id = "INT343B",
		.drv_name = "kbl_n88l25_s4567",
		.fw_filename = "intel/dsp_fw_kbl.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &kbl_codecs,
<<<<<<< HEAD
		.pdata = &skl_dmic_data,
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{
		.id = "MX98357A",
		.drv_name = "kbl_n88l25_m98357a",
		.fw_filename = "intel/dsp_fw_kbl.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &kbl_codecs,
<<<<<<< HEAD
		.pdata = &skl_dmic_data,
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{
		.id = "MX98927",
		.drv_name = "kbl_r5514_5663_max",
		.fw_filename = "intel/dsp_fw_kbl.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &kbl_5663_5514_codecs,
<<<<<<< HEAD
		.pdata = &skl_dmic_data,
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{
		.id = "MX98927",
		.drv_name = "kbl_rt5663_m98927",
		.fw_filename = "intel/dsp_fw_kbl.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &kbl_poppy_codecs,
<<<<<<< HEAD
		.pdata = &skl_dmic_data,
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{
		.id = "10EC5663",
		.drv_name = "kbl_rt5663",
		.fw_filename = "intel/dsp_fw_kbl.bin",
	},
	{
		.id = "DLGS7219",
		.drv_name = "kbl_da7219_mx98357a",
		.fw_filename = "intel/dsp_fw_kbl.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &kbl_7219_98357_codecs,
<<<<<<< HEAD
		.pdata = &skl_dmic_data,
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{
		.id = "DLGS7219",
		.drv_name = "kbl_da7219_max98927",
		.fw_filename = "intel/dsp_fw_kbl.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &kbl_7219_98927_codecs,
<<<<<<< HEAD
		.pdata = &skl_dmic_data
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{
		.id = "10EC5660",
		.drv_name = "kbl_rt5660",
		.fw_filename = "intel/dsp_fw_kbl.bin",
	},
	{
		.id = "10EC3277",
		.drv_name = "kbl_rt5660",
		.fw_filename = "intel/dsp_fw_kbl.bin",
	},
	{
		.id = "DLGS7219",
		.drv_name = "kbl_da7219_mx98373",
		.fw_filename = "intel/dsp_fw_kbl.bin",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &kbl_7219_98373_codecs,
<<<<<<< HEAD
		.pdata = &skl_dmic_data
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{
		.id = "MX98373",
		.drv_name = "kbl_max98373",
		.fw_filename = "intel/dsp_fw_kbl.bin",
<<<<<<< HEAD
		.pdata = &skl_dmic_data
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	},
	{},
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_kbl_machines);
