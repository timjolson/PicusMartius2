import serial
import time
import struct



def start():
    #Open serial connection
    global ser
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    ser.write(chr(128))#Starts the Open Interface
    ser.write(chr(131))#Changes to Safe Mode
    time.sleep(1)


def move(vr, vl):#for moment and taking pictures
    # # # scale for relative speed control
    # vr = vr
    # vl = vl
    # m = max(abs(vr), abs(vl), 255)
    # vr = round(float(vr)/m)*255
    # vl = round(float(vl)/m)*255

    #Changes the input velocities into serial byte Sequence
    cmd=struct.pack(">Bhh", 145, vr, vl)
    ser.write(cmd)

def stop():
    ser.write(chr(173))
