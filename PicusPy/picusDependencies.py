'''Imports all custom picus*.py helpers
'''

# import things here with names to use in other scripts
import picusData as config
import picusPackets as packets
from picusUtils import picusUtils as utils
from myplot import myplot as plt
from picusPlot import picusPlot as multiplot


# run directly to display docstrings
if __name__ == '__main__':
    import picusData
    import picusPackets
    import picusUtils
    import myplot
    import picusPlot
    
    print('picusData :: ' + picusData.__doc__)
    print('picusPackets :: ' + picusPackets.__doc__)
    print('picusUtils :: ' + picusUtils.__doc__)
    print('myplot :: ' + myplot.__doc__)
    print('picusPlot :: ' + picusPlot.__doc__)
    
    print('config.Picus :: ' + config.Picus.__doc__)
    print('config.Martius :: ' + config.Martius.__doc__)
    print('config.DRIVE_vals :: ' + config.DRIVE_vals.__doc__)
    