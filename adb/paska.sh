#!/bin/bash -x
sudo adb push out/target/product/xcover/system.img /sdcard/
sudo adb shell dd if=/sdcard/system.img of=/dev/block/stl17

