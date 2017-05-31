#ifndef Sticks_h
#define Sticks_h
#include <Arduino.h>
#include "EEPROMCustom.h"

//structure to save/load values in EEPROM
struct StickVals{
	short min;
	short neutral;
	short max;
	byte reversed;
    
    /* loc is required */
    uint_16t loc;   //location in EEPROM
    
    //Constructor sets default values
	StickVals(){min=-1000;max=2000;reversed=0;};
};

class SticksClass{
    char Name[8];       //name of the stick
	byte pin;           //analog pin to read input
    bool reverse;       //values need to be reversed
    short deadband;     //range around neutral to read as neutral
    StickVals values;   //structure of calibration values
	
    byte calibrating;   //step in calibration sequence
    bool Calibrated;    //stick is calibrated =1, not =0
    
    bool cn;            //flag, added current neutral reading to average
    short calCount;     //number of values added to the neutral average
    short tempInput;    //temporary input before math or mapping
	short mapped;       //stick value mapped to 0:+1000, 500 is neutral
    short pos_range;    //size of positive stick range
    short neg_range;    //size of negative stick range
	long caltemp;       //netural calibration total
	uint32_t calTime;   //time the calibration step started
	
public:
	void PrintName(){Serial.print(Name);};  //print stick name
	const char* GetName(){return Name;};    //get stick name as char array
	short Deadband(short _value);           //apply deadband to value (must be in +/- format, 0 is neutral here)
	short Use();                            //reads or calibrates joystick
	void Calibrate();                       //begins calibration sequence
	bool IsCalibrated(){return Calibrated;};//returns whether stick is calibrated or not
	short Read();                           //reads stick, returns mapped value
	void Save();                            //save calibration values to EEPROM
	void Init(const char* _name,byte _pin,short _deadband,bool _reverse,StickVals* _vals);//set things
	
	//constructor
	SticksClass(){};    //When using this constructor, Init() must be called before interaction with stick
	SticksClass(const char* _name,byte _pin,short _deadband,bool _reverse,StickVals* _vals);
};

#endif