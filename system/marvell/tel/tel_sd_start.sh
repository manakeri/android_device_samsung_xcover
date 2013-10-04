# for checking VBAT_MIN SYMP#458827
echo "-9a" > /proc/driver/88pm860x 
echo `date` VBAT-MIN in `cat /proc/driver/88pm860x` >> /tmp/vbat_min.txt 
cat /tmp/vbat_min.txt > /dev/kmsg 

#exit 0
echo "SD TEL START is running!!"
/marvell/tel/diag_mmi force_mount
export LOGCAT_DIR=/sdcard/logcat
export OLD_LOGCAT_DIR=/sdcard/logcat_old
export LOGCAT_FS_BOOT=/data/log

exit 0

# find new ramdump files and touch to update timestamps
ls -l /sdcard/RAMDUMP*|busybox awk '/20[2-9][0-9]-01-01 00:00/ { print "/sdcard/" $(NF); }' | busybox xargs busybox touch

# if logs from old run is still present, remove it
if [ -e $OLD_LOGCAT_DIR ]; then
	rm -r $OLD_LOGCAT_DIR
fi
# move logs from last reset to old folder
if [ -e $LOGCAT_DIR ]; then
	mv $LOGCAT_DIR $OLD_LOGCAT_DIR	
fi
# create folder for logcat files
mkdir $LOGCAT_DIR

# we expect logcat to be running since it is started in run_android_mtil.sh
# so we need to kill all logcat processes 
killall -9 logcat
exit
# move logcat logs from file system to SD card 
cp -r -f $LOGCAT_FS_BOOT/logcat* $LOGCAT_DIR
rm -f $LOGCAT_FS_BOOT/logcat*
# start logcat in new location (SD card)
logcat -b main -f $LOGCAT_DIR/logcat.log -r 1024 -n 10 -v long &
logcat -b radio -f $LOGCAT_DIR/logcat-radio.log -r 1024 -n 10 -v long &
logcat -b events -f $LOGCAT_DIR/logcat-events.log -r 1024 -n 10 -v long &

