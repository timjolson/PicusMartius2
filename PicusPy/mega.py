# This script was setup to manage receiving TCP data and sending to an arduino/serial object.
# This was integrated directly into the main script, but a class to do this would be beneficial.
# In the case of a class, TCP could be skipped for a method.

import serial, time, sys
from tcp_rx import *
from tcp_tx import *
from picusData import *
import pickle

# TCP receiver
manager = start_local_receive(Picus.local_ports['M'])

# time tracking
start = time.time()
last = start

# connect to serial object
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.01)
# arduino takes a while to wake up
time.sleep(1)

print("MEGA Started")

# do things
while True:
    # update time
    curr = time.time()

    # get TCP data
    rx = local_receive(manager)

    # if we have TCP data
    if rx:
        # track time
        last = curr

        # unpack TCP data
        rx = pickle.loads(bytes(rx, 'utf-8'))

        # if it's a tuple (that's how we send the stuff to this script)
        if type(rx) is tuple:
            # print the two values
            # values are (pin number, PWM write value)
            print('%d,%d' % (rx[0], rx[1]))

            # format and write the data
            ser.write(bytes('&&', 'utf-8')+bytes(str(rx[0]), 'ascii')+bytes(',', 'utf-8')+bytes(str(rx[1]), 'ascii')+bytes(',', 'utf-8'))

            # wait
            time.sleep(0.005)

    # if we have not received any TCP in 3 seconds, stop script
    if (curr - last) > 3:
        break

# close serial connection
ser.close()

# delete serial object
del ser

print("mega stopped")

