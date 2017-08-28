# import wiringpi2 as droid
import time, sys
from tcp_rx import *
from tcp_tx import *
from picusData import *

def pinWrite(pin, val):
	local_send(Picus.local_ports['M'], bytes(pickle.dumps((pin, val))))

manager = start_local_receive(Picus.local_ports['R'])

start = time.time()
last = start

# droid.wiringPiSetupGpio()

wheel = [2, 3, 12]
# wheel = [2]

for w in wheel:
    pinWrite(w, PWM.neutral)
    time.sleep(0.01)
    # droid.pinMode(w, 1)
    # droid.softPwmCreate(w, 0, PWM.freqRange)
    # droid.softPwmWrite(w, PWM.neutral)

print("Right Drive Started")

while True:
    curr = time.time()
    rx = local_receive(manager)
    if rx:
        last = curr
        if type(rx) == float:
            for w in wheel:
                pinWrite(w, PWM.neutral + int(rx*PWM.range))
                time.sleep(0.01)
                #print(PWM.neutral + int(rx*PWM.range))
                # droid.softPwmWrite(w, PWM.neutral + int(rx*PWM.range))
        else:
            print("Right Drive stopped")
            for w in wheel:
                pinWrite(w, PWM.neutral)
                time.sleep(0.01)
                # droid.softPwmWrite(w, PWM.neutral)
            break

    if (curr - last) > Picus.driveTimeout:
        for w in wheel:
            pinWrite(w, PWM.neutral)
            time.sleep(0.01)
            # droid.softPwmWrite(w, PWM.neutral)

for w in wheel:
    pinWrite(w, PWM.neutral)
    time.sleep(0.01)
    # droid.softPwmWrite(w, PWM.neutral)

exit(0)