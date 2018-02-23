# Interfacing with single-board computers
[**Putty**](http://portableapps.com/apps/internet/putty_portable)

Allows remote connection to networked boards. Access through shell/command line can be expanded to include windowed objects using [X, X11, and similar](https://en.wikipedia.org/wiki/X_Window_System).

[**WinSCP**](http://portableapps.com/apps/internet/winscp_portable)

Allows remote connection with simple file management through explorer style windows, with drag/drop option.

# Setup instructions for the Odroid

## On separate machine

### Format SD Card as FAT
If SD Card is already setup to boot, format the partition named *boot*.

### Get minimal OS image for "XU4"
https://odroid.in/ubuntu_16.04lts/

### Install on computer (portables exist for most):
1. Putty - ssh for commands / terminal stuff
2. WinSCP - for file access / drag & drop
3. 7zip - to extract OS image
4. win32 disk imager - to burn OS image

### Burn image
1. use 7zip to extract the os image
2. use win32 to burn image to sd card
3. verify image (re-burn if needed)
---
## Initial setup

### Hook up Odroid
1. put in SD card
2. make sure boot switch is set to uSD
2. keyboard
3. monitor
4. connect to an internet source
5. connect power last, will boot automatically

### Update Odroid
Default user/password is root/odroid
    
    # Update package version lists
    sudo apt-get update
    # Update packages
    sudo apt-get upgrade
    # accept recommendations / if console settings comes up, hit ENTER on each page
    # Update kernel
    sudo apt-get dist-upgrade
    sudo reboot now
    # Log in
    # Check for updates again
    sudo apt-get update
    sudo apt-get upgrade

### Make new user account (don't do things in root too much)
    
    # Make user
    sudo adduser WHATEVERUSERNAMEYOUWANT
    # Give user sudo rights
    usermod -aG sudo WHATEVERUSERNAMEYOUWANT
    sudo reboot now
---

## Installations

### To assist in offline/repeated setup on Ubuntu/SBCs:
#### Use bash scripts from [PicusMartius/Odroid/install_offline](https://github.com/niuroverteam/PicusMartius/tree/master/Odroid/install_offline)
#### To use them
    # first make each script executable
    chmod +x SCRIPT
#### Each time a script is run, a directory can be provided {-p}
    # specify download to path / restore from path / install from path
    ./SCRIPT -p DIRECTORY
#### Each script can download a local repository of packages {-d}
    # download relevant packages
    ./SCRIPT -d
#### The py_deps and linux_deps scripts support installation from the local repo {-i}
    # install relevant packages
    ./SCRIPT -i
    # download to DIRECTORY and install from it
    ./SCRIPT -p DIRECTORY -d -i
#### The linux_deps and ros_deps support restoring downloaded packages to the system's apt-get cache directory {-r}
    # restore relevant packages
    ./SCRIPT -r
    # packages may then be installed from cache with standard apt-get, without specifying a package path
****
### A few handy tools *(included when using the offline bash scripts)*
#### Use htop to kill processes, view cpu and ram usage, etc
1. sudo apt-get install htop
#### Use i2c tool's 'i2cdetect' to detect device addresses
2. apt-get install i2c-tools
---

### To use picuspy package
Follow installation instructions at
  https://github.com/NIURoverTeam/picuspy
  
---

### Connecting external storage
#### Make directory to mount into
1. mkdir /media/usb
#### Mount:
1. sudo mount /dev/sda1 /media/usb
#### Unmount:
1. sudo umount /dev/sda1
---

### After all the setup over the internet, set static IP
#### Backup network settings file
1. sudo mv /etc/network/interfaces /etc/network/interfaces.dhcp
#### Copy the 'interfaces' file to /etc/network
2. sudo cp WHEREVERTHEFILEIS/interfaces /etc/network/
3. reboot
#### To re-enable DHCP instead of static IP
1. comment out the whole second block in the interfaces file or swap the files
2. reboot
---

### Connection Test (Windows)
1. ping IP of odroid
2. if ping works, see if Putty can connect

### To actually connect and do things
1. Putty - ssh for commands / terminal stuff
2. WinSCP - for file access / drag & drop
---

## BASH
### For executable bash scripts
1. Make your script
2. first line must be #!/bin/bash
3. sudo chmod +x SCRIPT
4. run with: ./SCRIPT

### Multiple command syntax
#### Run A and then B, regardless of success of A
>A; B
#### Run B if A succeeded
>A && B
#### Run B if A failed
>A || B
#### Run A in background
>A &
