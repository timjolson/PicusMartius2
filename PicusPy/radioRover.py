import sys, time, datetime, serial, os
from tcp_tx import *
from tcp_rx import *
from picusDependencies import config, packets, utils
from UDP_Library import *
from PID import PID
import numpy as np


drivemega = serial.Serial('/dev/ttyACM0', 115200, timeout=0.01)
# armmega = serial.Serial('/dev/ttyACM1', 115200, timeout=0.01)
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
            # print('mega:%a'%(result))
            if len(result) > 8:
                if result[0] == ord('&'):
                    if result[1] == ord('$'):
                        _ret = [int(result[3]), int(result[5]), int(result[7]), int(result[9])]
                        # print('res:%a' % (_ret))
                        return _ret
                    else:
                        return None
            else:
                print('incomplete')
                return None

# def armWrite(pin,val):
#     try:
#         armmega.write(
#             bytes('&&', 'utf-8') + bytes(str(pin), 'ascii') + bytes(',', 'utf-8') + bytes(str(val), 'ascii') + bytes(',', 'utf-8')
#         )
#     except:
#         print('no armmega on pin write')

# def armRead():
#     try:
#         armmega.write(bytes('&$', 'utf-8'))
#         time.sleep(0.0015)
#     except:
#         print('no armmega on request')
#     else:
#         val = [0] * 10
#         try:
#             result = armmega.readline()
#         except:
#             print('no armmega on read')
#         else:
#             if len(result) > 8:
#                 if result[0] == ord('&'):
#                     if result[1] == ord('$'):
#                         _ret = [int(result[3]), int(result[5]), int(result[7]), int(result[9])]
#                         print('res:%a' % (_ret))
#                         return [result[3], result[5], result[7], result[9]]
#             else:
#                 print('incomplete')
#                 return None

# feedback with wheels straight
centers = [101, 103, 113, 87]
# pids for steering
PIDlist = [
    PID(0.002, 0.0, 0.0005, deadband=4, refreshTime=0.01), \
    PID(-0.0026, 0.0, -0.0005, deadband=4, refreshTime=0.01), \
    PID(-0.002, 0.0, -0.0005, deadband=4, refreshTime=0.01), \
    PID(0.0022, 0.0, 0.0005, deadband=4, refreshTime=0.01) \
    ]
for i, p in enumerate(PIDlist):
    p.enable(centers[i])
    p.setSP(0)
E = [0] * len(PIDlist)  # position errors
S = [0] * len(PIDlist)  # speed/output

print("\r\nStarting Rover @" + str(datetime.datetime.fromtimestamp(time.time())))

# addresses (set in picusData)
myAddr = config.Picus.conn24       # rover @2.4GHz
ctrlAddr = config.Martius.conn24   # controller @2.4GHz

# create radio object to send and receive
radio = UDP_Library( myAddr, ctrlAddr)
# print("receiving on port %d" %(myAddr[1]))

lp = config.Picus.local_ports

# empty control structure so we can read for buttons without
# received data  ( button 1 stops script on rover )
rx = packets.ControlStruct()

allDRIVE = config.Left+config.Right

for p in config.SteerEnable:
    driveWrite(p,254)
for p in allDRIVE:
    driveWrite(p, config.DRIVE_vals.neutral)
for p in config.Steer:
    driveWrite(p, config.STEER_vals.neutral)

# timing info
start = time.time() # start of script
lastPrint = start   # last print for waiting
lastDat = start     # last received data
lastSteer = start   # last steering info

