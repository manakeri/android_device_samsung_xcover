# Release name
PRODUCT_RELEASE_NAME := xcover

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_mini_phone.mk)

# Inherit CDMA stuff
$(call inherit-product, vendor/cm/config/gsm.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/xcover/full_xcover.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := xcover
PRODUCT_NAME := cm_xcover
PRODUCT_BRAND := samsung
PRODUCT_MODEL := GT-S5690
PRODUCT_MANUFACTURER := samsung

