from pygame import joystick as pj
from pygame import event, init, error
from time import time, sleep

class Joystick():
    '''class uses pygame to monitor and return status of various USB controllers via a dictionary. 
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    obj = Joystick(joyID, freq, blocking, reconnect, wait,
                buttons={name:index, name:index},
                axes={name:index, name:index})
        
        joyID (int): set its ID
        freq (float): set reading frequency (Hz)
        blocking (bool): whether a disconnection blocks the process
        reconnect (float): time between reconnect attempts
        wait (float): time to wait between reconnect attempts
        
        buttons, axes (dict): dictionaries mapping a button or axis index (value)
            to a friendly name (key).
            
            add '-' before an axis label to reverse it
            {'-x':0} assigns 'x' as key for axis 0, and reverses stick direction
            
            'hat' key's value is tuple (x, y) where x is -1/0/1, y is -1/0/1.
        
    .read() If blocking, will loop infinitely until a joystick is connected, returns updated dictionary.
        Without blocking, returns updated dictionary at reading frequency, None otherwise or when disconnected.
        
    ._connect() quits and re-inits pygame.joystick. returns whether connection is successful
    
    #TODO: 
    0. change disconnect/reconnect to accomodate multiple controllers
    1. test multiple controllers
    2. build in assertions for legit dictionary associations
    '''
    
    def __init__(self, joyID=0, freq=10, blocking=True,
                reconnect = 3, wait = 1,
                buttons={i+1:i for i in range(12)},
                axes={'x':0, '-y':1, 'w':2, '-z':3}
                ):
        '''See Joystick doc
        '''
        
        # start pygame to handle joysticks 
        init() # pygame.init()
        
        self.axes = axes  # store axis associations
        self.buttons = buttons  # store button associations
        self.control = dict() # store return values
        
        self.joyID = joyID  # store joystick ID within pygame for future use
        self.J = None       # this instance's pygame joystick object
        
        # reading and connecting
        self.refresh = 1/freq   # read period
        self.freq = freq        # read frequency
        self.last_read = time() # last time controller read
        self.last_connect = time() # last time reconnected
        self.blocking = blocking # T/F block process on disconnect
        self.reconnect = reconnect # reconnect period
        self.wait = wait        # time between reconnect attempts
        
        # try to connect to joystick
        # wait between attempts
        while not self._connect():
            if not self.blocking:
                self.stat = 'invalid'
                break
            sleep(self.wait) # time.sleep()
        
        if self.stat == 'valid':
            print("Joystick " + str(self.joyID) + " initialized")       # print out ID
            print("Number of Buttons " + str(self.J.get_numbuttons()) +
                ", Number of Axes " + str(self.J.get_numaxes()))        # print stick info
        
    def read(self):
        '''reads joystick and returns a ControlStruct() containing the values 
        at the reading frequency, returns None on other calls
        '''
        
        # every few seconds re-establish joystick
        if time() - self.last_connect > self.reconnect:
            self.last_connect = time()
            
            # wait between attempts
            while not self._connect():
                if not self.blocking:
                    self.stat = 'invalid'
                    break
                sleep(self.wait) # time.sleep()
        
        if self.stat == 'valid':
            # if reading period has passed
            if (time() - self.last_read) > self.refresh:
                self.last_read = time()
                
                # update pygame controllers, etc
                event.pump() # pygame.event
                
                # empty the dict
                self.control = dict.fromkeys(self.control.keys(), 0)
                
                # for buttons associated in __init__
                for k, v in self.buttons.items():
                    # read the button, add key and value to dict
                    self.control.update({k: self.J.get_button(v)})
                    
                for k, v in self.axes.items():
                    if k.startswith('-'):
                        self.control.update({k[1:]: -1*self.J.get_axis(v)})
                    else:
                        self.control.update({k: self.J.get_axis(v)})
                
                #self.control.update({'hatx': self.J.get_hat(0)[0], 'haty': self.J.get_hat(0)[1]})
                self.control.update({'hat': self.J.get_hat(0)})
                
                # return dict() of values
                return self.control
            
            # not time to read
            return None
        
        # invalid / disconnected
        else:
            return None
        
    def _connect(self):
        '''Re-connect to joystick by: quit and re-init pygame.joystick
        '''
        pj.quit()
        pj.init()
        try:
            self.J = pj.Joystick(self.joyID)   # this instance's joystick object
            self.J.init()
        except:
            print("Joystick " + str(self.joyID) + " not readable; blocking = " + str(self.blocking))
            return False
        
        # keep track of number buttons and analog axes
        self.numButtons = self.J.get_numbuttons()               # store number of buttons
        self.numAxes = self.J.get_numaxes()                     # store number of axes
        self.stat = 'valid'
        return True
        