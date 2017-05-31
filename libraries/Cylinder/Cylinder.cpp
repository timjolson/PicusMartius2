#include "Cylinder.h"

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Arduino.h>
#include "Variables.h"
#include "EEPROMCustom.h"

extern uint32_t CurrentTime;
extern const short PWMmax, PWMmin, PWMneutral;

JOINT lowerJoint, upperJoint, wristJoint;
CalcPd lowerPID, upperPID, wristPID;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/****				loc     range	dynamic 		Kdyn	Kp			Ki		Kd*/
PIDstruct PID_lower=	{ 	0, 		0, 		_cyl::cUP, 		0,		0.02,		0, 		0.036,};
PIDstruct PID_upper=	{ 	0, 		0, 		_cyl::cUP, 		0,		0.015,	 	0, 		0.06,};
PIDstruct PID_wrist=	{ 	0, 		0, 		_cyl::cUP, 		0,		0.015,	 	0, 		0.15,};

/**				encoder pin		dir pin    prop pin    dirRev	cal time  	calpos		enc dir	 tolerance***/
//PropCyl cyl_lower(14/*A0-A1*/,		12,			9,			1,		3000,	_cyl::cUP,		1,		10);
Cylinder cyl_lower(14/*A0-A1*/,		12,			9,			1,		3000,	1,			10,		&lowerJoint,	&lowerPID);
//PropCyl cyl_upper(15/*A1-A2*/,		14,			11,			1,		3000,	_cyl::cUP,		-1,		10);
Cylinder cyl_upper(15/*A1-A2*/,		14,			11,			1,		3000,	-1,			10,		&upperJoint,	&upperPID);
//PropCyl cyl_wrist(16/*A2-A3*/,		15,			8,			1,		3000,	_cyl::cOUT,		1,		25);
Cylinder cyl_wrist(16/*A2-A3*/,		15,			8,			1,		3000,	1,			10,		&wristJoint,	&wristPID);


void ArmPWMinit(){
  pwm.begin();
  pwm.setPWMFreq(1000);
  uint8_t twbrbackup = TWBR;
  TWBR = 12;
};//end ArmPWMinit

Cylinder::Cylinder(int _encoderPin, int _directionPin, int _proportionalPin,
	bool _directionReverse, short _calibrationTime, short _encoderDirection,
	short _tolerance, struct JOINT* _JOINTobj, CalcPd* _PIDobj)
{	
	encoderPin = _encoderPin;	//encoder input pin
	dirPin = _directionPin;		//pin for directional valve
	propPin = _proportionalPin;		//pin for proportional exhaust valve
	reverse= _directionReverse;		//is the PID direction reversed
	calTimeReq = _calibrationTime;	//calibration time for each step
	encoderDir = _encoderDirection;	//multiplier to reverse encoder->position direction
	tolerance = _tolerance;		//encoder tolerance to be considered in position
	
	reference = *_JOINTobj;	//copy struct of EEPROM saved values
	PID = *_PIDobj;		//PID data
	
	//init things
	rawInput =500;
	rawLast =0;
	changeTime = 350;	//direction changing timeout
	calibrating = -1;	//not currently trying to calibrate
	calTime = -1;		//reset things for first calibration step
	Calibrated =0;		//whether or not the axis is calibrated
};

void Cylinder::Init(const char* _name, bool _reverse, bool _calpos, bool _startpos){
	_EEPROMinit(&reference);	//initially read struct
	
	calpos = _calpos;		//position to calibrate edge
	
	encoder = reference.encoder;//copy encoder info to object
	raw = reference.raw;		//copy raw info to object
	jointangles = reference.angle;//copy physical angle info to object
	
	strcpy(Name,_name);			//display name for motor
	
	pinMode(encoderPin, INPUT);	//encoder pin is input
	
	pwm.setPWM(propPin, 0, 2000); //set proportional pin half-way open
	pwm.setPWM(dirPin, 0, _startpos*PWMmax + (1-_startpos)*PWMmin);//set direction valve
	
	PID.SetSp(reverse?_reverse*1100:(1000-_reverse*1100));//beginning setpoint is -10% or 110%
};

