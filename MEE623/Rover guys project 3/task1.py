from stopwatch import StopWatch
import cv2, sys
from combined import combined
from PID import PID
from mv import *

# goal pixel location of centroid
screenCenter = 330

# goal pixel width of bat
batWidth = 200

# object to retrieve frames
video = cv2.VideoCapture(0)

# stopwatch object for time tracking
watch = StopWatch()

# start serial connection to motors
start()

# setup turning PID
turnpid = PID(0.00056, 0.00, 0.00075, 30, 0.00, 1)
turnpid.setSP(screenCenter)  # centroid x / alignment

# setup distance PID
distpid = PID(0.004, 0.0, 0.0013, 12, 0.01, 1)
distpid.setSP(batWidth)  # width pixels / distance
driveEnable = False

# allow turning pid to calculate and run
turnpid.enable()

# put the loop in a try, so a keyboard stop lets us stop drive motors and camera objects
try:
    # run loop until we stop it
    while(True):
        ret, f = video.read()
        ret2, frame = video.read()
        if ret2 == True:
            wid, jcent = combined(frame)
        else:
            print('Error, no image')

        disterr, drive = distpid.get(wid)
        turnerr, turn = turnpid.get(jcent)

        # wait the first 4 seconds, then enable driving fwd/back
        if watch.lap() > 4.0 and not driveEnable:
            distpid.enable()
            driveEnable = True

        # when we get close to the bat distance target, stop the driving
        if disterr < 30:
            distpid.disable()

        # left and right wheel speeds
        left = 0
        right = 0

        # only drive forward
        drive = drive if drive > 0. else 0.
        drive = drive if jcent > 0. else 0.

        # right and left wheel speeds include distance and turning
        right += turn*255.
        right += (1-abs(turn))*drive*255.
        left += turn*-255.
        left += (1-abs(turn))*drive*255.

        # send speed to wheels
        move(right,left)

        # Print some info
        # print("drive:%1.2f\t\tturn:%1.2f\t\tdisterr:%1.2f\t\ttturnerr:%1.2f" %(drive,turn,disterr,turnerr))

# keyboard interrupt
except KeyboardInterrupt:
    # stop motors
    stop()

    # release video stuff
    video.release()

    print("exited script")

except:
    e = sys.exc_info()[0]
    print(e)

# finally:
#     # stop motors
#     stop()
#
#     # release video stuff
#     video.release()


