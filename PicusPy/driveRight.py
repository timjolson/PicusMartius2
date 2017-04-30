import wiringpi2 as droid
import time, sys
from tcp_rx import *
from tcp_tx import *
from picusData import *

lp = Picus.local_ports

start = time.time()
last = start

droid.wiringPiSetupGpio()

wheel = [21, 18, 22]

for w in wheel:
    droid.pinMode(w, 1)
    droid.softPwmCreate(w, 0, PWM.freqRange)
    droid.softPwmWrite(w, PWM.neutral)

manager = start_local_receive(lp['R'])

print("Right Drive Started")

while True:
    curr = time.time()
    rx = local_receive(manager)
    if rx:
        last = curr
        if type(rx) == float:
            for w in wheel:
                droid.softPwmWrite(w, PWM.neutral + int(rx*PWM.range))
        else:
            print("Right Drive stopped")
            for w in wheel:
                droid.softPwmWrite(w, PWM.neutral)
            break

    if (curr - last) > Picus.driveTimeout:
        for w in wheel:
            droid.softPwmWrite(w, PWM.neutral)

    if curr - last > 10:
        break

for w in wheel:
    droid.softPwmWrite(w, PWM.neutral)

exit(0)