import matplotlib.pyplot as plt
from numpy import floor
from myplot import myplot
from datastream import datastream

class picusPlot:

    windowCounter = 1

    def __init__(self, xnum, ynum):
        plt.ion()  # set interactive plotting
        self.windowIdx = self.windowCounter
        self.windowCounter += 1
        self.setNumPlots(xnum, ynum)
        self.plots = []  # store data for each stream, and substream
        self.titles = []  # list of subplot titles
        self.xlabels = []
        self.ylabels = []

    def setNumPlots(self,xnum, ynum):
        self.numplots = [xnum, ynum]
        self.figure, self.areas = plt.subplots(xnum, ynum)
        plt.autoscale(False)

    def addPlot(self, subplt):
        self.plots.append(subplt)
        self.titles.append(subplt.title)
        self.xlabels.append(subplt.xlabel)
        self.ylabels.append(subplt.ylabel)

    def open(self):
        return plt.fignum_exists(self.windowIdx)

    def draw(self):
        plt.figure(self.windowIdx)

        for j, subplot in enumerate(self.plots):
            substreams, lims, ylines = subplot.getStreams()

            r = int(floor(j / self.numplots[1]))
            c = j % self.numplots[1]
            try:
                subplotArea = self.areas[r, c]
            except IndexError:
                subplotArea = self.areas[c]
            except TypeError:
                subplotArea = self.areas

            subplotArea.clear()

            for STR in substreams:
                subplotArea.plot(STR.X, STR.Y, STR.style)

            subplotArea.axis(lims)
            if subplot.fixed is True:
                subplotArea.plot([lims[0], lims[1]], [ylines[0], ylines[0]], 'k--')
                subplotArea.plot([lims[0], lims[1]], [ylines[1], ylines[1]], 'k--')

            if subplot.grid is True: subplotArea.grid(True)
            if subplot.title != '': subplotArea.set_title(self.titles[j])
            if subplot.xlabel !='': subplotArea.set_xlabel(self.xlabels[j])
            if subplot.ylabel !='': subplotArea.set_ylabel(self.ylabels[j])

        plt.pause(.001)

if __name__ == "__main__":
    import time
    from stopwatch import *
    import numpy as np

    start = time.time()
    now = start

    Dstream = []
    Dstream.append(datastream('r-'))
    # Dstream.append(datastream('g-.'))
    # Dstream.append(datastream('b--'))

    mysubplot = []
    mysubplot.append(myplot(True, 2, 'constructor title', 'c-x', 'c-y'))
    # mysubplot.append(myplot(True, 1))
    # mysubplot[1].setTitle('set title')
    # mysubplot.append(myplot(True, 2))
    # mysubplot[2].setLabels('list title', 'list x', 'list y')

    [ mysubplot[i].addStream(Dstream[i]) for i in range(len(Dstream)) ]

    plotter = picusPlot(1, 1)
    [ plotter.addPlot(mysubplot[i]) for i in range(len(mysubplot))]

    while plotter.open():
        # print(time.time() - now)
        now = time.time()
        elapsed = now - start
        for idx, STR in enumerate(Dstream):
            STR.add(elapsed, np.exp(-.05*elapsed)*np.sin(6*elapsed + idx))
        plotter.draw()