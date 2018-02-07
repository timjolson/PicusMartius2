import numpy as np
import cv2
import time

def bgrfix(image):
    # start = time.time()
    # b,g,r = cv2.split(image)
    # print str(time.time() - start)

    #Notice the difference in time it takes to run this function in the main code with using Numpy format vs. the cv2 function
    time1 = time.time()
    b = image[:,:,0]
    g = image[:,:,1]
    r = image[:,:,2]
    print str(time.time() - time1)

    img2 = cv2.merge([r,g,b])
    return img2