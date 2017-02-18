# Joystick Library - David Ziliak

import pygame
import time # only used for the examples at the bottom

pygame.init()
pygame.joystick.init()

class joystick_Library():

    stat = "invalid"
    last_out_of_deadband = []

    def __init__(self, joyID):
        if(pygame.joystick.get_init()==0):
            stat = "invalid"
            print("Oh fuck, something broke")
        elif(pygame.joystick.get_count()<1):
            stat = "invalid"
            print("No Joystick detected")
        else:
            print("Joystick Library Initialized")
        self.joyID = joyID

    def start_Joystick(self):
        try:
            self.Joystick = pygame.joystick.Joystick(self.joyID)
            pygame.joystick.Joystick(self.joyID).init()
            print("Joystick " + str(self.joyID+1) + " initialized")
            self.numButtons = pygame.joystick.Joystick(self.joyID).get_numbuttons()
            self.numAxes = pygame.joystick.Joystick(self.joyID).get_numaxes()
            for x in self.numAxes:
                self.last_out_of_deadband[x] = time.clock()
            print("Number of Buttons " + str(pygame.joystick.Joystick(self.joyID).get_numbuttons()) +
                    ", Number of Axes " + str(pygame.joystick.Joystick(self.joyID).get_numaxes()))
            stat = "valid"
        except:
            stat = "invalid"

    def read_RawJoystick(self):
        self.axes = []
        self.buttons = []
        if stat == "valid":
            #pygame.event.pump()
            for x in range(0,self.numButtons):
                a = pygame.joystick.Joystick(self.joyID).get_button(x)
                self.buttons.append(a)
            for x in range(0,self.numAxes):
                a = pygame.joystick.Joystick(self.joyID).get_axis(x)
                self.axes.append(a)
        else:
            self.axes.append(0)
            self.buttons.append(0)

    def read_Joystick(self):
        self.axes = []
        self.buttons = []
        if self.stat == "valid":
            #pygame.event.pump()
            for x in range(0,self.numButtons):
                a = pygame.joystick.Joystick(self.joyID).get_button(x)
                self.buttons.append(a)
            for x in range(0,self.numAxes):
                a = pygame.joystick.Joystick(self.joyID).get_axis(x) + 1
                b = int (a *255 / 2)
                self.axes.append(b)
        else:
            self.axes.append(0)
            self.buttons.append(0)

    def get_Joystick_Str(self):
        try:
            self.read_Joystick()
            z = round( ( 128 - self.axes[2] ) * 80/255 )
            y = 255 - self.axes[1]
            x = self.axes[0]

            if (abs(x) < deadband):
                x = 0
            else:
                self.last_out_of_deadband[2] = time.clock()
            if (abs(y) < deadband):
                y = 0
            else:
                self.last_out_of_deadband[3] = time.clock()
            if (abs(z) < deadband):
                z = 0
            else:
                self.last_out_of_deadband[0] = time.clock()

            return (str(x) + "x" + str(y) + "y" + str(z) + "z") #TODO: removed +10? from z output, changed 255- to 128-
        except:
            return "0"

    def get_Controller_Str(self):
        try:
            read_Joystick()

            x = self.axes[2] - 128
            y = self.axes[3] - 128
            w = self.axes[0]

            if (abs(x) < deadband):
                x = 0
            else:
                self.last_out_of_deadband[2] = time.clock()
            if (abs(y) < deadband):
                y = 0
            else:
                self.last_out_of_deadband[3] = time.clock()
            if (abs(w) < deadband):
                w = 0
            else:
                self.last_out_of_deadband[0] = time.clock()

            #TODO: was self.axes[0] or w
            if (x != 0):
                angle = round(math.degrees(math.atan2(-(y), x)))
            else:
                angle = 0
            if (angle < 0):
                angle += 360

            radius = math.sqrt(y * y + x * x)

            if (radius != 0):
                cosdir = (x) / radius
                sindir = (y) / radius
                if (abs(cosdir) > abs(sindir)):
                    usedir = cosdir
                else:
                    usedir = sindir
                maxamp = abs(128 / usedir)
                radius /= maxamp
            else:
                radius = 0

            radius = round(radius * 255)

            return (str(angle) + "a" + str(radius) + "r" + str(round((w - 128) * (99 / 255))) + "w") #TODO: removed + 50 from w output
        except:
            return "0"

    def get_joystick_db_time(self, axis):
        if axis == "z":
            axis = 2
        elif axis == "y":
            axis = 1
        elif axis == "x":
            axis = 0
        else:
            return 0
        if self.stat=="valid":
            return ( time.clock() - self.last_out_of_deadband[axis] )
        else:
            return 0

    def get_controller_db_time(self, axis):
        if axis == "x":
            axis = 2
        elif axis == "y":
            axis = 3
        elif axis == "w":
            axis = 0
        else:
            return 0
        if self.stat=="valid":
            return ( time.clock() - self.last_out_of_deadband[axis] )
        else:
            return 0

#example of how the code runs

if __name__ == "__main__":
    joystick = joystick_Library(0)
    joystick.start_Joystick()
    while(True):
        pygame.event.pump()
        joystick.read_Joystick()
        print(joystick.axes)
        print(joystick.buttons)
        time.sleep(1/20)

