#!/bin/bash -x
#rm ramdisk-recovery.img
out/host/linux-x86/bin/mkbootfs device/samsung/xcover/ext4_modules > ramdisk-modules.cpio
out/host/linux-x86/bin/minigzip < ramdisk-modules.cpio > ramdisk-modules.img
sudo adb push ramdisk-modules.img /sdcard/
sudo adb shell su -c dd if=/sdcard/ramdisk-modules.img of=/dev/block/bml12
sudo adb shell dd if=/sdcard/ramdisk-modules.img of=/dev/block/bml12
#sudo adb shell reboot recovery

