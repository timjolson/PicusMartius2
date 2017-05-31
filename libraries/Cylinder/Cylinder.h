#ifndef Cylinder_h
#define Cylinder_h
#include <Arduino.h>
#include "Variables.h"
#include "PID.h"

void ArmPWMinit();

enum _cyl { cUP=1, cDOWN=0, cIN=0, cOUT=1};

class Cylinder {
	short rawInput, rawPos;
	short velocity, acceleration, speed;
	short lastMin, lastMax, rawLast;
	short encoderDir;
	JOINTVALUES encoder, position, raw, jointangles;
	JOINT reference;
	int dirPin, propPin, encoderPin, loops;
	bool lastDir, changed, override;
	bool reverse, direction, calpos;
	int changeTime;
	int32_t calTime, lastChange;
	short calTimeReq;
	float currentPD;
	char Name[10];
	CalcPd PID;
	uint16_t pos;
	int16_t angle;
	short tolerance;
	bool Calibrated;
	
public:
	short calibrating;
	bool PRINT;
	void Read();
	void Calibrate();
	void Step(byte _startstep);
	void GoToMax();
	void GoToMin();
	void GoDir(bool _dir);
	void Hold();
	void Move();
	void Operate();
	uint16_t GetAngle(){return angle;};
	uint8_t GetPos(){return pos;};
	short GetRawInput(){return rawInput;};
	short GetRawPos(){return rawPos;};
	short GetSpeed(){return speed;};
	void SetSp(int32_t _sp){PID.SetSp(_sp);};
	void SetAngles(short _min,short _max);
	void SetSpeed(int _speed){speed = _speed;override=1;};
	void SetDir(int _dir){direction = _dir;override=1;};
	void SetAuto(){override=0;};
	void Save();
	void Init(const char* _name, bool _reverse, bool _calpos, bool _startpos);
	
	//constructor
	Cylinder(int _encoderPin, int _directionPin, int _proportionalPin,
		bool _directionReverse, short _calibrationTime, short _encoderDirection,
		short _tolerance, struct JOINT* _JOINTobj, CalcPd* _PIDobj);
};//end PropCyl Class




#endif
