#######################Added by Jim###########################

SDCARD_DEBUG_FLAG="/marvell/tel/NVM/yuhuatel/sdcard_debug"
NOCOMM_FLAG="/marvell/tel/NVM/yuhuatel/NO_COMM_ACAT"

## Jim: check whether debug flag is marked
if [ -e $SDCARD_DEBUG_FLAG ]; then
    busybox rm /mrvlsys/diag_bsp.cfg
    busybox cp /marvell/etc/diag_bsp_sd.cfg /mrvlsys/diag_bsp.cfg
    chmod 0666 /dev/log/main
    chmod 0666 /dev/log/radio
    chmod 0666 /dev/log/events
    chmod 0666 /dev/log/system    
else
    busybox rm /mrvlsys/diag_bsp.cfg
    busybox cp /marvell/etc/diag_bsp_usb.cfg /mrvlsys/diag_bsp.cfg
fi    

#######################Added by Jim End###########################

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
insmod ./hwmap.ko
mknod /dev/hwmap c 237 0
chmod 660 /dev/hwmap
chown system.system /dev/hwmap

## Added by YH
chown system.system /dev/gpo

export NVM_ROOT_DIR="/marvell/tel/NVM"
ml_setid $mrvlacc -- mkdir $NVM_ROOT_DIR
# Run the upgrade check script to detect and fix NVM contents produced by another version
ml_setid $mrvlacc -- ./busybox sh /marvell/tel/nvm_upgrade.sh

# Release COMM from reset
if [ ! -e $NOCOMM_FLAG ]; then
./hwacc w 0x40F5001C   1
echo 1 > /sys/power/cp
fi

# Uncomment the next line in order to disable Suspend and Freezer
#echo "Marvell" > "/sys/power/wake_lock"
# Uncomment the next line in order to enable Suspend and disable Freezer
#echo 1 > /sys/power/android_freezer_disable


chmod 766 /marvell/etc/diag_bsp.cfg
ml_setid $mrvlacc -- mkdir /data/etc

# Set error log location to /data volume, default is NVM
# Should be before mtsd starts as diag should be able to override the setting
# do not use mkdir -p as it fails due to ro fs (busybox mkdir -p is ok)
if [ ! -e /data/log ]; then 
    ml_setid $mrvlacc -- mkdir /data/log
fi 

ml_setid $mrvlacc -- sh -c "echo "/data/log" > /mrvlsys/diag_log_path"

ml_setid $mrvlacc -- busybox cp /marvell/etc/asound.conf /data/etc/asound.conf
echo 0 > /proc/sys/kernel/hung_task_timeout_secs
ml_setid $mrvlacc -- busybox sh diag_port_conf.sh

insmod osadev.ko
insmod seh.ko

## control qos mode - default off 
## create /marvell/tel/qos_mode_on to enable on boot
if [ -e /marvell/tel/qos_mode_on ]; then
    insmod acipcdev.ko param_qos_mode=1
else
    insmod acipcdev.ko param_qos_mode=0
fi

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

eval $sysperm_root_roth

insmod bt_tty.ko
chmod 760 /dev/btduntty*
chown system.bluetooth /dev/btduntty*

# Enable GPS support
insmod gps_sirf.ko
chown system.system /dev/gps_sirf
chown system.system /dev/ttyS2

# delay 1 second to get udevd notified about the devices creation
# in the modules above, before atcmdsrv and audioserver start
sleep 1
./mtsd --secure &
sleep 1
./mtilatcmd -S &


## YH debug.
export USB_ETHERNET_FILE="/marvell/tel/run_ether_usb.sh"
export USB_ANDROID_FILE="/marvell/tel/run_android_usb.sh"
export NVM_ROOT_DIR="/marvell/tel/NVM"

## YH gadget method
if [ -f $USB_ETHERNET_FILE ]; then
     busybox sh $USB_ETHERNET_FILE
else
if [ -f $USB_ANDROID_FILE ]; then
     setprop ctl.stop adbd
     busybox sh $USB_ANDROID_FILE
     adbd &
else
port=`cat /mrvlsys/at_port.txt`
if [ $port == 'UART' ]; then
        #Set AT and Diag over UART

    ./mtilatcmd -S -u 1&
else
    busybox ln -s /dev/ttyDIAG0 /dev/ttygserial
    busybox ln -s /dev/ttyGS0 /dev/ttymodem
    insmod ppp.ko
    dvfiles="/dev/ttyDIAG* /dev/ttyGS*"
    eval $sysperm_root
    # set default usb composite configuration, triggers usb enumeration
    #./mtilatcmd -S -m /dev/ttymodem &
    #./mtilatcmd -S -m /dev/btduntty1 &
