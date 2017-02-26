#https://github.com/WiringPi/WiringPi-Python
#http://odroid.com/dokuwiki/doku.php?id=en:tinkering_kit_light_level_meter_with_driver

#~ 1. Install python and setuptools

#~ apt-get install python-setuptools python-dev python3-setuptools python3-dev

#~ 2. Get the wiringPi2 Python library 

#~ git clone https://github.com/synthead/WiringPi2-Python.git

#~ 3. Build firstly the wiringPi library

#~ cd WiringPi2-Python/
#~ rm -rf WiringPi/

#~ 4. Get and build the wiringPi library compatible ODROID-C1

#~ git clone https://github.com/hardkernel/wiringPi
#~ cd wiringPi
#~ ./build
#~ cd .
#~ mv wiringPi WiringPi <— change as the WiringPi2-Python default

#~ 5. Build WiringPi2 Python 
#~ cd ..
#~ ./build.sh


#http://odroid.com/dokuwiki/doku.php?id=en:xu3_hardware_gpio

#!/usr/bin/python
import wiringpi2 as wpi
import time
 
leds = [7, 0, 2, 3, 12, 13, 14, 21, 22, 23]
wpi.wiringPiSetup()
 
# GPOI pin setup
for x in leds:
	wpi.pinMode(x, 1)
	wpi.digitalWrite(x, 0)
 
adc_unit = 4095 / len(leds)
while True:
	time.sleep(0.05)
	adcValue = wpi.analogRead(0)
	ledPos = adcValue / adc_unit
	for x in leds:
		wpi.digitalWrite(x, 0)
 
	for x in xrange(ledPos):
		wpi.digitalWrite(leds[x], 1)