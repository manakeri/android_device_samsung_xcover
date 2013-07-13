# Inherit AOSP device configuration for epic.
$(call inherit-product, device/samsung/xcover/device_xcover.mk)

# Inherit some common cyanogenmod stuff.
$(call inherit-product, vendor/cyanogen/products/common_full.mk)

PRODUCT_PACKAGE_OVERLAYS += vendor/cyanogen/overlay/xcover

#
# Setup device specific product configuration.
#
PRODUCT_NAME := cyanogen_xcover
PRODUCT_BRAND := samsung
PRODUCT_DEVICE := xcover
PRODUCT_MODEL := GT-S5690
PRODUCT_MANUFACTURER := samsung

# Release name and versioning
PRODUCT_RELEASE_NAME := xcover
PRODUCT_VERSION_DEVICE_SPECIFIC :=
-include vendor/cyanogen/products/common_versions.mk

PRODUCT_COPY_FILES +=  \
    vendor/cyanogen/prebuilt/hdpi/media/bootanimation.zip:system/media/bootanimation.zip
