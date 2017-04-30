# PID.py
# PID closed loop library
import time


class PID:

    # constructor, pass in constants, refresh interval, and smoothing option
    def __init__(self, kP, kI, kD, deadband=0, refreshTime=0.03, smooth=True, add=True):
        # status
        self.lastCalc = time.time()
        self.enabled = False  # is controller enabled
        self.setpoint = 0.0  # goal for controller to reach
        self.position = 0.0
        self.control = 0.0  # speed/output value (scale +/- 0.0 to 1.0)
        self.error = 0.0  # current error
        self.derr = 0.0  # last rate of error change

        # gains
        self.kP = kP
        self.kI = kI/1000.0
        self.kD = kD

        # option variables
        self.deadband = deadband                    # deadband zone size
        self.smooth = smooth            # smooth maintains control value when setpoint is changed, otherwise it is reset to 0.0
        self.refreshTime = refreshTime  # time interval (sec) between new calculations
        self.additive = add             # whether control is cumulative or calculated from scratch each time

        # backend variables
        self.integral = 0.0
        self.prevPos = None
        # self.prevError = 0.0
        self.skipDeriv = True

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
        self.position = position
        self.error = self.setpoint - position

        # if the controller is enabled
        if self.enabled == 1 or self.enabled is True:
            # get current time
            now = time.time()
            # time since last calculation
            self.dt = now - self.lastCalc

            # if it's time to refresh and something weird has not happened (dt more than .5 second)
            if self.refreshTime < self.dt and self.dt < 0.5:
                # update integral
                self.integral += self.kI * self.error

                # delta error, scale by dt to increase consistency
                # self.derr = self.error - self.prevError
                self.derr = position - self.prevPos if self.skipDeriv is False else 0.0
                self.derr /= self.dt
                self.skipDeriv = False

                # save error
                # self.prevError = self.error
                self.prevPos = position

                if abs(self.error) < self.deadband:
                    # self.error = 0.0
                    self.integral = 0.0
                    self.control = 0.0
                    return self.error, self.control

                # set control speed
                if self.additive:
                    self.control += self.kP * self.error - self.integral + self.kD * self.derr
                else:
                    self.control = self.kP * self.error - self.integral + self.kD * self.derr

                # check for saturation and limit control to +/-1
                self.control = -1.0 if self.control < -1.0 else ( 1.0 if self.control > 1.0 else self.control)

                # keep track of when we did this
                self.lastCalc = now

            # dt is sufficient, but it's been a long time since math
            elif self.dt > self.refreshTime:
                # don't change values that will probably be way wrong, but allow continuation
                self.lastCalc = now

                # save error
                self.prevError = self.error
                self.prevPos = position

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
        self.skipDeriv = True

    def enable(self, pos_=0.0):
        #self.disable()
        if self.enabled is False:
            self.skipDeriv = True
        self.enabled = True
        self.prevPos = pos_

    def setGains(self, kP, kI, kD, enable=0):
        self.kP = kP
        self.kI = kI
        self.kD = kD
        if enable == 1 or enable is True:
            self.enable()
        else:
            self.disable()

    def setSP(self, sp):
        if sp != self.setpoint:
            # if we want to reset speed/output with goal change
            if self.smooth is not True:
                self.control = 0.0
            # reset integral and update setpoint
                self.integral = 0.0
            self.setpoint = sp


if __name__ == "__main__":
    from myplot import myplot
    from datastream import datastream
    import numpy as np

    errstream = datastream('r','error')
    positionstream = datastream('b','position')
    pidstream = datastream('g','PID')
    goalstream = datastream('k','goal')
    dat = myplot('PID Example', 'time', 'value', fixed=True, timespan=10, show=True, legend=True, grid=True, showMinMax=False)
    dat.setY(-10,10)
    dat.addStream(errstream)
    dat.addStream(positionstream)
    dat.addStream(goalstream)
    dat.addStream(pidstream)

    # slow rise tuning
    thing = PID(0.5, 0.00, 0., deadband=0, refreshTime=0.05, add=True, smooth=True)
    pos = [-5]  # initial position
    thing.enable(pos[0])

    goallist = [-3, 1, 4]
    goalidx = 0

    kplist = [0.2, 0.0150, 0.000]
    kdlist = [0.000, 0.0001, 0.005]
    titlelist = ['slow rise', 'underdamped', 'overshoot', 'well-tuned']
    gainidx = 0


    start = time.time()
    while dat.loop():
        time.sleep(0.01)
        # now = time.time()
        dt = time.time() - start
        # start = now
        goalidx += 1 if np.mod(dt, 6) < 0.045 else 0
        goalidx = np.mod(goalidx, len(goallist))
        goal = goallist[goalidx]

        gainidx += 1 if np.mod(dt, 30) < 0.045 else 0
        gainidx = np.mod(gainidx, len(kplist))

        if np.mod(goalidx,2) == 0:
            thing.setGains(kplist[gainidx], 0.0, kdlist[gainidx], 0)
            thing.enable()
        else:
            thing.setGains(kplist[gainidx], 0.0, kdlist[gainidx], 1)

        thing.setSP(goal)

        E, S = thing.get(pos[len(pos) - 1])

        pos.append(pos[len(pos) - 1] + 0.5 * S)

        goalstream.add(dt, goal)
        errstream.add(dt, E)
        pidstream.add(dt, S)
        positionstream.add(dt, thing.position)

        dat.setTitle('PID Example\n' + titlelist[gainidx])
        dat.show()
