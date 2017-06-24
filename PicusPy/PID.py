# PID.py
# PID closed loop library
# Written by Tim Olson - timjolson@user.noreplay.github.com
# 
# Example at bottom requires 'numpy', and custom modules 'datastream' and 'myplot'.
# Will live-plot a simulated PID that cycles gain tunings.
import time


class PID:

    # constructor - pass in gains, deadband, refresh interval, smoothing option,
    # and whether to accumulate control values
    def __init__(self, kP, kI, kD, deadband=0.0, refreshTime=0.1, smooth=True, add=True):
        # status
        self.lastCalc = time.time()# track the last time PID was calculated
        self.enabled = False    # is controller enabled
        self.setpoint = 0.0     # goal for controller to reach
        self.position = 0.0     # current position of system
        self.control = 0.0      # speed/output value (scale +/- 0.0 to 1.0)
        self.error = 0.0        # current error
        # self.derr = 0.0       # rate of error change (superseded by dpos)
        self.dpos = 0.0         # rate of position change
        
        # gains
        self.kP = kP        # proportional gain
        self.kI = kI/1000.0 # integral gain
        self.kD = kD        # derivative gain
        
        # option variables
        self.deadband = deadband        # deadband zone size (output set to 0 if |error|<deadband
        self.smooth = smooth            # smooth maintains the control value when setpoint is changed, otherwise it is reset to 0.0
        self.refreshTime = refreshTime  # time interval (sec) between new calculations
        self.additive = add             # whether control is cumulative or calculated from scratch each time
        
        # backend variables
        self.integral = 0.0     # store integral term
        # self.prevError = 0.0 # system's previous error (superseded by prevPos)
        self.prevPos = None     # system's previous position
        self.skipDeriv = True   # whether to skip derivative calculation (first cycle after enable() 
                                # or setpoint change could produce instability)
        
    # get pid error and control value, pass in position and optionally setpoint
    # run this all the time and enable() and disable() the obj when needed
    # 
    # error, control = obj.get()
    def get(self, position, setpoint=None):
        ## a setpoint was passed in
        if setpoint is not None:
            # change setpoint
            self.setSP(setpoint)
        
        ## update error
        self.position = position
        self.error = self.setpoint - position

        ## if the controller is enabled
        if self.enabled == 1 or self.enabled is True:
            # get current time
            now = time.time()
            # time since last calculation
            self.dt = now - self.lastCalc
            
            ## if it's time to refresh and something weird has not happened (dt more than 3*refresh seconds)
            if self.refreshTime < self.dt and self.dt < self.refreshTime*3:
                ## update integral
                # adding right into the term removes large jumps on setpoint change
                self.integral += self.kI * self.error
                
                ## delta error
                # self.derr = self.error - self.prevError # superseded by change 
                # in position (dpos), which == change in error but on a physical
                # system is smooth (no jump on setpoint change) if feedback is smooth
                self.dpos =  (self.prevPos if self.skipDeriv is False else 0.0) - position
                self.dpos /= self.dt    # change in position / time = rate
                
                ## save error
                # self.prevError = self.error (superseded by prevPos)
                self.skipDeriv = False  # have a previous position now, derivative exists
                self.prevPos = position # track for derivative next cycle
                
                ## check deadband zone
                if abs(self.error) < self.deadband:
                    self.integral = 0.0
                    self.control = 0.0
                    return self.error, self.control

                ## set control value
                # it's either cumulative or from scratch
                if self.additive is True or self.additive == 1:
                    self.control += self.kP * self.error + self.integral + self.kD * self.dpos
                else:
                    self.control = self.kP * self.error + self.integral + self.kD * self.dpos
                
                ## check for saturation and limit control to +/-1
                self.control = -1.0 if self.control < -1.0 else ( 1.0 if self.control > 1.0 else self.control)
            
            ## if dt is sufficient to refresh, but it's been a long time 
            ## since math (prone to large error or jumps)
            elif self.dt > self.refreshTime*3:
                # don't change values that will probably be way wrong, but allow continuation
                # save error
                self.prevError = self.error
                self.prevPos = position
                print("PID timing error")
            
            ## keep track of when we calculated
            self.lastCalc = now
            
            ## was not time to recalculate and was not long enough for an error
            ## just return the previous values
            return self.error, self.control

        ## controller is disabled
        else:
            # track time so next calculation can avoid error
            self.lastCalc = time.time()
            # return the error regardless of PID running, and a 0 control value
            return self.error, 0.0
    
    # disables calculations and control output
    def disable(self):
        self.control = 0.0
        self.integral = 0.0
        self.prevError = 0.0
        self.enabled = False
        self.skipDeriv = True

    # enables calculations and control output
    # optionally pass in the system's position
    def enable(self, pos_=0.0):
        if self.enabled is False:
            self.skipDeriv = True
        self.enabled = True
        self.prevPos = pos_
    
    # change the calculation gains
    # pass in all three gains
    # optionally pass in whether or not to enable() the object at the same time
    def setGains(self, kP, kI, kD, enable=0):
        self.kP = kP
        self.kI = kI
        self.kD = kD
        if enable == 1 or enable is True:
            self.enable()
        else:
            # if enable not desired, make sure disabled
            self.disable()
    
    # change the object's setpoint or goal
    # pass in the setpoint
    def setSP(self, sp):
        # setpoint is different than the last
        if sp != self.setpoint:
            # if we want to reset speed/output with goal change
            if self.smooth is not True and self.smooth != 1:
                self.control = 0.0
                # reset integral
                self.integral = 0.0
                self.skipDeriv = True
            else:
                self.skipDeriv = False
            
            # update setpoint
            self.setpoint = sp


