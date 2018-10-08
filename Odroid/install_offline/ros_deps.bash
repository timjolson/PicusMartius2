#! /bin/bash

# Search for ROS packages
# apt-cache search ros-kinetic-PACKAGE

# Tutorials
# http://wiki.ros.org/ROS/Tutorials


# List tools and ROS packages
packages="ros-kinetic-desktop-full ros-kinetic-joy ros-kinetic-ros-tutorials ros-kinetic-pcl-ros ros-kinetic-image-transport ros-kinetic-desktop ros-kinetic-ros-comm ros-kinetic-rviz ros-kinetic-rqt ros-kinetic-rqt-common-plugins ros-kinetic-ros-base ros-kinetic-rosserial-arduino ros-kinetic-rosserial-xbee"

# Default download path
BACKUP_PATH=./ros_packages
DOWN_FLAG=0
RESTORE_FLAG=0

# Structure inspired by http://linuxcommand.org/lc3_wss0120.php
HELP()
{
    echo "usage: ros_deps [[-d | --download] [-p | --path backup_path] [-r | --restore] [-h | --help]]"
}

while [ "$1" != "" ]; do
    case $1 in
        -d | --download )       DOWN_FLAG=1
                                ;;
        -h | --help )           HELP
                                exit
                                ;;
        -p | --path )           shift
                                BACKUP_PATH="$1"
                                ;;
        -r | --restore )        shift
                                RESTORE_FLAG=1
                                ;;
        * )                     HELP
                                exit 1
    esac
    shift
done


if [ "$DOWN_FLAG" = "1" ]; then
    printf $"*****\nDownloading packages to [$BACKUP_PATH]\n*****\n"
    ####
    # Allow ROS package downloading
    # http://wiki.ros.org/kinetic/Installation/Ubuntu
    sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
    sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116

    # Update package lists and such
    sudo apt-get update

    # Download packages
    mkdir "$BACKUP_PATH"
    mkdir "$BACKUP_PATH"/partial
    sudo apt-get -d -y -o dir::cache::archives="$BACKUP_PATH" install $packages
    sudo chmod -R 755 "$BACKUP_PATH"/.
    
    printf $"\ndone\n"
fi

if [ "$RESTORE_FLAG" = "1" ]; then
    eval $(apt-config shell CACHE Dir::Cache)
    eval $(apt-config shell ARCHIVES Dir::Cache::archives)
    CACHE_PATH=/${CACHE}/${ARCHIVES}
    
    printf $"*****\nRestoring packages from [$BACKUP_PATH] to \n[$CACHE_PATH]\n*****\n"
    
    BACKUP_PATH="$(readlink -f $"$BACKUP_PATH"/)"
    sudo cp -r "$BACKUP_PATH"/. $CACHE_PATH
    
    printf $"\ndone\n"
fi
