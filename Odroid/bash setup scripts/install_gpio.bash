#!/bin/bash

# Cloning WiringPi2
git clone https://github.com/hardkernel/WiringPi2-Python.git
cd WiringPi2-Python

# Installing WiringPi
git submodule init
git submodule update
swig3.0 -python -threads wiringpi.i
sudo python3 setup.py install
cd ..

# Installing ADCs
git clone https://github.com/adafruit/Adafruit_Python_ADS1x15.git
cd Adafruit_Python_ADS1x15
sudo python3 setup.py install
cd ..
