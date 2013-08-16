#
# This script should be run early during telephony initialization
# before CP and the NVM server start.
# Compares the version_signature files in NVM and in /marvell/tel/nvm_org
# If mismatch is detected, performs a sequence of actions on the NVM
# contents to ensure the NVM contents will be compatible with SW.
# - NVM type 1 files (RO): copy from /marvell/tel/nvm_org into NVM;
# - NVM type 2 files (RW): delete the file in NVM;
# - NVM type 3 files (all others): retain the NVM files as is.
# Records the upgrade event in NVM file upgrade_record.txt
# Copies the version_signature from /marvell/tel/nvm_org into NVM

nvm_dir=/Linux/Marvell/NVM
nvm_org=/marvell/tel/nvm_org
nvm_ts=version_signature

if [ ! -e $nvm_dir/$nvm_ts ]; then
	echo "Initial state" > $nvm_dir/$nvm_ts
fi
ver_old=`cat $nvm_dir/$nvm_ts`
ver_new=`cat $nvm_org/$nvm_ts`

if [ "$ver_old" != "$ver_new" ]; then
	echo "Upgrade on `date`:" >> $nvm_dir/upgrade_record.txt
	echo "	from:	$ver_old">> $nvm_dir/upgrade_record.txt
	echo "	to:	$ver_new" >> $nvm_dir/upgrade_record.txt
	rm -f $nvm_dir/EEHandlerConfig_Linux.nvm
	rm -f $nvm_dir/AudioTTY.bin
	rm -f $nvm_dir/afcDacTable.nvm
	rm -f $nvm_dir/TTPCom_NRAM2_KNOWN_SIM_DATA.gki
	rm -f $nvm_dir/TTPCom_NRAM2_BA_LIST_FILE.gki
	rm -f $nvm_dir/TTPCom_NRAM2_EQUIV_PLMNS.gki
	rm -f $nvm_dir/TTPCom_NRAM2_MOBILE_EQUIPMENT_DATA.gki
	rm -f $nvm_dir/TTPCom_NRAM2_FDD_LIST_DATA.gki
	cp -f $nvm_org/* $nvm_dir
fi
