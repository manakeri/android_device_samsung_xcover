#--------------------------------------------------------------------------
#
# FILE: sd_card_inside.sh
# 
# Checks SD-card presence (not mount presence)
# The SD card is "mmc" device which could have different numbers
# but always goes with the save device "sdhci-pxa.1" in the K35 GingerBread
# The presence is NOT upon GPIO-detect but upon successful "conversation".
#
# Oridinary is used by ErrorHandler (EEH) for RAMDUMP condition
#
# If card is present - exit 0
#
# Do NOT add eny "echo" print here!
#--------------------------------------------------------------------------
#

## Remove commentary if need to make "always as present"
#exit 0

if [ -e /sys/devices/platform/sdhci-pxa.1/mmc_host/mmc0 ]
then
  if [ -e /sys/bus/mmc/drivers/mmcblk/mmc0* ]
  then
    exit 0
  fi
fi

if [ -e /sys/devices/platform/sdhci-pxa.1/mmc_host/mmc1 ]
then
  if [ -e /sys/bus/mmc/drivers/mmcblk/mmc1* ]
  then
    exit 0
  fi
fi

exit 1
