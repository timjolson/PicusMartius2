from picuspy.utils import multi_proc # add_process() and remove_inactive()
from multiprocessing import active_children

import time # general timing

# store running processes
# format is {function_name: multiprocessing.Process}
processes = dict()

# build list of functions to run with optional arguments
# format is [ (function1, daemon, arg_dict)),
#             (function2, daemon, arg_value)
#             (function3, daemon) ]
funcs_to_run = []


########
# make functions or import them to be run in parallel

########
# joysticks
def joysticks(vars):
    # import Joystick class directly from module
    from picuspy.interface import Joystick
    
    # initialize joystick, try connecting
    joystick = Joystick(freq=vars['freq'], blocking=vars['blocking'],
        buttons={1:0, 9:8}, axes={'x':0, 'y':1})
    
    start = time.time()
    if 'length' in vars.keys(): # set a length of time to run
        while time.time() - start < vars['length']:
            labels, out, j = [],[], joystick.read()
            # if read() returned a dict
            if j:
                # for each thing in the dict
                for k, v in j.items():
                    labels.append(k) # print key/label
                    # print floats to 2 decimals
                    out.append('{0:.2f}'.format(v) if type(v)==float else v)
                # actually output
                print(labels)
                print(out)
                
                # pressing button 9 exits
                if j[9]:
                    print('button exited script')
                    del j, start, Joystick, joystick
                    break
                    
    else: # length not set, run forever or until button 9 pressed
        while True:
            labels, out, j = [],[], joystick.read()
            if j:
                # for each thing in the dict
                for k, v in j.items():
                    labels.append(k) # print key/label
                    # print floats to 2 decimals
                    out.append('{0:.2f}'.format(v) if type(v)==float else v)
                # actually output
                print(labels)
                print(out)
                
                # pressing button 9 exits
                if j[9]:
                    print('button exited script')
                    del j, start, Joystick, joystick
                    break

########
# heartbeat
def heartbeat(period):
    while True:
        time.sleep(period)
        print('.')

########
# test function
def test_active():
    time.sleep(2.5)


if __name__ == '__main__':
    #######
    # add functions, their daemon status, and arguments to funcs_to_run
    funcs_to_run = [
        (joysticks, False, {'freq':15, 'blocking':False}),
        (heartbeat, True, .2),
        (test_active, False)
        ]
        
    #######
    # NOTE: heartbeat is a daemon, so when test_active is finished,
    # joystick will be limiting process (treated as daemon in multi_loop(ignore=[]))
    
    #######
    # run the processes
    for func in funcs_to_run:
        multi_proc.add_process(processes, func)
    
    #######
    # monitor processes(except daemons and ignored) for end, do something also
    while multi_proc.multi_loop(processes, ignore=['joysticks']):
        print('X') #heartbeat
        time.sleep(1)
    
    # after all but daemons and ignored have ended
    else:
        #######
        # add individual process whenever, up until cleanup()
        multi_proc.add_process(processes, (test_active, False))
        
        
        #######
        # end any remaining processes
        # in this example, the above WHILE runs until the only
        # remaining processes are daemons or specified in multi_loop()
        # to be treated as daemons
        multi_proc.cleanup(processes, wait_for_daemons=False)
        
        if len(active_children())>0:
            print(active_children())
            raise Exception('Orphaned processes remain')
        
        