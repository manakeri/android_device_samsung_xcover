#!/system/xbin/sh
# Dump extra CP DDR areas needed for debug (those eeh does not dump by default)
eeh -s 0 300000 $1

