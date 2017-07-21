'''Imports all custom picus*.py helpers
'''
import picusData as config
import picusPackets as packets
from picusUtils import picusUtils as utils

if __name__ == '__main__':
    import picusData
    import picusPackets
    import picusUtils
    
    print('picusData :: ' + picusData.__doc__)
    print('picusPackets :: ' + picusPackets.__doc__)
    print('picusUtils :: ' + picusUtils.__doc__)
    
    print('config.Picus :: ' + config.Picus.__doc__)
    print('config.Martius :: ' + config.Martius.__doc__)
    print('config.DRIVE_vals :: ' + config.DRIVE_vals.__doc__)
    print('config.SteerEnable :: ' + config.SteerEnable.__doc__)