import sys, time

class PID():
    kP = 0.0
    kI = 0.0
    kD = 0.0

    enabled = False
    smooth = False

    refreshTime = 0.02

    error = 0.0
    prevError = 0.0

    integral = 0.0
    setpoint = 0.0

    control = 0.0

    def __init__(self, kP, kI, kD, refreshTime, smooth):
        self.lastCalc = time.time()
        self.kP = kP
        self.kI = kI/100.0
        self.kD = kD
        self.smooth = smooth
        self.refreshTime = refreshTime

    def Get(self, setpoint, position):
        if self.enabled:
            now = time.time()
            dt = now - self.lastCalc
            self.error = setpoint - position
            if dt > self.refreshTime:
                self.integral = self.integral - self.error

                self.derr = self.error - self.prevError
                self.derr = self.derr/dt
                self.prevError = self.error

                self.control = self.control + self.kP * self.error + self.kI * self.integral + self.kD * self.derr

                if setpoint != self.setpoint:
                    if self.smooth != True:
                        self.control = 0.0
                    self.integral = 0.0
                    self.setpoint = setpoint

                if abs(self.control) > 1.0:
                    self.control = -1 if self.control < 0 else 1

                self.lastCalc = now

            return self.error, self.control
        else:
            return self.error, 0.0

    def Disable(self):
        self.control = 0.0
        self.enabled = False

    def Enable(self):
        self.enabled = True


if __name__ == "__main__":
    import matplotlib.pyplot as plt

    # slow rise tuning
    thing = PID(0.0051, 0.0, 0.005, 0.02, True)
    thing.Enable()

    # start position and storage
    start = time.time()
    pos = [-5] # initial position
    times = [0]
    error = [0]
    speed = [0]
    setpoint = 0 # initial setpoint
    set1 = False # have we changed to first tuning set
    set2 = False # have we changed to second tuning set
    set3 = False

    # loop for 16 seconds, changing tuning and setpoint for three setups
    while time.time() - start < 16:
        dt = time.time()-start
        if dt > 13 and (not set3):
            # best tuning
            thing = PID(0.2, 0.01, .015, .02, True)
            thing.Enable()
            setpoint = -4
            set3 = True
        elif dt > 8 and (not set2):
            # underdamped
            thing = PID(0.003, 0.0, 0.0001, .02, True)
            thing.Enable()
            setpoint = 0
            set2 = True
        elif dt > 5 and (not set1):
            # underdamped
            thing = PID(0.2, 0.01, .005, .02, True)
            thing.Enable()
            setpoint = 6
            set1 = True

        # PID returns error and speed
        E, S = thing.Get(setpoint, pos[len(pos)-1])

        # store error, speed, time for plotting
        error.append(E)
        speed.append(S)
        times.append(dt)

        # update position, increases by 50% speed
        pos.append(pos[len(pos)-1] + 0.5*S)

        # wait reasonable amount of time before function gets called again
        time.sleep(.01)

    # plotting
    plt.plot(times, pos,   'b-', label='position')
    plt.plot(times, error, 'r-', label='error')
    plt.plot(times, speed, 'g-', label='PID')
    plt.plot([min(times), max(times)],[6, 6],'--')
    plt.plot([min(times), max(times)], [-4, -4], 'k--')
    plt.plot([min(times), max(times)], [1,1], 'b--')
    plt.plot([min(times), max(times)], [-1,-1], 'b--')
    plt.xlabel('time (seconds)')
    plt.title('slow rise, underdamped, too much kP vs kD, well-tuned')
    plt.grid(True)
    plt.legend()
    plt.show()

