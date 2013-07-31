echo 1 > /sys/power/cp
sleep 5

insmod /lib/modules/g_serial.ko
insmod seh.ko
insmod mslbddk.ko
insmod cci_datastub.ko
insmod ccinetdev.ko
insmod cidatatty.ko
insmod citty.ko

./atcmdsrv -D yes&
./audioserver &
./eeh &
