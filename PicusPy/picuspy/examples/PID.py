'''Example use of PID class in motion package
'''
import time
from picuspy.motion import PID

from picuspy.data import Myplot
from picuspy.data import Datastream
from numpy import mod

### example setup options
timePerGoal = 4 # time between goal/setpoint changes
pos = 1       # initial position for 'physical system'
goallist = [-1, 1] # list of setpoints (each gain set will use all goals)
goalidx = 0     # starting setpoint in goallist

# set of gains
kp = 0.1
ki = 0.0
kd = 0.068

# create PID
pidObj = PID(
    kp, ki, kd,   # gains
    deadband=0., refresh_time=0.08, add=True,    # options
    ramp_up=0.2   # ramping limits (time to full speed/ full stop)
    )

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
myplotObj.setY(-2,4)

# add the datastreams to the myplot object (or window/graph)
myplotObj.add_stream(errstream)
myplotObj.add_stream(goalstream)
myplotObj.add_stream(pidstream)
myplotObj.add_stream(positionstream)

# set first goal
goal = goallist[goalidx]
# set first setpoint
pidObj.setSP(goal)
# set first graph title
myplotObj.setTitle('PID Example\nclose to exit')

# enable the PID object (means it will calculate and return control values)
pidObj.enable(pos)

# timing
runStart = time.time()
goalStart = time.time()


## this is where the fun happens
# Myplot.loop() will clear its window each cycle and break the
# while condition when the window is closed
while myplotObj.loop():
    now = time.time()
    
    # let the PID try to reach the setpoint for specified timePerGoal
    # then change to next setpoint
    if (now - goalStart)>timePerGoal:
        # increments index for goal, loops it
        goalidx = mod(goalidx+1, len(goallist))
        # get new setpoint
        goal = goallist[goalidx]
        
        # setSP could be run constantly, but only needs to be run
        # when the setpoint is changed
        # setSP() tells the PID what value to try to reach (setpoint)
        pidObj.setSP(goal)
        
        # update timing
        goalStart = now
    
    # end stepping through goals
    
    
    # error, speed = obj.get(system's last position)
    E, S = pidObj.get(pos)
    
    # 'simulated' system
    # add the control speed to the last position, subtract gravity
    pos += S
    
    ## add data points to the datastream objects
    goalstream.add(now - runStart, goal)
    errstream.add(now - runStart, E)
    # or update the streams thru the myplot object
    myplotObj.update_stream(2, now - runStart, S)
    myplotObj.streams[-1].add(now - runStart, pos)
    
    # display the graph
    myplotObj.show()
    