#ifndef POSITIION_H
#define POSITIION_H

#include "Gyro.h"
#include "Mouse.h"

#define MAX_ANGLE_OFFSET     1 // rotation can be off by 1 degree
#define MAX_LOCATION_OFFSET 50 // 50mm out of x,y acceptable

struct Position_Target
{
   int x;
   int y;
   float angle;
};

class Position
{
public:
	Position();
	Position(int,int,float); // initialize with xy Az value
	int getX(); // returns the x position in mm from the corner
	int getY();
	float getZAngle(); // returns angle in degrees
	int getVelocity(); // velocity in mm/s
	void setX(int);
	void setY(int);
	void setZAngle(float);
	void update();
	void reset();
private:
	// constructors should be updated to include error-correction values

	uint32_t _lastupdate; // millis() of last check
	int _x; // arena should be about 6000mm x 6000mm
	int _y;
	float _angleZ;
	float _av; // angular velocity
	float _velocity; // velocity in m/s
};

#endif
