#!/system/bin/sh
# Dump extra CP DDR areas needed for debug (those eeh does not dump by default)
/system/bin/eeh -s 0 300000 $1

