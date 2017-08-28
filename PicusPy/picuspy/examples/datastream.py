'''examples/datastream.py
Written by Tim Olson - timjolson@user.noreplay.github.com
Will generate three graphs of the same data, each with different timespan for the x axis.
'''

from picuspy.data import Datastream


# Example generates two datastreams, then updates them for 3 lengths.
# Close each plot to see the next.
# Script will continue running until all plots are closed.
import numpy as np
from matplotlib import pyplot as plt

# set elapsed time
elapsed = 0

# make 2 datastream objects
line1 = Datastream('b-.', 'label1')
line2 = Datastream('k', 'label2')

# test adding x and y's for 80 cycles
for elapsed in range(80):
    # add oscillating and decaying data to (dat)
    line1.add(elapsed, np.exp(-0.1*elapsed)*np.sin(.5*elapsed))
    # add decaying data to (line2)
    line2.add(elapsed, elapsed*2/(elapsed*elapsed+.1))

# plot last 100 datapoints (if X values were timestamps, you'd use seconds)
line1.update(100)
line2.update(100)
plt.plot(line1.X, line1.Y, line1.style, label=line1.label)
plt.plot(line2.X, line2.Y, line2.style, label=line2.label)
plt.title('last 100 data points\nclose for next step')
plt.legend()
# show grid
plt.grid(True)
# display plot
plt.show()

# plot last 70 datapoints
line1.update(70)
line2.update(70)
plt.plot(line1.X, line1.Y, line1.style, label=line1.label)
plt.plot(line2.X, line2.Y, line2.style, label=line2.label)
plt.title('last 70 data points\nclose for next step')
plt.legend()
plt.grid(True)
plt.show()

# plot last 60 datapoints
line1.update(60)
line2.update(60)
plt.plot(line1.X, line1.Y, line1.style, label=line1.label)
plt.plot(line2.X, line2.Y, line2.style, label=line2.label)
plt.title('last 60 data points\nclose to finish')
plt.legend()
plt.grid(True)
plt.show()
