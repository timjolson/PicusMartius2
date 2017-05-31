#ifndef Motor_h
#define Motor_h
//#include <Arduino.h>
//#include <Servo.h>
//#include "Variables.h"
#include "..\Button\Button.h"

class Motor {
	short velocity, acceleration, speed, deadband;
	int sigPin, minPin, maxPin;
	bool reverse, direction;
	short servoNum;
	char Name[10];
	
	ButtonClass* maxLim;
	ButtonClass* minLim;
	
	uint64_t lastChange;
	bool Calibrated;
	
public:
	short calibrating;
	bool PRINT;
	void Calibrate();
	void CalibrateMove();
	void Step(byte startstep);
	void Move();
	void Operate();
	void Write(int val);
	short GetSpeed(){return speed;};
	void Init(const char* Name, bool _reverse);
	
	//constructor
	Motor(int signalPin,int minPin,int maxPin);
	
};//end MotorAxis Class


#endif
