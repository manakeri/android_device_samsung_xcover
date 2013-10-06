#!/system/xbin/sh

cd /marvell/tel/

cntt_start=`date "+%s"`

#########################################################
vrfperm='located=`ls -d $vrfiles`; chmod 770 $located; chown root.system $located'
vrfiles="/mnt/nvm
         /mnt/nvm/*
         /marvell/*
         /marvell/Linux/*
         /marvell/Linux/Marvell/*
         /marvell/etc/*
         /marvell/tel/*
         /marvell/tel/NVM/*
         /marvell/tel/nvm_org/*"
eval $vrfperm

vrfperm='located=`ls -d $vrfiles`; chmod 775 $located; chown root.system $located'
vrfiles="/mnt/nvm
         /mnt/nvm/*
         /marvell/tel/nvm_org/*"
eval $vrfperm

sysperm_syst='located=`ls -d $dvfiles`; chmod 660 $located; chown system.system $located'
sysperm_root='located=`ls -d $dvfiles`; chmod 660 $located; chown root.root $located'
dvfiles="/dev/bml2"
eval $sysperm_root

dvfiles="/dev/pxa_sim
         /dev/pm860x_hsdetect
         /dev/rtcmon
         /dev/rtc*
         /dev/freezer_device
         /dev/dvfm"
eval $sysperm_root

# Give access to console for system users
dvfiles="/dev/ttyS0 /dev/console /dev/spa"
eval $sysperm_syst
#########################################################

# Keep suplementary group root for accessing /sys files
mrvlacc="system system keystore radio bluetooth inet net_raw net_admin vpn wifi sdcard_rw net_bt net_bt_admin diag log audio shell root"
#########################################################

export NVM_ROOT_DIR="/mnt/nvm"
nvm_upgrade.sh

hwacc w 0x40F5001C 1
echo 1 > /sys/power/cp

chmod 766 /marvell/etc/diag_bsp.cfg
if [ ! -e /data/etc ]; then
    mkdir /data/etc
fi
if [ ! -e /data/log ]; then
    mkdir /data/log
fi

echo "/data/log" > /mrvlsys/diag_log_path

cp /marvell/etc/asound.conf /data/etc/asound.conf

echo 0 > /proc/sys/kernel/hung_task_timeout_secs

ml_setid $mrvlacc -- ./busybox sh diag_port_conf.sh

dvfiles="/sys/bus/platform/drivers/88pm860x-charger/88pm860x-charger.1/control
         /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-rtc/rtcOffset
         /sys/devices/platform/pxa95x-i2c.0/i2c-0/0-0034/88pm860x-battery.0/*
         /sys/power/cp
         /sys/devices/virtual/switch/h2w/state
         /sys/power/android_freezer_disable
         /sys/devices/platform/pxa-u2o/composite
         /dev/ramfile
         /dev/mipsram
         /dev/osadrv
         /dev/seh
         /dev/acipc
         /dev/acipcddrv
         /dev/ccidatastub
         /dev/citty*
         /dev/cctdev*
         /dev/ccichar
         /dev/cidatatty*
         /dev/cctdatadev*"
eval $sysperm_root

dvfiles="/dev/ttyDIAG*
         /dev/ttyGS*"
eval $sysperm_root

mtsd --secure &
#mtilatcmd -S &
mtilatcmd -S -m /dev/ttymodem &
#mtilatcmd -S -m /dev/btduntty1 &
audioserver -S -D yes &
eeh -s -D yes -M yes &
validationif --secure &

# for checking VBAT_MIN SYMP#458827
echo "-9a" > /proc/driver/88pm860x 
echo `date` VBAT-MIN in `cat /proc/driver/88pm860x` > /tmp/vbat_min.txt 
cat /tmp/vbat_min.txt > /dev/kmsg

export LOGCAT_DIR=/data/log
mkdir $LOGCAT_DIR
chmod 777 $LOGCAT_DIR
ml_setid $mrvlacc -- logcat -v time -b main -f $LOGCAT_DIR/logcat.log -r 1024 -n 5 &
ml_setid $mrvlacc -- logcat -v time -b radio -f $LOGCAT_DIR/logcat-radio.log -r 1024 -n 5 &
logcat -v time -b events -f $LOGCAT_DIR/logcat-events.log -r 1024 -n 5 &

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


