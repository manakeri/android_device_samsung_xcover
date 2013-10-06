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
etc/audio
etc/firmware
etc/firmware/mrvl
etc/labtool
etc/permissions
etc/ppp
etc/wifi
lib
lib/egl
lib/hw
lib/gst
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

#bin/rild
#bin/mediaserver
#bin/surfaceflinger
#bin/pppd
#lib/llbril.so
#lib/llbpixelflinger.so
#lib/llbsurfaceflinger.so
#lib/llbaudioflinger.so
#lib/libcamera_client.so
#lib/libcamera.so
#lib/libcameraservice.so
#lib/libseccameraadaptor.so
#lib/libseccamera.so
#etc/permissions/android.hardware.camera.autofocus.xml
#etc/permissions/android.hardware.camera.flash-autofocus.xml
#etc/permissions/android.hardware.location.gps.xml
#etc/permissions/android.hardware.telephony.gsm.xml
#etc/permissions/android.hardware.touchscreen.multitouch.xml
#etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml
#etc/permissions/android.hardware.sensor.proximity.xml
#etc/permissions/android.hardware.wifi.xml
#etc/permissions/com.marvell.android.mtil_library.xml
#etc/permissions/com.samsung.device.xml
#etc/permissions/com.samsung.android.sec_platform_library.xml
#etc/permissions/com.sec.android.keystring.xml
#etc/permissions/com.sec.android.riloemhook.xml
#etc/permissions/seccamera.xml
#etc/permissions/sec_feature.xml
#lib/hw/gralloc.default.so
#lib/libsoundpool.so

FILES="
etc/vold.fstab
etc/FileMonitorService.conf
etc/gps.conf
etc/gst.conf
etc/dbus.conf
etc/sirfgps.conf
etc/mrvl.cfg
etc/wifidirect_defaults.conf
etc/audio/LVVEFS_Rx_Configuration.txt
etc/audio/Rx_ControlParams_EARPIECE_WIDEBAND.txt
etc/audio/Rx_ControlParams_WIRED_HEADPHONE_WIDEBAND.txt
etc/audio/Tx_ControlParams_EARPIECE_WIDEBAND.txt
etc/audio/Tx_ControlParams_WIRED_HEADPHONE_WIDEBAND.txt
etc/audio/LVVEFS_Tx_Configuration.txt
etc/audio/Rx_ControlParams_SPEAKER_WIDEBAND.txt
etc/audio/Rx_ControlParams_WIRED_HEADSET_WIDEBAND.txt
etc/audio/Tx_ControlParams_SPEAKER_WIDEBAND.txt
etc/audio/Tx_ControlParams_WIRED_HEADSET_WIDEBAND.txt
etc/labtool/FEM_RFMD_5365.ini
etc/labtool/SetUp.ini
etc/wifi/wpa_supplicant.conf
etc/firmware/mrvl/WlanCalData_ext.conf
etc/firmware/mrvl/w8787_wlan_SDIO_bt_SDIO.bin
etc/firmware/mrvl/sd8787_uapsta.bin

bin/bmmtest
bin/charging_mode
bin/cryptsetup
bin/dumpmesg
bin/FileMonitorService
bin/gst-inspect-0.10
bin/gst-launch-0.10
bin/gst-run-0.10
bin/gst-typefind-0.10
bin/gst-xmlinspect-0.10
bin/llnker
bin/MarvellWirelessDaemon
bin/mcfg
bin/memsicd
bin/mfgloader
bin/mpdc
bin/mpdc_d
bin/mpdc_svr
bin/mrvlhcitool
bin/mwu
bin/mwu_cli
bin/netd
bin/npsmobex
bin/omx_tests
bin/passcodemanager
bin/playlpm
bin/pxidle
bin/pxksymaddr
bin/qrngd
bin/qrngtest
bin/rfkill
bin/samsung_debug
bin/test_client
bin/uap.conf
bin/wlandutservice
bin/wpa_supplicant

lib/libpxcmext.so
lib/libpxcmprofiler.so
lib/libpxcssprofiler.so
lib/libpxhsprofiler.so
lib/libpxsdk.so
lib/libpxserial.so
lib/libpxutils.so

