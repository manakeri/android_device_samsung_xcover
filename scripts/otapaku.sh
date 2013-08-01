rm -rf out/target/product/xcover/{root/,recovery/,*.img,*.zip}
rm -rf out/target/product/xcover/obj/PACKAGING/target_files_intermediates/
make otapackage -j2
#rm out/target/product/xcover/recovery/root/sbin/fat.format
rm out/target/product/xcover/recovery/root/sbin/tune2fs
#rm out/target/product/xcover/recovery/root/sbin/parted
rm out/target/product/xcover/recovery/root/sbin/e2fsck
rm out/target/product/xcover/recovery/root/sbin/sdparted

out/host/linux-x86/bin/mkbootfs out/target/product/xcover/recovery/root > out/target/product/xcover/recovery.cpio
out/host/linux-x86/bin/minigzip < out/target/product/xcover/recovery.cpio > out/target/product/xcover/recovery.img

device/samsung/xcover/releasetools/ota_from_target_files -v -p out/host/linux-x86 -k build/target/product/security/testkey --backup=true --override_device=xcover,GT-S5690 out/target/product/xcover/obj/PACKAGING/target_files_intermediates/cm_xcover-target_files-eng.teemu.zip cm_xcover-ota-eng.teemu.zip

rm -rf tempp
unzip out/target/product/xcover/cm_xcover-ota-eng.teemu.zip -d tempp
cat tempp/META-INF/com/google/android/updater-script

sudo adb push cm_xcover-ota-eng.teemu.zip /sdcard/
