# Parse /tmp/tasklist.txt ($1), identify all pids present
# and print certain proc FS files for all the threads of these pids
tlist=$1
outname="$2/ap_thread_info.txt"
delim="------------------------------------------------------------------------------------------"

echo -----------FS info ------------- >> $outname
echo /proc/sys/fs/file-nr >> $outname
cat  /proc/sys/fs/file-nr >> $outname
echo /proc/sys/fs/dentry-state >> $outname
cat  /proc/sys/fs/dentry-state >> $outname
busybox echo ---- mount --------------------- >> $outname
mount >> $outname
busybox echo ---- Disk-Free ----------------- >> $outname
busybox df >> $outname
busybox echo ---- Disk-Usage ---------------- >> $outname
busybox du -s /marvell >> $outname
busybox du -s /data >> $outname
busybox du -s /data/log >> $outname
busybox du -s /sdcard >> $outname
busybox echo $delim >> $outname

busybox cat $tlist > $outname
# get pid of the first telephony application = atcmdsrv
busybox echo $delim >> $outname
busybox ps >> $outname
pidlist=`busybox cut -f 2 -d':' /mrvlsys/tasklist.txt|busybox sort -n -u`
for pid in $pidlist; do
busybox echo $delim >> $outname
busybox cat /proc/$pid/task/*/status >> $outname
busybox echo $delim >> $outname
busybox cat /proc/$pid/task/*/sched >> $outname
busybox echo $delim >> $outname
busybox cat /proc/$pid/maps >> $outname
busybox echo $delim >> $outname
busybox cat /sys/devices/system/cpu/cpu0/trace >> $outname
busybox echo $delim >> $outname
done
