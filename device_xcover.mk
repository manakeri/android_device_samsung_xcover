LOCAL_PATH := device/samsung/xcover

$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full.mk)
$(call inherit-product-if-exists, vendor/samsung/xcover/xcover-vendor.mk)

ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
	$(LOCAL_KERNEL):kernel \
	$(call find-copy-subdir-files,*,device/samsung/xcover/modules,system/lib/modules)
    
PRODUCT_PACKAGES += \
	libvmetahal \
	libpmemhelper \
	libphycontmem \
	libgcu \
	libbmm

  
PRODUCT_LOCALES += ldpi
PRODUCT_NAME := cyanogen_xcover
PRODUCT_DEVICE := xcover
PRODUCT_BRAND := samsung
PRODUCT_MANUFACTURER := samsung
PRODUCT_BOARD := marvell
PRODUCT_MODEL := xcover
PRODUCT_PLATFORM := marvell

