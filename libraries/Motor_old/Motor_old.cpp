#include "Motor.h"

#include <Arduino.h>
#include <Servo.h>
#include "Variables.h"
#include "../Button/Button.h"

extern uint32_t CurrentTime;
extern const short ServoMax, ServoMin, ServoNeutral;
extern Servo* Servos;
extern int NextServoNum;

/**			signal pin	minPin	maxPin***/
Motor generic_mot(2,		3,		4);

Motor::Motor(int _signalPin, int _minPin, int _maxPin)
{
	sigPin = _signalPin;		//signal(pwm or servo) output pin
	servoNum = NextServoNum++;
	minPin = _minPin;
	maxPin = _maxPin;
};

void Motor::Init(const char* _name, bool _reverse){
	strcpy(Name,_name);			//display name for motor
	reverse= _reverse;		//if the motor uses reverse operation
	
	pinMode(sigPin, OUTPUT);	//servo pin is output
	pinMode(minPin, INPUT);
	pinMode(maxPin, INPUT);
	
	Servos[servoNum].attach(sigPin);//servo operates on pin
};

void Motor::Write(int _val){
	Servos[servoNum].write(_val);	//write value to servo
};

void Motor::Calibrate(){

	//~ switch (calibrating){
		//~ case 0://standby
			//~ if (CurrentTime%900 == 0){
				//~ Serial.print(Name); Serial.println(" ready");
			//~ }
			//~ Calibrated=0;		//set status as no longer calibrated
			//~ Write(ServoNeutral);//don't move motor
		//~ break;
		//~ case 1://reset encoder values
			//~ Serial.print(Name);Serial.println("::\tCalibrate Encoder");
			//~ //min will be reduced to actual, max will be increased to actual
			//~ encoder.min=1500; encoder.max=0;
			//~ calibrating++;	//next step
		//~ break;
		//~ case 2://calibrate encoder values
			//~ rawInput = analogRead(encoderPin);//read encoder voltage
		//~ /**	active calibration		**/
			//~ if (rawInput > encoder.max){//exceeded old max value
				//~ encoder.max = rawInput;//update max
				//~ encoder.range = encoder.max - encoder.min;//update range
				//~ Serial.print("NewEncMax");Serial.println( encoder.max);
			//~ }
			//~ if (rawInput < encoder.min){//below old min value
				//~ encoder.min = rawInput;//update min
				//~ encoder.range = encoder.max - encoder.min;//update range
				//~ Serial.print("NewEncMin");Serial.println( encoder.min);
			//~ }
			//~ CalibrateMove();	//allow hard commanded movement
		//~ break;
		//~ case 3://set encoder range, print values
			//~ Write(ServoNeutral);	//don't move motor
			//~ encoder.range = encoder.max - encoder.min;	//set range from new min and max
			//~ Serial.print("Min:");Serial.print("\t");Serial.print(encoder.min);Serial.print("\t");
			//~ Serial.print("Max:");Serial.print("\t");Serial.print(encoder.max);Serial.print("\t");
			//~ Serial.print("Range:");Serial.print("\t");Serial.print(encoder.range);Serial.print("\n");
			
			//~ //prep to calibrate range of motion
			//~ Serial.print(Name);Serial.println("::\tCalibrate Range");
			//~ //min will be reduced to actual, max will be increased to actual
			//~ raw.min=60000; raw.max=-60000;
			//~ calibrating++;	//next step
		//~ break;
		//~ case 4://calibrate raw range of motion
			//~ Read();//read joint using calibrated encoder
		//~ /**	active calibration		**/
			//~ if (rawPos > raw.max){ //exceeded old max value
				//~ raw.max = rawPos;//update max
				//~ raw.range = raw.max - raw.min;	//set range from new min and max
				//~ Serial.print("NewRawMax");Serial.println( raw.max);
			//~ }
			//~ if (rawPos < raw.min){//below old min value
				//~ raw.min = rawPos;//update min
				//~ raw.range = raw.max - raw.min;//set range from new min and max
				//~ Serial.print("NewRawMin");Serial.println( raw.min);
			//~ }
			//~ CalibrateMove();	//allow hard commanded movement
		//~ break;
		//~ case 5://set raw range
			//~ Write(ServoNeutral);	//don't move motor
			//~ raw.range = raw.max - raw.min;		//set range from new min and max
			//~ Serial.print("Min:");Serial.print("\t");Serial.print(raw.min);Serial.print("\t");
			//~ Serial.print("Max:");Serial.print("\t");Serial.print(raw.max);Serial.print("\t");
			//~ Serial.print("Range:");Serial.print("\t");Serial.print(raw.range);Serial.print("\n");
			//~ calibrating++;	//next step
		//~ break;
		//~ case 6://prep to calibrate edge
			//~ Serial.print(Name);Serial.println("::\tCalibrate Edge");
			//~ //min will be reduced to actual, max will be increased to actual
			//~ position.min = 60000;position.max = -60000;
			//~ calibrating++;	//next step
		//~ break;
		//~ case 7://calibrate edge
			//~ Read();//read joint using calibrated encoder
			//~ if (calPos==_motor::mOUT){		//if position to calibrate from is motor out
				//~ if (rawPos > position.max){	//max position exceeded
					//~ position.max = rawPos;	//update max
					//~ position.min = position.max - raw.range;//update min
					//~ Serial.print("NewPosMax");Serial.println(position.max);
				//~ }
			//~ }
			//~ else //_motor::mIN
			//~ {
				//~ if (rawPos < position.min){	//min position exceeded
					//~ position.min = rawPos;	//update min
					//~ position.max = position.min + raw.range;//update max
					//~ Serial.print("NewPosMin");Serial.println(position.min);
				//~ }
			//~ }
			//~ CalibrateMove();	//allow hard commanded movement
		//~ break;
		//~ case 8://set edge and range
			//~ Write(ServoNeutral);		//don't move motor
			//~ position.range = raw.range;	//update position range
			//~ Serial.print(Name); Serial.println("::\tdone");
			
			//~ calibrating=-1;	//return to inactive calibration
			//~ Calibrated=1;	//status is calibrated
			//~ Save();			//save info to EEPROM
		//~ break;
		
		//~ default:
			//~ Read();				//read encoder values, etc
			//~ Write(ServoNeutral);//don't move motor
			//~ calibrating = -1;	//no longer calibrating
		//~ break;
		
	//~ }
};//end Calibrate

