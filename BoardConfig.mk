USE_CAMERA_STUB := true

# inherit from the proprietary version
-include vendor/samsung/xcover/BoardConfigVendor.mk

TARGET_NO_BOOTLOADER := true
TARGET_BOARD_PLATFORM := unknown
TARGET_CPU_ABI := armeabi
TARGET_BOOTLOADER_BOARD_NAME := xcover

#BOARD_KERNEL_CMDLINE := no_console_suspend
BOARD_KERNEL_CMDLINE := rdinit=/busybox/rdinit androidboot.console=ttyS0 console=ttyS0,115200 mem=240M@0x80000000 comm_v75 uart_dma vmalloc=0x18000000 reserve_pmem=0x2800000
BOARD_KERNEL_BASE := 0x80000000
BOARD_KERNEL_PAGESIZE := 4096
# BOARD_PAGE_SIZE := 0x00001000

#   1: 0x00000000-0x00040000 0x00040000      1        0
#   2: 0x00040000-0x00080000 0x00040000      1        1
#   3: 0x00080000-0x000c0000 0x00040000      1        2
#   4: 0x000c0000-0x00180000 0x000c0000      3        3
#   5: 0x00180000-0x00240000 0x000c0000      3        4
#   6: 0x00240000-0x00280000 0x00040000      1        5
#   7: 0x00280000-0x00340000 0x000c0000      3        6
#   8: 0x00340000-0x00940000 0x00600000     24        7
#   9: 0x00940000-0x00b00000 0x001c0000      7        8
#  10: 0x00b00000-0x00b40000 0x00040000      1        9
#  11: 0x00b40000-0x00f40000 0x00400000     16       10
#  12: 0x00f40000-0x01340000 0x00400000     16       11
#  13: 0x01340000-0x01380000 0x00040000      1       12
#  14: 0x01380000-0x013c0000 0x00040000      1       13
#  15: 0x013c0000-0x01540000 0x00180000      6       14
#  16: 0x01540000-0x10bc0000 0x0f680000    986       20
#  17: 0x10bc0000-0x1b7c0000 0x0ac00000    688       21
#  18: 0x1b7c0000-0x1c1c0000 0x00a00000     40       22
#  19: 0x1c1c0000-0x1c7c0000 0x00600000     24       23
#  20: 0x1c7c0000-0x1e540000 0x01d80000    118       24
#  21: 0x1e540000-0x1ea40000 0x00500000     20       25
#  22: 0x1ea40000-0x1f440000 0x00a00000     40       26
#  23: 0x1f440000-0x1f480000 0x00040000      1       15
#  24: 0x1f480000-0x1f4c0000 0x00040000      1       16
#  25: 0x1f4c0000-0x1f500000 0x00040000      1       17
#/dev/stl16 /system rfs
#/dev/stl17 /data rfs
#/dev/stl18 /marvell rfs
#/dev/stl19 /mnt/nvm rfs
#/dev/stl20 /cache rfs
#/dev/stl21 /mnt/.lfs j4fs
#/dev/stl22 /efs rfs

BOARD_BOOTIMAGE_PARTITION_SIZE := 0x00400000
BOARD_BML_BOOT := /dev/block/bml11
BOARD_BOOTIMAGE_PARTITION := /dev/block/bml11

BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x00400000
BOARD_BML_RECOVERY := /dev/block/bml12
BOARD_RECOVERY_DEVICE := /dev/block/bml12
BOARD_RECOVERYIMAGE_PARTITION := /dev/block/bml12

BOARD_SYSTEMIMAGE_PARTITION_SIZE := 0x0f680000
BOARD_SYSTEM_DEVICE := /dev/block/stl16

BOARD_USERDATAIMAGE_PARTITION_SIZE := 0x0ac00000
BOARD_DATA_DEVICE := /dev/block/stl17

BOARD_CACHE_DEVICE := /dev/block/stl20

BOARD_FLASH_BLOCK_SIZE := 131072

BOARD_CUSTOM_RECOVERY_KEYMAPPING:= ../../device/samsung/xcover/recovery_ui.c
BOARD_HAS_NO_MISC_PARTITION := true
BOARD_HAS_DOWNLOAD_MODE := true

TARGET_PREBUILT_KERNEL := device/samsung/xcover/kernel

#BOARD_HAS_NO_SELECT_BUTTON := true
# Use this flag if the board has a ext4 partition larger than 2gb
#BOARD_HAS_LARGE_FILESYSTEM := true