fi
fi
fi

if [ -e $NOCOMM_FLAG ]; then
echo "philipssetup started"
 /system/bin/philipssetup 
echo "philipssetup End"
fi

./audioserver -S -D yes &

## Jim: check whether debug flag is marked, if debug mode, don't use silent reset.
if [ -e $SDCARD_DEBUG_FLAG ]; then
./eeh -s -D yes -M yes &
else
./eeh -s -D yes -M yes -a 5&
fi

./validationif --secure &

#set shell as non freezable
echo "sh 1" > /sys/power/freeze_process/frz_process

#disable D0CS set user constrain on OP 0
#echo  0, 0  > /sys/devices/system/cpu/cpu0/enable_op
#echo 0, 0  > /sys/devices/system/cpu/cpu0/control


 
## YH gadget.update by zenith
setprop sys.acat.mode yes
setprop sys.usb.config acm,diag

## attention. replace following to bootcompleted.sh
## if [ -e  /data/data/com.android.contacts/serial_debug ]; then
##     busybox dmesg -n 7
## else
##     busybox dmesg -n 1
## fi


## YH do it. 
## Uncomment some of the following lines to create logcat files on target (for debug)
LOGCAT_DIR="/data/log/terminal_log"
RAMDUMP_FILE="/sdcard/RAMDUMP0000.gz"
RAMDUMP_TEXT_FILE="/sdcard/RAMDUMP0000.txt"

if [ -e $SDCARD_DEBUG_FLAG ]; then

    while [ `grep -c sdcard /proc/mounts`  == 0 ]
    do
        echo "Wait for SD card mounted"
        sleep 2
    done

    echo "SD Card mounted"
    sleep 2    

    TIME=`date "+%Y-%m-%d-%H-%M-%S"`
    RAMDUMP_SAVED_DIR="RAMDUMP-LOG-$TIME"
    LAST_REBOOT_SAVED_DIR="LAST_REBOOT-LOG-$TIME"
    
    if [ -e $RAMDUMP_FILE ] || [ -e $RAMDUMP_TEXT_FILE ]; then
        busybox mkdir /sdcard/$RAMDUMP_SAVED_DIR
        mv $RAMDUMP_FILE /sdcard/$RAMDUMP_SAVED_DIR
        mv $RAMDUMP_TEXT_FILE /sdcard/$RAMDUMP_SAVED_DIR
        busybox chmod -R 777 /data/log
	busybox chown -R 1000:1000 /data/log
        busybox cp -rf /data/log /sdcard/$RAMDUMP_SAVED_DIR
        echo "Ramdump files saved successfully"
    fi
    
    busybox mkdir /sdcard/$LAST_REBOOT_SAVED_DIR
    busybox cp -rf /data/log /sdcard/$LAST_REBOOT_SAVED_DIR
    busybox cp -f  /data/anr/traces.txt /sdcard/$LAST_REBOOT_SAVED_DIR
    
    busybox rm -rf /data/log
    busybox rm -rf /data/anr/traces.txt
    busybox mkdir -p $LOGCAT_DIR
        
    echo 1 > /sys/class/yh_nodes/misc/cp_reset
    /marvell/tel/diag_mmi start
        
    chmod 777 $LOGCAT_DIR	
    logcat -f $LOGCAT_DIR/logcat.log -r 512 -n 4 -v time &
    logcat -b radio -f $LOGCAT_DIR/logcat-radio.log -r 512 -n 16 -v time &
    logcat -b events -f $LOGCAT_DIR/logcat-events.log -r 512 -n 4 -v time &
    cat /proc/kmsg > $LOGCAT_DIR/kernel.txt &
    chmod 777 $LOGCAT_DIR/*    
	    
    kernel_log_size=0
    const_number=0
	while [ $const_number == 0 ]
     do
	       kernel_log_size=`ls -l /data/log/terminal_log/kernel.txt | awk '{print $4}'`
	       if [ $kernel_log_size -ge 1048576 ]; then
	            killall cat
	            busybox cp -f $LOGCAT_DIR/kernel.txt $LOGCAT_DIR/kernel.txt.1
	            cat /dev/null > $LOGCAT_DIR/kernel.txt
                cat /proc/kmsg > $LOGCAT_DIR/kernel.txt &  
	       else
	           sleep 60
	       fi    
     done
fi
## end of not do it.

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
