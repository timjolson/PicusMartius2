'''picusPlot class combines multiple myplot objects into a single figure/window.
Written by Tim Olson - timjolson@user.noreplay.github.com
Example requires 'numpy', generates and plots 3  of same dataset.
#TODO: replace matplotlib with something faster
'''
import matplotlib.pyplot as plt
from numpy import floor
from picuspy.data import Myplot, Datastream, PicusPlot

from time import time
from numpy import exp, sin

start = time()
now = start

# make datastream objects and set styles
datastream_list = []
datastream_list.append(Datastream('r-'))
datastream_list.append(Datastream('g-.'))
datastream_list.append(Datastream('b--'))

# make myplot objects, set their labels and such
myplot_list = []
myplot_list.append(Myplot('constructor title', 'c-x', 'c-y', fixed=True, timespan=2, showMinMax=False))
myplot_list.append(Myplot(fixed=True, timespan=1, showMinMax=True))
myplot_list[1].setTitle('set title')
myplot_list.append(Myplot(fixed=False, timespan=2))
myplot_list[2].setLabels('list title', 'list x', 'list y')

# add a datastream object to each myplot object
[ myplot_list[i].add_stream(datastream_list[i]) for i in range(len(datastream_list)) ]

# make picusPlot object to display multiple myplot objects
picusPlot_obj = PicusPlot(2, 2)
# add myplot objects to the picusPlot object
[ picusPlot_obj.addPlot(myplot_list[i]) for i in range(len(myplot_list))]

# loop through frames as they are generated
while picusPlot_obj.isOpen():
    now = time()
    elapsed = now - start
    
    # use elapsed time to calculate new data for each datastream
    for idx, stream in enumerate(datastream_list):
        stream.add(elapsed, exp(-.05*elapsed)*sin(6*elapsed + idx))
    
    # display newest frame
    picusPlot_obj.draw()
        