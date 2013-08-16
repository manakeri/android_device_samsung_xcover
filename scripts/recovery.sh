#!/bin/bash -x
rm out/target/product/xcover/recovery/root/res/images/stitch.png
#rm out/target/product/xcover/recovery/root/sbin/fat.format
#rm out/target/product/xcover/recovery/root/sbin/tune2fs
#rm out/target/product/xcover/recovery/root/sbin/parted
rm out/target/product/xcover/recovery/root/sbin/e2fsck
#rm out/target/product/xcover/recovery/root/sbin/sdparted
out/host/linux-x86/bin/mkbootfs out/target/product/xcover/recovery/root > out/target/product/xcover/ramdisk-recovery.cpio
out/host/linux-x86/bin/minigzip < out/target/product/xcover/ramdisk-recovery.cpio > out/target/product/xcover/ramdisk-recovery.img
#sudo adb push out/target/product/xcover/ramdisk-recovery.img /sdcard/
#sudo adb shell dd if=/sdcard/ramdisk-recovery.img of=/dev/block/bml15 && sudo adb reboot recovery
