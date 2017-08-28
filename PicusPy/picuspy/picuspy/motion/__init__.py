'''package includes PID loop, motor controllers, joint controllers.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    Classes::
    PID: run classic closed loop controller
    
    #TODO: make joint controllers, motor controllers, combine feedback with
    #TODO: filters (e.g. Kalman)
'''

from picuspy.motion.PID import PID
