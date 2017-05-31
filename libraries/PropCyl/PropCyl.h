#ifndef PropCyl_h
#define PropCyl_h
#include <Arduino.h>
#include "Variables.h"
#include "PID.h"

void ArmPWMinit();

enum _cyl { cUP=1, cDOWN=0, cIN=0, cOUT=1};

class PropCyl {
	short rawInput;
	short rawPos;
	short encoderDir;
	JOINTVALUES encoder;
	JOINTVALUES position;
	JOINTVALUES raw;
	JOINTVALUES jointangles;
	JOINT reference;
	int dirPin, propPin, encoderPin, loops;
	bool lastDir;
	bool reverse;
	bool calpos;
	bool startpos;
	uint16_t changeTime;
	uint16_t calTime, lastChange;
	short calTimeReq;
	float currentPD;
	char Name[10];
	CalcPid* PID;
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
	void SetSp(int16_t _sp){PID->SetSp(_sp);};
	void SetAngles(short _min,short _max);
	void Save();
	void Init(const char* _name, bool _reverse, bool _calpos, bool _startpos);
	
	//constructor
	PropCyl(int _encoderPin, int _directionPin, int _proportionalPin,
		short _calibrationTime, short _encoderDirection,
		short _tolerance, struct JOINT* _JOINTobj, CalcPid* _PIDobj);
};//end PropCyl Class




#endif
