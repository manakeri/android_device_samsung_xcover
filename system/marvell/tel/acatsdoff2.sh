#!/system/bin/sh

#Stop emmc record
/marvell/tel/diag_mmi stop

#Restore to Usb Mode
busybox rm /marvell/etc/diag_bsp.cfg
busybox cp /marvell/etc/diag_bsp_usb.cfg /marvell/etc/diag_bsp.cfg


