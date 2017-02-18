# http://www.lfd.uci.edu/~gohlke/pythonlibs/#pygame
# pygame‑1.9.2a0‑cp35‑none‑win_amd64.whl

# attempting to run pygame in 3.5

# success! pip install --use-wheel <package>

# this is used to show how to get info from joysticks and get values

import pygame
import time
import os
os.system('cls' if os.name == 'nt' else 'clear')

pygame.init()
pygame.joystick.init()

def tempClass():
    axes_temp = ""
    button_temp = ""
    for x in range(0,3):
        temp = pygame.joystick.Joystick(0).get_axis(x)
        axes_temp = axes_temp + axes[x] + ": " + str(temp) + "    "
    for y in range(0,8):
        temp = pygame.joystick.Joystick(0).get_button(y)
        button_temp = button_temp + "Button " + str(y) + ": " + str(temp) + "    "

    print(axes_temp)
    print(button_temp)

axes = ['x_axis', 'y_axis', 'throttle']

print("Number of Joysticks " + str(pygame.joystick.get_count()))

if(pygame.joystick.get_count()<1):
    print("No Joystick detected")

joystick = pygame.joystick.Joystick(0)

pygame.joystick.Joystick(0).init()

print("Number of Buttons " + str(pygame.joystick.Joystick(0).get_numbuttons()) +
      "Number of Axes " + str(pygame.joystick.Joystick(0).get_numaxes()))

axis = 0.0
button = 0

while(True):
    pygame.event.pump()
    tempClass()
    time.sleep(1/60)

