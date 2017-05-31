#ifndef Knobs_h
#define Knobs_h
#include <Arduino.h>
#include "EEPROMCustom.h"

struct KnobVals{
	int loc;
	short min;
	short max;
	KnobVals(){min=-1000;max=1000;};
};

class KnobClass{
	KnobVals values;
	byte calibrating, pin;
	bool Calibrated;
	short tempInput;
	short mapped, range;
	uint32_t calTime;
	char Name[8];
	
public:
	void PrintName(){Serial.print(Name);};
	const char* GetName(){return Name;};
	short Use();
	void Calibrate();
	bool IsCalibrated(){return Calibrated;};
	short Read();
	void Save();
	void Init(const char* _name,byte _pin,KnobVals* _vals);
	
	//constructor
	KnobClass(){};
	KnobClass(const char* _name,byte _pin,KnobVals* _vals);
};

#endif