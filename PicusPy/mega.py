import serial, time, sys
from tcp_rx import *
from tcp_tx import *
from picusData import *
import pickle

manager = start_local_receive(Picus.local_ports['M'])

start = time.time()
last = start

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.01)
time.sleep(1)
print("MEGA Started")


while True:
    curr = time.time()
    rx = local_receive(manager)
    if rx:
        last = curr
        rx = pickle.loads(bytes(rx, 'utf-8'))
        if type(rx) is tuple:
            print('%d,%d' % (rx[0], rx[1]))
            # if ser.isOpen():
            ser.write(bytes('&&', 'utf-8')+bytes(str(rx[0]), 'ascii')+bytes(',', 'utf-8')+bytes(str(rx[1]), 'ascii')+bytes(',', 'utf-8'))
            time.sleep(0.005)
            # else:
            #     try:
            #         ser.open()
            #     except:
            #         pass
            # print(ser.readline())

    if (curr - last) > 3:
        break

ser.close()
del ser

print("mega stopped")

