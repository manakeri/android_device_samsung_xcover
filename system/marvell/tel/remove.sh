killall atcmdsrv

sleep 1

rmmod g_serial.ko
echo "rmmod g_serial done"

rmmod cci_datastub.ko
echo "rmmod cci_datastub done"

rmmod citty.ko
echo "rmmod citty done"

rmmod ccinetdev.ko
echo "rmmod ccinetdev done"

rmmod cidatatty.ko
echo "rmmod cidatatty done"

sleep 2

rmmod mslbddk.ko
echo "rmmod mslbddk done"
