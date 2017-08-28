from picuspy.utils import multi_proc # add_process(), remove_inactive(), multi_loop()
from multiprocessing import active_children

import time # general timing

# store running processes
# format is {function_name: multiprocessing.Process}
processes = dict()

# build list of functions to run with optional arguments
# format is [ (function1, daemon, {arg_name:arg_value, arg_name:arg_value})),
#             (function2, daemon, arg_value)
#             (function3, daemon) ]
funcs_to_run = []

########
# make functions or import them to be run in parallel
def datastream():
    import datastream
def joystick():
    import joystick
def myplot():
    import myplot
def picusplot():
    import picusplot
def PID():
    import PID


if __name__ == '__main__':
    ########
    # add functions, their daemon status, and arguments to funcs_to_run
    funcs_to_run = [
        (datastream, False),
        (joystick, True),
        (myplot, False),
        (picusplot, False),
        (PID, False)
        ]
    
    ########
    # run the processes
    for func in funcs_to_run:
        multi_proc.add_process(processes, func)
    
    ########
    # monitor processes for end, do something also
    while multi_proc.multi_loop(processes):
    #while multi_proc.multi_loop(processes), daemons=['joystick']):
        print('.') # heartbeat
        time.sleep(1)
    
    # after all but daemons have ended
    else:
        ########
        # end any remaining processes
        # in this example, the above WHILE runs until the only
        # remaining processes are daemons or specified in multi_loop
        # as daemons
        multi_proc.cleanup(processes, wait_for_daemons=False)
        
        if len(active_children())>0:
            print(active_children())
            raise Exception('Orphaned processes remain')
            
    