void Cylinder::Calibrate(){
	switch (calibrating){
		case 0:
			//standby
			if (CurrentTime%900 == 0){
				Serial.print(Name); Serial.println(" ready");
			}
			calTime = -1;
		break;
		case 1://calibrate encoder values
			if (calTime == -1){//initialize this step
				Serial.println(Name);
				Serial.println("Calibrate Encoder");
				calTime = CurrentTime;
				//min will be reduced to actual, max will be increased to actual
				encoder.min=1500; encoder.max=0;
			}
			rawInput = analogRead(encoderPin);//read encoder voltage
		/**	active calibration		**/
			if (rawInput > encoder.max){//exceeded old max value
				calTime = CurrentTime;//start timer over
				encoder.max = rawInput;//update max
				encoder.range = encoder.max - encoder.min;//update range
				Serial.print("NewEncMax");Serial.println( encoder.max);
			}
			if (rawInput < encoder.min){//below old min value
				calTime = CurrentTime;//start timer over
				encoder.min = rawInput;//update min
				encoder.range = encoder.max - encoder.min;//update range
				Serial.print("NewEncMin");Serial.println( encoder.min);
			}
			if (calTime>0){
		/**	met time requirement?	**/
				if (CurrentTime - calTime > (uint16_t)calTimeReq){
					encoder.range = encoder.max - encoder.min;	//set range from new min and max
					calTime =  0;		//don't run more calibration, ready to start next step
					Serial.print("Min:");Serial.print("\t");Serial.print(encoder.min);Serial.print("\t");
					Serial.print("Max:");Serial.print("\t");Serial.print(encoder.max);Serial.print("\t");
					Serial.print("Range:");Serial.print("\t");Serial.print(encoder.range);Serial.print("\n");
				}// >calTimeReq
			}// >0
		break;
		
		case 2://calibrate range of motion
			if (calTime == 0){//initialize this step
				Serial.println(Name);
				Serial.println("Calibrate Range");
				calTime = CurrentTime;
				//min will be reduced to actual, max will be increased to actual
				raw.min=60000; raw.max=-60000;
			}
			Read();//read joint using calibrated encoder
		/**	active calibration		**/
			if (rawPos > raw.max){ //exceeded old max value
				calTime = CurrentTime;//start timer over
				raw.max = rawPos;//update max
				raw.range = raw.max - raw.min;	//set range from new min and max
				Serial.print("NewRawMax");Serial.println( raw.max);
			}
			if (rawPos < raw.min){//below old min value
				calTime = CurrentTime;//start timer over
				raw.min = rawPos;//update min
				raw.range = raw.max - raw.min;//set range from new min and max
				Serial.print("NewRawMin");Serial.println( raw.min);
			}
			if (calTime >0){
		/**	met time requirement?	**/
				if (CurrentTime - calTime > (uint16_t)calTimeReq){
					raw.range = raw.max - raw.min;		//set range from new min and max
					calTime =  -1;		//don't run more calibration
					Serial.print("Min:");Serial.print("\t");Serial.print(raw.min);Serial.print("\t");
					Serial.print("Max:");Serial.print("\t");Serial.print(raw.max);Serial.print("\t");
					Serial.print("Range:");Serial.print("\t");Serial.print(raw.range);Serial.print("\n");
				}// >calTimeReq
			}// >0
		break;
		
		case 3://calibrate range of motion from edge
			if (calTime == -1){//initialize this step
				Serial.println(Name);
				Serial.println("Calibrate Edge");
				calTime = CurrentTime;
				//min will be reduced to actual, max will be increased to actual
				position.min = 60000;position.max = -60000;
			}
			if (calpos==_cyl::cDOWN){//if calibration position is down
				GoToMin();
		/**	active calibration		**/
				if (rawPos < position.min){//below old min value
					calTime = CurrentTime;//start timer over
					position.min = rawPos;//update min
					position.max = position.min + raw.range;//update max from edge and min
					Serial.print("NewPosMin");Serial.println( position.min);
				}
				if (calTime >0){
		/**	met time requirement?	**/
					if (CurrentTime - calTime > (uint16_t)calTimeReq){
						pwm.setPWM(propPin, 0, PWMmin);
						position.range = position.max - position.min;//update range
						calTime =  0;	//don't run more calibration
						Serial.print("Min:");Serial.print("\t");Serial.print(position.min);Serial.print("\t");
						Serial.print("Max:");Serial.print("\t");Serial.print(position.max);Serial.print("\t");
						Serial.print("Range:");Serial.print("\t");Serial.print(position.range);Serial.print("\n");
						Calibrated =1;
					}// >calTimeReq
				}// >0
			}//end if down
			else{//calibration position up
				GoToMax();
		/**	active calibration		**/
				if (rawPos > position.max){ //exceeded old max value
					calTime = CurrentTime;//start timer over
					position.max = rawPos;//update max
					position.min = position.max - raw.range;//update min from edge and max
					Serial.print("NewPosMax");Serial.println( position.max);
				}
				if (calTime >0){
		/**	met time requirement?	**/
					if (CurrentTime - calTime > (uint16_t)calTimeReq){
						pwm.setPWM(propPin, 0, PWMmin);
						position.range = position.max - position.min;//update range
						calTime =  0;	//don't run more calibration
						Serial.print("Min:");Serial.print("\t");Serial.print(position.min);Serial.print("\t");
						Serial.print("Max:");Serial.print("\t");Serial.print(position.max);Serial.print("\t");
						Serial.print("Range:");Serial.print("\t");Serial.print(position.range);Serial.print("\n");
						Calibrated =1;
					}// >calTimeReq
				}// >0
			}//end not up
		break;
		
		default:
			Read();
			calTime =-1;		//ready to start either step 1 or step 3
			if (CurrentTime%900 == 0){
				Serial.print(Name); Serial.println(" done");
			}
			calibrating = -1;	//no longer calibrating
		break;
		
	}
};//end Calibrate

