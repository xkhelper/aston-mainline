/* SPDX-License-Identifier: MIT */
#ifndef __NVKM_DEVICE_PCI_H__
#define __NVKM_DEVICE_PCI_H__
#include <core/device.h>

struct nvkm_device_pci {
	struct nvkm_device device;
	struct pci_dev *pdev;
	bool suspend;
};

int nvkm_device_pci_new(struct pci_dev *, const char *cfg, const char *dbg,
<<<<<<< HEAD
			bool detect, bool mmio, u64 subdev_mask,
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
			struct nvkm_device **);
#endif
