#
# Allow debug info collection under some flag
#
if [ -e /data/log/dump_production_flag ]; then
    if [ "$1" != "I_respawned" ]; then
        # respawn myself with output redirection
        cat /proc/kmsg > /data/log/dump_production_log &
        #echo "==== respawn /marvell/tel/run_android_bsp_only.sh ====" > /dev/ttyS0
        cd /marvell/tel
        ./busybox sh ./run_android_bsp_only.sh I_respawned $* 1>/dev/kmsg 2>&1
        exit $?
    fi
    # removing "I_respawned" parameter
    shift
fi

# Run the upgrade check script to detect and fix NVM contents produced by another version
./busybox sh /marvell/tel/nvm_upgrade.sh

echo 1 > /sys/power/cp

echo "Marvell" > "/sys/power/wake_lock"
export USB_ETHERNET_FILE="/marvell/tel/run_ether_usb.sh"
export USB_ANDROID_FILE="/marvell/tel/run_android_usb.sh"
export NVM_ROOT_DIR="/marvell/tel/NVM"

#if [ ! -e $NVM_ROOT_DIR ]; then
    mkdir $NVM_ROOT_DIR
#fi
mount -t vfat /dev/block/mmcblk1p1  /sdcard
# try /dev/block/mmcblk0 if mmcblk0p1 fails
mount -t vfat /dev/block/mmcblk0  /sdcard

/data/bin/busybox cp /data/bin/busybox /marvell/tel .
chmod 777 busybox
busybox cp /marvell/etc/asound.conf /etc/asound.conf
busybox cp /marvell/etc/asound.conf /data/etc/asound.conf
/marvell/tel/diag_mmi force_mount

echo 0 > /proc/sys/kernel/hung_task_timeout_secs
busybox sh rtc_offset.sh
busybox sh diag_port_conf.sh
insmod osadev.ko
insmod seh.ko
#busybox insmod mslbddk.ko use_dma_tx=0,1,1,1,1,0,0 use_dma_rx=0,1,1,1,1,0,0
insmod acipcdev.ko
insmod cci_datastub.ko
sleep 1
insmod citty.ko
insmod ccinetdev.ko
insmod cidatatty.ko

# Enable GPS support
insmod gps_sirf.ko
chown system.system /dev/gps_sirf
chown system.system /dev/ttyS2

insmod ./hwmap.ko
busybox mknod /dev/hwmap c 237 0


port=`cat /mrvlsys/at_port.txt`
if [ $port == 'UART' ]; then
    ./mtsd &
    sleep 1
#Set AT and Diag over UART
    ./mtilatcmd  -u 1&
    ./mtilatcmd &
fi

if [ -f $USB_ETHERNET_FILE ]; then
    busybox sh $USB_ETHERNET_FILE
    #The following 2 lines are here only on develpoment stage. they should be removed at last
    echo busybox ash > /marvell/tel/rem.sh
    busybox telnetd -l sh /marvell/tel/rem.sh
else
if [ -f $USB_ANDROID_FILE ]; then
    setprop ctl.stop adbd
    busybox sh $USB_ANDROID_FILE
    adbd &
else

if [ $port == 'UART' ]; then
    echo UART mode
else
    busybox mknod /dev/ttygserial c 127 0
    busybox ln -s /dev/ttymodem0 /dev/ttymodem
    insmod gs_modem.ko
    insmod g_serial.ko
    ./mtsd &
    sleep 1
    ./mtilatcmd -m /dev/ttymodem&
    ./mtilatcmd &
fi
fi
fi
#The Diag driver works with /mnt/mmc
#Therefore it is linked to /mnt/SD1
#ln -s /mnt/SD1 /mnt/mmc

# Set error log location to /data volume, default is NVM
# Should be before mtsd starts as diag should be able to override the setting
if [ ! -e /data/log ]; then mkdir /data/log; fi # do not use mkdir -p as it fails due to ro fs (busybox mkdir -p is ok)
echo "/data/log" > /mrvlsys/diag_log_path

# delay 1 second to get udevd notified about the devices creation
# in the modules above, before atcmdsrv and audioserver start
chmod 666 /dev/seh

./audioserver -D yes&
echo "0" > /mrvlsys/eeh_cp_sr_force_no
./eeh -D yes -M yes&
#chen: added validation if to prodoction mode to allow small file write interface
./validationif &

# init WTPSP command
insmod /marvell/tel/wtpsp.ko
/busybox/bin/mknod /dev/wtm c 150 0

#The below commands will force product point 1
echo unset d0cs > /sys/power/deepidle
echo unset d1 > /sys/power/deepidle
echo unset d2 > /sys/power/deepidle
echo unset cg > /sys/power/deepidle

busybox sh "/marvell/tel/load_mfg_8787.sh"

sleep 5
echo 1 > /sys/class/graphics/fb0/blank

# Uncomment some of the following lines to create logcat files on target (for debug)
#export LOGCAT_DIR=/sdcard/logcat
#export LOGCAT_DIR=/data/log
#mkdir $LOGCAT_DIR
#chmod 777 $LOGCAT_DIR
#logcat -b main -f $LOGCAT_DIR/logcat.log -r 1024 -n 5 &
#logcat -b radio -f $LOGCAT_DIR/logcat-radio.log -r 1024 -n 5 &
#logcat -b events -f $LOGCAT_DIR/logcat-events.log -r 1024 -n 5 &
#chmod 777 $LOGCAT_DIR/logcat*
