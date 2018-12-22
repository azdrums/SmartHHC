#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
#   TODO: Add MacOS deployment
#   sudo chmod +x ./tools/macdeployqt.app/Contents/MacOS/macdeployqt
#   sudo ./tools/macdeployqt.app/Contents/MacOS/macdeployqt src/SmartHHC.app -dmg
#   user=$(id -nu)
#   sudo chown ${user} src/SmartHHC.dmg
#   mv src/SmartHHC.dmg .
    export DEPLOYFILE=SmartHHC.dmg
else
#   linuxdeployqt uses this for naming the file
    export VERSION=$(git rev-parse --short HEAD)
    wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    chmod a+x linuxdeployqt-continuous-x86_64.AppImage
    linuxdeployqt-continuous-x86_64.AppImage SmartHHC/usr/share/applications/*.desktop -appimage -no-plugins -no-strip -no-translations -verbose=2
    export DEPLOYFILE=SmartHHC-$VERSION-x86_64.AppImage
fi
