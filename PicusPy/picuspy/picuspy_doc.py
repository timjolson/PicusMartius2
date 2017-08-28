'''import things here to call up doc strings in one place
'''
from picuspy import motion, data, interface, comms, gpio, adc, utils, arduino


if __name__ == '__main__':
    print('motion :: ' + str(motion.__doc__))
    print('motion.PID :: ' + str(motion.PID.__doc__))
    print('data :: ' + str(data.__doc__))
    print('data.Datastream :: ' + str(data.Datastream.__doc__))
    print('data.Myplot :: ' + str(data.Myplot.__doc__))
    print('data.PicusPlot :: ' + str(data.PicusPlot.__doc__))
    print('interface :: ' + str(interface.__doc__))
    print('interface.Joystick :: ' + str(interface.Joystick.__doc__))
    