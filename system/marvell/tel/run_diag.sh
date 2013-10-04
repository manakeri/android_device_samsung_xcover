echo 1 > /sys/power/cp
sleep 5

insmod /lib/modules/g_serial.ko
insmod seh.ko
insmod mslbddk.ko
insmod cci_datastub.ko
insmod ccinetdev.ko
insmod cidatatty.ko
insmod citty.ko

/system/bin/atcmdsrv -D yes&
/system/bin/audioserver &
/system/bin/eeh &
