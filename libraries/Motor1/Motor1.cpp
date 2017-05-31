#include "Motor1.h"

const short ServoMax = 2000;
const short ServoMin = 1000;
const short SERVONEUTRAL = 1500;

int _NextServoNum = 0;

Motor::Motor(int _signalPin, int _minPin, int _maxPin)
{
	sigPin = _signalPin;		//signal(pwm or servo) output pin
	servoNum = _NextServoNum++;
	maxPin = _maxPin;
}

void Motor::Init(const char* _name, bool _reverse){
	strcpy(Name,_name);			//display name for motor
	reverse = _reverse;		//if the motor uses reverse operation

	TestServo.attach(sigPin);
	Write(SERVONEUTRAL);
	deadband = 2;
}

void Motor::Write(int pulse_width_us)
{
    // create a pulse of width pulse_width_us
    TestServo.writeMicroseconds(pulse_width_us);
}

// speed ranges from -100 to 100 could change this if wanted
void Motor::Move(int speed)
{
    if(abs(speed) < deadband)
    {
        Write(SERVONEUTRAL);	// dont move motor
    }
    else
    {
        // basic setup
        
        if (!reverse)
        {
            Write(-1*speed + 1500);
        }
        else
        {
            Write(speed + 1500);
        }
        
    }
}