lib/lib_R2VS_ARM_GA_Library_for_EUROPA.so

lib/libMrvlOmxConfig.so
lib/libMrvlOmx.so
lib/libMrvlOmxWrapper.so
lib/libomx_mrvl_sharedlibrary.so
lib/libomx_sharedlibrary.so
lib/libMarvellWireless.so
lib/libaudiopolicy.so
lib/libaudiopath.so
lib/libaudio.so
lib/libagps_MTIL.so
lib/libasound.so
lib/libasound_module_ctl_phone.so
lib/libasound_module_pcm_amr.so
lib/libasound_module_pcm_phone.so
lib/libasound_module_pcm_wb_phone.so
lib/libCaddoARMLNXExt.so
lib/libCaddoARMLNX.so
lib/libsoundalive.so

lib/lib_gsd4t.so
lib/lib_gsd4t_factory.so
lib/libmrvldut.so

lib/libpmemhelper.so
lib/libphycontmem.so
lib/libbmm.so
lib/libGAL.so
lib/libgcu.so

lib/libmarvell-ril.so
lib/libsecutil.so
lib/libriloemhook.so
lib/libsvcmd.so

lib/libsamsungAcousticeq.so
lib/lib_Samsung_Acoustic_Module_Llite.so
lib/lib_Samsung_Resampler.so
lib/libsamsungSoundbooster.so
lib/lib_Samsung_Sound_Booster.so

lib/egl/libEGL_MRVL.so
lib/egl/libGLESv1_CM_MRVL.so
lib/egl/libGLESv2_MRVL.so

lib/hw/sensors.default.so
lib/hw/gps.default.so
lib/hw/lights.default.so

lib/libaccelcal.so
lib/libaccsensorcal.so
lib/libaccsensorcaltest.so
lib/libacc.so
lib/libarccamera.so
lib/libbtstereo_server.so
lib/libbtstereo.so
lib/libCaMotion.so
lib/libcaps.so
lib/libdhwr.so
lib/libdprw.so
lib/libdrm2_jni.so
lib/libEEHJni.so
lib/libFileMonitorService.so
lib/libflac.so
lib/libgio-2.0.so
lib/libglib-2.0.so
lib/libgmodule-2.0.so
lib/libgobject-2.0.so
lib/libgstapp-0.10.so
lib/libgstaudio-0.10.so
lib/libgstbase-0.10.so
lib/libgstcontroller-0.10.so
lib/libgstdataprotocol-0.10.so
lib/libgstinterfaces-0.10.so
lib/libgstmetadataretriever.so
lib/libgstnet-0.10.so
lib/libgstnetbuffer-0.10.so
lib/libgstpbutils-0.10.so
lib/libgst_player.so
lib/libgstreamer-0.10.so
lib/libgstriff-0.10.so
lib/libgstrtp-0.10.so
lib/libgstrtsp-0.10.so
lib/libgstsdp-0.10.so
lib/libgsttag-0.10.so
lib/libgstvideo-0.10.so
lib/libgthread-2.0.so
lib/libiasbtapi.so
lib/libiasbtjni.so
lib/libiconv.so
lib/libicrctrlsvr.so
lib/libkeystring_jni.so
lib/liblvvefs.so
lib/libMappa.so
lib/libmtiltrace.so
lib/libmwu_client.so
lib/liboil.so
lib/libomadrm.so
lib/libomx_aacdec_sharedlibrary.so
lib/libomx_amrdec_sharedlibrary.so
lib/libomx_amrenc_sharedlibrary.so
lib/libomx_avcdec_sharedlibrary.so
lib/libomx_m4vdec_sharedlibrary.so
lib/libomx_mp3dec_sharedlibrary.so
lib/libopencore_author.so
lib/libopencore_common.so
lib/libopencore_downloadreg.so
lib/libopencore_download.so
lib/libopencorehw.so
lib/libopencore_mp4localreg.so
lib/libopencore_mp4local.so
lib/libopencore_net_support.so
lib/libopencore_player.so
lib/libopencore_rtspreg.so
lib/libopencore_rtsp.so
lib/libOrgan.so
lib/libPanoraMax1.so
lib/libreference-cdma-sms.so
lib/libscreencapture.so
lib/libsimal.so
lib/libsisodrm.so
lib/libskiagl.so
lib/libsoup.so
lib/libstagefright_color_conversion.so
lib/libstagefright_hardware_renderer.so
lib/libstagefrighthw.so
lib/libtflua.so
lib/libtfutils.so
lib/libtiffanyjni.so
lib/libttssynthproxy.so
lib/libvoicesearch.so
lib/libwlandut.so
lib/libwmlscriptcore.so

