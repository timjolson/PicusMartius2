#include "MotorAxis.h"

#include <Arduino.h>
#include <Servo.h>
#include "Variables.h"
#include "EEPROMCustom.h"

extern uint32_t CurrentTime;
extern const short ServoMax, ServoMin, ServoNeutral;
extern Servo* Servos;
extern int NextServoNum;

JOINT gripperJoint;	//place to save joint values
CalcPd gripperPID;	//object to calculate PID

/****					loc     range	dynamic		Kdyn	Kp		Ki		Kd*/
PIDstruct PID_gripper={0, 		0,		0, 			0, 		0.005,	0.0, 	0.47,};

/**					encoder pin		signal pin  dirRev	encDir	toler	***/
MotorAxis mot_gripper(14/*A0-A1*/,	2,			0,		1,		10,		&gripperJoint, &gripperPID);


MotorAxis::MotorAxis(int _encoderPin, int _signalPin, bool _reverse, 
	bool _encoderDirection ,short _tolerance, struct JOINT* _JOINTobj, CalcPd* _PIDobj)
{
	encoderPin = _encoderPin;	//encoder input pin
	sigPin = _signalPin;		//signal(pwm or servo) output pin
	reverse= _reverse;		//if the motor uses reverse operation
	encoderDir = _encoderDirection;	//encoder direction (multipier, 1 or -1)
	tolerance = _tolerance;		//tolerance of encoder counts to be considered in position
	servoNum = NextServoNum++;	//index to reference servo array
	
	reference = *_JOINTobj;	//copy struct of EEPROM saved values
	PID = *_PIDobj;				//PID data
	
	//init things
	pinMode(encoderPin,INPUT);
	rawInput =500;
	rawLast =0;
	Calibrated =0;		//whether or not the axis is calibrated
	calibrating = -1;	//not currently trying to calibrate
};

void MotorAxis::Init(const char* _name, bool _calPos){
//void MotorAxis::Init(struct JOINT* _J, CalcPd* _PID, const char* _name, bool _calPos){
	_EEPROMinit(&reference);	//initially read struct
	
	calPos = _calPos;	//calibrate edge position
	
	encoder = reference.encoder;//copy encoder info to object
	raw = reference.raw;		//copy raw info to object
	
	strcpy(Name,_name);			//display name for motor
	
	pinMode(encoderPin, INPUT);	//encoder pin is input
	pinMode(sigPin, OUTPUT);	//servo pin is output
	
	Servos[servoNum].attach(sigPin);//servo operates on pin
	PID.SetSp(500);				//beginning setpoint is 50%
};

void MotorAxis::Write(int _val){
	Servos[servoNum].write(_val);	//write value to servo
};

void MotorAxis::Calibrate(){
	switch (calibrating){
		case 0://standby
			if (CurrentTime%900 == 0){
				Serial.print(Name); Serial.println(" ready");
			}
			Calibrated=0;		//set status as no longer calibrated
			Write(ServoNeutral);//don't move motor
		break;
		case 1://reset encoder values
			Serial.print(Name);Serial.println("::\tCalibrate Encoder");
			//min will be reduced to actual, max will be increased to actual
			encoder.min=1500; encoder.max=0;
			calibrating++;	//next step
		break;
		case 2://calibrate encoder values
			rawInput = analogRead(encoderPin);//read encoder voltage
		/**	active calibration		**/
			if (rawInput > encoder.max){//exceeded old max value
				encoder.max = rawInput;//update max
				encoder.range = encoder.max - encoder.min;//update range
				Serial.print("NewEncMax");Serial.println( encoder.max);
			}
			if (rawInput < encoder.min){//below old min value
				encoder.min = rawInput;//update min
				encoder.range = encoder.max - encoder.min;//update range
				Serial.print("NewEncMin");Serial.println( encoder.min);
			}
			CalibrateMove();	//allow hard commanded movement
		break;
		case 3://set encoder range, print values
			Write(ServoNeutral);	//don't move motor
			encoder.range = encoder.max - encoder.min;	//set range from new min and max
			Serial.print("Min:");Serial.print("\t");Serial.print(encoder.min);Serial.print("\t");
			Serial.print("Max:");Serial.print("\t");Serial.print(encoder.max);Serial.print("\t");
			Serial.print("Range:");Serial.print("\t");Serial.print(encoder.range);Serial.print("\n");
			
			//prep to calibrate range of motion
			Serial.print(Name);Serial.println("::\tCalibrate Range");
			//min will be reduced to actual, max will be increased to actual
			raw.min=60000; raw.max=-60000;
			calibrating++;	//next step
		break;
		case 4://calibrate raw range of motion
			Read();//read joint using calibrated encoder
		/**	active calibration		**/
			if (rawPos > raw.max){ //exceeded old max value
				raw.max = rawPos;//update max
				raw.range = raw.max - raw.min;	//set range from new min and max
				Serial.print("NewRawMax");Serial.println( raw.max);
			}
			if (rawPos < raw.min){//below old min value
				raw.min = rawPos;//update min
				raw.range = raw.max - raw.min;//set range from new min and max
				Serial.print("NewRawMin");Serial.println( raw.min);
			}
			CalibrateMove();	//allow hard commanded movement
		break;
		case 5://set raw range
			Write(ServoNeutral);	//don't move motor
			raw.range = raw.max - raw.min;		//set range from new min and max
			Serial.print("Min:");Serial.print("\t");Serial.print(raw.min);Serial.print("\t");
			Serial.print("Max:");Serial.print("\t");Serial.print(raw.max);Serial.print("\t");
			Serial.print("Range:");Serial.print("\t");Serial.print(raw.range);Serial.print("\n");
			calibrating++;	//next step
		break;
		case 6://prep to calibrate edge
			Serial.print(Name);Serial.println("::\tCalibrate Edge");
			//min will be reduced to actual, max will be increased to actual
			position.min = 60000;position.max = -60000;
			calibrating++;	//next step
		break;
		case 7://calibrate edge
			Read();//read joint using calibrated encoder
			if (calPos==_motor::mOUT){		//if position to calibrate from is motor out
				if (rawPos > position.max){	//max position exceeded
					position.max = rawPos;	//update max
					position.min = position.max - raw.range;//update min
					Serial.print("NewPosMax");Serial.println(position.max);
				}
			}
			else //_motor::mIN
			{
				if (rawPos < position.min){	//min position exceeded
					position.min = rawPos;	//update min
					position.max = position.min + raw.range;//update max
					Serial.print("NewPosMin");Serial.println(position.min);
				}
			}
			CalibrateMove();	//allow hard commanded movement
		break;
		case 8://set edge and range
			Write(ServoNeutral);		//don't move motor
			position.range = raw.range;	//update position range
			Serial.print(Name); Serial.println("::\tdone");
			
			calibrating=-1;	//return to inactive calibration
			Calibrated=1;	//status is calibrated
			Save();			//save info to EEPROM
		break;
		
		default:
			Read();				//read encoder values, etc
			Write(ServoNeutral);//don't move motor
			calibrating = -1;	//no longer calibrating
		break;
		
	}
};//end Calibrate

