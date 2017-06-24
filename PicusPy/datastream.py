# datastream.py
# datastream class handles information for a data set
# Written by Tim Olson - timjolson@user.noreplay.github.com
#
# Example at bottom requires 'numpy' and 'matplotlib'.
# Will generate three graphs of the same data, each with different
# timespan for the x axis.


class datastream:

    # constructor - pass in optional plotting style and label for the stream(~= a line in matlab terms)
    def __init__(self, style='k', label=''):
        # initialize attributes
        self.minx = 0   # minimum x in data set
        self.maxx = 0   # maximum x in data set
        self.miny = 0   # min y in data set
        self.maxy = 0   # max y in data set
        self.xrange=0   # range of x values
        self.yrange=0   # range of y values
        self.style = style  # line plotting style
        self.label = label  # line label on plots

        # store x and y coordinates (each update stores both, pairs stick together)
        self.X = []
        self.Y = []

    # add a datapoint to the stream
    def add(self, x_, y_):
        self.X.append(x_)
        self.Y.append(y_)

    # update the stream to remove data older than the timepsan (ts)
    # and change the display min and max
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
    
        # placed inside a try to prevent empty streams from breaking
        try:
            # update the min and max time
            self.minx = self.X[0]
            self.maxx = self.X[-1]
            
            # update min and max Y values and range
            self.miny = min(self.Y)
            self.maxy = max(self.Y)
        except IndexError:
            #no data points in stream
            self.minx = 0
            self.maxx = 0
        finally:
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
    line1 = datastream('b-.')
    line2 = datastream('k')

    # test adding x and y's for 80 cycles
    for elapsed in range(80):
        # add oscillating and decaying data to (dat)
        line1.add(elapsed, np.exp(-0.1*elapsed)*np.sin(.5*elapsed))
        # add decaying data to (line2)
        line2.add(elapsed, elapsed*2/(elapsed*elapsed+.1))

    # test methods (just making sure they don't break)
    line1.getDataPoints()
    line2.getDataPoints()
    line1.getDataTup()
    line2.getDataTup()

    # plot last 100 datapoints (if X values were timestamps, you'd use seconds)
    line1.update(100)
    line2.update(100)
    plt.plot(line1.X, line1.Y, line1.style)
    plt.plot(line2.X, line2.Y, line2.style)
    plt.title('last 100 data points')
    # show grid
    plt.grid(True)
    # display plot
    plt.show()

    # plot last 70 datapoints
    line1.update(70)
    line2.update(70)
    plt.plot(line1.X, line1.Y, line1.style)
    plt.plot(line2.X, line2.Y, line2.style)
    plt.title('last 70 data points')
    plt.grid(True)
    plt.show()

    # plot last 60 datapoints
    line1.update(60)
    line2.update(60)
    plt.plot(line1.X, line1.Y, line1.style)
    plt.plot(line2.X, line2.Y, line2.style)
    plt.title('last 60 data points')
    plt.grid(True)
    plt.show()

    print('finished')
