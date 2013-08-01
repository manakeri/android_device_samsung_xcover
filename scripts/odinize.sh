#!/bin/bash -x
cp out/target/product/xcover/ramdisk-recovery.img .
tar -H ustar -cf ramdisk-recovery.tar ramdisk-recovery.img
md5sum -t ramdisk-recovery.tar >> ramdisk-recovery.tar
mv ramdisk-recovery.tar ramdisk-recovery.tar.md5
