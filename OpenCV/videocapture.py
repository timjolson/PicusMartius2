import numpy as np
import cv2
import time
from binthresh import binthresh


def Main():
    try:
        video = cv2.VideoCapture(0) #Camera number for laptop (back is 0, front is 1, any additions are numerical after)
        retval, frame = video.read()  #Capture frame-by-frame, this first one stores the previous image
        ret, f = video.read()         #Capture frame-by-frame, this is the most recent image.
        cv2.namedWindow('Original Image', cv2.WINDOW_NORMAL)    #Create the window
        cv2.imshow('Original Image', f)                     #Show the window, remember you need cv2.waitKey()

        cv2.imwrite('video_pic.jpg', f) #Save the image into the directory that your python script is in with the given name and extension

        cv2.waitKey(0)                  #Wait for keystroke indefinitely
        video.release()                     #Release the video object
        cv2.destroyAllWindows()


        video2 = cv2.VideoCapture(0)

        while(True):
            start = time.time()
            reval2, frame2 = video2.read()
            ret2, f2 = video2.read()
            print 'Read frame: ' + str(time.time() - start)
            cv2.namedWindow('Video', cv2.WINDOW_NORMAL)
            cv2.imshow('Video', f2)

            gray = cv2.cvtColor(f2, cv2.COLOR_BGR2GRAY)

            cv2.namedWindow('Grayscale', cv2.WINDOW_NORMAL)
            cv2.imshow('Grayscale', gray)

            time1 = time.time()
            threshold = 124
            ret, bi = cv2.threshold(gray, threshold, 255, cv2.THRESH_BINARY)
            print 'OpenCV Function: ' + str(time.time() - time1)

            cv2.namedWindow('Binary 1', cv2.WINDOW_NORMAL)
            cv2.imshow('Binary 1', bi)

            time2 = time.time()
            threshold = 124
            bi2 = binthresh(gray, threshold)
            print 'Looping: ' + str(time.time() - time2)

            cv2.namedWindow('Binary 2', cv2.WINDOW_NORMAL)
            cv2.imshow('Binary 2', bi2)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        video2.release()
        cv2.destroyAllWindows()
    except:
        print 'Error, nothing done worked'

if __name__ == '__main__':
    Main()

