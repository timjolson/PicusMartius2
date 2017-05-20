import time
import pygame, os
from picusData import *
from joystick_Library import *
from UDP_Library import *

# try to initialize plugged-in controller
j = joystick_Library(0)
# set it up for use
j.start_Joystick()

# if this script is run on windows
if os.name == 'nt':
    WINDOWS = True
    # use radio for connection
    myAddr = Martius.conn24
    roverAddr = Picus.conn24
# otherwise
else:
    WINDOWS = False
    # use local connection (TODO: test this again)
    myAddr = ('localhost', Martius.conn24[1])
    roverAddr = ('localhost', Picus.conn24[1])

# this print will show your IP address or something, don't remember
#print([l for l in ([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")][:1], [[(s.connect(('8.8.8.8', 53)), s.getsockname()[0], s.close()) for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1]]) if l][0][0])

# set frequency of controller data sending
freq = 10
# keep track of last time sent
last = time.clock()

# set up communication to rover
rover = UDP_Library( myAddr, roverAddr )


# do all this stuff
while(True):
    # update time at beginning of loop
    curr = time.time()

    # if sending period has elapsed
    if curr - last > 1/freq:
        # update last send time
        last = curr

        # get joystick data
        pygame.event.pump()
        tx = j.read_Joystick()

        # if joystick reads properly
        if tx:
            # apply deadband on y-axis
            if abs(tx.y) < 0.024: tx.y = 0.0
            # make tx structure into packed string and send it to rover
            rover.sendDAT(pickle.dumps(tx))

        # if button 9 is pressed
        if tx.b9:
            # send structure (2nd time)
            rover.sendDAT(pickle.dumps(tx))
            # break big while loop
            break

    # if we're running in windows
    if WINDOWS==True:
        try:
            # check for received data and print it
            rx = 0
            rx = rover.receiveUDP()
            if rx!=0: print('rx:%a' %(rx))
        except:
            # not having data is okay
            pass

# return good exit code to system (useful in bash scripts)
exit(0)