#!/bin/sh

FAIL_IDLE_CYCLE=99.0
FATAL_IDLE_CYCLE=60.0
FAIL_D2_IDLE_CYCLE=98.2
IDLE_TEST_SLEEP=15
TIMER_MAX_EVENTS=1.5
TIMER_TEST_TIME=10

TMP_DUTYCYCLE=/mrvlsys/tmp_duty_cycle
TMP_CHECK=/mrvlsys/tmp_check
TMP_CHECK_D2=/mrvlsys/tmp_check_d2
TMP_TIMER_STATS=/mrvlsys/tmptimer_stats
TMP_WAKELOCKS=/mrvlsys/tmp_wakelocks
OUTPUT=/mrvlsys/tmp_output
FINAL_REPORT=/mrvlsys/tmp_final_report
PM_LOGGER_PRINT=/mrvlsys/pm_logger_print

START=$(date +%s)

#Start PM logger
mount -t debugfs none /sys/kernel/debug
echo 2 > /sys/kernel/debug/PM/pmLogger
echo 3 > /sys/kernel/debug/PM/pmLogger
echo y > /sys/module/printk/parameters/time

echo "==============================================" >>$FINAL_REPORT
echo "==============================================" >>$OUTPUT
echo "==============================================" >>$PM_LOGGER_PRINT
echo "Start" > $OUTPUT
echo "Start" > $FINAL_REPORT

#Standby the system
echo standby > /sys/power/state

#====================================Idle test
sleep 5
echo 1 > /sys/devices/system/cpu/cpu0/stats
sleep $IDLE_TEST_SLEEP
cat /sys/devices/system/cpu/cpu0/duty_cycle >$TMP_DUTYCYCLE
IDLE=`busybox awk '{i=match($0,"idle:");if(i)s+=substr($0,i+5,5)}
END {print s}' $TMP_DUTYCYCLE`
D2IDLE=`busybox awk '{i=match($0,"D2");if(i){w=match($0,"idle:");
if(w)s=substr($0,w+5,5)}} END {print s}' $TMP_DUTYCYCLE`
echo "$IDLE $FAIL_IDLE_CYCLE $FATAL_IDLE_CYCLE $D2IDLE
$FAIL_D2_IDLE_CYCLE" >$TMP_CHECK
IDLE_RESULT=`busybox awk '{if($1<$3) {print "FATAL"} else {if($1<$2)
 {print "failed"} else {print "passed"}}}' $TMP_CHECK`
D2_RESULT=`busybox awk '{if($4<$5){print "failed on d2"} else
{print "passed"}}' $TMP_CHECK`

echo "Total idle time 		:$IDLE%" >>$FINAL_REPORT
echo "Idle test is    		:$IDLE_RESULT" >>$FINAL_REPORT
echo "Total D2 idle time 		:$D2IDLE%" >>$FINAL_REPORT
echo "D2 test is      		:$D2_RESULT" >>$FINAL_REPORT
cat $TMP_DUTYCYCLE >>$OUTPUT

#====================================Suspend test
cat /proc/wakelocks >$TMP_WAKELOCKS
SUSPEND_RESULT=`busybox awk 'END {if(match($0, "deleted_wake_locks"))
 {print "passed"} else {print "failed"}}' $TMP_WAKELOCKS`
if busybox [ $SUSPEND_RESULT = 'failed' ]; then
	cat $TMP_WAKELOCKS >>$OUTPUT
	echo "Suspend test $SUSPEND_RESULT" >>$FINAL_REPORT
else
	echo "Suspend test $SUSPEND_RESULT" >>$FINAL_REPORT
fi

#====================================Timer test
sleep 5
echo 1 > /proc/timer_stats
sleep $TIMER_TEST_TIME
echo 0 > /proc/timer_stats
cat /proc/timer_stats >$TMP_TIMER_STATS
TIMER_RESULT=`busybox awk -v maxevt=$TIMER_MAX_EVENTS 'END
{if(match($0, "total events,")){if ($4 < maxevt) {print "passed"}
else {print "failed"} }}' $TMP_TIMER_STATS`
echo "Timer test $TIMER_RESULT" >>$FINAL_REPORT

#Stop PM logger
echo 4 > /sys/kernel/debug/PM/pmLogger
dmesg -c > /dev/null
echo 1 > /sys/kernel/debug/PM/pmLogger
dmesg -c >>$PM_LOGGER_PRINT


#Resume the system
echo on > /sys/power/state
#====================================Summary
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Test took $DIFF seconds" >>$FINAL_REPORT
echo "Done" >>$FINAL_REPORT
echo "==============================================" >>$PM_LOGGER_PRINT
echo "==============================================" >>$FINAL_REPORT
echo "==============================================" >>$OUTPUT
rm  $TMP_DUTYCYCLE
rm  $TMP_CHECK
rm  $TMP_WAKELOCKS
rm  $TMP_TIMER_STATS
cat $PM_LOGGER_PRINT
cat $OUTPUT
cat $FINAL_REPORT
#end
