'''package includes management of gpio pins through wiringpi2, and
    and emulator for when you're running windows.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    
    Classes::
    GPIO
    
    #TODO: test actual GPIOs, write doc for softpwm
'''

import os

# not running on windows
if os.name != 'nt':
    import wiringpi2

# running on windows
else:
    class wiringpi2():
        
        def __init__(self):
            # for Odroid xu4
            self.pinlist = [21, 22, 29, 188, 25, 190, 24, 19, 173, 171, 172, 31, \
                    30, 28, 187, 189, 191, 192, 174, 18, 210, 209]
            self.pwmlist = [22, 191] #TODO: i think on Odroid xu4 all digitals can be softPWM
            self.analoglist = [29] #TODO: odroid xu4 has two no analog pins, but don't use them
            
            from numpy import random
            self.rand=random
            
            #TODO: log
            print('Started wiringpi2 emulator')
            
        def wiringPiSetupGpio(self):
            #TODO: log
            print('wiringpi2.SetupGpio()')
            
        def check_pin(self, pin):
            if pin not in self.pinlist:
                #TODO: log
                raise Exception("Pin number {} not a valid pin".format(pin))
        
        def check_analog(self, pin):
            if pin not in self.analoglist:
                #TODO: log
                raise Exception("Pin number {} not a valid analog pin".format(pin))
                
        def check_pwm(self, pin):
            if pin not in self.pwmlist:
                #TODO: log
                raise Exception("Pin number {} not a valid pwm pin".format(pin))
        
        def pinMode(self, pin, mode):
            #TODO: log
            print('gpio.pinMode({}, {})'.format(pin, mode))
            self.check_pin(pin)
            
        def digitalWrite(self, pin, val):
            #TODO: log
            print('gpio.digitalWrite({}, {})'.format(pin, val))
            self.check_pin(pin)
            
        def digitalRead(self, pin, val=None):
            #TODO: log
            if val is None:
                output = self.rand.choice([0,1])
            else:
                output = val
            print('gpio.digitalRead({}, {})={}'.format(pin, val, output))
            self.check_pin(pin)
            return output
        
        def analogWrite(self, pin, val):
            #TODO: log
            print('gpio.analogWrite({}, {})'.format(pin, val))
            self.check_pin(pin)
            self.check_analog(pin)
            
        def analogRead(self, pin, val=None):
            #TODO: log
            if val is None:
                output = self.rand.choice(list(range(255)))
                # TODO: change this to match device's analog bit resolution
            else:
                output = val
            print('gpio.analogRead({}, {})={}'.format(pin, val, output))
            self.check_pin(pin)
            self.check_analog(pin)
            return output
        
        def softPwmCreate(self, pin, initial, range):
            #TODO: log
            print('gpio.softPwmCreate({}, {}, {})'.format(pin, initial, range))
            self.check_pin(pin)
            self.check_pwm(pin)
        
        def softPwmWrite(self, pin, val):
            #TODO: log
            print('gpio.softPwmWrite({}, {})'.format(pin, val))
            self.check_pin(pin)
            self.check_pwm(pin)
        
        def all_off(self):
            for p in self.pinlist:
                if p in self.analoglist:
                    self.analogWrite(p,0)
                elif p in self.pwmlist:
                    self.softPwmWrite(p,0)
                else:
                    self.digitalWrite(p,0)
            
        def test(self):
            # tests
            self.digitalRead(21)
            self.digitalRead(22, val=1)
            self.digitalRead(191, val=2)
            self.digitalRead(210, val=None)
            
            self.softPwmCreate(22, 0, 200)
            self.softPwmWrite(22, 50)
            
            self.analogRead(29)
            self.analogRead(29, val=100)
            self.analogRead(29, val=200)
            self.analogRead(29, val=None)
    
    if __name__ != '__main__':
        wiringpi2 = wiringpi2()

if __name__ == '__main__':    
    gpio = wiringpi2()
    gpio.wiringPiSetupGpio()
    gpio.test()
    gpio.all_off() # only available with emulator, not in wiringpi2
    