void Cylinder::Read(){
	rawInput = analogRead(encoderPin);
		
	int diff = rawInput - rawLast;		//change in position
	if (abs(diff) > encoder.range/2){
	//rolled over (difference between consecutive readings is more than 1/2 revolution)
		if (diff < 0) loops -=1;	//keep track of revolutions
		else loops +=1;
	}
	//calculate actual position of joint, revolutions*counts per revolution + shift for current reading
	rawPos = loops * encoder.range*encoderDir - encoderDir*rawInput;
	
	// ratio of current position / total positional range
	float ratio = (rawPos - position.min)/(float)position.range;
	pos = ratio*1000;	//percent * 10
	angle = jointangles.min + ratio * jointangles.range;
	
	//if (PRINT && CurrentTime%500 == 100) Serial.println(pos);
	
	rawLast = rawInput;
};//end Read

void Cylinder::GoToMin(){
	Read();		//always keep track of joint position
	
	//go up
	if (!reverse) pwm.setPWM(dirPin, 0, PWMmax);
	//go down
	else pwm.setPWM(dirPin, 0, PWMmin);
	
	//write speed
	pwm.setPWM(propPin, 0, 2500 );
}

void Cylinder::GoToMax(){
	Read();		//always keep track of joint position
	
	//go down
	if (!reverse) pwm.setPWM(dirPin, 0, PWMmin);
	//go up
	else pwm.setPWM(dirPin, 0, PWMmax);
	
	//write speed
	pwm.setPWM(propPin, 0, 2500 );
}

void Cylinder::Hold(){
	Read();		//always keep track of joint position
	
	//set speed to 0
	pwm.setPWM(propPin, 0, 0 );
	lastChange = CurrentTime;
}

