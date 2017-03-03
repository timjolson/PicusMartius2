# Joystick Library - David Ziliak

import pygame
import time # only used for the examples at the bottom
from picusData import *

pygame.init() # start pygame to handle joysticks


class joystick_Library():

    joyID = 0           # joystick ID within pygame
    stat = "invalid"    # startup checks
    last_out_of_deadband = []   # last time each axis was outside deadband
    axes = []           # store axis values
    buttons = []        # store button values

    # constructor
    def __init__(self, joyID):
        # if init does not work
        if(pygame.joystick.get_init()==0):
            self.stat = "invalid"
            print("Oh fuck, something broke")
        # if no joysticks are detected
        elif(pygame.joystick.get_count()<1):
            self.stat = "invalid"
            print("No Joystick detected")
            exit(1)
        # otherwise, it initialized
        else:
            self.stat = "invalid"
            print("Joystick Library Initialized")

        self.joyID = joyID  # store joystick ID

        self.J = pygame.joystick.Joystick(self.joyID)           # object's joystick object
        self.J.init()                                           # initialize joystick object
        print("Joystick " + str(self.joyID) + " initialized")   # print out ID
        self.numButtons = self.J.get_numbuttons()               # store number of buttons
        self.numAxes = self.J.get_numaxes()                     # store number of axes

    def start_Joystick(self):
        try:
            self.last_out_of_deadband = [0,0,0,0]
            for x in range(0, self.numAxes):
                self.last_out_of_deadband[x] = time.time()
            print("Number of Buttons " + str(self.J.get_numbuttons()) +
                    ", Number of Axes " + str(self.J.get_numaxes()))
            self.stat = "valid"
        except:
            self.stat = "invalid"
            raise Exception("Joystick " + str(self.joyID) + "Failed to start\n")


    def read_Joystick(self):
        pygame.event.pump()
        self.axes = [0,0,0,0]
        self.buttons = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]
        self.control = ControlStruct()
        if self.stat == "valid":
            for x in range(0,self.numButtons):
                self.buttons[x] = self.J.get_button(x)
            for x in range(0,self.numAxes):
                self.axes[x] = self.J.get_axis(x) + 1

            self.control.x = self.axes[0] -1
            self.control.y = 1- self.axes[1]
            self.control.w = self.axes[2] -1
            self.control.z = 1- self.axes[3]
            self.control.fb = self.buttons[0:4]
            self.control.tb = self.buttons[4:8]
            #print(self.control.x)
            return self.control
        else:
            raise Exception("shit broke")
            return self.control


    # def get_joystick_db_time(self, axis):
    #     if axis == "z":
    #         axis = 2
    #     elif axis == "y":
    #         axis = 1
    #     elif axis == "x":
    #         axis = 0
    #     else:
    #         return 0
    #     if self.stat=="valid":
    #         return ( time.clock() - self.last_out_of_deadband[axis] )
    #     else:
    #         return 0
    #
    # def get_controller_db_time(self, axis):
    #     if axis == "x":
    #         axis = 2
    #     elif axis == "y":
    #         axis = 3
    #     elif axis == "w":
    #         axis = 0
    #     else:
    #         return 0
    #     if self.stat=="valid":
    #         return ( time.clock() - self.last_out_of_deadband[axis] )
    #     else:
    #         return 0

#example of how the code runs

if __name__ == "__main__":
    last = time.time()  # last time data was processed
    freq = 5  # joystick read frequency

    joystick = joystick_Library(0)
    joystick.start_Joystick()
    while(True):
        curr = time.time()
        if curr - last > 1/freq:
            pygame.event.pump() # also inside read
            j = joystick.read_Joystick()
            last = curr
            print(j.x)
