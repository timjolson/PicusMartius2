#include "PropCyl.h"

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Arduino.h>
#include "Variables.h"
#include "EEPROMCustom.h"

extern uint32_t CurrentTime;
extern const short PWMmax, PWMmin, PWMneutral;

JOINT lowerJoint, upperJoint, wristJoint;
CalcPid lowerPID, upperPID, wristPID;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/****					range	kp		ki		kd		respTime*/
PIDstruct PID_lower={ 	0, 		0.002, 	0.000, 	0.26,		50, };
PIDstruct PID_upper={ 	0, 		0.003, 	0.0, 	0.64,		50, };
PIDstruct PID_wrist={ 	0, 		0.001, 	0.0, 	0.01,		50, };

/**				encoder pin		dir pin    prop pin    cal time 	enc dir	 tolerance	joint info		pid values***/
PropCyl cyl_lower(A1/*A13*/,		1,			2,		3000,		1,			25,		&lowerJoint,	&lowerPID);
PropCyl cyl_upper(A2/*A14*/,		12,			13,		3000,		-1,			30,		&upperJoint,	&upperPID);
PropCyl cyl_wrist(A3/*A15*/,		4,			15,		3000,		1,			25,		&wristJoint,	&wristPID);


void ArmPWMinit(){
	pwm.begin();
	pwm.setPWMFreq(1000);
	uint8_t twbrbackup = TWBR;
	TWBR = 12;
	
	pwm.setPWM(0 , 0, 4095);
	pwm.setPWM(3 , 0, 4095);
	pwm.setPWM(14 , 0, 4095);
	pwm.setPWM(5 , 0, 4095);
};//end ArmPWMinit

PropCyl::PropCyl(int _encoderPin, int _directionPin, int _proportionalPin,
	short _calibrationTime, short _encoderDirection,
	short _tolerance, struct JOINT* _JOINTobj, CalcPid* _PIDobj)
{	
	encoderPin = _encoderPin;	//encoder input pin
	dirPin = _directionPin;		//pin for directional valve
	propPin = _proportionalPin;		//pin for proportional exhaust valve
	calTimeReq = _calibrationTime;	//calibration time for each step
	encoderDir = _encoderDirection;	//multiplier to reverse encoder->position direction
	tolerance = _tolerance;		//encoder tolerance to be considered in position
	
	reference = *_JOINTobj;	//copy struct of EEPROM saved values
	PID = _PIDobj;		//PID data
	PID->SetAsSmooth();	//keeps control value when setpoint is changed
	//init things
	rawInput =500;
	changeTime = 150;	//direction changing timeout
	calibrating = -1;	//not currently trying to calibrate
	calTime = -1;		//reset things for first calibration step
	Calibrated =0;		//whether or not the axis is calibrated
};

void PropCyl::Init(const char* _name, bool _reverse, bool _calpos, bool _startpos){
	EEPROMinit(&reference);	//initially read struct
	
	calpos = _calpos;		//position to calibrate edge
	startpos = _startpos;	//start position after calibrations
	reverse= _reverse;		//is the PID direction reversed
	encoder = reference.encoder;//copy encoder info to object
	raw = reference.raw;		//copy raw info to object
	jointangles = reference.angle;//copy physical angle info to object
	
	strcpy(Name,_name);			//display name for motor
	
	pinMode(encoderPin, INPUT);	//encoder pin is input
	
	pwm.setPWM(propPin, 0, 2000); //set proportional pin half-way open
	pwm.setPWM(dirPin, 0, reverse?_startpos*PWMmax:(1-_startpos)*PWMmax);//set direction valve
	
};

void PropCyl::Calibrate(){
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
				Serial.print(Name);
				Serial.println(" Calibrate Encoder");
				calTime = CurrentTime;
				//min will be reduced to actual, max will be increased to actual
				encoder.min=8000; encoder.max=-1000;
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
				raw.min=8000; raw.max=-6000;
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
				position.min = 6000;position.max = -6000;
			}
			if (calpos==cDOWN){//if calibration position is down
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
			PID->Enable();	//allow control
			PID->SetSp(reverse?(1100-startpos*1200):-100+startpos*1200);//beginning setpoint is -10% or 110%
			Read();
			calTime =-1;		//ready to start either step 1 or step 3
			Serial.print(Name); Serial.println(" done");
			calibrating = -1;	//no longer calibrating
		break;
		
	}
};//end Calibrate

