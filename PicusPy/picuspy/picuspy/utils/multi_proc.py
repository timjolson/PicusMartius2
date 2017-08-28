from multiprocessing import Process, active_children
# https://pymotw.com/2/multiprocessing/basics.html

def add_process(procs, to_add):
    '''Adds function to multi-processing.
    procs = {name: process, ...}
    to_add = (func, daemon)
    '''
    assert type(procs)== dict, 'provide a dictionary of {name:Process}'
    assert type(to_add)== tuple, 'provide a tuple of (function, daemon-bool)'
    
    target = to_add[0] # function
    name = target.__name__ # function name is key in dictionaries
    
    if bool(procs):
        if name in procs.keys():
            print('Process already running, will not replace')
            return None
    
    # if the function has an arguments dict
    try:
        procs[name]= Process(name=name, target=target, args=(to_add[2], ))
    # does not have arguments dict
    except IndexError:
        procs[name]= Process(name=name, target=target)
    
    # store daemon setting
    procs[name].daemon = to_add[1]
    # begin the process
    procs[name].start()
    # log
    print(name + ' at PID ' + str(procs[name].pid) + ' started' + \
        (' as daemon' if to_add[1] else ''))

def remove_inactive(procs):
    '''Returns only active processes from dict procs, in format {name: process}'''
    assert type(procs)== dict().__class__, 'provide a dictionary of {name:Process}'
    
    inactive = [name for name in procs.keys() if procs[name] not in active_children()]
    for p in inactive:
        print(p + ' at PID ' + str(procs[p].pid) + ' ended')
        procs.pop(p)
    
def multi_loop(procs, ignore=[]):
    '''Returns Tuple (procs, status)
    procs (dict) contains all running processes
    status (bool) is False if procs is empty, True otherwise
    '''
    assert type(procs)== dict().__class__, 'provide a dictionary of {name:Process}'
    assert type(ignore)== list, 'provide a list of process name strings to treat as daemons'
    
    remove_inactive(procs)
    _procs = dict(procs)
    
    # don't include daemons in checking
    for k, v in procs.items():
        if v.daemon is True:
            try:
                _procs.pop(k)
            except KeyError:
                pass
    
    # don't include the [daemons] in checking
    if ignore:
        for k in ignore:
            try:
                _procs.pop(k)
            except KeyError:
                pass
    
    return bool(_procs)
    
def cleanup(procs, wait_for_daemons=False):
    assert type(procs)== dict().__class__, 'provide a dictionary of {name:Process}'
    assert type(wait_for_daemons)== type(True), 'wait_for_daemons must be True/False'
    from time import sleep
    
    if not wait_for_daemons:
        # end process, regardless of daemon
        for k, v in procs.items():
            v.terminate()
    
    # wait for all but daemons to end
    for k, v in procs.items():
        v.join()
    
    # allow for logging of process ends
    sleep(0.3)
    remove_inactive(procs)
    