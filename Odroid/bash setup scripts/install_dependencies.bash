#!/bin/bash

# Updating
sudo apt-get update
# Upgrading
sudo apt-get upgrade

# Install tools and python dependencies
sudo apt-get install htop i2c-tools swig3.0 git python-setuptools python-dev build-essential python3-setuptools python3-dev build-essential python3-pip python3-numpy python3-opengl python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
sudo pip3 install pyserial
sudo pip3 install --upgrade pip
