echo 1 > /sys/power/cp

insmod g_serial.ko

insmod mslbddk.ko
insmod cci_datastub.ko

insmod citty.ko
insmod ccinetdev.ko
insmod cidatatty.ko

# in the modules above, before atcmdsrv and audioserver start
sleep 1

./atcmdsrv -D yes&

