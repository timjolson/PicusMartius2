'''package includes data storage, processing, and display utilities.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    Classes::
    Datastream: store data points and name for a set of data
    Myplot: plot 1 to many Datastream objects on a single graph/pair of axes
    PicusPlot: plot multiple Datastream objects on multiple graphs in a single window
    
    #TODO: change matplotlib to something faster
'''

class Datastream:
    '''class stores data points (X[], Y[]) and a label for logging or plotting.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    obj = Datastream( line style, line label )
    
    .update(timespan) removes data with X value smaller than 'X[-1] - timespan'.
        X[] = timestamps lets you set timespan = length of data to keep
        X[] = indices lets you set timespan = number of data points to keep
    .add(x,y) adds a data point to the stream
    
    #TODO: add logging
    '''
    
    # constructor - pass in optional plotting style and label for the stream(~= a line in matlab terms)
    def __init__(self, style='k', label=''):
        '''See Datastream doc
        '''
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
    def add(self, x, y):
        '''add a data point
        '''
        self.X.append(x)
        self.Y.append(y)

    def update(self, timespan=10):
        '''update the stream to remove data with x less than 'X[-1] - timepsan'
        and change the display min and max
        '''
        # if X is not empty
        if len(self.X)>1:
            # X is time, so range is (last - first)
            self.xrange = self.X[-1] - self.X[0]

        # when the range of data is bigger than timespan
        while self.xrange > timespan:
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


class Myplot:
    '''class takes datastream objects and interactively plots them with pyplot.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    obj = Myplot(title, x label, y label, fixed, timespan, grid, show,
            show min/max, show legend, hard Y limits)
        *All arguments optional
        title (string): title for graph
        x and y labels (strings): axes labels
        fixed (bool): if y axis limits are fixed (expand to show alltime extremes)
            or dynamically adjust to min/max of currently displayed data
            (limits will always adjust if data is off-screen)
        timespan (float): duration of data to show (x axis range)
        grid (bool): whether to show grid on plot
        show (bool): whether to actually plot the data (object used for storage otherwise)
        showMinMax (bool): draw lines at min and max Y[] values
        legend (bool): show legend
        setY (list or tuple): manually set y limits ; e.g. (-1,2)
    
    #TODO:
    0. replace matplotlib with something faster
    1. test multiple objects at same time
    2. allow pause/play and save of figures
    '''
    
    #class attribute so you can have multiple plots at once (TODO:test that)
    fignum = 1
    
    plt = None
    
    def __init__(self, title='', xlab='', ylab='', fixed=True, timespan=5, grid=False, 
                show=False, showMinMax=False, legend=False, setY=None):
        '''See doc for Myplot
        '''
        # options
        self.fixed = fixed
        self.timespan = timespan
        self.title = title
        self.xlabel = xlab
        self.ylabel = ylab
        self.grid = grid
        self.showMinMax = showMinMax
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
        self.minx = -timespan
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
        if show: self.init_show()
    
    def setY(self, a, b):
        '''Set Y limits of plot. They are fixed to these values, unless new ones are specified or
        data exceeds them (they update to the new extremes).
        '''
        self.setMinY = min(a,b)
        self.setMaxY = max(a,b)
        self.setLims = True
    
    def setLabels(self, title, xlab, ylab):
        '''Set title, x-label, y-label
        '''
        self.setTitle(title)
        self.setXLabel(xlab)
        self.setYLabel(ylab)

    def setTitle(self, title):
        '''Set title
        '''
        self.title = title

    def setXLabel(self, lab):
        '''Set xlabel
        '''
        self.xlabel = lab

    def setYLabel(self, lab):
        '''Set ylabel
        '''
        self.ylabel = lab

    def add_stream(self, strm):
        '''Add a Datastream object to the plot
        '''
        self.streams.append(strm)

    def update_stream(self, idx, x, y):
        '''Add a data point to the Datastream object at index in Myplot
        '''
        self.streams[idx].add(x, y)

    def get_streams(self):
        '''Returns all streams in format:
            ([streams], (min(X), max(X), hard min y, hard max y), (min(Y), max(Y)))
        '''
        # list of datastream points
        self.ls = []

        # set plot min and max's
        self.minx, self.maxx, self.miny, self.maxy = -self.timespan, 0, 0, 0

        # update each stream, track largest and smallest values across all
        # add data to (ls)
        for stream in self.streams:
            stream.update(self.timespan)
            self.maxx = max(stream.maxx, self.maxx)
            self.miny = min(stream.miny, self.miny)
            self.maxy = max(stream.maxy, self.maxy)
            self.ls.append(stream)
        
        # force x axis range
        self.minx = self.maxx - self.timespan
        
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

    def loop(self):
        '''Clears graph, returns is_open() [use as condition for while()]
        '''
        open = self.is_open()
        if open: self.clear()
        return open

    def is_open(self):
        '''Return whether figure is open
        '''
        return self.plt.fignum_exists(self.myfignum)

    def clear(self):
        '''Clear the plot figure
        '''
        try:
            self.plt.clf()
        except:
            pass

    def init_show(self):
        '''Sets up matplotlib to display the figure
        '''
        
        from matplotlib import pyplot as plt
        self.plt = plt
        
        # set current figure
        self.fig = self.plt.figure()
        # get and store the figure number for later
        self.myfignum = self.plt.gcf().number
        # disable autoscale
        self.plt.autoscale(False)
        
        # we have been initialized for showing plot
        self.inited = True

    def show(self):
        '''Displays the figure
        '''
        # get all updated and processed stream info
        substreams, lims, ylines = self.get_streams()

        # set control to the relevant figure, not a rando one
        self.plt.figure(self.myfignum)

        # plot data with legend labels
        if self.legend:
            for STR in substreams:
                self.plt.plot(STR.X, STR.Y, STR.style, label=STR.label)
        # plot data without legend labels
        else:
            for STR in substreams:
                self.plt.plot(STR.X, STR.Y, STR.style)

        # set axis limits
        self.plt.axis(lims)

        # display min/max y value lines
        if self.showMinMax:
            self.plt.plot([lims[0], lims[1]], [ylines[0], ylines[0]], 'k--')
            self.plt.plot([lims[0], lims[1]], [ylines[1], ylines[1]], 'k--')

        # show grid
        if self.grid: self.plt.grid(True)

        # show title and axis labels
        if self.title != '': self.plt.title(self.title)
        if self.xlabel != '': self.plt.xlabel(self.xlabel)
        if self.ylabel != '': self.plt.ylabel(self.ylabel)

        # anchor legend top left
        if self.legend: self.plt.legend(bbox_to_anchor=(0, 1), loc=2, borderaxespad=0.)

        # actually display plot (pass in time to pause the data)
        self.plt.pause(0.0001)

    def close(self):
        '''Close the figure
        #TODO: add try/except to accomodate if figure not open
        '''
        self.plt.close(self.plt.figure(self.myfignum))
        
        
