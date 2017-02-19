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

