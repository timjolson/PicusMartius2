

from UDP_Library import *
from Joystick_Library import *
import time
import math

freq = 10 #update rate (Hz)
deadband = 2 #joystick deadband around center

address = '192.168.1.100'
port = 5000

raspberry_pi = UDP_Library( address, port)


joy1 = Joystick_Library(0)
joy2 = Joystick_Library(1)

joy1.start_Joystick()
joy2.start_Joystick()

verify = int(joy1.numAxes)

if verify != 3:
	joy1 = Joystick_Library(1)
	joy2 = Joystick_Library(0)

	joy1.start_Joystick()
	joy2.start_Joystick()

while(True):
	joy1.read_Joystick()
	joy1_str = (str(joy1.axes[0]) + "x" + str(255-joy1.axes[1]) + "y" + str(round((255-joy1.axes[2])*(80/255))+10) + "z")
	#print(joy1_str)
	raspberry_pi.sendUDP(joy1_str)
	time.sleep(1/(freq*2))
	joy2.read_Joystick()
	
	x = joy2.axes[2]-128
	y = joy2.axes[3]-128
	
	if (abs(x)<deadband):
		x = 0
	if (abs(y)<deadband):
		y = 0
	
	if(joy2.axes[0] != 0):
		angle = round(math.degrees(math.atan2(-(y), x)))
	else:
		angle = 0
	if (angle < 0):
		angle += 360
	
	radius = math.sqrt(y*y + x*x)
	
	if(radius != 0):
		cosdir = (x)/radius
		sindir = (y)/radius
		if(abs(cosdir) > abs(sindir)):
			usedir = cosdir
		else:
			usedir = sindir
		maxamp = abs(128/usedir)
		radius /= maxamp
	else:
		radius = 0

	radius = round(radius*255)
	#joy2_str = ("Pa" + str(angle) + "r" + str(radius) + "w" + str(round((joy2.axes[0]-128)/2)) + "!")
	joy2_str = (str(angle) + "a" + str(radius) + "r" + str(round((joy2.axes[0]-128)*(99/255))+50) + "w")
	#joy2_str = ("Pa" + str(angle) + "r" + str(radius) + "!")
	#print(joy2_str)
	raspberry_pi.sendUDP(joy2_str)
	time.sleep(1/(freq*2))
	print(joy1_str+joy2_str)