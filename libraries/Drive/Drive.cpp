#include "Drive.h"
#include <math.h>

#define PIN_MOTOR_FRONTRIGHT 25
#define PIN_MOTOR_FRONTLEFT  21
#define PIN_MOTOR_BACKRIGHT  22
#define PIN_MOTOR_BACKLEFT   23

Drive::Drive()
{
	/*_frontRight = DriveMotor(PIN_MOTOR_FRONTRIGHT, 2000, 1000, 1500);
	_frontLeft = DriveMotor(PIN_MOTOR_FRONTLEFT, 2000, 1000, 1500);
	_backRight = DriveMotor(PIN_MOTOR_BACKRIGHT, 2000, 1000, 1500);
	_backLeft = DriveMotor(PIN_MOTOR_BACKLEFT, 2000, 1000, 1500);*/
	DriveMotor _frontRight(PIN_MOTOR_FRONTRIGHT, 2000, 1000, 1500);
	DriveMotor _frontLeft(PIN_MOTOR_FRONTLEFT, 2000, 1000, 1500);
	DriveMotor _backRight(PIN_MOTOR_BACKRIGHT, 2000, 1000, 1500);
	DriveMotor _backLeft(PIN_MOTOR_BACKLEFT, 2000, 1000, 1500);

	_driveType = DRIVETYPE_TANK;
	_driveMode = DRIVEMODE_HALT;
	_driveController = DRIVECONTROLLER_REMOTE;

	_frontRight.enable();
	_frontLeft.enable();
	_backRight.enable();
	_backLeft.enable();
	_killed = false;
}

/*Drive::Drive(drive_type_t drivetype, int pin1, int pin3, int pin2, int pin4);)
{
	_frontRight = DriveMotor(pin1, 1000, 2000, 1500);
	_frontLeft = DriveMotor(pin2, 1000, 2000, 1500);
	_backRight = DriveMotor(pin3, 1000, 2000, 1500);
	_backLeft = DriveMotor(pin4, 1000, 2000, 1500);

	_driveType = drivetype;
	_driveMode = DRIVEMODE_HALT;
	_driveController = DRIVECONTROLLER_REMOTE;

	_frontRight.enable();
	_frontLeft.enable();
	_backRight.enable();
	_backLeft.enable();
	_killed = false;
}*/

void Drive::setSpeed(byte x, byte y, byte az) // -128,127
{
	static const int AB = 50; // scale for angle
	float speedx = map(x,-128,127,-100,100);
	float speedy = map(y,-128,127,-100,100);
	float angle = 180 - map(az,-128,127,360,0); // angle with 0 at the front
	angle += atan(y/x); // total angle

	// http://files.andymark.com/2008CON-Omni-Baker-McKenzie.pdf slide 34
	// FR	vy - vx + angle(a+b)
	// FL	vy + vx - angle(a+b)
	// RR	vy + vx + angle(a+b)
	// RL	vy - vx - angle(a+b)

	_frontRight.setMotorSpeedPercent(map(speedy - speedx + angle,-200-180*AB,200+180*AB,-100,100));
	_frontLeft.setMotorSpeedPercent (map(speedy + speedx - angle,-200-180*AB,200+180*AB,-100,100));
	_backRight.setMotorSpeedPercent (map(speedy + speedx + angle,-200-180*AB,200+180*AB,-100,100));
	_backLeft.setMotorSpeedPercent  (map(speedy - speedx - angle,-200-180*AB,200+180*AB,-100,100));

}

void Drive::goForward(float speed) // sets to a percent ..
{
	_frontRight.setMotorSpeedPercent(speed * 100);
	_frontLeft.setMotorSpeedPercent (speed * 100);
	_backRight.setMotorSpeedPercent (speed * 100);
	_backLeft.setMotorSpeedPercent  (speed * 100);
}

void Drive::goBackward(float speed)
{
	_frontRight.setMotorSpeedPercent(speed * 100 * (-1));
	_frontLeft.setMotorSpeedPercent (speed * 100 * (-1));
	_backRight.setMotorSpeedPercent (speed * 100 * (-1));
	_backLeft.setMotorSpeedPercent  (speed * 100 * (-1));
}

void Drive::translateRight(float speed)
{
	_frontRight.setMotorSpeedPercent(speed * 100 * (-1));
	_frontLeft.setMotorSpeedPercent (speed * 100);
	_backRight.setMotorSpeedPercent (speed * 100);
	_backLeft.setMotorSpeedPercent  (speed * 100 * (-1));
}

void Drive::translateLeft(float speed)
{
	_frontRight.setMotorSpeedPercent(speed * 100);
	_frontLeft.setMotorSpeedPercent (speed * 100 * (-1));
	_backRight.setMotorSpeedPercent (speed * 100 * (-1));
	_backLeft.setMotorSpeedPercent  (speed * 100);
}

void Drive::rotateRight(float speed)
{
	_frontRight.setMotorSpeedPercent(speed * 100 * (-1));
	_frontLeft.setMotorSpeedPercent (speed * 100);
	_backRight.setMotorSpeedPercent (speed * 100 * (-1));
	_backLeft.setMotorSpeedPercent  (speed * 100);
}

void Drive::rotateLeft(float speed)
{
	_frontRight.setMotorSpeedPercent(speed * 100);
	_frontLeft.setMotorSpeedPercent (speed * 100 * (-1));
	_backRight.setMotorSpeedPercent (speed * 100);
	_backLeft.setMotorSpeedPercent  (speed * 100 * (-1));
}

void Drive::update() // called as often as possible by main loop
{
	_frontRight.update();
	_frontLeft.update();
	_backRight.update();
	_backLeft.update();
}

/*void Drive::setDriveType(drive_type_t t) { _driveType = t; }
void Drive::setDriveMode(drive_mode_t m) { _driveMode = m; }

void Drive::Forward(float percent, int distance)
{

	moveRelative(0, distance);
}

void Drive::Backwards(float percent, int distance)
{
	moveRelative(0, -distance);
}

void Drive::moveRelative(int x, int y)
{
	position_target.angle = atan( (y - position.getY()) / (x - position.getX()) );
	position_target.x = position.getX() - x;
	position_target.y = position.getY() - y;
}

void Drive::moveAbsolute(int x, int y)
{
	position_target.angle = position.getZAngle();
	position_target.x = x;
	position_target.y = y;
}

void Drive::halt() // don't move
{
	_frontRight.disable();
	_frontLeft.disable();
	_backRight.disable();
	_backLeft.disable();
}

void Drive::rotate(float angle)
{
	position_target.angle = (position.getZAngle() + angle);
}

void Drive::rotateAbsolute(float angle)
{
	position_target.angle = angle;
}


void Drive::kill()
{
	_killed = true;
	_frontRight.disable();
	_frontLeft.disable();
	_backRight.disable();
	_backLeft.disable();
}

// disable driving
void Drive::enable()
{
	_frontRight.enable();
	_frontLeft.enable();
	_backRight.enable();
	_backLeft.enable(); // enable driving
}
*/
