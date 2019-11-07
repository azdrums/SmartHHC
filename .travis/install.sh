#!/bin/bash

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  sudo apt-get -y install libgl1-mesa-dev qt510base qt510serialport
  source /opt/qt*/bin/qt*-env.sh
elif [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
  brew link --force qt5
  export PATH=/usr/local/opt/qt5/bin:$PATH
fi
