# datastream class handles information for a data set:
# x and y coordinates
# the display min and max
# updates the stream to remove old data outside the requested timespan


class datastream:

    # constructor takes in optional plotting style and label for the stream(~= a line in matlab terms)
    def __init__(self, style_='k', label=''):
        # initialize attributes
        self.minx, self.maxx, self.miny, self.maxy, self.xrange, self.yrange = 0, 0, 0, 0, 0, 0
        self.style = style_
        self.label = label

        # store x and y coordinates (each update stores both, pairs stick together)
        self.X = []
        self.Y = []

    # add a datapoint to the stream
    def add(self, x_, y_):
        self.X.append(x_)
        self.Y.append(y_)

    # update the stream to remove data older than the timepsan (ts) and change the display min and max
    # NOTE: X is assumed to be timestamps
    def update(self, ts=10):
        # if X is not empty
        if len(self.X)>1:
            # X is time, so range is (last - first)
            self.xrange = self.X[-1] - self.X[0]

        # when the range of data is bigger than timespan
        while self.xrange > ts:
            # remove first data point
            self.X = self.X[1:]
            self.Y = self.Y[1:]
            # update the time range
            self.xrange = self.X[-1] - self.X[0]

        # update the min and max time
        self.minx = self.X[0]
        self.maxx = self.X[-1]

        # update min and max Y values and range
        self.miny = min(self.Y)
        self.maxy = max(self.Y)
        self.yrange = self.maxy - self.miny

    # get complete datastream as 2D list
    def getDataPoints(self):
        return [self.X, self.Y]

    # get complete datastream as tuples
    def getDataTup(self):
        return self.X, self.Y

# Example generates two datastreams, then updates them for 3 lengths.
# Close each plot to see the next.
# Script will continue running until all plots are closed.
if __name__ == "__main__":
    import time
    import numpy as np
    from matplotlib import pyplot as plt

    # set elapsed time
    elapsed = 0

    # make 2 datastream objects
    dat = datastream('b-.')
    dat2 = datastream('k')

    # test adding x and y's for 80 cycles
    while elapsed < 80:
        # add oscillating and decaying data to (dat)
        dat.add(elapsed, np.exp(-0.1*elapsed)*np.sin(.5*elapsed))
        # add decaying data to (dat2)
        dat2.add(elapsed, elapsed*2/(elapsed*elapsed+.1))
        elapsed += 1

    # test methods (make sure they don't break)
    dat.getDataPoints()
    dat2.getDataPoints()
    dat.getDataTup()
    dat2.getDataTup()

    # plot last 100 datapoints (if X values were timestamps, you'd use seconds)
    dat.update(100)
    dat2.update(100)
    plt.plot(dat.X, dat.Y, dat.style)
    plt.plot(dat2.X, dat2.Y, dat2.style)
    plt.title('100 data points')
    # show grid
    plt.grid(True)
    # display plot
    plt.show()

    # plot last 70 datapoints
    dat.update(70)
    dat2.update(70)
    plt.plot(dat.X, dat.Y, dat.style)
    plt.plot(dat2.X, dat2.Y, dat2.style)
    plt.title('70 data points')
    plt.grid(True)
    plt.show()

    # plot last 60 datapoints
    dat.update(60)
    dat2.update(60)
    plt.plot(dat.X, dat.Y, dat.style)
    plt.plot(dat2.X, dat2.Y, dat2.style)
    plt.title('60 data points')
    plt.grid(True)
    plt.show()

    print('finished')
