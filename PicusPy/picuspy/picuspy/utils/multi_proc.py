from multiprocessing import Process, active_children
# https://pymotw.com/2/multiprocessing/basics.html

def add_process(procs, locks, to_add):
    '''Adds function to multi-processing.
    procs = {name: Process, ...}
    locks = {name: Lock, ...}
    to_add = (func, daemon, args)
    '''
    assert type(procs)== dict, 'provide a dictionary of {name:Process}'
    assert type(locks)== dict, 'provide a dictionary of {name:Lock}'
    assert type(to_add)== tuple, 'provide a tuple of (function, daemon-bool)'
    
    target = to_add[0] # function
    name = target.__name__ # function name is key in dictionaries
    
    if bool(procs):
        if name in procs.keys():
            with locks['print']:
                print('Process already running, will not replace')
            return None
    
    # if the function has locks and an arguments dict
    try:
        procs[name]= Process(name=name, target=target, args=(locks, to_add[2]))
    
    # does not have arguments dict
    except IndexError:
        # has locks
        try:
            procs[name]= Process(name=name, target=target, args=(to_add[2], ))
        # does not have locks
        except IndexError:
            procs[name]= Process(name=name, target=target)
    
    # store daemon setting
    procs[name].daemon = to_add[1]
    # begin the process
    procs[name].start()
    # log
    with locks['print']:
        print(name + ' at PID ' + str(procs[name].pid) + ' started' + \
            (' as daemon' if to_add[1] else ''))

def remove_inactive(procs, locks):
    '''Returns only active processes from dict procs, in format {name: process}
    procs = {name: Process, ...}
    locks = {name: Lock, ...}
    '''
    assert type(procs)== dict().__class__, 'provide a dictionary of {name:Process}'
    
    inactive = [name for name in procs.keys() if procs[name] not in active_children()]
    with locks['print']:
        for p in inactive:
            print(p + ' at PID ' + str(procs[p].pid) + ' ended')
            procs.pop(p)
    
def multi_loop(procs, locks, ignore=[]):
    '''Returns status bool = False if procs is empty, True otherwise.
    any process name in ignore[] will be treated as a daemon/not considered
    in regards to procs being empty.
    procs = {name: Process, ...}
    locks = {name: Lock, ...}
    ignore = [name1, name2, ...]
    '''
    assert type(procs)== dict().__class__, 'provide a dictionary of {name:Process}'
    assert type(ignore)== list, 'provide a list of process name strings to treat as daemons'
    
    remove_inactive(procs, locks) # remove finished
    _procs = dict(procs) # make a new dict to filter
    
    # filter out daemons
    for k, v in procs.items():
        if v.daemon is True:
            try:
                _procs.pop(k)
            except KeyError:
                pass
    
    # filter out ignored
    if ignore:
        for k in ignore:
            try:
                _procs.pop(k)
            except KeyError:
                pass
    
    # return whether dict is empty or not
    return bool(_procs)
    
def cleanup(procs, locks, wait_for_daemons=False):
    '''terminate()s any remaining processes, with option to wait for daemons 
    to finish or terminate() them immediately.
    procs = {name: Process, ...}
    locks = {name: Lock, ...}
    '''
    assert type(procs)== dict().__class__, 'provide a dictionary of {name:Process}'
    assert type(wait_for_daemons)== type(True), 'wait_for_daemons must be True/False'
    from time import sleep
    
    # end process, regardless of daemon
    if not wait_for_daemons:
        for k, v in procs.items():
            v.terminate()
    
    # wait for all but daemons to end
    for k, v in procs.items():
        v.join()
    
    # allow for logging of process ends
    sleep(0.3)
    remove_inactive(procs, locks)
    