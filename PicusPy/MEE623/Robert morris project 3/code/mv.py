#import serial
import time
import struct
import cv2
import numpy as np
import takepic
import test
import custom

#Start robot
test.start()
time.sleep(1)
#print('Started Successfully')

#Initialize Starting Variables
iterations = 100
ideal_width = 160 #180 is exactly 12 inches from the center of the bat
center_of_frame = 320
pixel_error = 20
turning_error = 25
start_time = time.time()
#print('Starting variables initalized')

for run in range(0,iterations):

    img = takepic.picture()
    #print('Picture taken successfully')
    img = cv2.imread('Image.jpg')
    x, y, w, h, cx, cy, area = custom.process_image(img)
    #print('Processing done successfully')

    if not cx:  # List is empty
        print('Object not in frame. Spinning to find object')
        test.move(-150, 150)
        time.sleep(0.5)
        test.stop()
        test.start()
    elif cx <= (center_of_frame - turning_error):
        print('Object too far left. Turn left')
        test.move(20, -20)
        time.sleep(0.4)
        test.stop()
        test.start()
        time.sleep(0.2)
        #custom.traverse(w,ideal_width,pixel_error)
    elif cx >= (center_of_frame + turning_error):
        print('Object too far right. Turn right')
        test.move(-20, 20)
        time.sleep(0.4)
        test.stop()
        test.start()
        time.sleep(0.2)
        #custom.traverse(w, ideal_width, pixel_error)
    elif (cx > (center_of_frame - turning_error)) and (cx < (center_of_frame + turning_error)):
        print('Object centered. Do not turn')
        custom.traverse(w, ideal_width, pixel_error)
    else:
        print('ERROR.')

    current_time = time.time() - start_time
    print('Current Time is %s seconds' %(str(current_time)))

    if (current_time > 120):
        break


