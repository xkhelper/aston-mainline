#
# Makefile for the Synaptics TCM touchscreen driver.
#

# Each configuration option enables a list of files.



TCM_CORE=tcm/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/synaptics_hbp/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/synaptics_hbp/tcm/

synaptics_tcm2-y := syna_tcm2.o

synaptics_tcm2-y += \
					$(TCM_CORE)synaptics_touchcom_core_v1.o \
					$(TCM_CORE)synaptics_touchcom_core_v2.o \
 					$(TCM_CORE)synaptics_touchcom_func_base.o \
 					$(TCM_CORE)synaptics_touchcom_func_touch.o \
					$(TCM_CORE)synaptics_touchcom_func_reflash.o

synaptics_tcm2-y += syna_tcm2_platform_spi.o

obj-$(CONFIG_TOUCHSCREEN_SYNA_TCM2) += synaptics_tcm2.o

oplus_bsp_tp_custom-y := touch.o

obj-$(CONFIG_TOUCHPANEL_CUSTOM)	+= oplus_bsp_tp_custom.o