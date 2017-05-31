#include "Motor.h"

#include <Arduino.h>
//#include "..\Servo\src\Servo.h"
//#include "Variables.h"
//#include "../Button/Button.h"

extern uint32_t CurrentTime;
const short ServoMax = 2000;
const short ServoMin = 1000;
const short ServoNeutral = 1500;

extern Servo* Servos;

//Servo* Servo1;

int NextServoNum;

/**			signal pin	minPin	maxPin***/
//Motor generic_mot(2,		3,		4);

Motor::Motor(int _signalPin, int _minPin, int _maxPin)
{
	sigPin = _signalPin;		//signal(pwm or servo) output pin
	servoNum = NextServoNum++;
	minPin = _minPin;
	maxPin = _maxPin;

	//Servo Servo1;

}

void Motor::Init(const char* _name, bool _reverse){
	strcpy(Name,_name);			//display name for motor
	reverse = _reverse;		//if the motor uses reverse operation
	
	pinMode(sigPin, OUTPUT);	//servo pin is output
	pinMode(minPin, INPUT);
	pinMode(maxPin, INPUT);
	
	Servos->attach(sigPin); //servo operates on pin


	//Servos[1].attach(sigPin);
	
	deadband = 8;
};

void Motor::Write(int _val){
	Servos->write(_val); //write value to servo
};

// speed ranges from -100 to 100
// could change this if wanted
void Motor::Move(int speed){
	speed = map(speed, 100, -100, 2000, 1000);

	//Serial.print(speed);
	//Serial.println();

	if(abs(speed) < deadband){
		Write(ServoNeutral);	//dont move motor
		//if (PRINT && CurrentTime%300 == 0)
		//	Serial.println("_");	//debug print
	}
	else if (speed > 0){
		if (!reverse){
			Write(ServoNeutral+speed*(ServoMax-ServoNeutral)/100.0); //write speed
		}
		else {
			Write(ServoNeutral-speed*(ServoMax-ServoNeutral)/100.0); //write speed
		}
	
		// if (PRINT &&CurrentTime%300 == 0)
		//	Serial.println("^");	//debug print
	}
	else {	//speed <0
		if (!reverse){
			Write(ServoNeutral-speed*(ServoMax-ServoNeutral)/100.0); //write speed
		}
		else {
			Write(ServoNeutral+speed*(ServoMax-ServoNeutral)/100.0); //write speed
		}
		//if (PRINT &&CurrentTime%300 == 0)
		//	Serial.println("\\/");	//debug print
	}	
}
/*
void Motor::Step(byte _startstep){
	if (calibrating!=-1){//if we're ready to calibrate
		if (_startstep) calibrating = _startstep;	//start with a certain step (asdf# -> edge)
		else{					//if given 0, we simply go to the next step
			Calibrate();		//run calibration function to complete the step
			calibrating++;		//move to next step
		}
	}
};//end Step
*/

/*
void Motor::Operate(){
	if (calibrating!=-1){	//calibrating is in standby or running
		CalibrateMove();	//allow hard commanded movements
		Calibrate();		//run calibration sequence
	}
	else if (Calibrated==1){//not currently calibrating, and calibration is good
		Move();				//move via PID
	}
};//end Operate
*/
