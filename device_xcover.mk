$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_small.mk)

# The gps config appropriate for this device
#$(call inherit-product, device/common/gps/gps_us_supl.mk)

$(call inherit-product-if-exists, vendor/samsung/xcover/xcover-vendor.mk)

PRODUCT_NAME := cyanogen_xcover
PRODUCT_DEVICE := xcover
PRODUCT_MANUFACTURER := samsung
PRODUCT_BRAND := samsung
PRODUCT_MODEL := GT-S5690

DEVICE_PACKAGE_OVERLAYS += device/samsung/xcover/overlay

LOCAL_PATH := device/samsung/xcover
ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/prebuilt/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel

PRODUCT_COPY_FILES += device/samsung/xcover/prebuilt/asound.conf:system/etc/asound.conf
PRODUCT_COPY_FILES += device/samsung/xcover/prebuilt/vold.fstab:system/etc/vold.fstab
PRODUCT_COPY_FILES += device/samsung/xcover/prebuilt/init.rc:root/init.rc

PRODUCT_COPY_FILES += device/samsung/xcover/prebuilt/ueventd.pxa968.rc:root/ueventd.pxa968.rc
PRODUCT_COPY_FILES += device/samsung/xcover/prebuilt/init.pxa968.rc:root/init.pxa968.rc
PRODUCT_COPY_FILES += device/samsung/xcover/prebuilt/lpm.rc:root/lpm.rc

PRODUCT_PACKAGES += bml_over_mtd
PRODUCT_PACKAGES += setup_fs
PRODUCT_COPY_FILES += device/samsung/xcover/updater.sh:updater.sh

PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/samsung/xcover/prebuilt/modules,system/lib/modules)

PRODUCT_DEFAULT_PROPERTY_OVERRIDES := \
    persist.service.adb.enable=1
#    persist.sys.usb.config=mass_storage

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0

$(call inherit-product-if-exists, vendor/samsung/xcover/xcover-vendor.mk)