lib/gst/libgstcoreindexers.so
lib/gst/libgstffmpeg.so
lib/gst/libgstgdp.so
lib/gst/libgstrtsp.so
lib/gst/libgstalaw.so
lib/gst/libgstapedemux.so
lib/gst/libgstasfdemux.so
lib/gst/libgstaudioconvert.so
lib/gst/libgstdecodebin.so
lib/gst/libgstflv.so
lib/gst/libgsth264dec.so
lib/gst/libgstippmpeg2dec.so
lib/gst/libgstqtdemux.so
lib/gst/libgstsurfaceflingersink.so
lib/gst/libgstudp.so
lib/gst/libgstvmetadec.so
lib/gst/libgstwmadec.so
lib/gst/libgstapp.so
lib/gst/libgstaudioflingersink.so
lib/gst/libgstautodetect.so
lib/gst/libgstdecodebin2.so
lib/gst/libgstid3demux.so
lib/gst/libgstippamrnbenc.so
lib/gst/libgstmpeg4dec.so
lib/gst/libgstrtpmanager.so
lib/gst/libgstrtp.so
lib/gst/libgstsubparse.so
lib/gst/libgstaacdec.so
lib/gst/libgstalsasink.so
lib/gst/libgstaudiocachesink.so
lib/gst/libgstaudiorate.so
lib/gst/libgstaudiotestsrc.so
lib/gst/libgstautoconvert.so
lib/gst/libgstavi.so
lib/gst/libgstcoreelements.so
lib/gst/libgstflac.so
lib/gst/libgstflumpegdemux.so
lib/gst/libgsth263dec.so
lib/gst/libgstippaacenc.so
lib/gst/libgstippamrwbdec.so
lib/gst/libgstippamrwbenc.so
lib/gst/libgstmatroska.so
lib/gst/libgstmjpegdec.so
lib/gst/libgstmp3dec.so
lib/gst/libgstrtpmux.so
lib/gst/libgstsouphttpsrc.so
lib/gst/libgsttcp.so
lib/gst/libgsttypefindfunctions.so
lib/gst/libgstvideorate.so
lib/gst/libgstvideoscale.so
lib/gst/libgstvideotestsrc.so
lib/gst/libgstvolume.so
lib/gst/libgstwavparse.so
lib/gst/libgstwmvdec.so
lib/gst/libgstaudioresample.so
lib/gst/libgstffmpegcolorspace.so
lib/gst/libgstippamrnbdec.so
lib/gst/libgstmpegaudioparse.so
lib/gst/libgstmulaw.so
lib/gst/libgstplaybin.so

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
lib/llbsurfaceflinger.so
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

cameradata/datapattern_420sp.yuv
cameradata/datapattern_front_420sp.yuv
cameradata/datapattern_420sp_alkon.yuv
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
#	echo $FILE
	if [ "$ZIP" ]; then
		cp system/"$FILE" ../../../vendor/$VENDOR/$DEVICE/proprietary/$FILE
	else
		adb pull system/$FILE ../../../vendor/$VENDOR/$DEVICE/proprietary/$FILE
	fi
	echo "    vendor/samsung/xcover/proprietary/$FILE:system/$FILE \\" >> ../../../vendor/samsung/xcover/xcover-vendor-blobs.mk
done

chmod 0755 ../../../vendor/$VENDOR/$DEVICE/proprietary/bin/*
chmod 0664 ../../../vendor/$VENDOR/$DEVICE/proprietary/lib/*.so
chmod 0664 ../../../vendor/$VENDOR/$DEVICE/proprietary/lib/*/*.so
chmod 0755 ../../../vendor/$VENDOR/$DEVICE/proprietary/marvell/tel/*

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
