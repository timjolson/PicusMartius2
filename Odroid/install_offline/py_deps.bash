#! /bin/bash

# Python packages
packages="pygame pyserial numpy pyqt5==5.9 pyqtgraph matplotlib virtualenv configparser"

# Default download path
DOWN_PATH=./py_packages
DOWN_FLAG=0
INSTALL_FLAG=0

# Structure inspired by http://linuxcommand.org/lc3_wss0120.php
HELP()
{
    echo "usage: py_deps [[-d | --download] [-i | --install] [-p | --path download_path] [-h | --help]]"
}

while [ "$1" != "" ]; do
    case $1 in
        -d | --download )       DOWN_FLAG=1
                                ;;
        -i | --install )        INSTALL_FLAG=1
                                ;;
        -h | --help )           HELP
                                exit
                                ;;
        -p | --path )           shift
                                DOWN_PATH=$1
                                ;;
        * )                     HELP
                                exit 1
    esac
    shift
done

if [ "$DOWN_FLAG" = "1" ]; then
    printf $"*****\nDownloading Packages to [$DOWN_PATH]\n*****\n"
    pip3 download -d "$DOWN_PATH" $packages
    #pip3 download -d $DOWN_PATH scipy pandas sympy tensorflow keras

    printf $"*****\nCloning Repos\n*****\n"
    git clone https://github.com/niuroverteam/picuspy
    git clone https://github.com/adafruit/Adafruit_Python_ADS1x15.git
    git clone https://github.com/hardkernel/WiringPi2-Python.git
fi

if [ "$INSTALL_FLAG" = "1" ]; then
    printf $"*****\nInstalling Packages from [$DOWN_PATH]\n*****\n"
    sudo pip3 install --no-index $packages --find-links="$DOWN_PATH"
    #sudo pip3 download -d $DOWN_PATH scipy pandas sympy tensorflow keras
fi


# opencv apt-get packages
# python-opencv
# libjpeg8-dev libjasper-dev libpng12-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libgtk2.0-dev libatlas-base-dev