void MotorAxis::Read(){
	rawInput = analogRead(encoderPin);	//read encoder/potentiometer input
	
	int diff = rawPos - rawLast;		//change in position
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
	
	//if (PRINT && CurrentTime%500 == 100) Serial.println(pos);
	
	rawLast = rawInput;
};//end Read

void MotorAxis::Move(){
	Read();		//always keep track of joint position
	
	//if not override
		//use PID to set speed
		//use PID to set dir up/down
	//if override
		//use speed
		//use direction
	
	if (!override){
		PID.Enable();				//keep calculations going/start if stopped
		PID.SetDisp(pos);			//set current displacement
		currentPD = PID.GetPd();	//get pid calculation
		
		int err = PID.GetError();	//get how far off we are
		short sign = currentPD<0.01?0:(abs(currentPD)/currentPD);//if PID value !~0, get sign
		if((abs(err) < tolerance)||(sign==0)){
		//if sign==0, or we are within tolerance range
			Write(ServoNeutral);	//dont move motor
			
			PID.SetCtrl();	//set PID control to zero
		//	if (CurrentTime%300 == 0)
		//		Serial.println("_");	//debug print
		}
		else if (sign > 0){
			if (pos < position.max){	//not limited by position
				Write(ServoNeutral+currentPD*ServoNeutral); //write PID speed
			}
			else {
				Write(ServoNeutral);	//dont move motor
			}
		
		//	if (CurrentTime%300 == 0)
		//		Serial.println("^");	//debug print
		}
		else //if (sign < 0)
		{
			if (pos > position.min){	//not limited by position
				Write(ServoNeutral+currentPD*ServoNeutral);	//write PID speed
			}
			else	//limited by position
				Write(ServoNeutral);	//dont move motor
			
		//	if (CurrentTime%300 == 0)
		//		Serial.println("v");	//debug print
		}	
	}
	else{
		PID.Disable();	//set PID values to zero, so it wont move
	}
	
	if (PRINT){
		if (CurrentTime%300==0){
			Serial.print(pos);
			Serial.print('\t');
			Serial.println(currentPD);
		}
	}
}//end Move

void MotorAxis::Save(){
	reference.encoder = encoder;	//save encoder info, its important
	reference.raw.range = raw.range;//save range, its important
	reference.raw.min = 5000;	//set, save EEPROM
	reference.raw.max = 0;		//set to zero, save EEPROM
	reference.angle = {0,0,0};	//set to zeros so the EEPROM is burned out from randoms
	Serial.println(Name);		//what joint is trying to save
	EEPROMwrite(reference);		//save it (provides error/success print)
};//end Save

void MotorAxis::Step(byte _startstep){
	if (calibrating!=-1){//if we're ready to calibrate
		if (_startstep) calibrating = _startstep;	//start with a certain step (asdf# -> edge)
		else{					//if given 0, we simply go to the next step
			Calibrate();		//run calibration function to complete the step
			calibrating++;		//move to next step
		}
	}
};//end Step

void MotorAxis::Operate(){
	if (calibrating!=-1){	//calibrating is in standby or running
		CalibrateMove();	//allow hard commanded movements
		Calibrate();		//run calibration sequence
	}
	else if (Calibrated==1){//not currently calibrating, and calibration is good
		Move();				//move via PID
	}
};//end Operate

void MotorAxis::CalibrateMove(){
	if (direction==_motor::mOUT)	//if intended direction is motor out
	{
		if (CurrentTime - lastChange < 220)		//been running less than 220 millis	
			Write(reverse?ServoNeutral+0.3*ServoNeutral:ServoNeutral-0.3*ServoNeutral);
			//write 30% speed - if reverse, write other direction
		else
			Write(ServoNeutral);	//don't run motor
	}
	else	//intended direction is motor in
	{
		if (CurrentTime - lastChange < 220)		//been running less than 220 millis
			Write(reverse?ServoNeutral-0.3*ServoNeutral:ServoNeutral+0.3*ServoNeutral);
			//write 30% speed - if reverse, write other direction
		else
			Write(ServoNeutral);	//don't run motor
	}
	
	if (PRINT){
		if (CurrentTime%300==0){
			Serial.println(pos);
		}
	}
};//end CalibrateMove

void MotorAxis::GoDir(bool _dir){
	direction = _dir;			//set direction to move
	lastChange = CurrentTime;	//set start time of move
};//end GoDir