class PicusPlot:
    '''class combines multiple myplot objects into a single figure/window.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    obj = PicusPlot( num of rows, num of columns )
    
    #TODO:
    0. replace matplotlib with something faster
    1. allow pause/play and save of figures
    '''
    
    # so each instance knows its window
    windowCounter = 1
    
    plt = None
    
    def __init__(self, xnum, ynum, subplots=None):
        '''Constructor - set number of columns and rows, pass in list of Myplot objects
        '''
        
        import matplotlib.pyplot as plt
        self.plt = plt
        from numpy import floor
        self.floor = floor
        
        # track figure number
        self.windowIdx = self.windowCounter
        self.windowCounter += 1
        
        # set dimensions
        self.setNumPlots(xnum, ynum)
        if subplots:
            self.plots = subplots
        else:
            self.plots = []  # store data for each stream

    def setNumPlots(self,xnum, ynum):
        '''Set number of columns and rows
        '''
        self.numplots = [xnum, ynum]
        self.figure, self.areas = self.plt.subplots(xnum, ynum)
        self.plt.autoscale(False)

    def addPlot(self, subplt):
        '''Add a myplot object to the figure
        '''
        self.plots.append(subplt)

    def isOpen(self):
        '''Return whether figure is open
        '''
        return self.plt.fignum_exists(self.windowIdx)

    def draw(self):
        '''Show the plotted data
        '''
        self.plt.figure(self.windowIdx)

        for j, subplot in enumerate(self.plots):
            substreams, lims, ylines = subplot.get_streams()

            r = int(self.floor(j / self.numplots[1]))
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
            if subplot.fixed is True and subplot.showMinMax is True:
                subplotArea.plot([lims[0], lims[1]], [ylines[0], ylines[0]], 'k--')
                subplotArea.plot([lims[0], lims[1]], [ylines[1], ylines[1]], 'k--')

            if subplot.grid is True: subplotArea.grid(True)
            if subplot.title != '': subplotArea.set_title(subplot.title)
            if subplot.xlabel !='': subplotArea.set_xlabel(subplot.xlabel)
            if subplot.ylabel !='': subplotArea.set_ylabel(subplot.ylabel)

        # show figure
        self.plt.pause(0.0001)
