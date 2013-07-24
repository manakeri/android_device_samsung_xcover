# Release name
PRODUCT_RELEASE_NAME := xcover

# Inherit some common CM stuff.
#$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit CDMA stuff
#$(call inherit-product, vendor/cm/config/cdma.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/xcover/full_xcover.mk)

## Device identifier. This must come after all inclusions
#PRODUCT_DEVICE := xcover
#PRODUCT_NAME := cm_xcover
#PRODUCT_BRAND := samsung
#PRODUCT_MODEL := GT-S5690
#PRODUCT_MANUFACTURER := samsung

#Set build fingerprint / ID / Product Name ect.
#PRODUCT_BUILD_PROP_OVERRIDES += PRODUCT_NAME=SPH-D700 TARGET_DEVICE=SPH-D700 BUILD_FINGERPRINT=sprint/SPH-D700/SPH-D700:2.3.5/GINGERBREAD/EI22:user/release-keys PRIVATE_BUILD_DESC="SPH-D700-user 2.3.5 GINGERBREAD EI22 release-keys"
