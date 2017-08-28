import serial, time
from picusData import *

drivemega = serial.Serial('COM3', 115200, timeout=0.01)

time.sleep(1)
def driveWrite(pin,val):
    try:
        drivemega.write(
            bytes('&&', 'utf-8') + bytes(str(pin), 'ascii') + bytes(',', 'utf-8') + bytes(str(val), 'ascii') + bytes(',', 'utf-8')
        )
    except:
        print('no drivemega on pin write')

def driveRead():
    try:
        drivemega.write(bytes('&$', 'utf-8'))
        time.sleep(0.0015)
    except:
        print('no drivemega on request')
        return None
    else:
        val = [0] * 10
        try:
            result = drivemega.readline()
        except:
            print('no drivemega on read')
            return None
        else:
            print('mega:%a'%(result))
            if len(result) > 8:
                if result[0] == ord('&'):
                    if result[1] == ord('$'):
                        _ret = [int(result[3]), int(result[5]), int(result[7]), int(result[9])]
                        # print('res:%a' % (_ret))
                        return _ret
            else:
                print('incomplete')
                return None

allDRIVE = Left+Right

for p in SteerEnable:
    driveWrite(p,254)
for p in allDRIVE:
    driveWrite(p, DRIVE.neutral)
for p in Steer:
    driveWrite(p, STEER.neutral)


while True:
    steerAngles = driveRead()
    print(steerAngles)