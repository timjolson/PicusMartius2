#ifndef DRIVE_MOTOR_H
#define DRIVE_MOTOR_H

#include <Arduino.h>
//#include <Servo.h> // arduino servo library

#define MOTOR_UPDATESPEED 3000 // 3ms between updates

enum motor_direction_t { DRIVEDIRECTION_FORWARD=1, DRIVEDIRECTION_BACKWARD=-1, DRIVEDIRECTION_NEUTRAL=0 };

class DriveMotor {
public:
	DriveMotor();
	DriveMotor(uint8_t pin, uint16_t rawMax, uint16_t rawMin, uint16_t rawNeutral);

	/** Getters */
	uint8_t getPin();
	uint16_t getRawMin();
	uint16_t getRawMax();
	uint16_t getRawNeutral();
	uint16_t getCurrentRawSpeed();
	float getCurrentRawPercent();
	motor_direction_t getCurrentDirection();

	/** Setters */
	void setMotorSpeedPercent(float percent);
	//void setMotorSpeedRaw(uint16_t val);
	void setMotor(float);

	/** Public Methods */
	void update();
	void kill();
	void enable();
	void disable();
	bool isEnabled();
	bool isKilled();
	
private:
	/** Private Methods */
	void _kill();
	void _enable();
	
	/** Private Variables */
	//Servo _motor; // Arduino Servo Library
	uint8_t _pin;
	motor_direction_t _motorDirection;
	uint16_t _rawCurrentSpeed;
	uint16_t _rawMin;
	uint16_t _rawMax;
	uint16_t _rawNeutral;

	uint16_t _diffNeutralToMax;
	uint16_t _diffNeutralToMin;
	uint16_t	_percentForward;
	uint16_t	_percentBackwards;

	uint8_t _killed; // should probably be a bool (0,1)
	uint32_t _lastpulsestart; // millis() since last update
};

#endif
