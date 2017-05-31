#include "Position.h"
#include "Mouse.h"
#include <math.h> // for trig in update();

extern Gyro gyro1;
extern Gyro gyro2;
extern Mouse mouse;

Position::Position()
{
	Position(0,0,0.0f);
}

Position::Position(int x, int y, float angleZ)
{
	_x = x;
	_y = y;
	_angleZ = angleZ;
	_lastupdate = micros();
}

int Position::getX() {return _x; }
int Position::getY() {return _y; }
float Position::getZAngle() {return _angleZ; }
int Position::getVelocity() {return _velocity; }; // velocity in mm/s

void Position::setX(int x) { _x = x; }
void Position::setY(int y) { _y = y; }
void Position::setZAngle(float angleZ) { _angleZ = angleZ; }

void Position::update()
{
	// update the gyros and mouse - they will handle errors and not update their angles/xyz
	gyro1.update(); // update angular velocities
	gyro2.update();
	//mouse.update(); // update distance - now interrupting!

	// update the Z-angle
	float av = (gyro1.getZAngle() + gyro2.getZAngle()) / 2; // angular velocity in degrees/s FIXME: getZAngle() may return degrees/ms .. remove *1000 in next line in this case
	_angleZ = ( _av + av) * 1000000 / 2 / (micros() - _lastupdate); // average angular velocity in degrees/ms divided by the time in ms -> degrees
	_av = av; // update the old angular velocity with the latest
	
	_velocity = (micros() - _lastupdate) * ((mouse.getX() - _x) * (mouse.getY() - _y)); // time * change in distance
	// new xy position based upon velocity and angle FIXME: may not handle negative values/reverse correctly
	_x += _velocity * cos(_angleZ / 57.2957795); // magic number converts to radians
	_y += _velocity * sin(_angleZ / 57.2957795);
	
	_lastupdate = micros();
}

void Position::reset()
{
	gyro1.reset();
	gyro2.reset();
	mouse.reset();
}
