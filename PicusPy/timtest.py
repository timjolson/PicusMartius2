import wiringpi2 as droid
#from wiringpi2 import GPIO
import time
from UDP_Library import *
from tcp_rx import *
from tcp_tx import *
from picusData import *

lp = Picus.local_ports

start = time.time()
last = start

#droid = GPIO(1)
#wiringpi2.wiringPiSetupGpio()
droid.wiringPiSetupGpio()

wheel = [19,21,174,18,210,209]
steer = [22, 173, 171, 172]
steerdir = [192]

dir = 0
range = 20
duty = 0.0

droid.pinMode(22, 1)  # output
droid.digitalWrite(22, 0)  # turn off

droid.pinMode(192, 1)
droid.digitalWrite(192, 0)

droid.softPwmCreate(22, 0, range)
droid.softPwmWrite(22, 0)

manager = start_local_receive(lp['L'])

while True:
    curr = time.time()
    rx = local_receive(manager)
    if rx:
        last = curr
        if type(rx) == float:
            print(rx)
            droid.digitalWrite(192, rx > 0)
            droid.softPwmWrite(22, abs(int(rx * range)))
        else:
            droid.digitalWrite(22, 0)
            droid.digitalWrite(192, 0)
            break
    else:
        print("nd")

    if curr - last > 10:
        break

droid.digitalWrite(22, 0)
droid.digitalWrite(192, 0)