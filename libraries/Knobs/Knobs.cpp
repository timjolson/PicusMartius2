#include "Knobs.h"
#include <Arduino.h>
#include "EEPROMCustom.h"

#define PRINTCAL 0
#define PRINTKNOBS 0

KnobClass::KnobClass(const char* _name,byte _pin,KnobVals* _vals)
{
	Init(_name,_pin,_vals);
};

void KnobClass::Init(const char* _name,byte _pin,KnobVals* _vals)
{
	strcpy(Name,_name);
    pin=_pin;
	pinMode(pin,INPUT);
	values = *_vals;
	EEPROMinit(&values);
	range = values.max - values.min;
	
	//if eeprom init-ing is good, consider knob Calibrated
	if (0 <= values.min && values.min <= 50)	//min is reasonable
		if (970 <= values.max && values.max <= 1023)	//max is reasonable
			Calibrated = 1;
};

void KnobClass::Save()
{
	EEPROMwrite(values);
	//TODO: if write fails, give us error
};

void KnobClass::Calibrate(){calibrating=1;}

short KnobClass::Read()
{//read input from joystick
	//tempInput is set inside Use() = analogRead(pin)
	float temp = (int)tempInput - values.min;
	temp /= range;//current position
	mapped = temp * 255;	//scale between 0 and 255
	
	#if PRINTKNOBS
		Serial.print(Name);Serial.print('\t');
		Serial.print(tempInput);Serial.print('\t');
		Serial.print(temp);Serial.print('\t');
		Serial.print(mapped);Serial.print('\n');
	#endif
	return mapped;
};//end Read()

short KnobClass::Use()
{//calibrate inputs for min/max and neutral
	tempInput = analogRead(pin);
	switch (calibrating){
		case 1:	//start
			calTime = millis();
			values.max = -1000;
			values.min = 2000;
			Calibrated =0;
			calibrating++;//move to next step
			#if PRINTCAL
				Serial.println("Calibrate Knob Range");
			#endif
		break;
		case 2:	//min and max
			values.min = min(values.min,tempInput);	//save smaller value
			values.max = max(values.max,tempInput);	//save bigger value
			if (millis() - calTime > 9*1000){ //6 seconds have passed
				calibrating++;//move to next step
				range = values.max - values.min;
				calTime = millis();
				#if PRINTCAL
					Serial.print(Name);
					Serial.print("\tMin:");Serial.print(values.min);Serial.print('\t');
					Serial.print("Max:");Serial.print(values.max);Serial.println();
				#endif
			}
		break;
		case 3:
			calibrating = 0;		//no longer calibrating, so we can do other things
			Calibrated = 1;			//set stick as having been calibrated
			//todo: error check calibration
			Save();					//save values to EEPROM
		break;
		default:
			if (Calibrated)	//if stick has been calibrated
				return Read();	//do regular reading and mapping, return mapped value
			else return 0;
		break;
	}//end switch
	return 0;
};//end Use()