#!/system/bin/sh

# Enable rndis
echo 1 > /sys/class/usb_composite/rndis/enable

# Config wifi
echo 0 > /sys/class/rfkill/rfkill0/state

rmmod bt8xxx
rmmod sd8xxx
rmmod mlan

insmod /system/lib/modules/mlan.ko
insmod /system/lib/modules/sd8787.ko "mfg_mode=1 drv_mode=1 fw_name=mrvl/SD8787.bin" 
insmod /system/lib/modules/bt8787.ko

echo 1 > /sys/class/rfkill/rfkill0/state

hciconfig hci0 up
ifconfig usb0 192.168.1.10 up

/system/bin/mfgbridge &

