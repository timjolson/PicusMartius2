# PID.py
# PID closed loop library
import time


class PID:
    # control constants
    kP = 0.0    # proportional
    kI = 0.0    # integral
    kD = 0.0    # derivative

    # status
    enabled = False  # is controller enabled
    setpoint = 0.0  # goal for controller to reach
    control = 0.0   # speed/output value (scale +/- 0.0 to 1.0)
    error = 0.0     # current error
    derr = 0.0      # last rate of error change

    # options
    smooth = False      # smooth maintains control value when setpoint is changed, otherwise it is reset to 0.0
    refreshTime = 0.02  # time interval (sec) between new calculations

    # backend
    prevError = 0.0     # previous error
    integral = 0.0      # integral of the error

    # deadband zone size
    db = 0.0

    # constructor, pass in constants, refresh interval, and smoothing option
    def __init__(self, kP, kI, kD, db, refreshTime=0.03, smooth=0):
        self.lastCalc = time.time()
        self.kP = kP
        self.kI = kI/1000.0
        self.kD = kD
        self.db = db
        self.smooth = smooth
        self.refreshTime = refreshTime

    # get pid value, pass in position and goal
    def get(self, position, setpoint=None):
        if setpoint is not None:
            if setpoint != self.setpoint:
                # if we want to reset speed/output with goal change
                if self.smooth != True:
                    self.control = 0.0
                # reset integral and update setpoint
                self.integral = 0.0
                self.setpoint = setpoint

        # update error
        self.error = self.setpoint - position

        # if the controller is enabled
        if self.enabled == 1 or self.enabled is True:
            # get current time
            now = time.time()
            # time since last calculation
            self.dt = now - self.lastCalc

            # if it's time to refresh and something weird has not happened (dt more than .5 second)
            if self.refreshTime < self.dt and self.dt < 0.7:
                # update integral
                self.integral = self.integral + self.error

                # delta error, scale by dt to increase consistency
                self.derr = self.error - self.prevError

                # save error
                self.prevError = self.error

                if abs(self.error) < self.db:
                    # self.error = 0.0
                    self.integral = 0.0
                    self.control = 0.0
                    return self.error, self.control

                # set control speed
                self.control = self.kP * self.error - self.kI * self.integral + self.kD * self.derr

                # check for saturation
                if abs(self.control) > 1.0:
                    # limit control to +/-1
                    self.control = -1.0 if self.control < 0.0 else 1.0

                # keep track of when we did this
                self.lastCalc = now

            # dt is sufficient, but it's been a long time since math
            elif self.dt > self.refreshTime:
                # don't change values that will probably be way wrong, but allow continuation
                self.lastCalc = now

                # save error
                self.prevError = self.error

                print("PID timing error")

            return self.error, self.control

        # controller is disabled
        else:
            return self.error, 0.0

    def disable(self):
        self.control = 0.0
        self.integral = 0.0
        self.prevError = 0.0
        self.enabled = False

    def enable(self):
        self.disable()
        self.enabled = True

    def setGains(self, kP, kI, kD, enable=0):
        self.kP = kP
        self.kI = kI
        self.kD = kD
        self.disable()
        if enable == 1 or enable is True:
            self.enable()
        return self.enabled

    def setSP(self, sp):
        if sp != self.setpoint:
            # if we want to reset speed/output with goal change
            if self.smooth is not True:
                self.control = 0.0
            # reset integral and update setpoint
            self.integral = 0.0
            self.setpoint = sp