void PropCyl::Read(){
	short rawLast = rawInput;
	rawInput = analogRead(encoderPin);
	
	short diff = rawInput - rawLast;		//change in position
	if (abs(diff) > encoder.range/2){
	//rolled over (difference between consecutive readings is more than 1/2 revolution)
		if (diff < 0) loops -=1;	//keep track of revolutions
		else loops +=1;
	}
	//calculate actual position of joint, revolutions*counts per revolution + shift for current reading
	rawPos = loops * encoder.range*encoderDir - encoderDir*rawInput;
	
	// ratio of current position / total positional range
	float ratio = (rawPos - position.min)/(float)position.range;
	pos = ratio>0?ratio*1000:0;	//percent * 10
	angle = jointangles.min + ratio * jointangles.range;
	
	rawLast = rawInput;
};//end Read

void PropCyl::GoToMin(){
	Read();		//always keep track of joint position
	
	//go up
	if (!reverse) pwm.setPWM(dirPin, 0, PWMmax);
	//go down
	else pwm.setPWM(dirPin, 0, PWMmin);
	
	//write speed
	pwm.setPWM(propPin, 0, 2500 );
}

void PropCyl::GoToMax(){
	Read();		//always keep track of joint position
	
	//go down
	if (!reverse) pwm.setPWM(dirPin, 0, PWMmin);
	//go up
	else pwm.setPWM(dirPin, 0, PWMmax);
	
	//write speed
	pwm.setPWM(propPin, 0, 2500 );
}

void PropCyl::Hold(){
	Read();		//always keep track of joint position
	
	//set speed to 0
	pwm.setPWM(propPin, 0, 0 );
	lastChange = CurrentTime;
}

void PropCyl::Move(){
	Read();		//always keep track of joint position
	
	//use PID to set speed
	//use PID to set dir up/down
	
	PID->SetDisp(pos);			//set current displacement
	currentPD = PID->GetPid();	//get pid calculation
	
	short err = PID->GetError();	//how far off we are
	short sign = abs(currentPD)<0.01?0:(abs(currentPD)/currentPD);//if PID value !~0, get sign
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
		GoDir(cUP);
			
		//set speed
		pwm.setPWM(propPin, 0, int(abs(currentPD*PWMmax)));
			
		//if (CurrentTime%300 == 0)
		//	Serial.println("^");	//debug print
		lastDir = sign;
	}
	else if (sign < 0){
		GoDir(cDOWN);//set direction
			
		pwm.setPWM(propPin, 0, int(abs(currentPD*PWMmax)));//set speed
			
		//if (CurrentTime%300 == 0)
		//	Serial.println("v");	//debug print
		lastDir = sign;
	}


	if (PRINT){
		if (CurrentTime%300==0){
			Serial.print(pos);
			Serial.print('\t');
			Serial.println(currentPD);
		}
	}
};//end Move

void PropCyl::Save(){
	reference.encoder = encoder;	//save encoder info, its important
	reference.raw.range = raw.range;//save range, its important
	reference.raw.min = 5000;	//set, save EEPROM
	reference.raw.max = 0;		//set to zero, save EEPROM
	reference.angle = jointangles;	//set to zeros so the EEPROM is burned out from randoms
	Serial.print("Saved ");
	Serial.println(Name);		//what joint is trying to save
	EEPROMwrite(reference);		//save it (provides error/success print)
};//end Save

void PropCyl::SetAngles(short _min, short _max){
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

void PropCyl::Step(byte _startstep){
	if (calibrating!=-1){//if we're ready to calibrate
		if (_startstep) calibrating = _startstep;	//start with a certain step (3 -> edge)
		else{						//if given 0, we simply go to the next step
			calTime = calTimeReq;	//set time so we're done with this step
			Calibrate();			//run calibration function to complete the step
			calibrating++;			//move to next step
		}
	}
};//end Step

void PropCyl::Operate(){
	if (calibrating!=-1){	//calibrating is in standby or running
		Calibrate();		//run calibration sequence
	}
	else if (Calibrated==1){//not currently calibrating, and calibration is good
		Move();				//move via PID
	}
};//end Operate

void PropCyl::GoDir(bool _dir){
	if (CurrentTime - lastChange > (uint16_t)changeTime){//if we've wanted to go this direction for minimum time
		pwm.setPWM(dirPin, 0, (reverse)?_dir*PWMmax:(1 - _dir)*PWMmax);//set direction valve
		lastChange = CurrentTime;		//when we last changed direction
	}
	
};//end GoDir