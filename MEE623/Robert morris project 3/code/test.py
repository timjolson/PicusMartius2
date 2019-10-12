import serial
import time
import struct


def start():
    #Open serial connection
    global ser
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    ser.write(chr(128))#Starts the Open Interface
    ser.write(chr(131))#Changes to Safe Mode
    #time.sleep(1)

def move(vr, vl):#for moment and taking pictures
    #Changes the input velocities into serial byte Sequence
    cmd=struct.pack(">Bhh", 145, vr, vl)
    ser.write(cmd)
   
def stop():
    ser.write(chr(173))


i = 0
if i == 1:
    start()
    move(200, 200)
    time.sleep(4)
    stop()
    start()
    move(200, 0)
    time.sleep(2)
    stop()
    start()
    move(200, 200)
    time.sleep(3)
    stop()
    start()
    move(0, 200)
    time.sleep(2)
    stop()



