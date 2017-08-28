from time import time
from ..utils import *

class PID:
    '''applies the standard PID controller, 
    with a few adjustable options and increased functionality.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    obj = PID(kP, kI, kD, deadband, refresh_time, add, ramp_up, ramp_down, antiwind)
        kP (float): proportional gain
        kI (float): integral gain
        kD (float): derivative gain
        deadband (float): range around zero error where returned control value is set to zero
        refresh_time (float): time in seconds between recalculations
        add (bool): whether to add control values over time or start from zero for each calculation
        ramp_up (float): seconds to go from stopped to full speed
        ramp_down (float): seconds to go from full speed to stopped
        antiwind (flaot): saturation value for integral
    '''
    
    def __init__(self,
        kP, kI, kD,
        deadband=0.0, refresh_time=0.1, add=True,
        ramp_up=0.2, ramp_down=None, antiwind=None
        ):
        '''See PID doc
        '''
        # status
        self.lastCalc = time()# track the last time PID was calculated
        self.enabled = False    # is controller enabled
        self.setpoint = 0.0     # goal for controller to reach
        self.position = 0.0     # current position of system
        self.control = 0.0      # speed/output value (scale +/- 0.0 to 1.0)
        self.error = 0.0        # current error
        self.dpos = 0.0         # rate of position change
        
        # handle ramping up
        if ramp_up == None or ramp_up == 0.0: # set to no ramping limit
            self.ramp_up = 0.2            # 200 ms to full speed
        else:   # value specified
            self.ramp_up = abs(ramp_up) # max acceleration (seconds to ramp up to full speed)
        
        #handle ramping down
        if ramp_down is None or ramp_down == 0.0: # set to no ramping limit
            self.ramp_down = 0.5*self.ramp_up
        else:   # value specified
            self.ramp_down = abs(ramp_down)       # max deceleration (seconds to ramp fully down)
        
        # gains
        self.kP = kP        # proportional gain
        self.kI = kI/10.0   # integral gain
        self.kD = kD        # derivative gain
        
        # anti-windup limit for integral term
        self.antiwind = 1/max(abs(self.kI), 1e-8) if antiwind is None else antiwind
        
        # option variables
        self.deadband = deadband        # deadband zone size (output set to 0 if |error|<deadband
        self.refresh_time = refresh_time  # time interval (sec) between new calculations
        self.additive = add             # whether control is cumulative or calculated from scratch each time
        
        # backend variables
        self.integral = 0.0     # store integral term
        self.prevPos = None     # system's previous position
        self.skipDeriv = True   # whether to skip derivative calculation (first cycle after enable() 
                                # or setpoint change could produce instability)
        
    def get(self, position, setpoint=None):
        '''Get pid error and control value, pass in position and optionally setpoint.
        Run this all the time/short intervals and run enable() and disable() when needed.
        error, control = obj.get()
        '''
        
        ## a setpoint was passed in
        if setpoint is not None:
            # change setpoint
            self.setSP(setpoint)
        
        ## update error
        self.position = position
        self.error = self.setpoint - position

        ## if the controller is enabled
        if self.enabled is True:
            # get current time
            now = time()
            # time since last calculation
            self.dt = now - self.lastCalc
            
            ## if it's time to refresh and something weird has not happened (dt more than 3*refresh seconds)
            if self.refresh_time < self.dt and self.dt < self.refresh_time*4:
                ## update integral
                # adding right into the term removes large jumps on setpoint change
                self.integral += self.kI * self.error * self.dt
                self.integral = utils.constrain(self.integral, self.antiwind)
                
                ## delta error
                # change in position (dpos), which == change in error on 
                # a real physical system with 'continuous' feedback
                self.dpos =  (self.prevPos if self.skipDeriv is False else 0.0) - position
                self.dpos /= self.dt    # change in position / time = rate
                
                self.skipDeriv = False  # have a previous position now, derivative exists
                self.prevPos = position # track for derivative next cycle
                
                ## check deadband zone
                if abs(self.error) < self.deadband:
                    self.integral = 0.0
                    self.control = 0.0
                    
                    ## keep track of when we calculated
                    self.lastCalc = now
                    
                    return self.error, self.control
                
                ## limit ramping up and down
                # get ramp acceleration limit
                RU = self.dt/self.ramp_up
                RD = self.dt/self.ramp_down
                # calculate new control value
                new_control = self.kP * self.error + self.integral + self.kD * self.dpos
                # if not additive control, the change from last control is acceleration
                accel = new_control if self.additive is True else new_control - self.control
                
                ## apply ramping limits
                # when close to stopped (<3%), use ramp_up either direction
                if abs(self.control)<0.03:
                    temp = utils.constrain_decider(accel, -RU, RU, new_control)
                # when going forward, use ramp_down if new_control < 0 == decelerating
                elif self.control>0:
                    temp = utils.constrain_decider(accel, -RD, RU, new_control)
                # when going reverse, use ramp_down if new_control > 0 == decelerating
                else: # self.control<0:
                    temp = utils.constrain_decider(accel, -RU, RD, new_control)
                
                ## set control value
                # it's either cumulative or from scratch
                if self.additive is True:
                    self.control += temp
                else:
                    self.control = temp
                
                ## check for saturation and limit control to +/-1
                self.control = utils.constrain(self.control, 1.0)
            
            ## if dt is sufficient to refresh, but it's been a long time 
            ## since math (prone to large error or jumps)
            elif self.dt > self.refresh_time*3:
                # don't change values that will probably be way wrong, but allow continuation
                # save error
                # self.prevError = self.error
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
            self.lastCalc = time()
            # return the error regardless of PID running, and a 0 control value
            return self.error, 0.0
    
    def disable(self):
        '''Disables calculations and sets control output to 0.0
        '''
        self.control = 0.0
        self.integral = 0.0
        self.enabled = False
        self.skipDeriv = True

    def enable(self, pos=0.0):
        '''Enables calculations and control output.
        Optionally pass in the system's position.
        '''
        if self.enabled is False:
            self.skipDeriv = True
        self.enabled = True
        self.prevPos = pos
    
    def setGains(self, kP, kI, kD, enable=False, antiwind=None):
        '''Set the calculation gains.
        Pass in all three gains.
        Whether or not to enable() the object at the same time 
            Default disable()'s the object.
        A new anti-windup saturation value.
        '''
        self.kP = kP
        self.kI = kI/10.0
        self.kD = kD
        self.antiwind = 1/max(self.kI, 1e-8) if antiwind is None else antiwind
        
        if enable is True: # enable the controller
            self.enable()
        else: # if enable not desired, make sure disabled
            self.disable()
    
    def setSP(self, sp):
        '''Set the object's setpoint or goal.
        '''
        # setpoint is different than the last
        if sp != self.setpoint:
            self.skipDeriv = True
            # update setpoint
            self.setpoint = sp
