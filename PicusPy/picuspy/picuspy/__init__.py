'''picuspy combines the needs of the NIU Rover Team into a single package.

Subpackages include:
examples - import them to run

motion - run PID, joint controllers, motor controllers, etc
data - manage streamed data and plotting

arduino - manage serial devices (arduino) that run i/o
gpio - manage Odroid GPIO using WiringPi2

interface - manage joysticks, gui
rover - manage rover side

comms - manage radio / communication
adc - manage ADS1015 analog-to-digital-converters via Adafruit library

#TODO:
0. get comms going
1. make simpler examples
2. get arduino going
3. get gpio going
4. change prints in modules to logging
5. get adc going
6. work on gui
7. get generic i2c reading going
'''

import picuspy.interface
import picuspy.motion
import picuspy.data
import picuspy.comms
import picuspy.gpio
import picuspy.adc
import picuspy.utils
