'''Example use of PID class in motion package
'''
import time
from picuspy.motion import PID

from picuspy.data import Myplot
from picuspy.data import Datastream
from numpy import mod

### example setup options
timePerGoal = 4 # time between goal/setpoint changes
gravity = 0.0   # initial gravity (changes to 0.5 after full tuning cycle)
pos = [0]       # initial position for 'physical system'
goallist = [-2, 2, -2, 2] # list of setpoints (each gain set will use all goals)
goalidx = 0     # starting setpoint in goallist

# names of gain sets
titlelist = ['slow rise', 'underdamped', 'overshoot=underdamped', 'well-tuned']
# sets of gains
kplist = [0.01, 0.20, 0.20, 0.20]
kilist = [0.00, 0.00, 0.00, 0.00]
kdlist = [0.015, 0.035, 0.06, 0.075]
gainidx = 0 # choose set of starting gains

# make datastream objects, where we store data points 
# data can also be directly stored inside a myplot object
errstream = Datastream('r--','error')
pidstream = Datastream('g','PID control')
goalstream = Datastream('k--','setpoint')
positionstream = Datastream('b','position')

# make myplot object, this is a window/graph that scrolls with the data
myplotObj = Myplot('PID Example', 'time', 'value', fixed=True, timespan=7,
        show=True, legend=True, grid=True, showMinMax=False)
# set window y axis limits (optional)
myplotObj.setY(-4,8)

### not really options, the rest is functional
# add the datastreams to the myplot object (or window/graph)
myplotObj.add_stream(errstream)
myplotObj.add_stream(goalstream)
myplotObj.add_stream(pidstream)
myplotObj.add_stream(positionstream)

# set first goal
goal = goallist[goalidx]
# set first gains
pidObj = PID(
    kplist[gainidx], kilist[gainidx], kdlist[gainidx],  # gains
    deadband=0, refresh_time=0.08, add=True,             # options
    ramp_up=0.4, ramp_down=0.3                            # ramping limits (time to full speed/ full stop)
    )
# set first setpoint
pidObj.setSP(goal)
# set first graph title
myplotObj.setTitle('PID Example\n' + titlelist[gainidx])

# enable the PID object (means it will calculate and return control values)
pidObj.enable(pos[0])

## timing and counting
runStart = time.time()
goalStart = time.time()
cycleCount = 0
goalChanges = 0


## this is where the fun happens
# myplot.loop() will clear its window each cycle and break the
# while condition when the window is closed
while myplotObj.loop():
    now = time.time()
    
    # let the PID try to reach the setpoint for specified timePerGoal
    # then change to next setpoint
    if (now - goalStart)>timePerGoal:
        goalidx = mod(goalidx+1, len(goallist)) # increments index for goal, loops it
        goal = goallist[goalidx] # get new setpoint
        
        # setSP could be run constantly, but only needs to be run
        # when the setpoint is changed
        # setSP() tells the PID what value to try to reach (setpoint)
        pidObj.setSP(goal)
        
        # update timing
        goalStart = now
        # count the things we've done
        goalChanges += 1
        
        # when we change back to the first setpoint, increment the active gain set
        if mod(goalChanges,len(goallist)) == 0:
            gainidx = mod(gainidx+1, len(kplist)) # increment and loop gain indexer
            
            # after we've done a complete cycle of all gain sets, 
            # the PID gains are updated without enabling the PID,
            # then enabled right after (just showing both uses)
            if goalChanges < len(kplist):
                # gains set, PID not enabled
                pidObj.setGains(kplist[gainidx], kilist[gainidx], kdlist[gainidx], enable=True)                    
            else:
                # gains set and PID not enabled
                pidObj.setGains(kplist[gainidx], kilist[gainidx], kdlist[gainidx], enable=False)
                # pidObj.setGains(kplist[gainidx], kilist[gainidx], kdlist[gainidx])
                # enable calculation
                pidObj.enable()
            
            # update graph title
            myplotObj.setTitle('PID Example\n' + titlelist[gainidx])
        
        # after a complete cycle, add gravity to the 'simulation'
        if goalChanges >= (len(kplist) * len(goallist)):
            gravity = 0.5
            myplotObj.setTitle('PID Example with gravity\n' + titlelist[gainidx])
    
    # end stepping through goals and settings
    
    
    # error, speed = obj.get(system's last position)
    E, S = pidObj.get(pos[-1])
    
    # 'simulated' system
    # add the control speed to the last position, subtract gravity
    pos.append(pos[-1] + S - gravity)
    
    ## add data points to the datastream objects
    # datastream.add( x, y ) or datastream.add( (x, y) )
    goalstream.add(now - runStart, goal)
    errstream.add(now - runStart, E)
    # or update the streams thru the myplot object
    myplotObj.update_stream(2, now-runStart, S)
    myplotObj.streams[-1].add(now-runStart, pidObj.position)
    
    # display the graph
    myplotObj.show()
