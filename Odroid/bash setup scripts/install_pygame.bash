#!/bin/bash

# Installing pygame dependencies
sudo apt-get install mercurial libav-tools libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev libsmpeg-dev libsdl1.2-dev libportmidi-dev libswscale-dev libavformat-dev libavcodec-dev libtiff5-dev libx11-6 libx11-dev fluid-soundfont-gm timgm6mb-soundfont xfonts-base xfonts-100dpi xfonts-75dpi xfonts-cyrillic fontconfig fonts-freefont-ttf

# Cloning Pygame
cd Desktop/
hg clone https://bitbucket.org/pygame/pygame

# Installing Pygame
cd pygame
python3 setup.py build
sudo python3 setup.py install
cd ..
