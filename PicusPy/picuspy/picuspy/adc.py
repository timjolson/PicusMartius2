'''package includes use of ADS1015 adc connected directly to odroid, and an emulator
    for when you're running windows.
    Written by Tim Olson - timjolson@user.noreplay.github.com
    Based on Adafruit ADS1x15 documentation
    
    Classes::
    ADC
    
    #TODO: test with actual ADC (class/subclass may not be set up right)
    #TODO: add start_adc(), stop_adc(), get_last_result(), write docs
'''

import os

# not running on windows
if os.name != 'nt':
    # Import the ADS1x15 module.
    import Adafruit_ADS1x15
    class ADC(Adafruit_ADS1x15.ADS1015):
        def __init__(self, address, busnum, gain):
            self.adc = self.super.ADS1015(address=address, busnum=busnum)
            self.address = address
            self.busnum = busnum
            self.gain = gain
            # log
            print('ADC started at {}, bus {}, gain {}'.format(address, busnum, gain))
            
        def read_adc(self, channel, gain=None, rate=None):
            self.super.read_adc(channel, (self.gain if gain is None else gain, rate))
            
        # TODO: add start_adc(), stop_adc(), get_last_result()
        
# running on windows, emulate the ADC
else:
    class ADC():
        def __init__(self, address, busnum, gain):
            self.address = address
            self.busnum = busnum
            self.gain = gain
            
            from numpy import random
            self.rand = random
            
            # log
            print('ADC started at {}, bus {}'.format(address, busnum))
            
        def read_adc(self, channel, gain=None, rate=None):
            assert 0 <= channel <= 3, 'channel must be 0-3'
            output = self.rand.choice(list(range(1024)))
            
            # log
            print('ADC.read_adc() at {a}, bus {b}, channel {c} with gain {g}, rate {r} = {out}'\
                .format(a=self.address, b=self.busnum, c=channel,\
                g=(self.gain if gain is None else gain),\
                r=rate, out=output))
            
            return output
        
        # TODO: add start_adc(), stop_adc(), get_last_result()


# Rover Picus Martius stuff
# Address table on page 14 of I2C chip datasheet https://cdn-shop.adafruit.com/datasheets/ads1015.pdf
# bus 4 = pins 14, 16
# address=0x48 :: ground/none to ADDR
# address=0x49 :: 5V to ADDR
# address=0x4A :: SDA to ADDR
# address=0x4B :: SCL to ADDR

# Choose a gain of 1 for reading voltages from 0 to 4.09V.
# Or pick a different gain to change the range of voltages that are read:
# gain = 2/3 :: +/-6.144V
# gain = 1:: +/-4.096V
# gain = 2 :: +/-2.048V
# gain = 4 :: +/-1.024V
# gain = 8 :: +/-0.512V
# gain = 16 :: +/-0.256V
# See table 3 in the ADS1015/ADS1115 datasheet for more info on gain.

if __name__=='__main__':
    import time
    
    adc = ADC(address=0x48, busnum=4, gain=2/3)
    
    print('Reading ADS1x15 values, press Ctrl-C to quit...')
    # Print nice channel column headers.
    print('| {0:>6} | {1:>6} | {2:>6} | {3:>6} |'.format(*range(4)))
    print('-' * 37)
    # Main loop.
    while True:
        # Read all the ADC channel values in a list.
        values = [0]*4
        for i in range(3):
            # Read the specified ADC channel using the previously set gain value.
            values[i] = adc.read_adc(i)
        # Read channel with temporary gain
        values[3] = adc.read_adc(3, gain=2/3)
        
        # Print the ADC values.
        print('| {0:>6} | {1:>6} | {2:>6} | {3:>6} |'.format(*values))
        # Pause for half a second.
        time.sleep(0.5)
