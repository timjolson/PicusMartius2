import sys, time, datetime, serial
from tcp_tx import *
from tcp_rx import *
from picusData import *
from picusData import Left, Right, Steer, SteerEnable, ADCaddr
from UDP_Library import *
import Adafruit_ADS1x15

ADC = [0]*len(ADCaddr)
ADC[0] = Adafruit_ADS1x15.ADS1015(address=ADCaddr['Steer'], busnum=4)
def readADC(idx=0):
    val = [0]*4
    for i in range(4):
        try:
            val[i] = ADC[idx].read_adc(i, gain=2/3)
        except:
            val = None
            break
    return val

mega = serial.Serial('/dev/ttyACM0', 115200, timeout=0.02)
time.sleep(1)
def pinWrite(pin,val):
    mega.write(
        bytes('&&', 'utf-8') + bytes(str(pin), 'ascii') + bytes(',', 'utf-8') + bytes(str(val), 'ascii') + bytes(
            ',', 'utf-8'))

# timing info
start = time.time() # start of script
lastPrint = start        # last print for waiting
lastDat = start     # last received data

print("\r\nStarting Rover @" + str(datetime.datetime.fromtimestamp(start)))

# addresses (set in picusData)
myAddr = Picus.conn24       # rover @2.4GHz
ctrlAddr = Martius.conn24   # controller @2.4GHz

# create radio objetc to send and receive
radio = UDP_Library( myAddr, ctrlAddr)

lp = Picus.local_ports

# empty control structure so we can read for buttons without
# received data  ( button 1 stops script on rover )
rx = ControlStruct()

allDRIVE = Left+Right

for p in SteerEnable:
    pinWrite(p,254)
for p in allDRIVE:
    pinWrite(p, DRIVE.neutral)
for p in Steer:
    pinWrite(p, STEER.neutral)

try:
    # DO ALL OF THIS
    while True:
        curr = time.time()  # update current time
        trx = 0

        pot = readADC()
        if pot:
            print(pot[0])
        else:
            print('ADC disconnected')

        rx.clear()

        # if we're waiting for data, print that
        if (curr - lastPrint > 3) and (curr - lastDat) > 1:
            lastPrint = curr
            for p in SteerEnable:
                pinWrite(p, 0)
            for p in allDRIVE:
                pinWrite(p, DRIVE.neutral)
            for p in Steer:
                pinWrite(p, STEER.neutral)
            print("waiting")

        # attempt data receive
        try:
            trx = radio.receiveUDP()
        # there was a problem receiving
        except:
            pass

        # received data, it is not zero
        if trx:
            lastDat = curr          # update last good data time
            try:
                rx = pickle.loads(trx)  # unpack data
            except:
                print("no pickle") # but we like pickles
                # pickles are the best
                # pickles are my precious
                pass

            if type(rx) == ControlStruct:
                for p in Left: pinWrite(p, DRIVE.neutral + int(rx.z * DRIVE.range))
                # local_send(lp['L'], rx.z * scale)
                for p in Right: pinWrite(p, DRIVE.neutral + int(-1 * rx.z * DRIVE.range))
                # local_send(lp['R'], -1 * rx.z * scale)
                #print(rx.x)
                #print(int(rx.x * STEER.range))
                for p in Steer: pinWrite(p, int(rx.x * STEER.range) + STEER.neutral)
        # end if trx

        # check button 1 status
        if rx.fb[0]:
            print("button 1 stopped script")
            # stop script
            break

        # print("C:%1.4f\tL:%1.4f" %(curr,lastDat))
        # check being bored for 10 seconds
        if (curr - lastDat) > 10:
            print("time up")
            # stop script
            break
    # end while True
except KeyboardInterrupt:
    pass


for p in allDRIVE:
    pinWrite(p, DRIVE.neutral)
for p in SteerEnable:
    pinWrite(p,0)
for p in Steer:
    pinWrite(p, STEER.neutral)

local_send(lp['L'], "0")
local_send(lp['R'], "0")
local_send(lp['S'], "0")

mega.close()
del mega

exit(0)