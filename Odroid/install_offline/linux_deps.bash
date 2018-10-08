#! /bin/bash

# List tools and ROS packages
packages="htop i2c-tools build-essential cmake git pkg-config gfortran python3-pip ipython3 ipython3-notebook swig3.0 python3-setuptools"

# Default download path
BACKUP_PATH=./linux_packages
DOWN_FLAG=0
RESTORE_FLAG=0
INSTALL_FLAG=0

# Structure inspired by http://linuxcommand.org/lc3_wss0120.php
HELP()
{
    echo "usage: linux_deps [[-d | --download] [-i | --install] [-p | --path backup_path] [-r | --restore] [-h | --help]]"
}

while [ "$1" != "" ]; do
    case $1 in
        -d | --download )       DOWN_FLAG=1
                                ;;
        -h | --help )           HELP
                                exit
                                ;;
        -p | --path )         shift
                                BACKUP_PATH="$1"
                                ;;
        -r | --restore )        shift
                                RESTORE_FLAG=1
                                ;;
        -i | --install )        INSTALL_FLAG=1
                                ;;
        * )                     HELP
                                exit 1
    esac
    shift
done


if [ "$DOWN_FLAG" = "1" ]; then
    printf $"*****\nDownloading packages to [$BACKUP_PATH]\n*****\n"
    # Update package lists and such
    sudo apt-get update

    # Download packages
    mkdir "$BACKUP_PATH"
    mkdir "$BACKUP_PATH"/partial
    BACKUP_PATH="$(readlink -f $"$BACKUP_PATH"/)"
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

if [ "$INSTALL_FLAG" = "1" ]; then
    printf $"*****\nInstalling packages from [$BACKUP_PATH]\n*****\n"
    BACKUP_PATH="$(readlink -f $"$BACKUP_PATH"/)"
    sudo apt-get -o dir::cache::archives="$BACKUP_PATH" install $packages
fi
