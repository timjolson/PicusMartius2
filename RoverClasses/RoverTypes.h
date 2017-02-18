#ifndef RoverTypes_H
#define RoverTypes_H

//Header to identify packet type
enum HeaderType {
	ToArm,
	ToRover,
	FromArm,
	FromRover,
	Command,
	Photo,
	HighRes,
};

//Which command you're sending (when you do)
enum CommandType {
	RecordGPS,
	AddWaypoint,
	TakePhoto,
	DriveSoftStop,
	DriveHardStop,
	DriveSpeed,
	DriveAngle,
	ArmSoftStop,
	ArmHardStop,
	ArmGrab,
	ArmRelease,
	ArmStow,
	ArmUnstow,
};

//Status variables
struct StatusStruct {
	bool enabled;			//if subsystem is enabled
	bool EStop;				//if EStop is hit
	bool calibrated;		//if subsystem is calibrated (may need to be array)
	bool powerConnected;	//changing power source, etc.
	uint8_t calibrating;	//calibrating step (may need to be array)
	uint16_t communicating; //millis between good packet reads
};

//Controller/joystick data
struct ControlStruct {
	float x;	//4 proportional axes
	float y;
	float z;
	float w;
	bool[4] b;	//4 buttons
};

//GPS coordinates and compass rotation
struct GPSStruct {
	float lat;
	float lon;
    float alt;
	float compass;
};

//Absolute Orientation from IMU
struct IMUStruct {
	float roll;
	float pitch;
	float yaw;
};

//Sensor values, can roll all sensors into the one struct
struct SensorStruct {
	float[2] fVal;
	int[2] intVal;
};

//Stores Cartesian coordinates of each joint, plus rotation and tilt of wrist
struct ArmPositionStruct {
	float x;	//left\right
	float y;	//in\out
	float z;	//up\down
	float w;	//wrist angle
	float r;	//wrist rotation
};

//Stores encoder values for each motor with min and max
struct ArmRawStruct {
	float[6] motor; //current motor position
	float[6] min;	//min motor value (bottom of range)
	float[6] max;	//max motor value (top of range)
};

//Packet sent to arm
struct ToArmPacket {
	StatusStruct status;
	ArmPositionStruct goal;
	uint8_t grip;
};

//Packet coming from arm
struct FromArmPacket {
	StatusStruct status;
	uint16_t error;
	ArmPositionStruct position;
	ArmRawStruct raw;
};

//Arm axis PID values
struct ArmPIDVals {
	float[7] kP;
	float[7] kI;
	float[7] kD;
};

//Packet to ROVER
struct ToRoverPacket {
	StatusStruct status;
	uint8_t operateMode;
	ControlStruct stick;
	ControlStruct aux;
};

//Packet from ROVER
struct FromRoverPacket {
	StatusStruct status;
	GPSStruct position;
	float[4] wheelAngle;
	float[6] wheelSpeed;
	IMUStruct imu;
	uint16_t error;
};

//Rover things PID values
struct RoverPIDVals {
	float[8] kP;
	float[8] kI;
	float[8] kD;
};

//Structure that holds a picture, with geolocation and time data
struct PhotoStruct {
	uint16_t[2] size;
    bool* dat;
	sometype timestamp;
	GPSStruct location;
	IMUStruct imu;
};

//Structure for high resolution camera stuff
struct HighResStruct {
	int settingsorsomething;
	PhotoStruct photo;
};

//Struct to send commands to ROVER or Arm
struct CommandStruct {
	uint16_t cmdNum;
	float fVal;
	int intVal;
};


#endif