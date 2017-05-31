#ifndef Blink_h
#define Blink_h

#include <Arduino.h>

/*
LED blinking library, allows setting on/off and
increasing or decreasing time between blinks (time entered in SECONDS)
*/

byte led = 13;			//led pin
uint32_t _lastBlink =0;	//last time the led status changed
bool ledStatus =0;		//current led status
float blinkSpeed = 0.5;	//time in SECONDS between led status change

void _RunBlink(uint32_t _time, float _speed){
	if (_speed ==0){	//write on
		digitalWrite(led, 1);//led on
	}
	else if (_speed == 5){	//write off
		digitalWrite(led, 0);//led off
	}
	else if ((_time - _lastBlink)>1000*((_speed>0)?_speed:0.05)){
		// time (millis) since last change >? [speed(SECONDS)*1000->MILLIS -or- speed is not real, default to 50 MILLIS]
		//if its time to change the led
		//then change status, and write to pin
		ledStatus=1-ledStatus;
		digitalWrite(led,(ledStatus));
		_lastBlink = _time;	//keep track for timing
	}
}//end RunBlink

void _LedOn(){
	//set led solid on
	blinkSpeed = 0;
}//end _BlinkOn

void _LedOff(){
	//set led solid off
	blinkSpeed = 5;
}//end _LedOff

void _ResetBlink(){
	//reset to default blink speed (1/2 second)
	blinkSpeed = 0.5;
}//end _ResetBlink

void _BlinkFaster(){
	//increase blink speed
	blinkSpeed -= .25;
	blinkSpeed = max(blinkSpeed, 0);//limit to 0(solid off) or greater
}//end _BlinkFaster

void _BlinkSlower(){
	//decrease blink speed
	blinkSpeed += .25;
	blinkSpeed = min(blinkSpeed, 5);//limit to 5(solid on) or less
}//end _BlinkFaster

void _SetBlinkPin(int _pin){
	led = _pin;
}

#endif
