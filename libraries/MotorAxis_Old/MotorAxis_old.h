#ifndef MotorAxis_h
#define MotorAxis_h
#include <Arduino.h>
#include <Servo.h>
#include "Variables.h"
#include "PID.h"

class MotorAxis {
	short rawInput, rawPos;
	short velocity, acceleration, speed;
	short lastMin, lastMax, rawLast;
	short encoderDir;
	JOINTVALUES encoder, position, raw;
	JOINT reference;
	int sigPin, encoderPin, loops;
	bool reverse, direction, calPos, override;
	int32_t lastChange;
	short servoNum;
	float currentPD;
	char Name[10];
	
	uint16_t pos;
	short tolerance;
	bool Calibrated;
	
public:
	CalcPd PID;

	short calibrating;
	bool PRINT;
	void Read();
	void Calibrate();
	void CalibrateMove();
	void Step(byte startstep);
	void GoDir(bool direction);
	void Move();
	void Operate();
	void Write(int val);
	uint8_t GetPos(){return pos;};
	short GetRawInput(){return rawInput;};
	short GetRawPos(){return rawPos;};
	short GetSpeed(){return speed;};
	void SetSp(int32_t _sp){PID.SetSp(_sp);};
	void SetSpeed(int _speed){speed = _speed;override=1;};
	void SetAuto(){override=0;};
	void Save();
	void Init(const char* Name, bool calibrationPosition);
	
	//constructor
	MotorAxis(int encoderPin,int signalPin,bool reverse,
		bool encoderDirection,short tolerance,
		struct JOINT* _JOINTobj, CalcPd* PIDobj);
	
};//end MotorAxis Class


#endif
