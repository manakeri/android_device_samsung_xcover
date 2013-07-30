device/samsung/xcover/releasetools/ota_from_target_files -v -p out/host/linux-x86 -k build/target/product/security/testkey --backup=true --override_device=xcover,GT-S5690 out/target/product/xcover/obj/PACKAGING/target_files_intermediates/cm_xcover-target_files-eng.teemu.zip out/target/product/xcover/cm_xcover-ota-eng.teemu.zip
rm -rf tempp
unzip out/target/product/xcover/cm_xcover-ota-eng.teemu.zip -d tempp
cat tempp/META-INF/com/google/android/updater-script
