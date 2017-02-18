import pygame
from pygame.locals import *

red = (255,0,0)
green = (0,255,0)
blue = (0,0,255)
darkBlue = (0,0,128)
white = (255,255,255)
black = (0,0,0)
pink = (255,200,200)

pygame.init()

pygame.display.init()

screen_width=1000
screen_height=1000
screen=pygame.display.set_mode([screen_width,screen_height])

while(True):
    pygame.display.set_caption('Score')
    font = pygame.font.Font(None, 50)
    text = font.render(('s + s2'), 1, (255,255,255) )
    screen.blit(text, (0,0))
    pygame.display.update()
