/* SPDX-License-Identifier: MIT */
#ifndef __NVIF_DRIVER_H__
#define __NVIF_DRIVER_H__
#include <nvif/os.h>
struct nvif_client;

struct nvif_driver {
	const char *name;
	int (*init)(const char *name, u64 device, const char *cfg,
		    const char *dbg, void **priv);
<<<<<<< HEAD
	void (*fini)(void *priv);
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	int (*suspend)(void *priv);
	int (*resume)(void *priv);
	int (*ioctl)(void *priv, void *data, u32 size, void **hack);
	void __iomem *(*map)(void *priv, u64 handle, u32 size);
	void (*unmap)(void *priv, void __iomem *ptr, u32 size);
<<<<<<< HEAD
	bool keep;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
};

int nvif_driver_init(const char *drv, const char *cfg, const char *dbg,
		     const char *name, u64 device, struct nvif_client *);

extern const struct nvif_driver nvif_driver_nvkm;
<<<<<<< HEAD
extern const struct nvif_driver nvif_driver_drm;
extern const struct nvif_driver nvif_driver_lib;
extern const struct nvif_driver nvif_driver_null;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#endif
