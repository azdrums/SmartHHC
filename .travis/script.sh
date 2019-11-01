#!/bin/bash

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  qmake ${PROJECT_NAME}.pro -spec linux-g++ CONFIG+=release PREFIX=/usr
  make -j$(nproc)
  make INSTALL_ROOT=${PROJECT_NAME} install
elif [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
  qmake ${PROJECT_NAME}.pro -spec macx-clang CONFIG+=release
  make -j$(sysctl -n hw.physicalcpu)
fi
