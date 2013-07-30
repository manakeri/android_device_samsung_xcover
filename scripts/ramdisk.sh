#!/bin/bash -x
#rm ramdisk-recovery.img
out/host/linux-x86/bin/mkbootfs out/target/product/xcover/root > out/target/product/xcover/ramdisk.cpio
out/host/linux-x86/bin/minigzip < out/target/product/xcover/ramdisk.cpio > ramdisk.img
sudo adb push ramdisk.img /sdcard/
sudo adb shell dd if=/sdcard/ramdisk.img of=/dev/block/bml13
sudo adb shell reboot
