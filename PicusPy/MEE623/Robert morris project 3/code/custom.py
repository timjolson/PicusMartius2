import time
import struct
import cv2
import numpy as np
import os
import test
import takepic

#Define command to go to data folder
def gotodata():
    # Check current working directory.
    retval = os.getcwd()
    print "Current working directory %s" % retval

    # Now change the directory
    os.chdir("..")
    os.chdir(os.getcwd() + "\data")
    retval = os.getcwd()
    print "Directory changed successfully %s" % retval


#Define command to go to code folder
def gotocode():
    # Check current working directory.
    retval = os.getcwd()
    print "Current working directory %s" % retval
    os.chdir("..")
    os.chdir(os.getcwd() + "\code")
    retval = os.getcwd()
    print "Directory changed successfully %s" % retval


#Define a function to process and find information from images
def process_image(i):

    # Define filtering ranges for HSV filters
    lower_filter = np.array([40, 84, 10])
    upper_filter = np.array([50, 128, 255])

    #try
    for j in range(1,2):
        #Import image (used for calibrating)
        #img = cv2.imread('frame%s.jpg' %(i))

        #Import image (used for testing)
        img = i

        #Convert the read image into hsv and threshold
        hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        filtered_img = cv2.inRange(hsv_img, lower_filter, upper_filter)
        mask = cv2.medianBlur(filtered_img,5)

        ret, thresh = cv2.threshold(mask, 127, 255, 0)

        if not np.any(thresh):
            print('No object found, cannot process furthur')
            x = 0
            y = 0
            w = 0
            h = 0
            cx = 0
            cy = 0
            area = 0
            #x, y, w, h, cx, cy, area = 0
        else:
            #print(thresh)
            contours, threshold = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            #_, contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            areas = [cv2.contourArea(c) for c in contours]
            max_index = np.argmax(areas)
            cnt = contours[max_index]
            M = cv2.moments(cnt)
            print('Countours found')

            # Find centroid
            cx = int(M['m10'] / M['m00'])
            cy = int(M['m01'] / M['m00'])

            # Find area of bat
            area = cv2.contourArea(cnt)

            # Find bounding rectangle
            x, y, w, h = cv2.boundingRect(cnt)

        return x, y, w, h, cx, cy, area

    #except:
    #    print 'Error, nothing done worked'


#Move forward or backwards depending on object distance (set motor speeds in here)
def traverse(w,ideal_width,pixel_error):

    if w <= (ideal_width - pixel_error):
        print('Object too far away. Moving forward.')
        if w <= (ideal_width - 3*pixel_error):
            test.move(1000, 1000)
            time.sleep(1)
            test.stop()
            test.start()
            print('Object extremely far. Light it up.')
        elif w <= (ideal_width - 2*pixel_error):
            test.move(420, 420)
            time.sleep(0.5)
            test.stop()
            test.start()
            print('Object very far. Blaze it.')
        else:
            test.move(200, 200)
            time.sleep(0.3)
            test.stop()
            test.start()
            print('Object is far. Medium speed')
    elif w <= (ideal_width - 0.5*pixel_error):
        test.move(100, 100)
        time.sleep(0.4)
        test.stop()
        test.start()
        print('Object almost there. Going slowly')
    elif w >= (ideal_width + pixel_error):
        test.move(-100, -100)
        time.sleep(0.5)
        test.stop()
        test.start()
        print('Object too close. Backing up.')
    else:
        print('Object within optimal distance.')