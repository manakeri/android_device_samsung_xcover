#!/system/bin/sh
# Dump extra CP DDR areas needed for debug (those eeh does not dump by default)
/marvell/tel/eeh -S 0 300000 $1

