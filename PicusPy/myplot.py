# myplot class takes datastream objects and
# interactively plots them with pyplot.
# Display options are set in constructor.

# To better understand the options, play with the example

from datastream import datastream
from matplotlib import pyplot as plt

class myplot:

    # class attribute so you can have multiple plots at once (TODO:test that)
    fignum = 1

    # constructor
    # arguments:
    # title
    # x label
    # y label
    # fixed - if vertical limits are fixed or dynamically adjust to min/max of data
    #         (limits will always adjust if data is off-screen)
    # timespan - duration of data to show
    # grid - whether to show grid on plot
    # show - whether to actually plot the data (object used for storage otherwise)
    # showMinMax - draw lines at min and max data values
    # legend - show legend
    # setY - manually set y limits (pass in list)
    def __init__(self, title='', xlab='', ylab='', fixed=True, timespan=5, grid=False, show=False,
                 showMinMax=False, legend=False, setY=None):
        # options
        self.fixed = fixed
        self.timespan = timespan
        self.title = title
        self.xlabel = xlab
        self.ylabel = ylab
        self.grid = grid
        self.showMM = showMinMax
        self.legend = legend

        # list of
        self.ls = []
        # list of datastreams
        self.streams = []

        # y value of min and max lines
        self.markYmin = 0
        self.markYmax = 0

        # y value of display min and max
        self.permYmax = 0
        self.permYmin = 0

        # min and max x and y values for the figure
        self.minx = 0
        self.maxx = 0
        self.miny = 0
        self.maxy = 0

        # keep my figure number
        self.myfignum = self.fignum
        self.fignum += 1

        # by default, limits are not manually set
        self.setLims = False

        # manual y limits
        if setY: self.setY(setY[0], setY[1])

        # if we show the plot, it needs to be initialized for that
        self.inited = False
        # show plot
        if show: self.initShow()

    # set y limits manually
    def setY(self, a, b):
        self.setMinY = min(a,b)
        self.setMaxY = max(a,b)
        self.setLims = True

    # set  title and labels
    def setLabels(self, title, xlab, ylab):
        self.title = title
        self.xlabel = xlab
        self.ylabel = ylab

    # set title
    def setTitle(self, title):
        self.title = title

    # set x label
    def setXLabel(self, lab):
        self.xlabel = lab

    # set y label
    def setYLabel(self, lab):
        self.ylabel = lab

    # add datastream object to the plot
    def addStream(self, strm_):
        self.streams.append(strm_)

    # update data stream at an index with x and y values
    # pass in as tuple or two values
    def updateStream(self, idx, x, y=None):
        if y is None and type(x) is tuple:
            self.streams[idx].add(x)
        else:
            self.streams[idx].add(x, y)

    # get the streams after updating and processing them
    def getStreams(self):
        # list of datastream points
        self.ls = []

        # set plot min and max's
        self.minx, self.maxx, self.miny, self.maxy = 0, 0, 0, 0

        # update each stream, track largest and smallest values across all
        # add data to (ls)
        for stream in self.streams:
            stream.update(self.timespan)
            self.minx = max(stream.minx, self.minx)
            self.maxx = max(stream.maxx, self.maxx)
            self.miny = min(stream.miny, self.miny)
            self.maxy = max(stream.maxy, self.maxy)
            self.ls.append(stream)

        # if limits are automatic
        if self.setLims is False:
            # set display limits by largest/smallest ever data
            self.permYmin = min(self.miny, self.permYmin)
            self.permYmax = max(self.maxy, self.permYmax)

            # if display limits are fixed
            if self.fixed:
                # return data, plot limits, min and max y's
                return self.ls, (self.minx, self.maxx, self.permYmin, self.permYmax), (self.miny, self.maxy)
            else:
                # return data, plot limits, min and max y's
                return self.ls, (self.minx, self.maxx, self.miny, self.maxy), (self.miny, self.maxy)
        # limits are manual
        else:
            # return data, plot limits, min and max y's
            return self.ls, (self.minx, self.maxx, self.setMinY, self.setMaxY), (self.miny, self.maxy)

    # use loop() as condition in a while() to:
    # 1. clear the plot each call
    # 2. break the while loop when plot is closed
    def loop(self):
        open = self.isOpen()
        if open: self.clear()
        return open

    # return if plot is open
    def isOpen(self):
        return plt.fignum_exists(self.myfignum)

    # clear the plot
    def clear(self):
        try:
            plt.clf()
        except:
            pass

    # set up to actually show the plot
    def initShow(self):
        # set current figure
        self.fig = plt.figure()
        # get and store the figure number for later
        self.myfignum = plt.gcf().number
        # disable autoscale
        plt.autoscale(False)
        # set plotting interactive (updates as it goes)
        plt.ion()  # set interactive plotting
        # we have been initialized for showing plot
        self.inited = True

    # show the plotted data
    def show(self):
        # get all updated and processed stream info
        substreams, lims, ylines = self.getStreams()

        # set control to the relevant figure, not a rando one
        plt.figure(self.myfignum)

        # plot data with legend labels
        if self.legend:
            for STR in substreams:
                plt.plot(STR.X, STR.Y, STR.style, label=STR.label)
        # plot data without legend lables
        else:
            for STR in substreams:
                plt.plot(STR.X, STR.Y, STR.style)

        # set axis limits
        plt.axis(lims)

        # display min/max y value lines
        if self.showMM:
            plt.plot([lims[0], lims[1]], [ylines[0], ylines[0]], 'k--')
            plt.plot([lims[0], lims[1]], [ylines[1], ylines[1]], 'k--')

        # show grid
        if self.grid: plt.grid(True)

        # show title and axis labels
        plt.title(self.title)
        plt.xlabel(self.xlabel)
        plt.ylabel(self.ylabel)

        # anchor legend top left
        if self.legend: plt.legend(bbox_to_anchor=(0, 1), loc=2, borderaxespad=0.)

        # actually display plot (pass in time to pause the data)
        plt.pause(0.001)


# example
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

    # create some datastreams, with plot styles and labels
    feedbackData = datastream('r', 'feedback')
    positionData = datastream('g', 'position')
    diff = datastream('k', 'error')

    # add data streams to the myplot instance
    dat.addStream(feedbackData)
    dat.addStream(positionData)
    dat.addStream(diff)

    # loop through showing data
    # notice loop() as the while condition, this clears the plot and stops the loop when plot is closed
    while dat.loop():
        # update time
        now = time.time() - start

        # loop through datastreams in dat, generate and add data
        for idx, STR in enumerate(dat.streams):
            if idx<2:
                STR.add(now, np.exp(-.05*now)*np.sin(now + np.pi/(1+idx)))

        # or add data to a stream specifically
        err = feedbackData.Y[-1] - positionData.Y[-1]
        diff.add(now, err)

        # draw the plot
        dat.show()
