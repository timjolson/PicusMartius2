# Setup instructions for the Odroid

### Format SD Card as FAT

### Get minimal OS image for xu4
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

### Hook up Odroid
1. put in SD card
2. make sure boot switch is set to uSD
2. keyboard
3. monitor
4. connect to an internet source
5. connect power last, will boot automatically

## Update Odroid
1. default user/password is root/odroid
2. sudo apt-get update
3. sudo apt-get upgrade
4. accept recommendations / if console settings comes up, hit ENTER on each page
5. sudo apt-get dist-upgrade
6. reboot
7. log in
8. sudo apt-get update
9. sudo apt-get upgrade

### Make new user account
#### (don't do things in root too much)
1. sudo adduser WHATEVERUSERNAMEYOUWANT
#### give user sudo rights
2. usermod -aG sudo WHATEVERUSERNAMEYOUWANT
3. reboot

### Install python
1. sudo apt-get install python-setuptools python-dev python3-setuptools python3-dev
2. sudo apt-get install swig2.0 python-pip python3-pip
3. sudo pip install --upgrade pip enum34

### Install other tools
#### Use htop to kill processes, view cpu and ram usage, etc
1. sudo apt-get install htop
#### Use i2c tool's 'i2cdetect' to detect device addresses
2. apt-get install i2c-tools
#### Install pyserial
3. sudo pip install pyserial
#### Install numpy
4. sudo pip install numpy
#### Install pygame
5. sudo pip install pygame

### To use the ADCs on the Odroid
#### get package from Adafruit
#### https://github.com/adafruit/Adafruit_Python_ADS1X15
1. apt-get install git build-essential python3-dev
#### make directory
2. mkdir scripts
#### go into directory
3. cd scripts
#### download files
4. git clone https://github.com/adafruit/Adafruit_Python_ADS1x15.git
#### go into directory
5. cd Adafruit_Python_ADS1x15
#### install the tool
6. sudo python setup.py install

### Connecting external storage
#### make directory to mount into
1. mkdir /media/usb
#### mount:
1. sudo mount /dev/sda1 /media/usb
#### unmount:
1. sudo umount /dev/sda1

### After all the setup over the internet, set static IP
#### backup network settings file
1. sudo mv /etc/network/interfaces /etc/network/interfaces.dhcp
#### copy the 'interfaces' file to /etc/network
2. sudo cp WHEREVERTHEFILEIS/interfaces /etc/network/
3. reboot
#### to re-enable DHCP instead of static IP
1. comment out the whole second block in the interfaces file or swap the files
2. reboot

### Connection Test (Windows)
1. ping IP of odroid
2. if ping works, see if Putty can connect

### To actually connect and do things
1. Putty - ssh for commands / terminal stuff
2. WinSCP - for file access / drag & drop

### For executable bash scripts
1. Make your script
2. first line must be #!/bin/bash
3. sudo chmod +x SCRIPT
4. run with: ./SCRIPT

### Multiple command syntax
#### Run A and then B, regardless of success of A
A; B
#### Run B if A succeeded
A && B
#### Run B if A failed
A || B
#### Run A in background
A &
