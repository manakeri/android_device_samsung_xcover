
#check if user set input parameter correctly
if [ -z "$1" ]; then
	echo "Usage: setAPN.sh [orange|cellcom|att|tmo|ults]"
	exit 1
elif [ "$1" != "orange" ] && [ "$1" != "cellcom" ] && [ "$1" != "att" ] && [ "$1" != "tmo" ] && [ "$1" != "ults" ]; then
		echo "Usage: setAPN.sh [orange|cellcom|att|tmo|ults]"
		echo "user input $1"
		exit 1
fi

echo "update data base with APNs for all operators"
cd /
cd /data/data/com.android.providers.telephony/databases/
if [ ! -f telephony.db ] ; then
	echo "telephony data base does not exists! can't set APN"	
	echo "need to be registered to the NW to set APN"
	exit 0
fi

#Enter only relevant fields in the data base so we will be FW compatible
sqlite3 telephony.db "insert into carriers (name, numeric, mcc, mnc, apn , current) values('Cellcom','42502','425','02','internetg', 1);"
sqlite3 telephony.db "insert into carriers (name, numeric, mcc, mnc, apn , current) values('Orange','42501','425','01','internet', 1);"
sqlite3 telephony.db "insert into carriers (name, numeric, mcc, mnc, apn , current) values('AT&T','310410','310','410','wap.cingular', 1);"
sqlite3 telephony.db "insert into carriers (name, numeric, mcc, mnc, apn , current) values('T-Mobile','310260','310','260','wap.voicestream.com', 1);"
sqlite3 telephony.db "insert into carriers (name, numeric, mcc, mnc, apn , current) values('ULTS','001010','001','010','ults', 1);"

if [ ! -d /data/data/com.android.providers.telephony/shared_prefs/ ] ; then
	echo "create defualt connection mxl file"
	mkdir /data/data/com.android.providers.telephony/shared_prefs
	cd /
	cd /data/data/com.android.providers.telephony/shared_prefs/
	echo "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>" > preferred-apn.xml
	echo '<map>' >> preferred-apn.xml
	echo '<long name="apn_id" value="4" />' >> preferred-apn.xml
	echo '</map>' >> preferred-apn.xml 
fi

# set the defualt APN value to:
# 3 for Cellcom
# 4 for Orange
# 5 for AT&T
# 6 for T-Mobile
# 7 for ULTS
cd /
cd /data/data/com.android.providers.telephony/shared_prefs/

if [ "$1" = "cellcom" ]; then
	echo "Setting Cellcom as default APN ..."
	busybox sed -i -e "s/value=\".*\"/value=\"3\"/" preferred-apn.xml
else
if [ "$1" = "orange" ]; then
	echo "Setting Orange as default APN ..."
	busybox sed -i -e "s/value=\".*\"/value=\"4\"/" preferred-apn.xml
else
if [ "$1" = "att" ]; then
	echo "Setting AT&T as default APN ..."
	busybox sed -i -e "s/value=\".*\"/value=\"5\"/" preferred-apn.xml
else
if [ "$1" = "tmo" ]; then
	echo "Setting T-Mobile as default APN ..."
	busybox sed -i -e "s/value=\".*\"/value=\"6\"/" preferred-apn.xml
else
	echo "Setting ULTS as default APN ..."
	busybox sed -i -e "s/value=\".*\"/value=\"7\"/" preferred-apn.xml
fi
fi
fi
fi

echo "setAPN: Done"
# reboot
echo "you need to reboot the UE for APN to become active !!"
echo "you need to reboot the UE for APN to become active !!"  
echo "you need to reboot the UE for APN to become active !!"
