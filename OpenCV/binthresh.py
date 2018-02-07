import numpy as np
import cv2

def binthresh(image, threshold):
    imagesize = image.shape
    for i in range(0, imagesize[0], 1):
        for j in range(0, imagesize[1], 1):
            # if image.item(i,j) > threshold:
            #     image.itemset((i,j),255)
            # else:
            #     image.itemset((i,j),0)
            if image[i,j] > threshold:
                image[i,j] = 255
            else:
                image[i,j] = 0
    return image