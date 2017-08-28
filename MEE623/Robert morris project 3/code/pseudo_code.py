import time
import struct
import cv2
import numpy as np
import os
from matplotlib import pyplot as plot
#import test

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

    try:
    #for j in range(1,2):
        #Import image (used for calibrating)
        img = cv2.imread('frame%s.jpg' %(i))

        #Import image (used for testing)
        #img = i

        #Convert the read image into hsv and threshold
        hsv_img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        filtered_img = cv2.inRange(hsv_img, lower_filter, upper_filter)
        mask = cv2.medianBlur(filtered_img,5)

        show = 1
        if show:
            plot.figure(1)
            plot.subplot(221)
            plot.imshow(img)

            plot.subplot(222)
            plot.imshow(hsv_img)

            plot.subplot(223)
            plot.imshow(filtered_img)

            plot.subplot(224)
            plot.imshow(mask)

            plot.show()

            #Begin scanning of image for information
            cv2.namedWindow('Read Image', cv2.WINDOW_NORMAL)

        ret, thresh = cv2.threshold(mask, 127, 255, 0)
        _, contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        areas = [cv2.contourArea(c) for c in contours]
        max_index = np.argmax(areas)
        cnt = contours[max_index]
        M = cv2.moments(cnt)

        # Find centroid
        cx = int(M['m10'] / M['m00'])
        cy = int(M['m01'] / M['m00'])

        # Find area of bat
        area = cv2.contourArea(cnt)

        # Find bounding rectangle
        x, y, w, h = cv2.boundingRect(cnt)

        if show:
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.imshow('Read Image', img)
            cv2.waitKey()
            cv2.destroyAllWindows()

        return x, y, w, h, cx, cy, area

    except:
        print 'Error, nothing done worked'


#Move forward or backwards depending on object distance (set motor speeds in here)
def traverse(w):
    if w <= (ideal_width - pixel_error):
        print('Object too far away. Moving forward.')
        if w <= (ideal_width - 2*pixel_error):
            test.start()
            test.move(300,300)
            time.sleep(1)
            test.stop()
            print('Object moving away quickly. Motors move at double speed.')
        else:
            test.start()
            test.move(200, 200)
            time.sleep(1)
            test.stop()
    elif w >= (ideal_width + pixel_error):
        print('Object too close. Backing up.')
        test.start()
        test.move(-200, -200)
        time.sleep(1)
        test.stop()
    else:
        print('Object within optimal distance.')


#Execute main code here
#__name__ = __main__
#__name__ = 0

if __name__ == '__main__':
    start_range = 10
    end_range = 28
    ideal_width = 250
    center_of_frame = 320
    pixel_error = 30
    gotodata()

#Process each image in the data folder for calibration
    for i in range(start_range,end_range):
        x, y, w, h, cx, cy, area = process_image(i)

        if not cx: #List is empty
            print('Object not in frame. Spinning to find object')
        elif cx <= (center_of_frame - pixel_error):
            print('Object too far left')
            #traverse(w)
        elif cx >= (center_of_frame + pixel_error):
            print('Object too far right')
            #traverse(w)
        elif (cx > (center_of_frame - pixel_error)) and (cx < (center_of_frame + pixel_error)):
            print('Object centered')
            #traverse(w)
        else:
            print('ERROR.')
    cv2.waitKey(0)