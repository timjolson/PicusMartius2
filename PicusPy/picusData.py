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
    conn24 = ("192.168.1.200", 7008)  # this is the rover @2.4GHz
    localTimeout = 0.01  # socket timeout

    driveTimeout = 0.5  # safety timeout for drive system
    armTimeout = 0.06  # safety timeout for arm system
    sciTimeout = 0.06  # safety timeout for science system

    location = []  # current GPS stuff
    waypoints = []  # list of known good GPS waypoints
    targets = []  # list of target GPS waypoints

    mode = PicusMode.standby # rover operating mode
    local_ports = {'L': 6000, 'R': 6001, 'S': 6002} #

class Martius:
    conn24 = ("192.168.1.100", 6008)  # this is the controller @2.4GHz
    localTimeout = 0.01  # socket timeout

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
	def __init__(self):
		self.clear()
	def clear(self):
		self.x = 0
		self.y = 0
		self.z = 0
		self.w = 0
		self.fb=[0,0,0,0]
		self.tb=[0,0,0,0]
		self.dpad=0


#GPS coordinates and compass rotation
class GPSStruct():
	lat = 0
	lon = 0
	alt = 0
	compass = 0


#Absolute Orientation from IMU
class IMUStruct():
	roll = 0
	pitch = 0
	yaw = 0


#Sensor values, can roll all sensors into the one struct
class SensorStruct():
	fVal = [0,0]
	intVal = [0,0]


#Stores Cartesian coordinates of each joint, plus rotation and tilt of wrist
class ArmPositionStruct():
	x = 0	#left\right
	y = 0	#in\out
	z = 0	#up\down
	w = 0	#wrist angle
	r = 0	#wrist rotation


#Stores encoder values for each motor with min and max
class ArmRawStruct():
	motor = [0,0,0,0,0,0]   #current motor position
	min = [0,0,0,0,0,0] 	#min motor value (bottom of range)
	max = [0,0,0,0,0,0] 	#max motor value (top of range)


#Packet sent to arm
class ToArmPacket():
	status = StatusStruct
	goal = ArmPositionStruct
	grip = 0


#Packet coming from arm
class FromArmPacket():
	status = StatusStruct
	error = 0
	position = ArmPositionStruct
	raw = ArmRawStruct


#Arm axis PID values
class ArmPIDVals():
	kP = [0,0,0,0,0,0,0]
	kI = [0,0,0,0,0,0,0]
	kD = [0,0,0,0,0,0,0]


#Packet to ROVER
class ToRoverPacket():
	status = StatusStruct
	operateMode = PicusMode.driving
	stick = ControlStruct
	#aux = ControlStruct


#Packet from ROVER
class FromRoverPacket():
	status = StatusStruct
	position = GPSStruct
	wheelAngle = 0
	wheelSpeed = 0
	imu = IMUStruct
	error = 0


#Rover things PID values
class RoverPIDVals():
	kP = [0,0,0,0,0,0,0,0,0,0]
	kI = [0,0,0,0,0,0,0,0,0,0]
	kD = [0,0,0,0,0,0,0,0,0,0]


#Structure that holds a picture, with geolocation and time data
class PhotoStruct():
	size = [0,0]
    #dat = image data
	#sometype timestamp
	location = GPSStruct
	imu = IMUStruct


#Structure for high resolution camera stuff
class HighResStruct():
	settingsorsomething = 0
	photo = PhotoStruct


#Struct to send commands to ROVER or Arm
class CommandStruct():
	cmdNum = 0
	fVal = 0.0
	intVal = 0


class PWM():
	freqRange = 30
	neutral = 14
	min = 7
	max = 22
	range = max - neutral