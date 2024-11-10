/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2019 Intel Corporation
 */

#ifndef __INTEL_ACPI_H__
#define __INTEL_ACPI_H__

<<<<<<< HEAD
struct drm_i915_private;
=======
struct intel_display;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

#ifdef CONFIG_ACPI
void intel_register_dsm_handler(void);
void intel_unregister_dsm_handler(void);
<<<<<<< HEAD
void intel_dsm_get_bios_data_funcs_supported(struct drm_i915_private *i915);
void intel_acpi_device_id_update(struct drm_i915_private *i915);
void intel_acpi_assign_connector_fwnodes(struct drm_i915_private *i915);
void intel_acpi_video_register(struct drm_i915_private *i915);
=======
void intel_dsm_get_bios_data_funcs_supported(struct intel_display *display);
void intel_acpi_device_id_update(struct intel_display *display);
void intel_acpi_assign_connector_fwnodes(struct intel_display *display);
void intel_acpi_video_register(struct intel_display *display);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#else
static inline void intel_register_dsm_handler(void) { return; }
static inline void intel_unregister_dsm_handler(void) { return; }
static inline
<<<<<<< HEAD
void intel_dsm_get_bios_data_funcs_supported(struct drm_i915_private *i915) { return; }
static inline
void intel_acpi_device_id_update(struct drm_i915_private *i915) { return; }
static inline
void intel_acpi_assign_connector_fwnodes(struct drm_i915_private *i915) { return; }
static inline
void intel_acpi_video_register(struct drm_i915_private *i915) { return; }
=======
void intel_dsm_get_bios_data_funcs_supported(struct intel_display *display) { return; }
static inline
void intel_acpi_device_id_update(struct intel_display *display) { return; }
static inline
void intel_acpi_assign_connector_fwnodes(struct intel_display *display) { return; }
static inline
void intel_acpi_video_register(struct intel_display *display) { return; }
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
#endif /* CONFIG_ACPI */

#endif /* __INTEL_ACPI_H__ */
