#!/system/bin/sh

ifconfig wlan0 down
hciconfig hci0 down
rmmod bt8xxx
rmmod sd8xxx
rmmod mlan

