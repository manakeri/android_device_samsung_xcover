#!/bin/sh

ifconfig uap0 down
hciconfig hci0 down
rmmod bt8xxx
rmmod sd8xxx
rmmod mlan

echo 1 > /sys/class/rfkill/rfkill0/state

insmod /system/lib/modules/mlan.ko
insmod /system/lib/modules/sd8787.ko "mfg_mode=1 drv_mode=1 fw_name=mrvl/w8787_mfg_wlan_sdio_bt_sdio_wx.bin"
insmod /system/lib/modules/bt8787.ko
hciconfig hci0 up
