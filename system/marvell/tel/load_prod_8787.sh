#!/system/xbin/sh

rmmod sd8xxx
rmmod mlan

echo 0 > /sys/class/rfkill/rfkill0/state
echo 1 > /sys/class/rfkill/rfkill0/state

insmod /system/lib/modules/mlan.ko
insmod /system/lib/modules/sd8787.ko

ifconfig uap0 up
ifconfig mlan0 up
