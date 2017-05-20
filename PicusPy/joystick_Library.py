# Joystick Library - started by David Ziliak, heavy modified by Tim Olson

import pygame
import time # only used for the examples at the bottom
from picusData import *

pygame.init() # start pygame to handle joysticks


class joystick_Library():

    # constructor
    def __init__(self, joyID=0):
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

        self.axes = []  # store axis values
        self.buttons = []  # store button values

        self.joyID = joyID  # store joystick ID

        # J is our instance's reference to the pygame joystick instance
        self.J = pygame.joystick.Joystick(self.joyID)           # object's joystick object
        self.J.init()                                           # initialize joystick object
        print("Joystick " + str(self.joyID) + " initialized")   # print out ID

        # keep track of number buttons and analog axes
        self.numButtons = self.J.get_numbuttons()               # store number of buttons
        self.numAxes = self.J.get_numaxes()                     # store number of axes

    # initialize the joystick for our purposes
    def start_Joystick(self):
        # try to set things up
        try:
            # print stick info
            print("Number of Buttons " + str(self.J.get_numbuttons()) +
                    ", Number of Axes " + str(self.J.get_numaxes()))

            # until now, the stick was invalid
            self.stat = "valid"
            return True

        # there was a problem
        except:
            self.stat = "invalid"
            #raise Exception("Joystick " + str(self.joyID) + "Failed to start\n")
            return False

    # reads joystick and returns a ControlStruct() containing the values
    def read_Joystick(self):
        # update pygame controller
        pygame.event.pump()

        # set all axes, buttons = 0
        self.axes = [0,0,0,0]
        self.buttons = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]
        # make empty structure
        self.control = ControlStruct()

        # if the stick is valid
        if self.stat == "valid":
            # read and store all buttons
            for x in range(0,self.numButtons):
                self.buttons[x] = self.J.get_button(x)
            # read and store all axes, shift to +/- 1.0 range
            for x in range(0,self.numAxes):
                self.axes[x] = self.J.get_axis(x) + 1

            # reverse specific axes so they're intuitive
            self.control.x = self.axes[0] -1
            self.control.y = 1- self.axes[1]
            self.control.w = self.axes[2] -1
            self.control.z = 1- self.axes[3]

            # make face button list
            self.control.fb = self.buttons[0:4]
            # make trigger button list
            self.control.tb = self.buttons[4:8]

            # make a button 9 and 10 reference for ease of use
            self.control.b9 = self.buttons[8]
            self.control.b10 = self.buttons[9]

            # return structure
            return self.control
        else:
            # stick was not valid
            raise Exception("shit broke")
            return self.control


#example of how the code runs
if __name__ == "__main__":
    last = time.time()  # last time data was processed
    freq = 5  # joystick read frequency

    # use pygame to make controller instance
    joystick = joystick_Library(0)
    # set it up for us
    joystick.start_Joystick()

    # do things
    while(True):
        # update current time
        curr = time.time()

        # if reading period elapsed
        if curr - last > 1/freq:
            pygame.event.pump() # also called inside read (not necessary here)

            # read stick data
            j = joystick.read_Joystick()

            # update last time
            last = curr

            # print x axis
            print(j.x)
