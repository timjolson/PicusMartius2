# http://www.lfd.uci.edu/~gohlke/pythonlibs/#pygame
# pygame‑1.9.2a0‑cp35‑none‑win_amd64.whl

# attempting to run pygame in 3.5

# success! pip install --use-wheel <package>

# this is used to show how to get info from joysticks and get values

import pygame
import time
import os
os.system('cls' if os.name == 'nt' else 'clear')

red = (255,0,0)
green = (0,255,0)
blue = (0,0,255)
darkBlue = (0,0,128)
white = (255,255,255)
black = (0,0,0)
pink = (255,200,200)

pygame.init()
pygame.joystick.init()

#button = int 0 or 1
#axis = float -1.0 to +1.0

def tempClass():
    axes_temp = ""
    axes_temp2 = ""
    axes_temp3 = ""
    button_temp = ""
    button_temp2 = ""
    for x in range(0,3):
        temp = pygame.joystick.Joystick(0).get_axis(x)
        axes_temp = axes_temp + axes[x] + ": " + str(int(255-(temp + 1)/2*255) ) + "    "
    for y in range(0,8):
        temp = pygame.joystick.Joystick(0).get_button(y)
        button_temp = button_temp + "Button " + str(1+y) + ": " + str(temp) + "    "

    # for x in range(0,4):
    #     temp = pygame.joystick.Joystick(1).get_axis(x)
    #     axes_temp2 = axes_temp2 + axes[x] + ": " + str(int(255-(temp + 1)/2*255) ) + "    "
    # for x in range(0,4):
    #     temp = pygame.joystick.Joystick(1).get_axis(x)
    #     axes_temp3 = axes_temp3 + axes[x] + ": " + str(int(255-(temp + 1)/2*255) ) + "    "
    # for y in range(0,8):
    #     temp = pygame.joystick.Joystick(1).get_button(y)
    #     button_temp2 = button_temp2 + "Button " + str(1+y) + ": " + str(temp) + "    "
    #
    pygame.display.set_caption('Axis')
    font = pygame.font.Font(None, 27)
    text = font.render((axes_temp), 1, (255,255,255) )
    screen.blit(text, (0,0))

    pygame.display.set_caption('Axis')
    font = pygame.font.Font(None, 27)
    text = font.render((button_temp), 1, (255,255,255) )
    screen.blit(text, (0,30))

    pygame.display.set_caption('Axis')
    font = pygame.font.Font(None, 27)
    text = font.render((axes_temp3), 1, (255,255,255) )
    screen.blit(text, (0,90))

    pygame.display.set_caption('Axis')
    font = pygame.font.Font(None, 27)
    text = font.render((button_temp2), 1, (255,255,255) )
    screen.blit(text, (0,120))



    pygame.display.update()



pygame.display.init()

screen_width=1000
screen_height=150
screen=pygame.display.set_mode([screen_width,screen_height])


axes = ['x_axis', 'y_axis', 'throttle', "what"]

print("Number of Joysticks " + str(pygame.joystick.get_count()))

if(pygame.joystick.get_count()<1):
    print("No Joystick detected")

joystick = pygame.joystick.Joystick(0)

pygame.joystick.Joystick(0).init()
#pygame.joystick.Joystick(1).init()
print("Number of Buttons " + str(pygame.joystick.Joystick(0).get_numbuttons()) +
      " Number of Axes " + str(pygame.joystick.Joystick(0).get_numaxes()))

axis = 0.0
button = 0

while(True):
    pygame.event.pump()
    tempClass()
    screen.fill(black)

