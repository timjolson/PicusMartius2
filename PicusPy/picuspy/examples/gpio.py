from picuspy.gpio import wiringpi2 as droid
from time import sleep

droid.wiringPiSetupGpio()

range = 20

droid.pinMode(22, 1)  # output
droid.digitalWrite(22, 0)  # turn off

droid.pinMode(192, 1)
droid.digitalWrite(192, 0)

droid.softPwmCreate(22, 0, range)
droid.softPwmWrite(22, 0)

for v in [1, 0, 0, 1, 1, 0]:
    print(v)
    droid.digitalWrite(192, v)
    droid.softPwmWrite(22, int(5*v + .5*range))
    sleep(1) # time.sleep()

droid.digitalWrite(22, 0)
droid.digitalWrite(192, 0)
