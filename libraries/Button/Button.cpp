#include "Button.h"
#include <Arduino.h>

extern uint32_t CurrentTime;	//updates every cycle with millis()
#define debounceTime 40 //millis to wait before reporting another button press or switch change

void ButtonClass::PrintName(){Serial.println(Name);};	//print name of button (mostly for debugging)
const char* ButtonClass::GetName(){return Name;};		//get name of button

ButtonClass::ButtonClass(int _pin, bool _norm, const char* _name)
{
	Set(_pin,_norm,_name);
};

void ButtonClass::Set(int _pin, bool _norm, const char* _name)
{
	/*set input pin, normal position of switch, button name, initializes as input*/
	pin=_pin;
    pinMode(pin, INPUT);
    strcpy(Name,_name);
    lastPressStat=norm=_norm;
    lastChangeStat=norm;
    lastReleaseStat=norm;
    lastSwitchStat=norm;
    lastChangeTime=0;
    lastPressTime=0;
    lastSwitchTime=0;
    heldPrint = 0;
};

uint16_t ButtonClass::Pressed() //get (1) on press, elapsed of hold, 0 otherwise
{
	uint32_t _return=0;	//start our return value at 0
	//if enough time has elapsed since last press to debounce the button
	if (lastPressTime+debounceTime < CurrentTime){
        current=digitalRead(pin);	//read pin
        if (norm){      //if normally closed
            if ((!current)&&(lastPressStat)){//if button reads open, but previously read closed
                _return = 1;		//set return 1
                lastPressTime = timer = CurrentTime; //set new time for debounce comparison
            }
            else if ((!lastPressStat) && (!current)) //held open
                _return = CurrentTime - timer;
        }
        else if (!norm){    //if normally open
            if ((current)&&(!lastPressStat)){//if button reads closed, but previously read open
                _return = 1;		//set return 1
                lastPressTime = timer = CurrentTime; //set new time for debounce comparison
            }
            else if (lastPressStat && current)       //held closed
                _return = CurrentTime - timer;
        }
        
        lastPressStat=current;	//keep track of previous status to compare
	}
    
    return _return;		//return the default (0) or pressed (1), or duration held
};//end checkPressed()

bool ButtonClass::Released()	//get a 1 on the cycle a button is (debounced)released, 0 otherwise
{
  bool _return=0;	//start our return value at 0
  //if enough time has elapsed since last press to debounce the button
  if (lastReleaseTime+debounceTime < CurrentTime){
	  current=digitalRead(pin);	//read pin
	  if (norm){		//if normally closed
		  if (current>lastReleaseStat){	//if button reads closed, but previously read open
			  _return = 1;		//set return 1
			  lastReleaseTime=CurrentTime;//set new time for debounce comparison
		  }
	  }
	  else{				//if normally open
		  if (current<lastReleaseStat){	//if button reads open, but previously read closed
			  _return = 1;		//set return 1
			  lastReleaseTime=CurrentTime; //set new time for debounce comparison
		  }
	  }
	  lastReleaseStat=current;	//keep track of previous status to compare
  }
  return _return;		//return the default (0) or released (1)
};//end checkReleased()

bool ButtonClass::Changed()	//get a 1 on the cycle a button is (debounced)changed, 0 otherwise
{
	bool _return=0;	//start our return value at 0
	//if enough time has elapsed since last press to debounce the button
	if (lastChangeTime+debounceTime <CurrentTime){
		current = digitalRead(pin);	//read pin
		if (current==lastChangeStat){	//if button reads the same as before
			_return = 0;		//set return 0
			lastChangeTime=CurrentTime;	//set new time for debounce comparison
		}
		else{					//if button reads different than last reading
			lastChangeStat=current;		//keep track of previous status to compare
			_return = 1;		//set return 1
			lastChangeTime=CurrentTime;	//set new time for debounce comparison
		}
	}
	return _return;				//return the default (0) or changed (1)
};//end checkChanged()

bool ButtonClass::checkSwitch()	//get current setting of switch w/debouncing
{
	bool _return=lastSwitchStat;	//start our return value where it was before
	current=digitalRead(pin);	//read pin
	if (current != lastSwitchStat) lastSwitchTime = CurrentTime;	//if switch has changed, start timing over again
	
	//if the switch has been consistent for at least debounceTime
	if (lastSwitchTime + debounceTime < CurrentTime){
		_return = current;	//update switch setting
	}
	lastSwitchStat=current;		//keep track of previous status to compare
	
	return _return;			//return the last consistent setting
};//end checkSwitch()

bool ButtonClass::checkHold()	//get status of button on cycle called (no debouncing)
{
	return (digitalRead(pin)!=norm);
};//end checkHold()

bool ButtonClass::Held(uint16_t dur)     //returns 1 if button has been pressed for duration, 0 if not
{
    bool _return = 0;
    if ((Pressed()>=dur)&&(!heldPrint))  //meets duration and has not return positive
    {
        _return = 1;
        heldPrint = 1;  //don't return positive again
    }
    if (!Pressed())
        heldPrint = 0;  //allow to return positive again
    return _return;
};//end checkHeld(n)

void ButtonClass::AutoNorm()
{
    byte test = 0;
    delay(2);
    test += digitalRead(pin);
    delay(2);
    test += digitalRead(pin);
    delay(2);
    test += digitalRead(pin);
    delay(2);
    test += digitalRead(pin);
    delay(2);
    test += digitalRead(pin);
    
    norm = (test >= 3)?1:0;
}