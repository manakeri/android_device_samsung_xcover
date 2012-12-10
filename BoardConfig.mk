# inherit from the proprietary version
-include vendor/samsung/xcover/BoardConfigVendor.mk

TARGET_BOARD_PLATFORM := marvell
TARGET_BOOTLOADER_BOARD_NAME := pxa968
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
# TARGET_ARCH := arm
# TARGET_ARCH_VARIANT := armv5te

TARGET_NO_KERNEL := false
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# BOARD_USES_GENERIC_AUDIO := true
# BOARD_USES_ALSA_AUDIO := true
# TARGET_PROVIDES_LIBAUDIO := true
# HAVE_HTC_AUDIO_DRIVER := false

# BOARD_HAVE_BLUETOOTH := true
# BOARD_HAVE_FM_RADIO := true
# BOARD_USES_OVERLAY := true
# USE_CAMERA_STUB := false
# BOARD_ENABLE_GSTREAMER := true
# BOARD_ENABLE_FAST_OVERLAY := true

# TARGET_RECOVERY_PIXEL_FORMAT := "BGRA_8888" 
# USE_OPENGL_RENDERER := true
BOARD_LDPI_RECOVERY := true
BOARD_NO_32BPP := true
BOARD_NO_PAGE_FLIPPING := true

# TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"
# BOARD_USES_HGL := true
# BOARD_CAMERA_HAVE_FLASH := true

# TARGET_PROVIDES_INIT := true
TARGET_PROVIDES_INIT_TARGET_RC := true
TARGET_RECOVERY_INITRC := device/samsung/xcover/recovery.rc
BOARD_EGL_CFG := device/samsung/xcover/egl.cfg

BOARD_NAND_PAGE_SIZE := 4096
BOARD_NAND_SPARE_SIZE := 128
# BOARD_KERNEL_CMDLINE := init=/init rdinit=/sbin/recovery androidboot.console=ttyS0 console=ttyS0,115200 mem=240M@0x80000000 comm_v75 uart_dma vmalloc=0x18000000 reserve_pmem=0x2800000
# BOARD_KERNEL_CMDLINE := console=ttyS0,115200 init=/init rdinit=/sbin/recovery no_console_suspend
# BOARD_KERNEL_CMDLINE := console=ttyFIQ0,115200 init=/init mem=240M@0x80000000 comm_v75 uart_dma vmalloc=0x18000000 reserve_pmem=0x2800000 no_console_suspend

BOARD_KERNEL_BASE := 0x80000000
BOARD_KERNEL_PAGESIZE := 4096

# TARGET_KERNEL_CONFIG := alkon_03_defconfig
TARGET_PREBUILT_KERNEL := device/samsung/xcover/kernel

#/dev/stl16 /system rfs
#/dev/stl17 /data rfs
#/dev/stl18 /marvell rfs
#/dev/stl19 /mnt/nvm rfs
#/dev/stl20 /cache rfs
#/dev/stl21 /mnt/.lfs j4fs
#/dev/stl22 /efs rfs

TARGET_USERIMAGES_USE_EXT4 := false

BOARD_RECOVERY_HANDLES_MOUNT := true
BOARD_BML_BOOT := /dev/block/bml11
BOARD_BML_RECOVERY := /dev/block/bml12

# BOARD_BOOTIMAGE_PARTITION_SIZE := 0x00400000
# BOARD_BOOTIMAGE_PARTITION := /dev/block/bml11
# BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x00400000
# BOARD_RECOVERY_DEVICE := /dev/block/bml11
# BOARD_RECOVERY_PARTITION := /dev/block/bml11
# BOARD_RECOVERYIMAGE_PARTITION := /dev/block/bml11

BOARD_SYSTEM_DEVICE := /dev/block/stl16
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 0x0f680000

BOARD_DATA_DEVICE := /dev/block/stl17
BOARD_USERDATAIMAGE_PARTITION_SIZE := 0x0ac00000

BOARD_CACHE_DEVICE := /dev/block/stl20

BOARD_FLASH_BLOCK_SIZE := 131072

BOARD_CUSTOM_RECOVERY_KEYMAPPING:= ../../device/samsung/xcover/recovery_ui.c
BOARD_HAS_NO_MISC_PARTITION := true
BOARD_HAS_DOWNLOAD_MODE := true
BOARD_HAS_NO_SELECT_BUTTON := true

# BOARD_USES_BML_OVER_MTD := true
