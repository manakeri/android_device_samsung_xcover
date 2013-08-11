#!/bin/bash -x

COMPRESS=9
API=10


rm -rf out/
rm -rf build/
mkdir build

java -Xmx512m -jar baksmali.jar -x $1.odex -a $API -d .
java -Xmx512M -jar smali.jar out/ -o build/classes.dex

cd build
7za x -y ../$1.apk >> ../Log-APK.txt
7za a -y -mx9 -tzip Done.apk * >> ../Log-APK.txt
mv -f Done.apk Done.zip
7za d -y Done.zip >> ../Log-APK.txt
mv -f Done.zip Done.apk
mv -f Done.apk ../deodexed/$1.apk

