#!/bin/bash -x
#rm ramdisk-recovery.img
out/host/linux-x86/bin/mkbootfs out/target/product/xcover/recovery/root > out/target/product/xcover/ramdisk-recovery.cpio
out/host/linux-x86/bin/minigzip < out/target/product/xcover/ramdisk-recovery.cpio > ramdisk-recovery.img
sudo adb push ramdisk-recovery.img /sdcard/
sudo adb shell su -c dd if=/sdcard/ramdisk-recovery.img of=/dev/block/bml15
sudo adb shell dd if=/sdcard/ramdisk-recovery.img of=/dev/block/bml15
sudo adb shell reboot recovery
sudo adb shell su -c reboot recovery
