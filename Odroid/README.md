# Setup instructions for the Odroid

## On separate machine

### Format SD Card as FAT
if SD Card is already setup to boot, format the partition named *boot*

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
1. default user/password is root/odroid
2. sudo apt-get update
3. sudo apt-get upgrade
4. accept recommendations / if console settings comes up, hit ENTER on each page
5. sudo apt-get dist-upgrade
6. reboot
7. log in
8. sudo apt-get update
9. sudo apt-get upgrade

### Make new user account (don't do things in root too much)
1. sudo adduser WHATEVERUSERNAMEYOUWANT
#### Give user sudo rights
2. usermod -aG sudo WHATEVERUSERNAMEYOUWANT
3. reboot
---

## Installations

### Install python
1. sudo apt-get install python-setuptools python-dev python3-setuptools python3-dev
2. sudo apt-get install swig3.0 python-pip python3-pip git build-essential
---

### Install other tools
#### Use htop to kill processes, view cpu and ram usage, etc
1. sudo apt-get install htop
#### Use i2c tool's 'i2cdetect' to detect device addresses
2. apt-get install i2c-tools
---

### To use onboard GPIO with WiringPi2
#### Will use hardkernel repo at https://github.com/hardkernel/WiringPi2-Python
#### Clone repo to a reasonable directory
1. git clone https://github.com/hardkernel/WiringPi2-Python.git
#### Enter repo folder
2. cd WiringPi2-Python
#### Do git stuff
3. git submodule init
4. git submodule update
#### Compile stuff
5. swig3.0 -python -threads wiringpi.i
#### Install for python
6. sudo python3 setup.py install
#### Test
7. sudo nano test.py
8. change the **print XXXX** lines to **print(XXXX)** for python3
9. save and close test.py
10. sudo python3 test.py
---

### To use the ADCs on the Odroid
#### Use package from Adafruit https://github.com/adafruit/Adafruit_Python_ADS1X15
#### Clone repo to reasonable directory
1. git clone https://github.com/adafruit/Adafruit_Python_ADS1x15.git
#### Go into directory
2. cd Adafruit_Python_ADS1x15
#### Install the tool
3. sudo python3 setup.py install
#### Test
4. TODO
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
