import time
import os
from picuspy import interface, motion, data, comms
from picuspy.utils import deadband

# try to initialize plugged-in controller
j = interface.Joystick(freq=15)

# if this script is run on windows
if os.name == 'nt':
    WINDOWS = True
    # use radio for connection
    myAddr = comms.settings.Martius.conn24
    roverAddr = comms.settings.Picus.conn24
# otherwise
else:
    WINDOWS = False
    # use local connection (TODO: test this again)
    myAddr = ('localhost', comms.settings.Martius.conn24[1])
    roverAddr = ('localhost', comms.settings.Picus.conn24[1])

# this print will show your IP address or something, don't remember
#print([l for l in ([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")][:1], [[(s.connect(('8.8.8.8', 53)), s.getsockname()[0], s.close()) for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1]]) if l][0][0])

# keep track of last time sent
last = time.time()
curr = last

# set up communication to rover
rover = comms.UDP_Library( myAddr, roverAddr )


# do all this stuff until 10 seconds of no controller
while(curr - last<10):
    # update time at beginning of loop
    curr = time.time()

    # get joystick data
    tx = j.read()
    
    # if joystick reads/refreshes
    if tx:
        # update timing
        last = curr
        
        # apply deadband on y-axis
        #if abs(tx.y) < 0.024: tx.y = 0.0
        tx.y = utils.deadband(tx.y, 0.024)
        
        # make tx structure into packed string and send it to rover
        rover.sendDAT(tx)

        # if button 9 is pressed
        if tx.b9:
            # send structure (2nd time)
            rover.sendDAT(tx)
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