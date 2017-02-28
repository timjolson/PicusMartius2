import sys, time
from picusData import *
from UDP_Library import *
from tcp_tx import *
from tcp_rx import *

# timing info
start = time.time() # start of script
last = start        # last print for waiting
lastDat = start     # last received data

# addresses (set in picusData)
myAddr = Picus.conn24       # rover @2.4GHz
ctrlAddr = Martius.conn24   # controller @2.4GHz

# create radio objetc to send and receive
radio = UDP_Library( myAddr, ctrlAddr)

lp = Picus.local_ports

# empty control structure so we can read for buttons without
# received data  ( button 1 stops script on rover )
rx = ControlStruct()

# DO ALL OF THIS
while True:
    curr = time.time()  # update current time
    trx = 0

    rx.clear()

    # if we're waiting for data, print that
    if curr - last > 3 and curr - lastDat > 1:
        last = curr
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
            print("no pickle")
            pass

        if type(rx) == ControlStruct:
            local_send(lp['L'], rx.z)
            local_send(lp['R'], -rx.z)
            local_send(lp['S'], rx.x)
            print(rx.z)

    # end if trx

    # check button 1 status
    if rx.fb[0]:
        print("button 1 stopped script")
        # stop script
        break

    # check being bored for 10 seconds
    if curr - lastDat > 10:
        print("time up")
        # stop script
        break
# end while True

local_send(lp['L'], "0")
local_send(lp['R'], "0")
local_send(lp['S'], "0")

