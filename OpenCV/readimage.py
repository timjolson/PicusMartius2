import numpy as np
import cv2
from matplotlib import pyplot as plot
from imagefix import bgrfix
import time

def Main():
    try:
        #start = time.time()
        img = cv2.imread('Picus.jpg', 1)    #There are three options for reading in an image:
                                            #1. cv2.IMREAD_COLOR (can pass a 1 instead)
                                            #2. cv2.IMREAD_GRAYSCALE (can pass a 0 instead)
                                            #3. cv2.IMREAD_UNCHANGED (can pass a -1 instead) (includes alpha channel of image)
                                                #Alpha compositing is used to combine an image with a background to appear as
                                                #partially or fully transparent. Tha alpha channel stores the matte information (geometry and such of image) so
                                                #different parts can be distinguished. Google Alpha compositing if you are interested in more

        #Error checking: use print out the arse
        #Print the time it takes to read in the image. Important knowledge for optimizing code
        #print str(time.time() - start)
        #print 'Successful read'    #Print to check if the code made it to this line. Remember, it is an intepreter so the code will run until it fails


        #currenttime = time.time()
        cv2.namedWindow('Read Image', cv2.WINDOW_NORMAL)    #This function is like the figure() one in matlab. It creates the window you
                                                            #use in order to show the image. The cv2.WINDOW_NORMAL is a flag that allows
                                                            #you to resize the window created. The default is cv2.WINDOW_AUTOSIZE which will
                                                            #size the window to your image size (this could be a problem for gigantor images

        #print str(time.time() - currenttime)


        #currenttime = time.time()
        cv2.imshow('Read Image', img)   #Pass in the window name you want to display it in first, then the image to be displayed
                                        #This function alone is not enough to actually display the window. If you run the program with just
                                        #this, the python.exe will stop working and your computer will $#1T itself

        #print str(time.time() - currenttime)


        #currenttime = time.time()
        cv2.waitKey(0) & 0xFF      #Thus the cv2.waitKey() function. The value passed in is the number of milliseconds to wait until going to the next
                            #line of code, waiting for any keyboard input. Passing in a 0 makes it wait indefinitely for a keyboard stroke thus
                            #your window will stay open until you press something. This function is necessary because within it is the code that
                            #actually displays the image from cv2.imshow(). Without it, nothing will show and errors happen

        #print str(time.time() - currenttime)

        cv2.destroyAllWindows() #This function is used to clean up everything once it is finished. Clearing the local memory and all of the windows
                                #prevents dumb things from happening later


        #Convert the read image into grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)    #the cv2.COLOR_BGR2GRAY if a flag that converts to grayscale. There are other flags
                                                        # that can do other conversions such as to HSV format

        #The image could be read as grayscale by changing the flag 1 in the imread to 0
        plot.figure(1)
        plot.subplot(111)
        plot.imshow(gray, cmap = 'gray')
        plot.show()

        #matplotlib contains pyplot which has plot functions similar to MatLab. The imshow and everything works the same way, the only difference
        #is the necessity to have the plot.show() function because then the window actually shows.
        plot.figure(2)
        plot.subplot(121)
        plot.imshow(img);
        plot.xticks([]), plot.yticks([]); #hides the tick values on x and y axes
        plot.title('First Title and Stuff')
        plot.xlabel('X Label and Stuff')

        #Notice how the image looks kind of funky compared to normal. This is because OpenCV reads in an image from the "bottom - up" thus it
        #reads the image in as a BGR image instead of a RGB image. PyPlot plots in RGB so the image needs to be flipped in order to show the image
        #correctly. Create a function to do so. Properly import the function to use it.

        img2 = bgrfix(img);

        plot.subplot(122)
        plot.imshow(img2);
        plot.title('Second Title and Stuff')
        plot.ylabel('Y Label and Stuff')
        plot.show()
    except:
        print 'Error, nothing done worked'

if __name__ == '__main__':
    Main()