void Cylinder::Move(){
	Read();		//always keep track of joint position
	
	//if not override
		//use PID to set speed
		//use PID to set dir up/down
	//if override
		//use speed
		//use direction
	
	if (!override){
		PID.Enable();				//keep calculations going
		PID.SetDisp(pos);			//set current displacement
		currentPD = PID.GetPd();	//get pid calculation
		
		int err = PID.GetError();	//how far off we are
		short sign = currentPD<0.01?0:(abs(currentPD)/currentPD);//if PID value !~0, get sign
		if((abs(err) < tolerance)||(sign==0)){
		//if sign==0, or we are within tolerance range
			Hold();
			//if (err>0)	//pwm.setPWM(dirPin, 0, (reverse)?PWMmax:0);//set direction valve
			//	goDir(PID.GetKdyn());
			//else //pwm.setPWM(dirPin, 0, (!reverse)?PWMmax:0);//set direction valve
			//	goDir(!PID.GetKdyn());
			//PID.SetCtrl();
		//	if (CurrentTime%300 == 0)
		//		Serial.println("_");	//debug print
		}
		else if (sign > 0){
			//set direction
			GoDir(_cyl::cUP);
			
			//set speed
			pwm.setPWM(propPin, 0, abs(currentPD*PWMmax));
			
			//if (CurrentTime%300 == 0)
			//	Serial.println("^");	//debug print
			lastDir = sign;
		}
		else if (sign < 0){
			GoDir(_cyl::cDOWN);//set direction
			
			pwm.setPWM(propPin, 0, abs(currentPD*PWMmax));//set speed
			
			//if (CurrentTime%300 == 0)
			//	Serial.println("v");	//debug print
			lastDir = sign;
		}
		
	}
	else{
		PID.Disable();
	}


	if (PRINT){
		if (CurrentTime%300==0){
			Serial.print(pos);
			Serial.print('\t');
			Serial.println(currentPD);
		}
	}
};//end Move

void Cylinder::Save(){
	reference.encoder = encoder;	//save encoder info, its important
	reference.raw.range = raw.range;//save range, its important
	reference.raw.min = 5000;	//set, save EEPROM
	reference.raw.max = 0;		//set to zero, save EEPROM
	reference.angle = jointangles;	//set to zeros so the EEPROM is burned out from randoms
	Serial.println(Name);		//what joint is trying to save
	EEPROMwrite(reference);		//save it (provides error/success print)
};//end Save

void Cylinder::SetAngles(short _min, short _max){
	if (calibrating!=-1){
		//set joint angles
		jointangles.min = _min;
		jointangles.max = _max;
		jointangles.range = _max - _min;
		
		Serial.println(Name);
		Serial.print("Min:");Serial.print("\t");Serial.print(jointangles.min);Serial.print("\t");
		Serial.print("Max:");Serial.print("\t");Serial.print(jointangles.max);Serial.print("\t");
		Serial.print("Range:");Serial.print("\t");Serial.print(jointangles.range);Serial.print("\n");
	}
};//end SetAngles

void Cylinder::Step(byte _startstep){
	if (calibrating!=-1){//if we're ready to calibrate
		if (_startstep) calibrating = _startstep;	//start with a certain step (3 -> edge)
		else{						//if given 0, we simply go to the next step
			calTime = calTimeReq;	//set time so we're done with this step
			Calibrate();			//run calibration function to complete the step
			calibrating++;			//move to next step
		}
	}
};//end Step

void Cylinder::Operate(){
	if (calibrating!=-1){	//calibrating is in standby or running
		Calibrate();		//run calibration sequence
	}
	else if (Calibrated==1){//not currently calibrating, and calibration is good
		Move();				//move via PID
	}
};//end Operate

void Cylinder::GoDir(bool _dir){
	if (CurrentTime - lastChange > (uint16_t)changeTime){//if we've wanted to go this direction for minimum time
		pwm.setPWM(dirPin, 0, (reverse)?_dir*PWMmax:(PWMmax - _dir*PWMmax));//set direction valve
		lastChange = CurrentTime;		//when we last changed direction
	}
};//end GoDir