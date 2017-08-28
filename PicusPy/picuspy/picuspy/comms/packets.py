'''Define packet types and structures for communication
'''
from ..utils.enum import *

#TODO: integrate Lightweight Communications and Marshalling (LCM) (http://lcm-proj.github.io/) or ROS

@unique # makes AutoNumber work like enum in C++)
class PicusMode(AutoNumber):
    '''Enumerated operating modes. ex.: mode = PicusMode.standby
    '''
    autonomous = ()
    standby = ()
    stationary = ()
    driving = ()
    collecting = ()
    
#Header to identify packet type
class HeaderType(AutoNumber):
    '''Enumerated packet header types
    '''
    ToArm = ()
    ToRover = ()
    FromArm = ()
    FromRover = ()
    Command = ()
    Photo = ()
    HighRes = ()


#Which command you're sending (when you do)
class CommandType(AutoNumber):
    '''Enumerated command structure types
    '''
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
    '''Status report
    '''
    enabled = False            #if subsystem is enabled
    EStop = False                #if EStop is hit
    calibrated = [0,0,0]            #if subsystem is calibrated (may need to be array)
    powerConnected = 0        #changing power source, etc.
    calibrating = 0        #calibrating step (may need to be array)
    communicating = 0  #millis between good packet reads


#Controller/joystick data
class ControlStruct():
    '''Joystick data
    '''
    def __init__(self, data=dict()):
        self.data = data

#GPS coordinates and compass rotation
class GPSStruct():
    '''GPS data
    '''
    lat = 0
    lon = 0
    alt = 0
    compass = 0
    temporal = 0

#Absolute Orientation from IMU
class IMUStruct():
    '''IMU data \ orientation
    '''
    roll = 0
    pitch = 0
    yaw = 0

#Sensor values, can roll all sensors into the one struct
class SensorStruct():
    '''Sensor values \ readings
    '''
    fVal = [0,0]
    intVal = [0,0]

#Stores Cartesian coordinates of each joint, plus rotation and tilt of wrist
class ArmPositionStruct():
    '''Arm joint positions (post-processing)
    '''
    x = 0    #left\right
    y = 0    #in\out
    z = 0    #up\down
    w = 0    #wrist angle
    r = 0    #wrist rotation

#Stores encoder values for each motor with min and max
class ArmRawStruct():
    '''Raw arm positions and limits (pre-processed)
    '''
    motor = [0,0,0,0,0,0]   #current motor position
    min = [0,0,0,0,0,0]     #min motor value (bottom of range)
    max = [0,0,0,0,0,0]     #max motor value (top of range)

#Packet sent to arm
class ToArmPacket():
    '''Packet to send to arm
    '''
    status = StatusStruct
    goal = ArmPositionStruct
    grip = 0

#Packet coming from arm
class FromArmPacket():
    '''Packet coming from arm
    '''
    status = StatusStruct
    error = 0
    position = ArmPositionStruct
    raw = ArmRawStruct

#Arm axis PID values
class ArmPIDVals():
    '''Arm PID values
    '''
    kP = [0]*6
    kI = [0]*6
    kD = [0]*6

#Packet to ROVER
class ToRoverPacket():
    '''Packet to send to rover
    '''
    status = StatusStruct
    operateMode = PicusMode.driving
    stick = ControlStruct
    #aux = ControlStruct

#Packet from ROVER
class FromRoverPacket():
    '''Packet coming from rover
    '''
    status = StatusStruct
    location = GPSStruct
    wheelAngle = 0
    wheelSpeed = 0
    imu = IMUStruct
    error = 0

#Rover things PID values
class RoverPIDVals():
    '''Drivetrain PID values
    '''
    kP = [0]*8
    kI = [0]*8
    kD = [0]*8

#Structure that holds a picture, with geolocation and time data
class PhotoStruct():
    '''A photo with time, GPS and IMU data
    '''
    size = [0,0]
    location = GPSStruct
    imu = IMUStruct
    #dat = image data
    #sometype timestamp
    
#Structure for high resolution camera stuff
class HighResStruct():
    '''High resolution photo
    '''
    settingsorsomething = 0
    photo = PhotoStruct

#Struct to send commands to ROVER or Arm
class CommandStruct():
    '''A command to the rover
    '''
    cmdNum = 0
    fVal = 0.0
    intVal = 0
    