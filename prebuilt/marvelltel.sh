#!/system/bin/sh

vrfperm='located=`ls -d $vrfiles`; chmod 770 $located; chown root.system $located'
vrfiles="/mnt/nvm /mnt/nvm/* /marvell/* /marvell/usr/* /marvell/usr/lib/* /marvell/usr/sbin/* /marvell/Linux/* /marvell/Linux/Marvell/* /marvell/etc/* /marvell/tel/* /marvell/tel/nvm_org/*"
eval $vrfperm

vrfperm='located=`ls -d $vrfiles`; chmod 775 $located; chown root.system $located'
vrfiles="/mnt/nvm /mnt/nvm/* /marvell/tel/nvm_org/*"
eval $vrfperm

sysperm_syst='located=`ls -d $dvfiles`; chmod 660 $located; chown system.system $located'
sysperm_root='located=`ls -d $dvfiles`; chmod 660 $located; chown root.root $located'

mknod /dev/bml2 b 137 2
mknod /dev/bml24 b 137 24
mkdir /dev/mtd/
ln -s /dev/bml2 /dev/mtd/mtd1

dvfiles="/dev/bml2 /dev/mtd/mtd1"
eval $sysperm_root

dvfiles="/dev/pxa_sim /dev/pm860x_hsdetect /dev/rtcmon /dev/rtc* /dev/freezer_device /dev/dvfm"
eval $sysperm_root

# Give access to console for system users
dvfiles="/dev/ttyS0 /dev/console /dev/spa"
eval $sysperm_syst
#########################################################

# Keep suplementary group root for accessing /sys files
mrvlacc="system system keystore radio bluetooth inet net_raw net_admin vpn wifi sdcard_rw net_bt net_bt_admin diag log audio shell root"
#########################################################

insmod /system/lib/modules/hwmap.ko
mknod /dev/hwmap c 237 0
chmod 660 /dev/hwmap
chown system.system /dev/hwmap

#/system/bin/ml_setid $mrvlacc -- busybox mkdir $NVM_ROOT_DIR
#/system/bin/ml_setid $mrvlacc -- busybox sh /system/lib/modules/nvm_upgrade.sh

echo "-- Release COMM from reset --"
/system/bin/hwacc w 0x40F5001C   1
echo 1 > /sys/power/cp

chmod 766 /etc/diag_bsp.cfg
if [ ! -e /data/etc ]; then
	/system/bin/ml_setid $mrvlacc -- busybox mkdir /data/etc
fi
if [ ! -e /data/log ]; then
	/system/bin/ml_setid $mrvlacc -- busybox mkdir /data/log
fi

echo "/data/log" > /mrvlsys/diag_log_path
cp /etc/asound.conf /data/etc/asound.conf
echo 0 > /proc/sys/kernel/hung_task_timeout_secs
#/system/lib/modules/diag_port_conf.sh

insmod /system/lib/modules/osadev.ko
insmod /system/lib/modules/seh.ko
insmod /system/lib/modules/acipcdev.ko param_qos_mode=0
insmod /system/lib/modules/cci_datastub.ko
sleep 1
insmod /system/lib/modules/citty.ko
insmod /system/lib/modules/ccinetdev.ko
insmod /system/lib/modules/cidatatty.ko

dvfiles="/sys/bus/platform/drivers/88pm860x-charger/88pm860x-charger.1/control 
         /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-rtc/rtcOffset 
         /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-battery.0/* 
         /sys/power/cp /sys/devices/virtual/switch/h2w/state /sys/power/android_freezer_disable
         /sys/devices/platform/pxa-u2o/composite /dev/ramfile /dev/mipsram /dev/osadrv /dev/seh /dev/acipc 
         /dev/acipcddrv /dev/ccidatastub /dev/citty* /dev/cctdev* /dev/ccichar /dev/cidatatty* /dev/cctdatadev*"
eval $sysperm_root

insmod /system/lib/modules/bt_tty.ko
chmod 760 /dev/btduntty*
chown system.bluetooth /dev/btduntty*

chown root.system /dev/citty0
chown root.system /dev/citty1
chown root.system /sys/class/switch/h2w/state

chmod 777 /dev/acipcddrv
busybox ln -s /dev/ttyDIAG0 /dev/ttygserial
busybox ln -s /dev/ttyGS0 /dev/ttymodem
insmod /system/lib/modules/ppp.ko
dvfiles="/dev/ttyDIAG* /dev/ttyGS*"
eval $sysperm_root

cp /etc/diag_bsp.cfg /mrvlsys

mount -o remount,rw /system

export NVM_ROOT_DIR="/mnt/nvm"
/system/bin/mtsd --secure &
sleep 1
/system/bin/mtilatcmd -S &
sleep 1
#/system/bin/audioserver -S -D yes &
/system/bin/eeh -s -D yes -M yes -a 5&
/system/bin/validationif --secure &

echo "sh 1" > /sys/power/freeze_process/frz_process
echo "serial_client 1" > /sys/power/freeze_process/frz_process

export LOGCAT_DIR=/data/log
mkdir $LOGCAT_DIR
chmod 777 $LOGCAT_DIR
logcat -v time -b main -f $LOGCAT_DIR/logcat.log -r 1024 -n 5 &
logcat -v time -b radio -f $LOGCAT_DIR/logcat-radio.log -r 1024 -n 5 &
logcat -v time -b events -f $LOGCAT_DIR/logcat-events.log -r 1024 -n 5 &
chmod 777 $LOGCAT_DIR/logcat*
chmod 777 /mnt/nvm/audio_hifi.tlv

echo "-9a" > /proc/driver/88pm860x 
echo `date` VBAT-MIN in `cat /proc/driver/88pm860x` > /tmp/vbat_min.txt 