if __name__ == "__main__":
    from myplot import myplot
    from datastream import datastream
    import numpy as np
    
    ### example setup options
    timePerGoal = 4 # time between goal/setpoint changes
    gravity = 0.0   # initial gravity (changes to 0.5 after full tuning cycle)
    pos = [0]       # initial position for 'physical system'
    goallist = [-2, 2] # list of setpoints (each gain set will use all goals)
    goalidx = 0     # starting setpoint in goallist
    
    # names of gain sets
    titlelist = ['slow rise', 'underdamped', 'overshoot', 'well-tuned']
    # sets of gains
    kplist = [0.02, 0.18, 0.20, 0.20]
    kilist = [0.00, 0.00, 0.00, 0.00]
    kdlist = [0.04, 0.03, 0.06, 0.09]
    gainidx = 0 # choose set of starting gains
    
    # make datastream objects, where we store data points
    errstream = datastream('r--','error')
    pidstream = datastream('g','PID control')
    goalstream = datastream('k--','setpoint')
    positionstream = datastream('b','position')
    
    # make myplot object, this is a window/graph that scrolls with the data
    myplotObj = myplot('PID Example', 'time', 'value', fixed=True, timespan=10,
            show=True, legend=True, grid=True, showMinMax=False)
    # set window y axis limits (optional)
    myplotObj.setY(-4,8)   
    
    ### not really options, the rest is functional
    # add the datastreams to the myplot object (or window/graph)
    myplotObj.addStream(errstream)
    myplotObj.addStream(goalstream)
    myplotObj.addStream(pidstream)
    myplotObj.addStream(positionstream)
    
    # set first goal, gains, setpoint, graph title
    goal = goallist[goalidx]
    pidObj = PID(kplist[gainidx], kilist[gainidx], kdlist[gainidx], deadband=0, refreshTime=0.08, add=True, smooth=True)
    pidObj.setSP(goal)
    myplotObj.setTitle('PID Example\n' + titlelist[gainidx])
    
    # enable the PID object (means it will calculate and return control values)
    pidObj.enable(pos[0])
    
    # timing and counting
    runStart = time.time()
    goalStart = time.time()
    cycleCount = 0
    goalChanges = 0
    
    
    ### this is where the fun happens
    # myplot.loop() will clear its window each cycle and break the while
    # when the window is closed
    while myplotObj.loop():
        now = time.time()
        
        # let the PID try to reach the setpoint for specified timePerGoal
        # then change to next setpoint
        if (now - goalStart)>timePerGoal:
            goalidx = np.mod(goalidx+1, len(goallist)) # increments index for goal, loops it
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
            if np.mod(goalChanges,len(goallist)) == 0:
                gainidx = np.mod(gainidx+1, len(kplist)) # increment and loop gain indexer
                
                # until we've done a complete cycle of all gain sets, 
                # the PID gains are updated without enabling the PID,
                # then enabled right after (just showing both uses)
                if goalChanges < len(kplist):
                    # gains set, PID not enabled
                    pidObj.setGains(kplist[gainidx], kilist[gainidx], kdlist[gainidx], 0)
                    # pidObj.setGains(kplist[gainidx], kilist[gainidx], kdlist[gainidx])
                    
                    # enable calculation
                    pidObj.enable()
                else:
                    # gains set and PID enabled
                    pidObj.setGains(kplist[gainidx], kilist[gainidx], kdlist[gainidx], 1)
                
                # update graph title
                myplotObj.setTitle('PID Example\n' + titlelist[gainidx])
            
            # after a complete cycle, add gravity to the 'simulation'
            if goalChanges >= (len(kplist) * len(goallist)):
                gravity = 0.5
                myplotObj.setTitle('PID Example with gravity\n' + titlelist[gainidx])
        
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
        myplotObj.updateStream(2, now-runStart, S)
        myplotObj.streams[-1].add(now-runStart, pidObj.position)
        
        # display the graph
        myplotObj.show()
