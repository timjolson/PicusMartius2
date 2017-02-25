# define settings and stuff for Picus(Rover) and Martius(Controller)
from picusEnum import *


@unique
class PicusMode(AutoNumber):
    # these are enumerations
    # see use below in class Picus, mode =
    autonomous = ()
    standby = ()
    stationary = ()
    driving = ()
    collecting = ()


class Picus:
    serverConn900 = ('localhost', 6000)  # this is the rover @900MHz
    serverConn24 = ('localhost', 6001)  # this is the rover @2.4GHz
    localTimeout = 0.02  # socket timeout

    driveTimeout = 0.06  # safety timeout for drive system
    armTimeout = 0.06  # safety timeout for arm system
    sciTimeout = 0.06  # safety timeout for science system

    location = []  # current GPS stuff
    waypoints = []  # list of known good GPS waypoints
    targets = []  # list of target GPS waypoints

    mode = PicusMode.standby # rover operating mode


class Martius:
    serverConn900 = ('localhost', 6000)  # this is the controller @900MHz
    serverConn24 = ('localhost', 6001)  # this is the controller @2.4GHz
    localTimeout = 0.02  # socket timeout

#Header to identify packet type
class HeaderType(AutoNumber):
	ToArm = ()
	ToRover = ()
	FromArm = ()
	FromRover = ()
	Command = ()
	Photo = ()
	HighRes = ()

#Which command you're sending (when you do)
class CommandType(AutoNumber):
	RecordGPS = ()
	AddWaypoint = ()
	TakePhoto = ()
	DriveSoftStop = ()
	DriveHardStop = ()
	DriveSpeed = ()
	DriveAngle = ()
	ArmSoftStop = ()
	ArmHardStop = ()
	ArmGrab = ()
	ArmRelease = ()
	ArmStow = ()
	ArmUnstow = ()

#Status variables
class StatusStruct():
	enabled = False			#if subsystem is enabled
	EStop = False				#if EStop is hit
	calibrated = [0,0,0]		    #if subsystem is calibrated (may need to be array)
	powerConnected = 0	    #changing power source, etc.
	calibrating = 0	    #calibrating step (may need to be array)
	communicating = 0  #millis between good packet reads


#Controller/joystick data
class ControlStruct():
	x = 0	#4 proportional axes
	y = 0
	z = 0
	w = 0
	b = [0,0,0,0]	#4 buttons


#~ #GPS coordinates and compass rotation
#~ class GPSStruct():
	#~ float lat
	#~ float lon
    #~ float alt
    #~ float compass
#~

#~ #Absolute Orientation from IMU
#~ class IMUStruct():
	#~ float roll
	#~ float pitch
	#~ float yaw
#~

#~ #Sensor values, can roll all sensors into the one struct
#~ class SensorStruct():
	#~ float[2] fVal
	#~ int[2] intVal
#~

#~ #Stores Cartesian coordinates of each joint, plus rotation and tilt of wrist
#~ class ArmPositionStruct():
	#~ float x	#left\right
	#~ float y	#in\out
	#~ float z	#up\down
	#~ float w	#wrist angle
	#~ float r	#wrist rotation
#~

#~ #Stores encoder values for each motor with min and max
#~ class ArmRawStruct():
	#~ float[6] motor #current motor position
	#~ float[6] min	#min motor value (bottom of range)
	#~ float[6] max	#max motor value (top of range)
#~

#~ #Packet sent to arm
#~ class ToArmPacket():
	#~ StatusStruct status
	#~ ArmPositionStruct goal
	#~ uint8_t grip
#~

#~ #Packet coming from arm
#~ class FromArmPacket():
	#~ StatusStruct status
	#~ uint16_t error
	#~ ArmPositionStruct position
	#~ ArmRawStruct raw
#~

#~ #Arm axis PID values
#~ class ArmPIDVals():
	#~ float[7] kP
	#~ float[7] kI
	#~ float[7] kD
#~

#Packet to ROVER
class ToRoverPacket():
	status = StatusStruct
	operateMode = PicusMode.driving
	stick = ControlStruct
	#aux = ControlStruct


#Packet from ROVER
class FromRoverPacket():
	status = StatusStruct
	#position = GPSStruct
	wheelAngle = 0
	wheelSpeed = 0
	#imu = IMUStruct
	error = 0


#~ #Rover things PID values
#~ class RoverPIDVals():
	#~ float[8] kP
	#~ float[8] kI
	#~ float[8] kD
#~

#~ #Structure that holds a picture, with geolocation and time data
#~ class PhotoStruct():
	#~ uint16_t[2] size
    #~ bool* dat
	#~ sometype timestamp
	#~ GPSStruct location
	#~ IMUStruct imu
#~

#~ #Structure for high resolution camera stuff
#~ class HighResStruct():
	#~ int settingsorsomething
	#~ PhotoStruct photo
#~

#~ #Struct to send commands to ROVER or Arm
#~ class CommandStruct():
	#~ uint16_t cmdNum
	#~ float fVal
	#~ int intVal
#~