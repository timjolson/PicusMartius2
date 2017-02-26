import time, socket, sys
from picusData import *
import wiringpi2 as wp

wp.wiringPiSetup()
wp.pinMode( #pinNum, 0=in 1=out 2=other)

print(wp.piBoardRev()))

wp.digitalRead(#pinNum)
wp.digitalWrite(#pinNum))

wp.pullUpDnControl(#pinNum, 2=active up 1=active down 0= deactivated)

#hardware PWM
