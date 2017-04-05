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
    db = 15

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


if __name__ == "__main__":
    import matplotlib.pyplot as plt
    from stopwatch import StopWatch
    # slow rise tuning
    thing = PID(0.00001, 0.00, 0.05, 0, 0.05)
    thing.enable()
    thing.setSP(0)

    # start position and storage
    start = time.time()
    pos = [-5]  # initial position
    times = [0]
    error = [0]
    speed = [0]
    setpoint = 0  # initial setpoint
    set1 = False  # have we changed to first tuning set
    set2 = False  # have we changed to second tuning set
    set3 = False  # have we changed to third tuning set
    set4 = False  # have we changed to third tuning set
    set5 = False  # have we changed to third tuning set
    set6 = False  # have we changed to third tuning set
    set7 = False

    t = StopWatch()
    dt = 0
    # loop for 20 seconds, changing tuning and setpoint for four setups
    while dt < 21:
        dt = time.time()-start
        if dt > 19 and (not set6):
            thing.setSP(0)
            set6 = True
        elif dt > 16 and (not set5):
            thing.setSP(-3)
            set5 = True
        elif dt > 13 and (not set4):
            # good tuning
            thing.setGains(0.15, 0.00, .4, 0, 1)
            thing.setSP(1)
            set4 = True
        elif dt > 8 and (not set3):
            # oscillating, way underdamped
            thing.setGains(0.08, 0.0, 0.01, 0)
            thing.enable()
            thing.setSP(-2)
            set3 = True
        elif dt > 6 and (not set2):
            thing.setSP(3)
            set2 = True
        elif dt > 4 and (not set1):
            # underdamped
            thing.setGains(0.05, 0.00, .15, 0, 0)
            thing.enable()
            thing.setSP(6)
            set1 = True

        # if not set7 and t.lap() > 1:
        #     time.sleep(0.5)
        #     set7 = True

        # PID returns error and speed
        E, S = thing.get(pos[len(pos)-1])

        # store error, speed, time for plotting
        error.append(E)
        speed.append(S)
        times.append(dt)

        # update position, increases by 50% speed
        pos.append(pos[len(pos)-1] + 0.5*S)

        # wait reasonable amount of time before function gets called again
        time.sleep(.005)

    # plotting
    plt.plot(times, pos,   'b-', label='position')
    plt.plot(times, error, 'r-', label='error')
    plt.plot(times, speed, 'g-', label='PID')
    plt.plot([min(times), max(times)],[6, 6],'k--',label='goal')
    plt.plot([min(times), max(times)], [-3, -3], 'k--')
    plt.plot([min(times), max(times)], [3, 3], 'k--')
    plt.plot([min(times), max(times)], [1,1], 'b--',label='goal, PID max')
    plt.plot([min(times), max(times)], [-1, -1], 'b--',label='PID min')
    plt.plot([min(times), max(times)], [-2,-2], 'b--')
    plt.plot([min(times), max(times)], [0, 0], 'g--',label='zero')
    plt.xlabel('time (seconds)')
    plt.title('slow rise, underdamped, way underdamped, well-tuned')
    plt.grid(True)
    plt.legend()
    plt.show()

