#!/bin/bash

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  source /opt/qt510/bin/qt510-env.sh
  export QT_SELECT=qt5
fi
