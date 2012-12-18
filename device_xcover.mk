$(call inherit-product, $(SRC_TARGET_DIR)/product/generic.mk)

ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
	$(LOCAL_KERNEL):kernel \
#	$(call find-copy-subdir-files,*,device/samsung/xcover/modules,system/lib/modules)

#PRODUCT_COPY_FILES += \
#    device/samsung/xcover/ueventd.marvell.rc:root/ueventd.marvell.rc \
#    device/samsung/xcover/lpm.rc:root/lpm.rc \
#    device/samsung/xcover/init.rc:root/init.rc \
#    device/samsung/xcover/egl.cfg:system/lib/egl/egl.cfg \
#    device/samsung/xcover/updater.sh:updater.sh 

# update utilities
#PRODUCT_PACKAGES += \
#	bml_over_mtd \
#	libbmlutils

#$(call inherit-product-if-exists, vendor/samsung/xcover/xcover-vendor.mk)