try:
    # DO ALL OF THIS
    while True:
        curr = time.time()  # update current time
        trx = 0

        rx.clear()

        # if we're waiting for data, print that
        if (curr - lastPrint > 3) and (curr - lastDat) > 1:
            lastPrint = curr
            print("waiting")

        # update feedback
        steerAngles = driveRead()
        if steerAngles is not None:
            lastSteer = curr
            for i, a in enumerate(steerAngles):
                steerAngles[i] = steerAngles[i] - centers[i]

                if steerAngles[i] > 0:
                    steerAngles[i] = steerAngles[i] / 25
                else:
                    steerAngles[i] =steerAngles[i] / 25

                steerAngles[i] = steerAngles[i] * 90
        else:
            print('no feedback')

        # attempt data receive
        try:
            trx = radio.receiveUDP()
        # there was a problem receiving
        except: pass

        dontSteer = False

        # received data, it is not zero
        if trx:
            try:
                rx = pickle.loads(trx)  # unpack data
            except:
                print("no pickle") # but we like pickles
                # pickles are the best
                # pickles are my precious
                pass
            else:
                # we received a control packet
                if type(rx) == ControlStruct:
                    lastDat = curr  # update last good data time
                    for p in config.Left: driveWrite(p, config.DRIVE_vals.neutral + int(rx.z * config.DRIVE_vals.range))
                    for p in config.Right: driveWrite(p, config.DRIVE_vals.neutral + int(-1 * rx.z * config.DRIVE_vals.range))

                    # button is not pressed
                    if not rx.tb[0]:
                        # print(rx.x * 50)
                        # update steering setpoints
                        for i, p in enumerate(PIDlist):
                            if i==0 or i==3:
                                p.setSP(rx.x * 60)
                            else:
                                p.setSP(-1* rx.x * 60)
                        dontSteer = False
                        keepx = rx.x

                    # button IS pressed
                    else:
                        # don't steer
                        for p in config.Steer: driveWrite(p, config.STEER_vals.neutral)
                        dontSteer = True

        # did not receive data, timed-out
        elif (curr - lastDat) > config.Picus.driveTimeout:
            # stop motors
            for p in config.Left: driveWrite(p, config.DRIVE_vals.neutral)
            for p in config.Right: driveWrite(p, config.DRIVE_vals.neutral)
            for p in config.Steer: driveWrite(p, config.STEER_vals.neutral)

        # did not receive data, but did not time-out
        if (curr - lastDat) < config.Picus.driveTimeout:
            # no rx, not getting feedback
            if curr - lastSteer > config.Picus.driveTimeout:
                # stop motors
                for p in config.Left: driveWrite(p, config.DRIVE_vals.neutral)
                for p in config.Right: driveWrite(p, config.DRIVE_vals.neutral)
                for p in config.Steer: driveWrite(p, config.STEER_vals.neutral)
            # no rx, but are getting feedback
            else:
                if dontSteer is False:
                    # update PIDs
                    for i, p in enumerate(PIDlist):
                        E[i], S[i] = PIDlist[i].get(steerAngles[i] if steerAngles is not None else 0.0)
                        # write their values
                        driveWrite(config.Steer[i], int(S[i] * config.STEER_vals.range) + config.STEER_vals.neutral)
                        # try:
                        #     if keepx:
                        #         print(int(keepx * config.STEER_vals.range) + config.STEER_vals.neutral)
                        #         driveWrite(config.Steer[i], int(keepx * config.STEER_vals.range) + config.STEER_vals.neutral)
                        # except:
                        #     pass
                else:
                    # don't steer
                    for p in config.Steer: driveWrite(p, config.STEER_vals.neutral)

        # print('a:%1.2f\ts:%1.2f\ta:%1.2f\ts:%1.2f\ta:%1.2f\ts:%1.2f\ta:%1.2f\ts:%1.2f\t'
        #       %(steerAngles[0], PIDlist[0].setpoint, steerAngles[1], PIDlist[1].setpoint,
        #         steerAngles[2], PIDlist[2].setpoint, steerAngles[3], PIDlist[3].setpoint))

        # check button 1 status
        if rx.b9:
            print("button stopped script")
            # stop script
            break

        # check being bored for 10 seconds
        if (curr - lastDat) > 10:
            print("time up")
            # stop script
            break

        # print(time.time() - curr)
    # end while True
except KeyboardInterrupt:
    pass


for p in allDRIVE:
    driveWrite(p, config.DRIVE_vals.neutral)
for p in config.SteerEnable:
    driveWrite(p,0)
for p in config.Steer:
    driveWrite(p, config.STEER_vals.neutral)

drivemega.close()
# armmega.close()
del drivemega
# del armmega

exit(0)