void Motor::Move(){
	if(abs(speed) < deadband){
		Write(ServoNeutral);	//dont move motor
		
		if (PRINT && CurrentTime%300 == 0)
			Serial.println("_");	//debug print
	}
	else if (speed > 0){
		if (!reverse){
			if (maxLim->checkHold()){
				Write(ServoNeutral);
			}
			else{
				Write(ServoNeutral+speed*(ServoMax-ServoNeutral)/1000.0); //write speed
			}
		}
		else {
			if (minLim->checkHold()){
				Write(ServoNeutral);
			}
			else{
				Write(ServoNeutral+speed*(ServoMax-ServoNeutral)/1000.0); //write speed
			}
		}
	
		if (PRINT &&CurrentTime%300 == 0)
			Serial.println("^");	//debug print
	}
	else {	//speed <0
		if (!reverse){
			if (minLim->checkHold()){
				Write(ServoNeutral);
			}
			else{
				Write(ServoNeutral+speed*(ServoMax-ServoNeutral)/1000.0); //write speed
			}
		}
		else {
			if (maxLim->checkHold()){
				Write(ServoNeutral);
			}
			else{
				Write(ServoNeutral+speed*(ServoMax-ServoNeutral)/1000.0); //write speed
			}
		}
	
		if (PRINT &&CurrentTime%300 == 0)
			Serial.println("\\/");	//debug print
	}
	
}//end Move

void Motor::Step(byte _startstep){
	if (calibrating!=-1){//if we're ready to calibrate
		if (_startstep) calibrating = _startstep;	//start with a certain step (asdf# -> edge)
		else{					//if given 0, we simply go to the next step
			Calibrate();		//run calibration function to complete the step
			calibrating++;		//move to next step
		}
	}
};//end Step

void Motor::Operate(){
	if (calibrating!=-1){	//calibrating is in standby or running
		CalibrateMove();	//allow hard commanded movements
		Calibrate();		//run calibration sequence
	}
	else if (Calibrated==1){//not currently calibrating, and calibration is good
		Move();				//move via PID
	}
};//end Operate

void Motor::CalibrateMove(){
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
			Serial.println(direction);
		}
	}
};//end CalibrateMove