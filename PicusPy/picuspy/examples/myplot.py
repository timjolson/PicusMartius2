'''myplot class takes datastream objects and interactively plots them with pyplot.
Written by Tim Olson - timjolson@user.noreplay.github.com
Example requires 'numpy', will generate curves and live-plot them.
#TODO: replace matplotlib with something faster
'''
from picuspy.data import Datastream
from picuspy.data import Myplot
from matplotlib import pyplot as plt

from numpy import exp, sin, pi
from time import time
start = time()

### create a myplot instance
# there are three ways to set labels
# 1. individually
# myplotobj = Myplot(show=True, timespan=5)
# myplotobj.setTitle('title')
# myplotobj.setXLabel('x here')
# myplotobj.setYLabel('y here')
#
# 2. in the constructor
myplotobj = Myplot('title', 'x here', 'y here',
    timespan=6, show=True, legend=True,
    setY=(-1.5,1.5), showMinMax=True
    )
#
# 3. all at once, not in constructor
# myplotobj.setLabels('title','x here', 'y here')

# create some datastreams, with plot styles and labels
feedbackData = Datastream('r', 'feedback')
positionData = Datastream('g', 'position')
diff = Datastream('k', 'error')

# add data streams to the myplot instance
myplotobj.add_stream(feedbackData)
myplotobj.add_stream(positionData)
myplotobj.add_stream(diff)

# loop through showing data
# notice loop() as the while condition, this clears the plot and stops
# the loop when the window is closed
elapsed = 0
while myplotobj.loop() and elapsed<20:
    # update time
    elapsed = time() - start

    # loop through datastreams in myplotobj, generate and add data
    for idx, STR in enumerate(myplotobj.streams):
        if idx<2:
            STR.add(elapsed, exp(-.05*elapsed)*sin(elapsed + pi/(1+idx)))

    # or add data to a stream specifically
    err = feedbackData.Y[-1] - positionData.Y[-1]
    diff.add(elapsed, err)

    # draw the plot
    myplotobj.show()
else:
    myplotobj.close()
