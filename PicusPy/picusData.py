'''Define communication settings, motor pins, and PWM values
for Picus(Rover) and Martius(Controller)
'''
from picusEnum import *


class Picus:
    '''Communication, and timeouts for Picus(rover)
    '''
    conn24 = ("192.168.1.200", 7008)  # this is the rover @2.4GHz
    localTimeout = 0.005  # socket timeout

    driveTimeout = 0.3  # safety timeout for drive system
    armTimeout = 0.06  # safety timeout for arm system
    sciTimeout = 0.06  # safety timeout for science system

    local_ports = {'L': 6000, 'R': 6001, 'S': 6002, 'M': 6003}  # ports for tcp communication


class Martius:
    '''Communication, and timeouts for Martius(Controller)
    '''
    conn24 = ("192.168.1.100", 6008)  # this is the controller @2.4GHz
    localTimeout = 0.01  # socket timeout


class DRIVE_vals():
    '''Values to write to driving motors
    '''
    neutral = 189
    min = 127
    max = 254
    range = max - neutral


class STEER_vals():
    '''Values to write to steering motors
    '''
    neutral = 127
    min = 0
    max = 254
    range = max - neutral
    

# Motor Pins
Left = [5,6,7]
Right = [2,3,12]
Steer = [11, 10, 8, 9]  # motor 1,3,4,6
SteerEnable = [24, 27, 28, 31]  # motor 1,3,4,6
Arm = [44,45,46]

# ADC Addresses
ADCaddr = {'Steer':0x48, 'Arm1':0x49, 'Arm2':0x4A, 'Spare':0x4B}

