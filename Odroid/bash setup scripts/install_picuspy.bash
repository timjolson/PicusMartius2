#!/bin/bash

# PyQt Stuff
sudo apt-get install python3-pyqt5
sudo pip3 install pyqtgraph

# Picuspy
git clone http://github.com/niuroverteam/picuspy
cd picuspy
sudo python3 setup.py develop

