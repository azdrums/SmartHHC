#!/bin/bash

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  sudo add-apt-repository ppa:beineri/opt-qt-5.10.1-trusty -y
  sudo apt-get update -qq
fi
