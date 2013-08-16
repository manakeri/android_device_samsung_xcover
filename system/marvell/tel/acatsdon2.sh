#!/system/bin/sh

busybox rm /marvell/etc/diag_bsp.cfg
busybox cp /marvell/etc/diag_bsp_sd2.cfg /marvell/etc/diag_bsp.cfg
sync
reboot
