#include "DriveMotor.h"

DriveMotor::DriveMotor() {} // don't use default constructor
DriveMotor::DriveMotor(uint8_t pin, uint16_t rawMax, uint16_t rawMin, uint16_t rawNeutral)
{
	// Set the variables
	_pin = pin;
	_rawMax = rawMax;
	_rawMin = rawMin;
	_rawNeutral = rawNeutral;

	_killed = 1; // 0 if killed and 1 if not (multipled in speed)
	_rawCurrentSpeed = _rawNeutral; // Set the current speed to the raw neutral value
	_motorDirection = DRIVEDIRECTION_NEUTRAL;

	//_diffNeutralToMax = _rawMax - _rawNeutral; // Pre-calculate the difference between neutral and min/max
	//_diffNeutralToMin = _rawNeutral - _rawMin; // both should be a little less than 500

	//Serial.println("motor created!");
	//_motor.attach(pin); // Servo Library
}

/*uint8_t DriveMotor::getPin() { return _pin; }
uint16_t DriveMotor::getRawMin() { return _rawMin; }
uint16_t DriveMotor::getRawMax() { return _rawMax; }
uint16_t DriveMotor::getRawNeutral() { return _rawNeutral; }
uint16_t DriveMotor::getCurrentRawSpeed() { return _rawCurrentSpeed; }
float DriveMotor::getCurrentRawPercent() { return _ }
motor_direction_t DriveMotor::getCurrentDirection() { return _motorDirection; }*/

void DriveMotor::kill() { _killed = 0; }
void DriveMotor::disable() { kill(); }
void DriveMotor::enable() { _killed = 1; }

void DriveMotor::setMotorSpeedPercent(float percent)
{
	if (percent > 0) {
		_percentForward = constrain(percent,0,100);
		_percentBackwards = 0;
	} else {
		_percentForward = 0;
		_percentBackwards = constrain(percent,0,100);
	}
}

/*uint16_t DriveMotor::getRawFromPercent(float percent)
{
	if (percent == 0) {
		return _rawNeutral;

	} else if (percent < 0) {
		// Map from -100 to 0
		return ((percent / 100.0f) * _diffNeutralToMin + _rawNeutral);

	} else if (percent > 0) {
		// Map from -100 to 0
		return ((percent / 100.0f) * _diffNeutralToMax + _rawNeutral);
	}

	// Should never reach this point
	return  _rawNeutral;
}

void DriveMotor::_setMotorSpeedRaw(uint16_t val)
{
	_rawCurrentSpeed = val;
	if (_killed)
		_motor.write(_rawNeutral); // keep killed
	else
		_motor.write(val);
}*/

void DriveMotor::update() // called as often as possible - obsoleted by servo library?
{
	// forward = 2ms
	// backwards = 1ms
	// neutral = 1.5ms
	if (micros() - _lastpulsestart > MOTOR_UPDATESPEED) // probably optimize out micros() by making it constant per sweep
		_lastpulsestart = micros(); // new pulse begins now 
		
	//if (micros() - _lastpulsestart < 1500 + _dir * _speedpercent * 500) { // send pulse to motor
	if (micros() - _lastpulsestart < _rawNeutral + /*(_killed) * */ ((_percentForward * _rawMax) - (_percentBackwards * _rawMin))) { // send pulse to motor
		digitalWrite(_pin,HIGH);
	} else { // wait between pulses
		digitalWrite(_pin,LOW);
	}
}
