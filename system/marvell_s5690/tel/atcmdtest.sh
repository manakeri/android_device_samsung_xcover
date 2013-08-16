ls /mrvlsys/test.txt >/dev/null 2>/dev/null

case $? in
    0)
	sh -i /dev/ttyS0 ;;
    1)
	echo 1 > /mrvlsys/test.txt
	/marvell/tel/serial_client ;;
esac

