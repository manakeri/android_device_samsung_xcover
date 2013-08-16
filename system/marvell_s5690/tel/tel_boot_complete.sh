i=0
while [ $i -lt 120 ]
do
	let "i += 1"
	if [ ! -e /mrvlsys/atcmdsrv_ok ]
	then
		sleep 1
	else
		sleep 20
		echo Booting Completed > /dev/console
		touch /mrvlsys/booting_completed
		exit
	fi
done

echo Booting Completed. But atcmdsrv is not ready... > /dev/console


