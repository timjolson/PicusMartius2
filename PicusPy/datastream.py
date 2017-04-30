# datastream class handles information for a data set:
# x and y coordinates
# the min and max
# updates the stream to remove old data outside the requested timespan

import numpy as np


class datastream:

    def __init__(self, style_='k', label=''):
        self.minx, self.maxx, self.miny, self.maxy, self.xrange, self.yrange = 0, 0, 0, 0, 0, 0
        self.style = style_
        self.X = []
        self.Y = []
        self.label = label

    def add(self, x_, y_):
        self.X.append(x_)
        self.Y.append(y_)

    def update(self, ts=10):
        if len(self.X)>1:
            self.xrange = self.X[-1] - self.X[0]

        while self.xrange > ts:
            self.X = self.X[1:]
            self.Y = self.Y[1:]
            self.xrange = self.X[-1] - self.X[0]

        self.minx = self.X[0]
        self.maxx = self.X[-1]

        self.miny = min(self.Y)
        self.maxy = max(self.Y)
        self.yrange = self.maxy - self.miny

    def getDataPoints(self):
        return [self.X, self.Y]

    def getDataTup(self):
        return self.X, self.Y

if __name__ == "__main__":
    import time
    from matplotlib import pyplot as plt
    elapsed = 0
    dat = datastream('b-.')
    dat2 = datastream('k')

    # test adding x and y's
    while elapsed < 80:
         dat.add(elapsed, np.exp(-0.1*elapsed)*np.sin(.5*elapsed))
         dat2.add(elapsed, elapsed*2/(elapsed*elapsed+.1))
         elapsed += 1

    # test methods
    dat.getDataPoints()
    dat2.getDataPoints()
    dat.getDataTup()
    dat2.getDataTup()

    # plot
    dat.update(100)
    dat2.update(100)
    plt.plot(dat.X, dat.Y, dat.style)
    plt.plot(dat2.X, dat2.Y, dat2.style)
    plt.grid(True)
    plt.show()

    dat.update(70)
    dat2.update(70)
    plt.plot(dat.X, dat.Y, dat.style)
    plt.plot(dat2.X, dat2.Y, dat2.style)
    plt.grid(True)
    plt.show()

    dat.update(60)
    dat2.update(60)
    plt.plot(dat.X, dat.Y, dat.style)
    plt.plot(dat2.X, dat2.Y, dat2.style)
    plt.grid(True)
    plt.show()

    print('finished')
