#ifndef DRIVE_H
#define DRIVE_H

#include <Arduino.h>
//#include <Position.h>
#include <DriveMotor.h>
#include "Pins.h"

//extern Position position;
//extern Position_Target position_target;

enum drive_type_t { DRIVETYPE_TANK, DRIVETYPE_MECHANUM };
enum drive_mode_t { DRIVEMODE_DRIVE, DRIVEMODE_ROTATE, DRIVEMODE_HALT };
enum drive_controller_t { DRIVECONTROLLER_AUTO, DRIVECONTROLLER_REMOTE };
//enum direction_t { DRIVEDIRECTION_FORWARD=1, DRIVEDIRECTION_BACKWARD=-1, DRIVEDIRECTION_NEUTRAL=0 };

class Drive
{
public:
	Drive();
	
	void update();
	void setDriveType(drive_type_t);
	void setDriveMode(drive_mode_t);
	void setDriveController(drive_controller_t);
	void setSpeed(byte,byte,byte);
	void goForward(float);
	void goBackward(float);
	void translateRight(float);
	void translateLeft(float);
	void rotateRight(float);
	void rotateLeft(float);
	void Forward(float,int); // go forward n inches
	void Backwards(float,int);
	void turn(float);
	void moveRelative(int,int); // move to x,y in inches
	void moveAbsolute(int,int);
	void halt(); // don't move
	void rotate(float);
	void rotateAbsolute(float);
	void kill(); // disable driving
	void enable(); // enable driving

private:
	bool _killed;
	drive_type_t _driveType;
	drive_mode_t _driveMode;
	drive_controller_t _driveController;
	DriveMotor _frontLeft;
	DriveMotor _frontRight;
	DriveMotor _backLeft;
	DriveMotor _backRight;

	/*DriveMotor _frontRight (PIN_MOTOR_FRONTRIGHT, 2000, 1000, 1500);
	DriveMotor _frontLeft(PIN_MOTOR_FRONTLEFT, 2000, 1000, 1500);
	DriveMotor _backRight(PIN_MOTOR_BACKRIGHT, 2000, 1000, 1500);
	DriveMotor _backLeft(PIN_MOTOR_BACKLEFT, 2000, 1000, 1500);*/
//	Position _position;
	
};

#endif
