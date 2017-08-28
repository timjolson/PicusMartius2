from time import time, sleep
from picuspy.interface import Joystick
from numpy import mod
from picuspy.comms import packets

# this one does not block when controller is disconnected (returns None)
# uses same CPU regardless of connection
joystick = Joystick(
    joyID=0, freq=3, blocking=False,
    reconnect=3, wait=1,
    axes={'x':0, '-y':1, 'w':2, '-z':3}
    )

# this one blocks when controller is disconnected (loops until detected)
# uses almost no CPU w/o connection
# CAUTION: this version will block anything else in the process from ocurring
# joystick = Joystick(joyID=0, freq=5, blocking=True)

start = time()

# make empty dict to store readings
j = dict()

# do things for 30 seconds
while (time() - start <30):
    # read stick data
    j = joystick.read()
    
    # easy printing, choose buttons and such
    print_list = [1, 9, 'z', 'hat']
    
    # used for printing
    labels=[]
    out=[]
    
    # print a heartbeat every second while running
    if mod(time() - start, 1)<.001:
        print('running')
        sleep(0.001) # time.sleep()
    
    # if read() returned a dict
    if j:
        # transmit(ControlStruct(j))
        
        # for each thing in the dict
        for k, v in j.items():
            # if we elected to print it
            if k in print_list:
                # print key/label
                labels.append(k) 
                
                # print floats to 2 decimals
                out.append('{0:.2f}'.format(v) if type(v)==float else v)
        
        # actually output
        print(labels)
        print(out)
        
        # pressing button 9 exits
        if j[9]:
            print('button exited script')
            break