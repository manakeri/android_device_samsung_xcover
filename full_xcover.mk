LOCAL_PATH := device/samsung/xcover

$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full.mk)
$(call inherit-product-if-exists, vendor/samsung/xcover/xcover-vendor.mk)

DEVICE_PACKAGE_OVERLAYS := device/samsung/epicmtd/overlay

ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
	$(LOCAL_KERNEL):kernel \
	$(call find-copy-subdir-files,*,device/samsung/xcover/modules,system/lib/modules)

PRODUCT_COPY_FILES += \
    device/samsung/xcover/ueventd.marvell.rc:root/ueventd.marvell.rc \
    device/samsung/xcover/lpm.rc:root/lpm.rc \
    device/samsung/xcover/init.rc:root/init.rc

PRODUCT_COPY_FILES += \
    device/samsung/xcover/egl.cfg:system/lib/egl/egl.cfg

# copy the filesystem converter
PRODUCT_COPY_FILES += \
  device/samsung/xcover/updater.sh:updater.sh

# update utilities
PRODUCT_PACKAGES += \
	bml_over_mtd \
	setup_fs

# The OpenGL ES API level that is natively supported by this device.
# This is a 16.16 fixed point number
PRODUCT_PROPERTY_OVERRIDES := \
    ro.opengles.version=131072
	    
PRODUCT_LOCALES += hdpi
PRODUCT_NAME := full_xcover
PRODUCT_DEVICE := xcover
PRODUCT_BRAND := samsung
PRODUCT_MANUFACTURER := samsung
PRODUCT_BOARD := marvell
PRODUCT_MODEL := xcover
PRODUCT_PLATFORM := marvell

