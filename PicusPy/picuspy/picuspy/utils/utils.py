'''Utility functions
'''

def constrain_2(val, less_lim, more_lim):
    '''Constrains a value to between less_lim and more_lim'''
    if val < less_lim:
        return less_lim
    elif val > more_lim:
        return more_lim
    else:
        return val
        
def constrain(val, lim):
    '''Constrains a vlue to between -1*lim and lim
    '''
    return constrain_2(val, -lim, lim)

def constrain_decider(decider, less_lim, more_lim, output):
    '''Compares decider to less_lim and more_lim, to return a *_lim or output.
    
    if decider < less_lim:
        return less_lim
    elif decider > more_lim:
        return more_lim
    else:
        return output
    '''
    if decider < less_lim:
        return less_lim
    elif decider > more_lim:
        return more_lim
    else:
        return output

def rescale(val, in_low, in_high, out_low, out_high, outside='raise'):
    ''' Rescales val in the input range in_low:in_high to the output range
    in_high:out_high.
    
    If result is outside the output range:
    outside='raise' := raise exception (default)
    outside='mid' := return middle of output range
    outside='constrain' := return the limiting value at respective end of output range
    outside='ignore' := return result regardless of range
    outside= value or None := return whatever outside is set to
    '''
    
    # calculate new value
    _ret = ((val - in_low)/(in_high - in_low)) * (out_high - out_low) + out_low
    
    # _ret is outside output range
    if _ret < out_low or _ret > out_high:
        # apply setting
        if outside == 'constrain':
            return constrain_2(_ret, out_low, out_high)
        elif outside == 'mid':
            return (out_high+out_low)/2 # return middle value
        elif outside == 'ignore':
            return _ret
        elif outside == 'raise':
            raise(Exception('Rescaling {} in {}:{} falls outside {}:{}'.format(val,in_low,in_high,out_low,out_high)))
        else: # value provided
            if _ret < out_low or _ret > out_high:
                return outside  # return provided 'outside' value
    
    # _ret is inside output range
    return _ret

def deadband_2(val, lower_cutoff, higher_cutoff, default=0.0):
    if val>lower_cutoff and val<higher_cutoff:
        return default
    else:
        return val

def deadband(val, range_cutoff, default=0.0):
    deadband_2(val, default-range_cutoff, default+range_cutoff)
        
        