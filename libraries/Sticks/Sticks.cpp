#include "Sticks.h"
#include <Arduino.h>
#include "EEPROMCustom.h"

//print calibration steps and values on Serial
#define PRINTCAL 1
//print live stick values
#define PRINTSTICKS 0

SticksClass::SticksClass(const char* _name,byte _pin,short _db,bool _reverse,StickVals* _vals)
{
	Init(_name,_pin,_db,_reverse,_vals);
}

void SticksClass::Init(const char* _name,byte _pin,short _db,bool _reverse,StickVals* _vals)
{
	strcpy(Name,_name); //stick name
    pin=_pin;           //analog pin
    deadband=_db;       //distance from neutral to be set = 0
	reverse=_reverse;   //whether to reverse the values
    pinMode(pin,INPUT); //set pin as input
	
    values = *_vals;    //structure to load/save min, max, neutral
	EEPROMinit(&values);//initialize the structure
	pos_range = values.max - values.neutral;    //set positive range of stick
	neg_range = values.neutral - values.min;    //set negative range of stick
	
	//if eeprom init-ing is good, consider stick Calibrated
	if (0 <= values.min && values.min <= 150)	//min is reasonable
		if (800 <= values.max && values.max <= 900)	//max is reasonable
			if (440 <= values.neutral && values.neutral <= 520)	//neutral is reasonable
				Calibrated = 1;
};

void SticksClass::Save()
{
	EEPROMwrite(values);    //save stick calibration to EEPROM
	//TODO if write fails, give us error
};

//Deadband(value) check to see if "value" is within "deadband" of zero
//and returns either 0 if it is, or "value" if it is not.
short SticksClass::Deadband(short _value)
{
	if(abs(_value) < deadband) return 0;
	else return _value;
};

//Begin calibrating sequence/mode
void SticksClass::Calibrate()
{
    calibrating=1;
};

//read input from stick, includes mapping
short SticksClass::Read()
{
	//tempInput is set inside Use(); = analogRead(pin)
	tempInput -= values.neutral;	//change to +/- range
	tempInput = Deadband(tempInput);	//apply deadband
	
	//adjust for non-center neutral
	if (!reverse)	//is stick not reversed?
	{
		if (tempInput<0)mapped=map(tempInput,-1*neg_range,0,0,500);		//full negative range mapped to % * 10
		else mapped=map(tempInput,0,pos_range,500,1000);				//full positive range mapped to % * 10
	}
	else	//stick is reversed
	{
		if (tempInput<0)mapped=map(tempInput,0,-1*neg_range,500,1000);	//full negative range mapped to % * 10, axis reversed
		else mapped=map(tempInput,0,pos_range,500,0);				//full positive range mapped to % * 10, axis reversed
	}
	#if PRINTSTICKS
	Serial.print(Name);Serial.print('\t');
	Serial.println(mapped);
	#endif
	return mapped;  //return value mapped from 0 to +1000, 500 being neutral
};//end Sticks Read()

short SticksClass::Use()
{
	tempInput = analogRead(pin);    //read input
	uint32_t Time = millis();       //get current processor time
    
    //calibrate inputs for min/max and neutral
	switch (calibrating){//switch based on calibrating, steps from 1 to 4\
                            0 reads and maps the stick and does not calibrate
		case 1:	//start
			caltemp = 0;//reset stored value
			calTime = Time;     //set time the calibrating started
			calCount = 0;       //counts calibration cycles(used for neutral)
			values.max = -1000; //any new value will be above this
			values.min = 2000;  //any new value will be below this
			Calibrated =0;  //set as not being calibrated
			calibrating++;//move to next step
			#if PRINTCAL
			Serial.println("Calibrate Stick Range");
			#endif
		break;
		case 2:	//set min and max
			values.min = min(values.min,tempInput);	//save smaller value
			values.max = max(values.max,tempInput);	//save bigger value
			if (Time - calTime > 6*1000){ //6 seconds have passed
				calibrating++;  //move to next step
				calTime = Time; //set time the next step starts
				#if PRINTCAL
                //prompt to put stick in neutral position
				Serial.println("Calibrate Neutrals");
				#endif
			}
		break;
		case 3:	//set neutrals
			if (((Time - calTime)%200 ==0)&&cn==0){//elapsed time is 200ms, have not added this instance yet
				caltemp += tempInput;	//every 200 ms add to averaging
				calCount++;			    //how many values will we average
				//Serial.print('\t');Serial.println(calCount);
				cn = 1;	//we've added this instance, don't do it again
			}
			else cn=0;	//reset adding ability when time no longer == 200ms
			
            if (Time - calTime > 4*1000){ //4 seconds have passed
				values.neutral = caltemp / calCount;	//average the neutral value
				calibrating++;		//move to next step
				calTime = Time;     //set next step start time
			}
		break;
		case 4://print and store values
			#if PRINTCAL
				Serial.print(Name);
				Serial.print("\tMin:");Serial.print(values.min);Serial.print('\t');
				Serial.print("Max:");Serial.print(values.max);Serial.print('\t');
				Serial.print("Neutral:"); Serial.println(values.neutral);
			#endif
			pos_range = values.max - values.neutral;	//set size of positive range on stick
			neg_range = values.neutral - values.min;	//set size of negative range on stick
			calibrating = 0;    //no longer calibrating, so we can do other things
			Calibrated = 1;	    //set stick as having been calibrated
			Save();				//save values to EEPROM
		break;
		default:
			if (Calibrated)	//if stick has been calibrated
				return Read();	//do regular reading and mapping, return mapped value
			else return 500;
		break;
	}//end switch
	return 500;
};//end SticksClass::Use()

