'''picusPlot class combines multiple myplot objects into a single figure/window.
Written by Tim Olson - timjolson@user.noreplay.github.com
Example requires 'numpy', generates and plots 3  of same dataset.
#TODO: replace matplotlib with something faster
'''
import matplotlib.pyplot as plt
from numpy import floor
from myplot import myplot
from datastream import datastream


class picusPlot:
    '''picusPlot class combines multiple myplot objects into a single figure/window.
    Written by Tim Olson, timjolson@user.noreplay.github.com
    Example requires 'numpy', plots 3 of same data set as it generates them.
    '''

    # so each instance knows its window
    windowCounter = 1

    def __init__(self, xnum, ynum):
        '''Constructor - set number of columns and rows
        '''
        # set interactive plotting
        # plt.ion()
        
        # track figure number
        self.windowIdx = self.windowCounter
        self.windowCounter += 1
        
        # set dimensions
        self.setNumPlots(xnum, ynum)
        self.plots = []  # store data for each stream

    def setNumPlots(self,xnum, ynum):
        '''Set number of columns and rows
        '''
        self.numplots = [xnum, ynum]
        self.figure, self.areas = plt.subplots(xnum, ynum)
        plt.autoscale(False)

    def addPlot(self, subplt):
        '''Add a myplot object to the figure
        '''
        self.plots.append(subplt)

    def isOpen(self):
        '''Return whether figure is open
        '''
        return plt.fignum_exists(self.windowIdx)

    def draw(self):
        '''Show the plotted data
        '''
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
            if subplot.title != '': subplotArea.set_title(subplot.title)
            if subplot.xlabel !='': subplotArea.set_xlabel(subplot.xlabel)
            if subplot.ylabel !='': subplotArea.set_ylabel(subplot.ylabel)

        # show figure
        plt.pause(0.0001)

if __name__ == "__main__":
    import time
    import numpy as np

    start = time.time()
    now = start

    # make datastream objects and set styles
    datastream_list = []
    datastream_list.append(datastream('r-'))
    datastream_list.append(datastream('g-.'))
    datastream_list.append(datastream('b--'))
    
    # make myplot objects, set their labels and such
    myplot_list = []
    myplot_list.append(myplot('constructor title', 'c-x', 'c-y', fixed=True, timespan=2))
    myplot_list.append(myplot(fixed=True, timespan=1))
    myplot_list[1].setTitle('set title')
    myplot_list.append(myplot(fixed=True, timespan=2))
    myplot_list[2].setLabels('list title', 'list x', 'list y')
    
    # add a datastream object to each myplot object
    [ myplot_list[i].addStream(datastream_list[i]) for i in range(len(datastream_list)) ]
    
    # make picusPlot object to display multiple myplot objects
    picusPlot_obj = picusPlot(2, 2)
    # add myplot objects to the picusPlot object
    [ picusPlot_obj.addPlot(myplot_list[i]) for i in range(len(myplot_list))]
    
    # loop through frames as they are generated
    while picusPlot_obj.isOpen():
        now = time.time()
        elapsed = now - start
        
        # use elapsed time to calculate new data for each datastream
        for idx, stream in enumerate(datastream_list):
            stream.add(elapsed, np.exp(-.05*elapsed)*np.sin(6*elapsed + idx))
        
        # display newest frame
        picusPlot_obj.draw()
        