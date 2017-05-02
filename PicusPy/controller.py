import pygame, sys, time, os  # only used for the examples at the bottom
from picusData import *
from joystick_Library import *
from UDP_Library import *
from numpy import mod

myAddr = Martius.conn24
roverAddr = Picus.conn24
freq = 10

j = joystick_Library(0)
j.start_Joystick()
last = time.clock()

rover = UDP_Library( myAddr, roverAddr )

while(True):
    curr = time.time()

    if curr - last > 1/freq:
        last = curr
        pygame.event.pump()
        tx = j.read_Joystick()
        if tx:
            if abs(tx.y) < 0.024: tx.y = 0.0
            rover.sendDAT(pickle.dumps(tx))
            print(tx.y)

    try:
        rx = 0
        rx = rover.receiveUDP()
        if rx!=0: print('rx:%a' %(rx))
    except:
        pass

exit(0)