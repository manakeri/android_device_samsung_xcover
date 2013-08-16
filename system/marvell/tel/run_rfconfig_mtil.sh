NAME="fn1"

NVM_ROOT_FOLDER="/marvell/tel/NVM"

COMCFG_1="/marvell/tel/NVM/COMCfg_1.csv"
COMCFG_128="/marvell/tel/NVM/COMCfg_128.csv"
COMCFG_125="/marvell/tel/NVM/COMCfg_125.csv"

GSM_CAL_DATA_1="/marvell/tel/NVM/GsmCalData_1.nvm"
WB_CAL_DATA_1="/marvell/tel/NVM/WbCalData_1.nvm"

GSM_CAL_DATA_128="/marvell/tel/NVM/GsmCalData_128.nvm"
WB_CAL_DATA_128="/marvell/tel/NVM/WbCalData_128.nvm"

GSM_CAL_DATA_125="/marvell/tel/NVM/GsmCalData_125.nvm"
WB_CAL_DATA_125="/marvell/tel/NVM/WbCalData_125.nvm"


Boardversion=0
Boardversion=`cat /sys/devices/virtual/yh_nodes/misc/hardware_version` 

if [ $Boardversion -eq 1 ]; then
	if [ -e $COMCFG_128 ]; then
		if [ ! -e  $NVM_ROOT_FOLDER/COMCfg.csv ]; then
			busybox cp -p    $COMCFG_128    $NVM_ROOT_FOLDER/COMCfg.csv
		fi
		if [ -e $GSM_CAL_DATA_128 ] &&  [ ! -e  $NVM_ROOT_FOLDER/GsmCalData.nvm ]; then
			busybox cp -p    $GSM_CAL_DATA_128    $NVM_ROOT_FOLDER/GsmCalData.nvm
		fi
		if [ -e $WB_CAL_DATA_128 ] &&  [ ! -e  $NVM_ROOT_FOLDER/WbCalData.nvm ]; then
			busybox cp -p    $WB_CAL_DATA_128    $NVM_ROOT_FOLDER/WbCalData.nvm
		fi
	fi
elif [ $Boardversion -eq 2 ]; then
	if [ -e $COMCFG_1 ]; then
		if [ ! -e  $NVM_ROOT_FOLDER/COMCfg.csv ]; then
			busybox cp -p    $COMCFG_1    $NVM_ROOT_FOLDER/COMCfg.csv
		fi
		if [ -e $GSM_CAL_DATA_1 ] &&  [ ! -e  $NVM_ROOT_FOLDER/GsmCalData.nvm ]; then
			busybox cp -p    $GSM_CAL_DATA_1    $NVM_ROOT_FOLDER/GsmCalData.nvm
		fi
		if [ -e $WB_CAL_DATA_1 ] && [ ! -e  $NVM_ROOT_FOLDER/WbCalData.nvm ]; then
			busybox cp -p    $WB_CAL_DATA_1    $NVM_ROOT_FOLDER/WbCalData.nvm
		fi
	fi
elif [ $Boardversion -eq 3 ]; then
	if [ -e $COMCFG_1 ]; then
		if [ ! -e  $NVM_ROOT_FOLDER/COMCfg.csv ]; then
			busybox cp -p    $COMCFG_1    $NVM_ROOT_FOLDER/COMCfg.csv
		fi
		if [ -e $GSM_CAL_DATA_1 ] &&  [ ! -e  $NVM_ROOT_FOLDER/GsmCalData.nvm ]; then
			busybox cp -p    $GSM_CAL_DATA_1    $NVM_ROOT_FOLDER/GsmCalData.nvm
		fi
		if [ -e $WB_CAL_DATA_1 ] && [ ! -e  $NVM_ROOT_FOLDER/WbCalData.nvm ]; then
			busybox cp -p    $WB_CAL_DATA_1    $NVM_ROOT_FOLDER/WbCalData.nvm
		fi
	fi
else	
	echo "Error: No RF COMCfg used"
fi

