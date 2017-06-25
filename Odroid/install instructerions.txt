# Setup instructions for the Odroid

## Format SD Card as NTFS

## Get minimal OS image for xu4
https://odroid.in/ubuntu_16.04lts/

## Install on computer (portables exist for most):
1. Putty - ssh for commands / terminal stuff
2. WinSCP - for file access / drag & drop
3. 7zip - to extract OS image
4. win32 disk imager - to burn OS image

## Burn image
1. use 7zip to extract the os image
2. use win32 to burn image to sd card
3. verify image (re-burn if needed)

## Hook up Odroid
1. put in SD card
2. make sure boot switch is set to uSD
2. keyboard
3. monitor
4. connect to an internet source
5. connect power last, will boot automatically

## update Odroid
1. default user/password is root/odroid
2. sudo apt-get update
3. sudo apt-get upgrade
4. accept recommendations / if console settings comes up, hit ENTER on each page
5. sudo apt-get dist-upgrade
6. reboot
7. log in
8. sudo apt-get update
9. sudo apt-get upgrade
# make new user account (don't do things in root too much)
10. sudo adduser WHATEVERUSERNAMEYOUWANT
# give user sudo rights
11. usermod -aG sudo WHATEVERUSERNAMEYOUWANT
12. reboot
13. log in
# use htop to kill processes, view cpu and ram usage, etc
14. sudo apt-get install htop

## install python3
1. sudo apt-get install python-setuptools python-dev python3-setuptools python3-dev
2. sudo apt-get install swig2.0 python-pip python3-pip
3. sudo pip install --upgrade pip enum34

## to use I2C on Odroid
1. apt-get install i2c-tools
# use i2cdetect to see connected devices
# to use the ADCs on the Odroid
# https://github.com/adafruit/Adafruit_Python_ADS1X15
2. apt-get install git build-essential python3-dev
# make directory
3. mkdir scripts
# go into directory
4. cd scripts
3. git clone https://github.com/adafruit/Adafruit_Python_ADS1x15.git
4. cd Adafruit_Python_ADS1x15
5. sudo python3 setup.py install
# install pyserial
6. sudo pip3 install pyserial
# install numpy
7. sudo pip3 install numpy

## to mount a flash drive
# example:
sudo mount /dev/sda1 /media/usb
# unmount:
sudo umount /dev/sda1

## After all the setup over the internet, set static IP
# backup file
1. sudo mv /etc/network/interfaces /etc/network/interfaces.backup
# copy the 'interfaces' file to /etc/network
2. sudo cp WHEREVERTHEFILEIS/interfaces /etc/network/
3. reboot
# to re-enable DHCP instead of static IP
1. comment out the whole second block in the interfaces file or swap the files
2. reboot

## Connection
1. Connect odroid to switch / radio
2. Connect computer to switch / radio

## Connection Test (Windows)
1. ping IP of odroid
for example, in cmd prompt:
ping 192.168.1.XXX
2. if ping works, see if Putty can connect

## To Actually connect
1. Putty - ssh for commands / terminal stuff
2. WinSCP - for file access / drag & drop
