#!/system/xbin/sh
i=0
while [ $i -lt 10 ]
do
        i=`expr $i + 1`
        if [ ! -e /mrvlsys/atcmdsrv_ok ]
        then
                echo wait for atcmdsrv ready...
                sleep 1
        else
                echo atcmdsrv waiting done!
                exit
        fi
done

if [ ! -e /mrvlsys/atcmdsrv_ok ]
then
        echo atcmdsrv not ready...
fi

