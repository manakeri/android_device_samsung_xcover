#!/system/xbin/bash -x

cntt_start=`date "+%s"`
# Comment following 2 lines if need to debug via usb0
# echo "run_android_mtil.sh: Put usb0 down"
# ifconfig usb0 down

#########################################################
vrfperm='located=`ls -d $vrfiles`; chmod 770 $located; chown root.system $located'
vrfiles="/mnt/nvm 
         /mnt/nvm/*  
         /marvell/* 
         /marvell/lib/* 
         /marvell/usr/* 
         /marvell/usr/lib/* 
         /marvell/usr/sbin/* 
         /marvell/Linux/* 
         /marvell/Linux/Marvell/* 
         /marvell/etc/* 
         /marvell/tel/* 
         /marvell/tel/NVM/* 
         /marvell/tel/nvm_org/*"
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
dvfiles="/dev/mtd/mtd1 /dev/block/mmcblk0 /dev/bml2"
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

## hw.sh - apply debug service first
insmod /system/lib/modules/hwmap.ko
mknod /dev/hwmap c 237 0
chmod 660 /dev/hwmap
chown system.system /dev/hwmap

export NVM_ROOT_DIR="/mnt/nvm"
nvm_upgrade.sh

# Release COMM from reset
hwacc w 0x40F5001C   1
echo 1 > /sys/power/cp

# Uncomment the next line in order to disable Suspend and Freezer
#echo "Marvell" > "/sys/power/wake_lock"
# Uncomment the next line in order to enable Suspend and disable Freezer
#echo 1 > /sys/power/android_freezer_disable

chmod 766 /marvell/etc/diag_bsp.cfg
mkdir /data/etc

if [ ! -e /data/log ]; then
    mkdir /data/log
fi
echo "/data/log" > /mrvlsys/diag_log_path
cp /marvell/etc/asound.conf /data/etc/asound.conf
echo 0 > /proc/sys/kernel/hung_task_timeout_secs
ml_setid $mrvlacc -- busybox sh diag_port_conf.sh
insmod /system/lib/modules/osadev.ko
insmod /system/lib/modules/seh.ko
## control qos mode - default off
## create /marvell/tel/qos_mode_on to enable on boot
if [ -e /marvell/tel/qos_mode_on ]; then
insmod /system/lib/modules/acipcdev.ko param_qos_mode=1
else
insmod /system/lib/modules/acipcdev.ko param_qos_mode=0
fi
insmod /system/lib/modules/cci_datastub.ko
sleep 1
insmod /system/lib/modules/citty.ko
insmod /system/lib/modules/ccinetdev.ko
insmod /system/lib/modules/cidatatty.ko

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

# Remove following remark to enable Bluetooth DUN support
insmod /system/lib/modules/bt_tty.ko
chmod 760 /dev/btduntty*
chown system.bluetooth /dev/btduntty*

# SAMSUNG_SSENI : For serial_client
chown root.system /dev/citty0
chown root.system /dev/citty1
chown root.system /sys/class/switch/h2w/state

#The Diag driver works with /mnt/mmc
#Therefore it is linked to /mnt/SD1
#ln -s /mnt/SD1 /mnt/mmc

# delay 1 second to get udevd notified about the devices creation
# in the modules above, before atcmdsrv and audioserver start
sleep 1
mtsd --secure &
sleep 1
mtilatcmd -S &

# add wakelock - start
# note: uncomment next line -  this may be needed if you dont have proper permissions to write into this device from java layer
chmod 777 /dev/acipcddrv
# add wakelock - end

ln -s /dev/ttyDIAG0 /dev/ttygserial
ln -s /dev/ttyGS0 /dev/ttymodem
insmod /system/lib/modules/ppp.ko
dvfiles="/dev/ttyDIAG* /dev/ttyGS*"
eval $sysperm_root
# set default usb composite configuration, triggers usb enumeration
# echo usb_mass_storage,acm,adb,diag > /sys/devices/platform/pxa-u2o/composite

#on property:persist.service.adb.enable=1
#   echo usb_mass_storage,acm,adb,diag > /sys/devices/platform/pxa-u2o/composite
#   start adbd
#on property:persist.service.adb.enable=0
#    echo usb_mass_storage,acm > /sys/devices/platform/pxa-u2o/composite
#    stop adbd
#    /system/bin/mtilatcmd -S -m /dev/ttymodem &
#    /system/bin/mtilatcmd -S -m /dev/btduntty1 &
#fi

audioserver -S -D yes &
eeh -s -D yes -M yes &
validationif --secure &

#set shell as non freezable
echo "sh 1" > /sys/power/freeze_process/frz_process
echo "serial_client 1" > /sys/power/freeze_process/frz_process

#disable D0CS set user constrain on OP 0
#echo  0, 0  > /sys/devices/system/cpu/cpu0/enable_op
#echo 0, 0  > /sys/devices/system/cpu/cpu0/control
###################################################################
# Not required any more for since 8787 A1 fixes the uplink issue
#insmod /system/lib/modules/mlan.ko
#insmod /system/lib/modules/sd8787.ko
#insmod /system/lib/modules/bt8787.ko
#echo 1 > /sys/class/rfkill/rfkill0/state
###################################################################


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
echo "-9a" > /proc/driver/88pm860x 
echo `date` VBAT-MIN in `cat /proc/driver/88pm860x` > /tmp/vbat_min.txt 
cat /tmp/vbat_min.txt > /dev/kmsg
