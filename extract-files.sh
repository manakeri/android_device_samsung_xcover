#!/bin/sh

# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

VENDOR=samsung
DEVICE=xcover

rm -rf ../../../vendor/$VENDOR/$DEVICE/*
mkdir -p ../../../vendor/$VENDOR/$DEVICE/proprietary

ZIP="true"

#if [ -f "$1" ]; then
#	rm -rf tmp
#	mkdir tmp
#	unzip -q "$1" -d tmp
#	if [ $? != 0 ]; then
#		echo "$1 is not a valid zip file. Bye."
#		exit 1
#	fi
#	ZIP="true"
#	echo "$1 successfully unzip'd. Copying files..."
#else
#	unset ZIP
#fi

DIRS="
bin
cameradata
etc
etc/firmware
etc/firmware/mrvl
lib
lib/egl
lib/hw
marvell
marvell/Linux
marvell/Linux/Marvell
marvell/etc
marvell/tel
marvell/tel/NVM
marvell/tel/nvm_org
marvell/usr
marvell/usr/lib
media
usr
usr/share
usr/share/alsa
usr/share/alsa/pcm
usr/share/alsa/cards
"

for DIR in $DIRS; do
	mkdir -p ../../../vendor/$VENDOR/$DEVICE/proprietary/$DIR
	chmod 0755 ../../../vendor/$VENDOR/$DEVICE/proprietary/$DIR
done

FILES="
etc/vold.fstab
etc/gps.conf
etc/sirfgps.conf
bin/llnker
bin/memsicd
bin/samsung_debug
bin/playlpm
bin/charging_mode
lib/lib_gsd4t.so
lib/libmarvell-ril.so
lib/libsecutil.so
lib/libriloemhook.so
lib/libsvcmd.so
lib/lib_gsd4t_factory.so
lib/libseccameraadaptor.so
lib/libseccamera.so
lib/libbmm.so
lib/libcamera.so
lib/libGAL.so
lib/libgcu.so
lib/egl/libEGL_MRVL.so
lib/egl/libGLESv1_CM_MRVL.so
lib/egl/libGLESv2_MRVL.so
lib/libpmemhelper.so
lib/libphycontmem.so
lib/libquramimagecodec.so
lib/libQmageDecoder.so
lib/hw/sensors.default.so
lib/hw/gps.default.so
lib/hw/lights.default.so
media/battery_charging_5.qmg
media/battery_charging_10.qmg
media/battery_charging_15.qmg
media/battery_charging_20.qmg
media/battery_charging_25.qmg
media/battery_charging_30.qmg
media/battery_charging_35.qmg
media/battery_charging_40.qmg
media/battery_charging_45.qmg
media/battery_charging_50.qmg
media/battery_charging_55.qmg
media/battery_charging_60.qmg
media/battery_charging_65.qmg
media/battery_charging_70.qmg
media/battery_charging_75.qmg
media/battery_charging_80.qmg
media/battery_charging_85.qmg
media/battery_charging_90.qmg
media/battery_charging_95.qmg
media/battery_charging_100.qmg
media/chargingwarning.qmg
media/Disconnected.qmg
media/TemperatureError.qmg
etc/mrvl.cfg
etc/firmware/mrvl/WlanCalData_ext.conf
etc/firmware/mrvl/w8787_wlan_SDIO_bt_SDIO.bin
etc/firmware/mrvl/sd8787_uapsta.bin
etc/wifidirect_defaults.conf
cameradata/datapattern_420sp.yuv
cameradata/datapattern_front_420sp.yuv
cameradata/datapattern_420sp_alkon.yuv
lib/llbutils.so
lib/llbwpa_client.so
lib/llbz.so
lib/llbmedia.so
lib/llbm.so
lib/llbmwu_client.so
lib/llbnativehelper.so
lib/llbnetutils.so
lib/llbnfc_ndef.so
lib/llbpixelflinger.so
lib/llbQmageDecoder.so
lib/llbquramimagecodec.so
lib/llbskiagl.so
lib/llbskia.so
lib/llbsonivox.so
lib/llbsqlite.so
lib/llbssl.so
lib/llbstdc++.so
lib/llbsurfaceflinger_client.so
lib/llbthread_db.so
lib/llbui.so
lib/llb_gsd4t.so
lib/llbgui.so
lib/llbhardware_legacy.so
lib/llbhardware.so
lib/llbiasbtapi.so
lib/llbicui18n.so
lib/llbicuuc.so
lib/llbjpeg.so
lib/llblog.so
lib/llbMappa.so
lib/llbMarvellWireless.so
lib/llbc.so
lib/llbcutils.so
lib/llbdl.so
lib/llbdvm.so
lib/llbEGL.so
lib/llbemoji.so
lib/llbETC1.so
lib/llbexpat.so
lib/llbGLESv1_CM.so
lib/llbGLESv2.so
lib/llb_gsd4t_factory.so
lib/llbasound.so
lib/llbbinder.so
lib/llbbluedroid.so
lib/llbcamera_client.so
lib/llbcrypto.so
lib/llbandroid_runtime.so
lib/llbmtiltrace.so
marvell/etc/.audiorc
marvell/etc/asound.conf
marvell/etc/diag_bsp.cfg
marvell/etc/diag_bsp_sdcard.cfg
marvell/etc/diag_bsp_usb.cfg
marvell/etc/protocols
marvell/tel/NVM/EMPTY
marvell/Linux/Marvell/EMPTY
marvell/tel/rtc_offsetapp
marvell/tel/rdfut
marvell/tel/serial_client
marvell/tel/bttest
marvell/tel/diag_mmi
marvell/tel/i2cacc
marvell/tel/launch
marvell/tel/launch_bsp
marvell/tel/mtil_client_template
marvell/tel/v_test
marvell/tel/mtilatcmd
marvell/tel/ml_setid
marvell/tel/validationif
marvell/tel/mtsd
marvell/tel/audioserver
marvell/tel/eeh
marvell/tel/hwacc
marvell/tel/run_android_mtil.sh
marvell/tel/nvm_upgrade.sh
marvell/tel/restart.sh
marvell/tel/run_diag.sh
marvell/tel/rtc_offset.sh
marvell/tel/atcmdtest.sh
marvell/tel/nvm_upgrade.sh
marvell/tel/imlpinmux.sh
marvell/tel/cpddrdump.sh
marvell/tel/run_android_bsp_only.sh
marvell/tel/tel_sd_start.sh
marvell/tel/wait.sh
marvell/tel/tel_sd_stop.sh
marvell/tel/diag_port_conf.sh
marvell/tel/load_mfg_8787.sh
marvell/tel/pm_sanity_check.sh
marvell/tel/run_android_mtil.sh
marvell/tel/sd_card_inside.sh
marvell/tel/set_apn.sh
marvell/tel/tel_boot_complete.sh
marvell/tel/threadinfo.sh
marvell/tel/load_prod_8787.sh
marvell/tel/remove.sh
marvell/tel/unload_8787.sh
marvell/tel/nvm_org/audio_avc.nvm
marvell/tel/nvm_org/audio_bluetooth.nvm
marvell/tel/nvm_org/audio_ctm.nvm
marvell/tel/nvm_org/audio_device.nvm
marvell/tel/nvm_org/audio_diamond.nvm
marvell/tel/nvm_org/audio_drc_volume.nvm
marvell/tel/nvm_org/audio_ec_fir_taps_sp.nvm
marvell/tel/nvm_org/audio_ec_fir_taps_wb_amr_sp.nvm
marvell/tel/nvm_org/audio_ec.nvm
marvell/tel/nvm_org/audio_eq.nvm
marvell/tel/nvm_org/audio_extAec.nvm
marvell/tel/nvm_org/audio_hifi.tlv
marvell/tel/nvm_org/audio_hpf_lpf.nvm
marvell/tel/nvm_org/audio_levante.nvm
marvell/tel/nvm_org/audio_ns.nvm
marvell/tel/nvm_org/audio_res.nvm
marvell/tel/nvm_org/audio_sidetone.nvm
marvell/tel/nvm_org/COMCfg.csv
marvell/tel/nvm_org/version_signature
usr/share/alsa/alsa.conf
usr/share/alsa/cards/aliases.conf
usr/share/alsa/pcm/center_lfe.conf
usr/share/alsa/pcm/default.conf
usr/share/alsa/pcm/dmix.conf
usr/share/alsa/pcm/dpl.conf
usr/share/alsa/pcm/dsnoop.conf
usr/share/alsa/pcm/front.conf
usr/share/alsa/pcm/iec958.conf
usr/share/alsa/pcm/modem.conf
usr/share/alsa/pcm/rear.conf
usr/share/alsa/pcm/side.conf
usr/share/alsa/pcm/surround40.conf
usr/share/alsa/pcm/surround41.conf
usr/share/alsa/pcm/surround50.conf
usr/share/alsa/pcm/surround51.conf
usr/share/alsa/pcm/surround71.conf
lib/libagps_MTIL.so
lib/libasound_module_ctl_phone.so
lib/libasound_module_pcm_amr.so
lib/libasound_module_pcm_phone.so
lib/libasound_module_pcm_wb_phone.so
lib/libCaddoARMLNXExt.so
lib/libCaddoARMLNX.so
"

(cat << EOF) | sed s/__DEVICE__/$DEVICE/g > ../../../vendor/samsung/$DEVICE/$DEVICE-vendor-blobs.mk
#
# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This file is generated by device/samsung/__DEVICE__/extract-makefiles.sh
#

EOF

echo "PRODUCT_COPY_FILES += \\" >> ../../../vendor/samsung/xcover/xcover-vendor-blobs.mk
for FILE in $FILES; do
	echo $FILE
	if [ "$ZIP" ]; then
		cp system/"$FILE" ../../../vendor/$VENDOR/$DEVICE/proprietary/$FILE
	else
		adb pull system/$FILE ../../../vendor/$VENDOR/$DEVICE/proprietary/$FILE
	fi
	echo "    vendor/samsung/xcover/proprietary/$FILE:system/$FILE \\" >> ../../../vendor/samsung/xcover/xcover-vendor-blobs.mk
done

chmod -v 0755 ../../../vendor/$VENDOR/$DEVICE/proprietary/bin/*
chmod -v 0664 ../../../vendor/$VENDOR/$DEVICE/proprietary/lib/*.so
chmod -v 0664 ../../../vendor/$VENDOR/$DEVICE/proprietary/lib/*/*.so
chmod -v 0755 ../../../vendor/$VENDOR/$DEVICE/proprietary/marvell/tel/*

#if [ "$ZIP" ]; then rm -rf tmp ; fi

(cat << EOF) | sed s/__DEVICE__/$DEVICE/g > ../../../vendor/samsung/$DEVICE/$DEVICE-vendor.mk
#
# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This file is generated by device/samsung/__DEVICE__/extract-makefiles.sh

# Pick up overlay for features that depend on non-open-source files
DEVICE_PACKAGE_OVERLAYS := vendor/samsung/__DEVICE__/overlay

\$(call inherit-product, vendor/samsung/__DEVICE__/__DEVICE__-vendor-blobs.mk)
EOF
