
# Comment following 2 lines if need to debug via usb0
echo "run_android_mtil.sh: Put usb0 down"
ifconfig usb0 down

#########################################################
## YH/jim change 770 to 771
vrfperm='located=`ls -d $vrfiles`; chmod 771 $located; chown root.system $located'
vrfiles="/mnt/nvm /mnt/nvm/*  
         /marvell/* 
         /marvell/usr/* /marvell/usr/lib/* /marvell/usr/sbin/* 
         /marvell/Linux/* /marvell/Linux/Marvell/* 
         /marvell/etc/* 
         /marvell/tel/* /marvell/tel/nvm_org/*
	/sys/devices/virtual/switch/h2w/*
	/sys/devices/virtual/switch/h3w/*"
eval $vrfperm

## YH/jim Change 660 to 771,or Battery read will be error in Android
sysperm_syst='located=`ls -d $dvfiles`; chmod 771 $located; chown system.system $located'
sysperm_root='located=`ls -d $dvfiles`; chmod 771 $located; chown root.root $located'
sysperm_root_roth='located=`ls -d $dvfiles`; chmod 771 $located; chown root.root $located'
#########################################################
# MRD partition 
# /dev/mtd/mtd1      - SAARB MG2 onenand
# /dev/block/mmcblk0 - SAARB MG1 mmc
# /dev/bml2          - Alkon FSR
dvfiles="/dev/block/mmcblk0"
eval $sysperm_root

#Added by YH/jim: brwxrwx--x  -> brwxrwx-
chmod 770  /dev/block/mmcblk0

dvfiles="/dev/pxa_sim /dev/pm860x_hsdetect  
         /dev/rtcmon /dev/rtc* /dev/freezer_device /dev/dvfm" 

eval $sysperm_root
# Give access to console for system users
dvfiles="/dev/ttyS0 /dev/console"
eval $sysperm_syst
#########################################################

# Keep suplementary group root for accessing /sys files
mrvlacc="system system keystore radio bluetooth  
         inet net_raw net_admin vpn wifi  
         sdcard_rw net_bt net_bt_admin diag log audio shell root"
#########################################################

## hw.sh - apply debug service first
./busybox sh /marvell/tel/nvm_upgrade.sh

## apply debug service first
insmod ./hwmap.ko
mknod /dev/hwmap c 237 0
chmod 660 /dev/hwmap
chown system.system /dev/hwmap

## Added by YH
chown system.system /dev/gpo
# Release COMM from reset
./hwacc w 0x40F5001C   1
echo 1 > /sys/power/cp

#get wake_lock to prevent suspend state
echo "Marvell" > "/sys/power/wake_lock"

export NVM_ROOT_DIR="/marvell/tel/NVM"

## YH busybox
#if [ ! -e $NVM_ROOT_DIR ]; then
	busybox mkdir -p $NVM_ROOT_DIR
#fi
ln -s /system/busybox busybox
chmod 777 busybox
## do it in init or init_bsp. YH
## busybox cp /marvell/etc/asound.conf /etc/asound.conf
chmod 766 /marvell/etc/diag_bsp.cfg
mkdir /data/etc
## YH added
chown system.system /data/etc
# Set error log location to /data volume, default is NVM
# Should be before mtsd starts as diag should be able to override the setting
if [ ! -e /data/log ]; then 
mkdir /data/log
chown system.system /data/log
fi # do not use mkdir -p as it fails due to ro fs (busybox mkdir -p is ok)
echo "/data/log" > /mrvlsys/diag_log_path
busybox cp -p /marvell/etc/asound.conf /data/etc/asound.conf

echo 0 > /proc/sys/kernel/hung_task_timeout_secs
busybox sh diag_port_conf.sh
insmod osadev.ko
insmod seh.ko
insmod acipcdev.ko
insmod cci_datastub.ko
sleep 1

insmod citty.ko
insmod ccinetdev.ko
insmod cidatatty.ko

## YH/jim:  Remove /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-rtc/rtcOffset, no this node
dvfiles="/sys/bus/platform/drivers/88pm860x-charger/88pm860x-charger.1/control
         /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-battery.0/*
         /sys/power/cp
         /sys/power/android_freezer_disable
         /dev/ramfile /dev/mipsram /dev/osadrv /dev/seh /dev/acipc 
         /dev/acipcddrv /dev/ccidatastub /dev/citty*  
         /dev/cctdev* /dev/ccichar /dev/cidatatty* /dev/cctdatadev*"
eval $sysperm_root

# init WTPSP command
insmod /marvell/tel/wtpsp.ko
## use YH busybox
busybox mknod /dev/wtm c 150 0

chmod 777 /sys/devices/platform/pxa-u2o/composite
busybox ln -s /dev/ttyDIAG0 /dev/ttygserial
busybox ln -s /dev/ttyGS0 /dev/ttymodem
insmod ppp.ko

sleep 1
./mtsd &
#alow mtsd to finish registration
sleep 1

port=`cat /mrvlsys/at_port.txt`
if [ $port == 'UART' ]; then
	#Set AT and Diag over UART
    ./mtilatcmd  -u 1&
else
	./mtilatcmd -m /dev/ttymodem&
fi

./mtilatcmd &

#force usb mode to work with modem and DIAG for production.
setprop sys.acat.mode yes
setprop sys.usb.config acm,diag

#SD card mount - need to update if controller changes
mount -t vfat /dev/block/mmcblk1p1  /sdcard
# try /dev/block/mmcblk0 if mmcblk0p1 fails
mount -t vfat /dev/block/mmcblk0  /sdcard

# Enable GPS support
insmod gps_sirf.ko
chown system.system /dev/gps_sirf
chown system.system /dev/ttyS2

chmod 666 /dev/seh

./audioserver -D yes&
./eeh -D yes -M yes&
#validation interface is needed for prodoction mode to allow small file write interface
./validationif &

#The below commands will force active state on (C0)
echo unset d0cs > /sys/power/deepidle
echo unset d1 > /sys/power/deepidle
echo unset d2 > /sys/power/deepidle
echo unset cg > /sys/power/deepidle

busybox sh "/marvell/tel/load_mfg_8787.sh"

#removed sleep as it is not needed.
#sleep 5

#turn off screen
echo 1 > /sys/class/graphics/fb0/blank

## YH, We do not need this log to /data/
## comment the following lines to prevent logcat files on target (used for debug)
## export LOGCAT_DIR=/data/log
## mkdir $LOGCAT_DIR
## chmod 777 $LOGCAT_DIR
## logcat -b main -f $LOGCAT_DIR/logcat.log -r 1024 -n 5 &
## logcat -b radio -f $LOGCAT_DIR/logcat-radio.log -r 1024 -n 5 &
##logcat -b events -f $LOGCAT_DIR/logcat-events.log -r 1024 -n 5 &
## chmod 777 $LOGCAT_DIR/logcat*
