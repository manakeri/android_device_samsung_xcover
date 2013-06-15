## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := xcover

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/xcover/device_xcover.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := xcover
PRODUCT_NAME := cm_xcover
PRODUCT_BRAND := samsung
PRODUCT_MODEL := xcover
PRODUCT_MANUFACTURER := samsung
