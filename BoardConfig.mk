# inherit from the proprietary version
-include vendor/samsung/xcover/BoardConfigVendor.mk

TARGET_BOARD_PLATFORM := marvell
TARGET_BOOTLOADER_BOARD_NAME := marvell
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi

TARGET_NO_KERNEL := false
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# media 
HAVE_HTC_AUDIO_DRIVER := false
BOARD_USES_GENERIC_AUDIO := true

USE_CAMERA_STUB := true
ifeq ($(USE_CAMERA_STUB),false)
BOARD_CAMERA_LIBRARIES := libcamera
endif

BOARD_ENABLE_GSTREAMER := true
BOARD_USES_ALSA_AUDIO := true
BUILD_WITH_ALSA_UTILS := true
BOARD_ENABLE_FAST_OVERLAY := true

BOARD_HAVE_BLUETOOTH := false
BOARD_HAVE_FM_RADIO := false

BOARD_UMS_LUNFILE := "/sys/devices/platform/usb_mass_storage/lun0/file"
TARGET_USE_CUSTOM_LUN_FILE_PATH := "/sys/devices/platform/usb_mass_storage/lun0/file"

BOARD_LDPI_RECOVERY := true
TARGET_PROVIDES_INIT_TARGET_RC := true
TARGET_RECOVERY_INITRC := "device/samsung/xcover/recovery.rc"

BOARD_CUSTOM_RECOVERY_KEYMAPPING := "../../device/samsung/xcover/recovery/recovery_ui.c"
BOARD_CUSTOM_GRAPHICS := "../../../device/samsung/xcover/recovery/graphics.c"

BOARD_EGL_CFG := "device/samsung/xcover/egl.cfg"

# kernel
BOARD_NAND_PAGE_SIZE := 4096
BOARD_NAND_SPARE_SIZE := 128
BOARD_KERNEL_CMDLINE := "rdinit=/busybox/rdinit androidboot.console=ttyS0 console=ttyS0,115200 mem=240M@0x80000000 comm_v75 uart_dma vmalloc=0x18000000 reserve_pmem=0x2800000"
BOARD_KERNEL_BASE := 0x80000000
BOARD_KERNEL_PAGESIZE := 4096
TARGET_PREBUILT_KERNEL := "device/samsung/xcover/kernel"
# TARGET_KERNEL_CONFIG := alkon_03_defconfig


# partitions
TARGET_USERIMAGES_USE_EXT4 := false
BOARD_RECOVERY_HANDLES_MOUNT := true	

BOARD_BML_BOOT := "/dev/block/bml11"
BOARD_BOOT_DEVICE := "/dev/block/bml11"

BOARD_BML_RECOVERY := "/dev/block/bml15"
BOARD_RECOVERY_DEVICE := /dev/block/bml15

BOARD_DATA_DEVICE := "/dev/block/stl17"
BOARD_DATA_FILESYSTEM := auto
BOARD_DATA_FILESYSTEM_OPTIONS := "llw,check=no,nosuid,nodev"

#BOARD_HAS_DATADATA := true
#BOARD_DATADATA_DEVICE := "/dev/block/stl17"
#BOARD_DATADATA_FILESYSTEM := auto
#BOARD_DATADATA_FILESYSTEM_OPTIONS := "llw,check=no,nosuid,nodev"

BOARD_SYSTEM_DEVICE := "/dev/block/stl16"
BOARD_SYSTEM_FILESYSTEM := auto
BOARD_SYSTEM_FILESYSTEM_OPTIONS := "llw,check=no"

BOARD_CACHE_DEVICE := "/dev/block/stl20"
BOARD_CACHE_FILESYSTEM := auto
BOARD_CACHE_FILESYSTEM_OPTIONS := "llw,check=no,nosuid,nodev"

BOARD_SDEXT_DEVICE := "/dev/block/mmcblk0p2"

BOARD_FLASH_BLOCK_SIZE := 131072

BOARD_HAS_NO_MISC_PARTITION := true
BOARD_HAS_DOWNLOAD_MODE := true
BOARD_HAS_NO_SELECT_BUTTON := true

# BOARD_USES_BML_OVER_MTD := false

# ICS FLAGS for compatibility
#BOARD_USE_LEGACY_TOUCHSCREEN := true
#TARGET_USES_OLD_LIBSENSORS_HAL := true
#TARGET_SENSORS_NO_OPEN_CHECK := true
#BOARD_PROVIDES_LIBRIL := true

