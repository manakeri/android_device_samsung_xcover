# this script create diag_bsp.cfg with diag port set according to kernel cmdline # this is temporary solution until a configuration manager will be implemented
i=`grep -c "PROD=1,1" /proc/cmdline`
j=`grep -c "PROD=1,2" /proc/cmdline`
if [ $i == '1' ]; then
#Set for Diag over UART
	cp /marvell/etc/diag_bsp.cfg /mrvlsys
	echo extr=3 >> /mrvlsys/diag_bsp.cfg
else
if [ $j == '1' ];then
#Set AT and Diag over UART
	cp /marvell/etc/diag_bsp.cfg /mrvlsys
	echo extr=3 >> /mrvlsys/diag_bsp.cfg
	echo delayConnectToExtr=1 >> /mrvlsys/diag_bsp.cfg
	echo UART > /mrvlsys/at_port.txt
else
	echo "diag_port_config did not change any files"
fi
fi
