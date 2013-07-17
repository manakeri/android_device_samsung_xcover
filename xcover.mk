#
# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# These is the hardware-specific overlay, which points to the location
# of hardware-specific resource overrides, typically the frameworks and
# application settings that are stored in resourced.


DEVICE_PACKAGE_OVERLAYS := device/samsung/xcover/overlay

# These are the hardware-specific configuration files
#PRODUCT_COPY_FILES := \
#    device/samsung/xcover/prebuilt/asound.conf:system/etc/asound.conf \
#    device/samsung/xcover/prebuilt/vold.fstab:system/etc/vold.fstab
#    device/samsung/xcover/prebuilt/egl.cfg:system/lib/egl/egl.cfg


PRODUCT_COPY_FILES += $(call find-copy-subdir-files,*,device/samsung/xcover/prebuilt/modules,system/lib/modules)

# Init files
#PRODUCT_COPY_FILES += \
#  device/samsung/epicmtd/ueventd.victory.rc:root/ueventd.victory.rc \
#  device/samsung/epicmtd/lpm.rc:root/lpm.rc \
#  device/samsung/epicmtd/init.victory.rc:root/init.victory.rc \
#  device/samsung/epicmtd/init.victory.usb.rc:root/init.victory.usb.rc

# update utilities
#PRODUCT_PACKAGES += \
#	bml_over_mtd \
#	setup_fs

# copy the filesystem converter
#PRODUCT_COPY_FILES += \
#  device/samsung/epicmtd/updater.sh:updater.sh

# Enable "Android debugging" in Settings menu.
# Enable USB mass storage (and adb) at boot.
PRODUCT_DEFAULT_PROPERTY_OVERRIDES := \
    persist.service.adb.enable=1 \
    persist.sys.usb.config=mass_storage

# The OpenGL ES API level that is natively supported by this device.
# This is a 16.16 fixed point number
PRODUCT_PROPERTY_OVERRIDES := \
    ro.opengles.version=131072



$(call inherit-product-if-exists, vendor/samsung/xcover/xcover-vendor.mk)
