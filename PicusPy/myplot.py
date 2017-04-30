# myplot class takes datastream objects and
# interactively plots them with pyplot
# display options are set in constructor
from datastream import datastream
from matplotlib import pyplot as plt

class myplot:

    fignum = 1

    def __init__(self, title='', xlab='', ylab='', fixed=True, timespan=5, grid=False, show=False,
                 showMinMax=False, legend=False, setY=None):
        self.fixed = fixed
        self.streams = []
        self.timespan = timespan
        self.title = title
        self.xlabel = xlab
        self.ylabel = ylab
        self.ls = []
        self.markYmin = 0
        self.markYmax = 0
        self.permYmax = 0
        self.permYmin = 0
        self.minx = 0
        self.maxx = 0
        self.miny = 0
        self.maxy = 0
        self.grid = grid
        self.myfignum = self.fignum
        self.fignum += 1
        self.inited = False
        self.legend = legend
        self.setLims = False
        self.showMM = showMinMax
        if type(setY) is not None: self.setY(setY[0], setY[1])
        if show: self.initShow()

    def setY(self, a, b):
        self.setMinY = min(a,b)
        self.setMaxY = max(a,b)
        self.setLims = True

    def setLabels(self, title, xlab, ylab):
        self.title = title
        self.xlabel = xlab
        self.ylabel = ylab

    def setTitle(self, title):
        self.title = title

    def setXLabel(self, lab):
        self.xlabel = lab

    def setYLabel(self, lab):
        self.ylabel = lab

    def addStream(self, strm_):
        self.streams.append(strm_)

    def updateStream(self, idx, x, y=None):
        if y is None and type(x) is tuple:
            self.streams[idx].add(x)
        else:
            self.streams[idx].add(x, y)

    def getStreams(self):
        self.ls = []

        self.minx, self.maxx, self.miny, self.maxy = 0, 0, 0, 0

        for stream in self.streams:
            stream.update(self.timespan)
            self.minx = max(stream.minx, self.minx)
            self.maxx = max(stream.maxx, self.maxx)
            self.miny = min(stream.miny, self.miny)
            self.maxy = max(stream.maxy, self.maxy)
            self.ls.append(stream)

        if self.setLims is False:
            self.permYmin = min(self.miny, self.permYmin)
            self.permYmax = max(self.maxy, self.permYmax)

            if self.fixed:
                return self.ls, (self.minx, self.maxx, self.permYmin, self.permYmax), (self.miny, self.maxy)
            else:
                return self.ls, (self.minx, self.maxx, self.miny, self.maxy), (self.miny, self.maxy)
        else:
            return self.ls, (self.minx, self.maxx, self.setMinY, self.setMaxY), (self.miny, self.maxy)

    def loop(self):
        open = self.isOpen()
        if open: self.clear()
        return open

    def isOpen(self):
        return plt.fignum_exists(self.myfignum)

    def clear(self):
        try:
            plt.clf()
        except:
            pass

    def initShow(self):
            self.fig = plt.figure()
            self.myfignum = plt.gcf().number
            plt.autoscale(False)
            plt.ion()  # set interactive plotting
            self.inited = True

    def show(self):
        substreams, lims, ylines = self.getStreams()

        plt.figure(self.myfignum)
        if self.legend:
            for STR in substreams:
                plt.plot(STR.X, STR.Y, STR.style, label=STR.label)
        else:
            for STR in substreams:
                plt.plot(STR.X, STR.Y, STR.style)

        plt.axis(lims)
        if self.showMM:
            plt.plot([lims[0], lims[1]], [ylines[0], ylines[0]], 'k--')
            plt.plot([lims[0], lims[1]], [ylines[1], ylines[1]], 'k--')
        if self.grid: plt.grid(True)
        plt.title(self.title)
        plt.xlabel(self.xlabel)
        plt.ylabel(self.ylabel)
        if self.legend: plt.legend(bbox_to_anchor=(0, 1), loc=2, borderaxespad=0.)
        plt.pause(0.001)


if __name__ == "__main__":
    import numpy as np
    import time
    start = time.time()

    # create a myplot instance
    # there are three ways to set labels
    # 1. individually
    # dat = myplot(True, 5)
    # dat.setTitle('title')
    # dat.setXLabel('x here')
    # dat.setYLabel('y here')

    # 2. in the constructor
    dat = myplot('title', 'x here', 'y here', timespan=6, show=True, legend=True, setY=(-1.5,1.5))
    # dat.setY(-1,2)

    # 3. all at once, outside constructor
    # dat.setLabels('title','x here', 'y here')

    # create some datastreams, with plot styles
    feedbackData = datastream('r', 'feedback')
    positionData = datastream('g', 'position')
    diff = datastream('k', 'error')

    # add data streams to the myplot instance
    dat.addStream(feedbackData)
    dat.addStream(positionData)
    dat.addStream(diff)

    while dat.loop():
        now = time.time() - start

        # loop through datastreams in dat, do math to generate data
        for idx, STR in enumerate(dat.streams):
            if idx<2:
                STR.add(now, np.exp(-.05*now)*np.sin(now + np.pi/(1+idx)))

        # or add data to a stream separately
        err = feedbackData.Y[-1] - positionData.Y[-1]
        diff.add(now, err)

        # draw the plot
        dat.show()
