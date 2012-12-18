#$(call inherit-product, $(SRC_TARGET_DIR)/product/full.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/mini.mk)
$(call inherit-product, device/samsung/xcover/device_xcover.mk)

PRODUCT_NAME := cm_xcover
PRODUCT_DEVICE := xcover
PRODUCT_BRAND := samsung
PRODUCT_MANUFACTURER := samsung
PRODUCT_MODEL := GT-S5690


