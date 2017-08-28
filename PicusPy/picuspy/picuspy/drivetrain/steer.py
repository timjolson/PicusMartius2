import serial
import time, sys
from tcp_rx import *
from tcp_tx import *
from picusData import *

def pinWrite(pin, val):
	local_send(Picus.local_ports['M'], bytes(pickle.dumps((pin, val))))

manager = start_local_receive(Picus.local_ports['S'])

start = time.time()
last = start

steer = [8, 9, 10, 11]
enable = 13

# local_send(lp['M'], bytes('&&', 'utf-8')+bytes(str(enable), 'utf-8')+bytes(1))
# local_send(lp['M'], pickle.dumps((enable,1)))
# droid.pinMode(enable, 1)
for s in steer:
    # local_send(lp['M'], bytes('&&', 'utf-8') + bytes(str(enable), 'utf-8') + bytes(1))
    # droid.pinMode(w, 1)
    # droid.softPwmCreate(w, 0, PWM.freqRange)
    # local_send(lp['M'], bytes(pickle.dumps((s,189))))
    pinWrite(s, PWM.neutral)
    time.sleep(0.01)
    # droid.softPwmWrite(w, PWM.neutral)

print("Steering Started")

try:
    while True:
        curr = time.time()
        rx = local_receive(manager)
        if rx:
            last = curr
            if type(rx) == float:
                #droid.digitalWrite(enable, 1)
                for s in steer:
                    # droid.softPwmWrite(s, PWM.neutral + int(rx*PWM.range))
                    #local_send(lp['M'], bytes('&&', 'utf-8') + bytes(str(s), 'utf-8') + bytes(rx*127 + 127))
                    #print('steer: %d' %(rx*127 + 127))
                    pinWrite(s, PWM.neutral + rx*PWM.range)
                    time.sleep(0.01)
                    #droid.softPwmWrite(s, int(rx*PWM.freqRange/2 + PWM.freqRange/2))
                pinWrite(enable, 254)
            else:
                print("Steering stopped")
                # for s in steer:
                #     droid.softPwmWrite(s, PWM.neutral)
                # droid.digitalWrite(enable, 0)
                break

        if (curr - last) > Picus.driveTimeout:
            pinWrite(enable, 0)
            time.sleep(0.005)
            #droid.digitalWrite(enable, 0)
            for s in steer:
                pinWrite(s, PWM.neutral)
                time.sleep(0.01)
                #droid.softPwmWrite(s, PWM.neutral)

except KeyboardInterrupt:
    pass

for s in steer:
    pinWrite(s, PWM.neutral)
    time.sleep(0.01)
    #droid.softPwmWrite(s, PWM.neutral)
pinWrite(enable, 0)
time.sleep(0.01)
#droid.digitalWrite(enable, 0)

exit(0)