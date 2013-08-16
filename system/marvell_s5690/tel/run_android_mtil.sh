#marvell idiotic script for tel

vrfperm='located=`ls -d $vrfiles`; chmod 770 $located; chown root.system $located'
vrfiles="/mnt/nvm /mnt/nvm/* /marvell/* /marvell/usr/* /marvell/usr/lib/* /marvell/usr/sbin/* /marvell/Linux/* /marvell/Linux/Marvell/* /marvell/etc/* /marvell/tel/* /marvell/tel/nvm_org/*"
eval $vrfperm

vrfperm='located=`ls -d $vrfiles`; chmod 775 $located; chown root.system $located'
vrfiles="/mnt/nvm /mnt/nvm/* /marvell/tel/nvm_org/*"
eval $vrfperm

sysperm_syst='located=`ls -d $dvfiles`; chmod 660 $located; chown system.system $located'
sysperm_root='located=`ls -d $dvfiles`; chmod 660 $located; chown root.root $located'

#########################################################
# MRD partition
# /dev/mtd/mtd1      - SAARB MG2 onenand
# /dev/block/mmcblk0 - SAARB MG1 mmc
# /dev/bml2          - Alkon FSR
dvfiles="/dev/bml2 /dev/mtd/mtd1"
eval $sysperm_root

mknod /dev/bml2 b 137 2
mknod /dev/bml24 b 137 24
mkdir /dev/mtd/
ln -s /dev/bml2 /dev/mtd/mtd1

dvfiles="/dev/pxa_sim /dev/pm860x_hsdetect /dev/rtcmon /dev/rtc* /dev/freezer_device /dev/dvfm"

eval $sysperm_root
# Give access to console for system users
dvfiles="/dev/ttyS0 /dev/console /dev/spa"
eval $sysperm_syst
#########################################################

# Keep suplementary group root for accessing /sys files
mrvlacc="system system keystore radio bluetooth inet net_raw net_admin vpn wifi sdcard_rw net_bt net_bt_admin diag log audio shell root"
#########################################################

## hw.sh - apply debug service first
insmod /marvell/tel/hwmap.ko
mknod /dev/hwmap c 237 0
chmod 660 /dev/hwmap
chown system.system /dev/hwmap

export NVM_ROOT_DIR="/marvell/tel/NVM"
/system/bin/ml_setid $mrvlacc -- busybox mkdir $NVM_ROOT_DIR
/system/bin/ml_setid $mrvlacc -- busybox sh /marvell/tel/nvm_upgrade.sh

# Release COMM from reset
/system/bin/hwacc w 0x40F5001C   1
echo 1 > /sys/power/cp

chmod 766 /marvell/etc/diag_bsp.cfg
/system/bin/ml_setid $mrvlacc -- busybox mkdir /data/etc
if [ ! -e /data/log ]; then
    /system/bin/ml_setid $mrvlacc -- busybox mkdir /data/log
fi

/system/bin/ml_setid $mrvlacc -- busybox sh -c "echo "/data/log" > /mrvlsys/diag_log_path"
/system/bin/ml_setid $mrvlacc -- busybox cp /marvell/etc/asound.conf /data/etc/asound.conf
echo 0 > /proc/sys/kernel/hung_task_timeout_secs
/system/bin/ml_setid $mrvlacc -- busybox sh /marvell/tel/diag_port_conf.sh

insmod /marvell/tel/osadev.ko
insmod /marvell/tel/seh.ko
if [ -e /marvell/tel/qos_mode_on ]; then
insmod /marvell/tel/acipcdev.ko param_qos_mode=1
else
insmod /marvell/tel/acipcdev.ko param_qos_mode=0
fi
insmod /marvell/tel/cci_datastub.ko
sleep 1
insmod /marvell/tel/citty.ko
insmod /marvell/tel/ccinetdev.ko
insmod /marvell/tel/cidatatty.ko

dvfiles="/sys/bus/platform/drivers/88pm860x-charger/88pm860x-charger.1/control
         /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-rtc/rtcOffset
         /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-battery.0/*
         /sys/power/cp
         /sys/devices/virtual/switch/h2w/state 
         /sys/power/android_freezer_disable
         /sys/devices/platform/pxa-u2o/composite 
         /dev/ramfile /dev/mipsram /dev/osadrv /dev/seh /dev/acipc 
         /dev/acipcddrv /dev/ccidatastub /dev/citty*  
         /dev/cctdev* /dev/ccichar /dev/cidatatty* /dev/cctdatadev*"
eval $sysperm_root

insmod /marvell/tel/bt_tty.ko
chmod 760 /dev/btduntty*
chown system.bluetooth /dev/btduntty*

chown root.system /dev/citty0
chown root.system /dev/citty1
chown root.system /sys/class/switch/h2w/state
sleep 1
/system/bin/mtsd --secure &
sleep 1
/system/bin/mtilatcmd -S &
chmod 777 /dev/acipcddrv
busybox ln -s /dev/ttyDIAG0 /dev/ttygserial
busybox ln -s /dev/ttyGS0 /dev/ttymodem
insmod /marvell/tel/ppp.ko
dvfiles="/dev/ttyDIAG* /dev/ttyGS*"
eval $sysperm_root

/system/bin/audioserver -S -D yes &
/system/bin/eeh -s -D yes -M yes &
/system/bin/validationif --secure &

#set shell as non freezable
#echo "sh 1" > /sys/power/freeze_process/frz_process
#echo "serial_client 1" > /sys/power/freeze_process/frz_process

# Uncomment some of the following lines to create logcat files on target (for debug)
#export LOGCAT_DIR=/sdcard/logcat
#export LOGCAT_DIR=/data/log
#mkdir $LOGCAT_DIR
#chmod 777 $LOGCAT_DIR
#ml_setid $mrvlacc -- logcat -v time -b main -f $LOGCAT_DIR/logcat.log -r 1024 -n 5 &
#ml_setid $mrvlacc -- logcat -v time -b radio -f $LOGCAT_DIR/logcat-radio.log -r 1024 -n 5 &
#logcat -v time -b events -f $LOGCAT_DIR/logcat-events.log -r 1024 -n 5 &
#chmod 777 $LOGCAT_DIR/logcat*
#chmod 777 /marvell/tel/NVM/audio_hifi.tlv

# Uncomment one of the following lines to change the USB default configuration after reboot (for debug)
# 1. ADB ACM USB_MASS_STORAGE DIAG (default)
# echo adb,acm,usb_mass_storage,diag > /sys/devices/platform/pxa-u2o/composite
# 2. ACM DIAG
# echo acm,diag > /sys/devices/platform/pxa-u2o/composite
# 3. ADB ACM USB_MASS_STORAGE
# echo adb,acm,usb_mass_storage > /sys/devices/platform/pxa-u2o/composite
# 4. ACM USB_MASS_STORAGE
# echo acm,usb_mass_storage > /sys/devices/platform/pxa-u2o/composite
# 5. USB_MASS_STORAGE
# echo usb_mass_storage > /sys/devices/platform/pxa-u2o/composite
# 6. RNDIS
# echo rndis > /sys/devices/platform/pxa-u2o/composite

# for checking VBAT_MIN SYMP#458827
#echo "-9a" > /proc/driver/88pm860x 
#echo `date` VBAT-MIN in `cat /proc/driver/88pm860x` > /tmp/vbat_min.txt 
#cat /tmp/vbat_min.txt > /dev/kmsg
