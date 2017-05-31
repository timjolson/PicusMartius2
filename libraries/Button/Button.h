#ifndef Button_h
#define Button_h
#include <Arduino.h>

/*
Button class allows for debouncing, reading, and custom naming of buttons and switches
*/
class ButtonClass {
	bool lastPressStat;		//last status of button, used for comparison
    bool lastChangeStat;		//last status of button, used for comparison
    bool lastSwitchStat;		//last status of button, used for comparison
    bool lastReleaseStat;		//last status of button, used for comparison
    bool current;	//current status of button, used for reading
    byte pin;		//digital input pin the button is connected to
    bool norm;		//if the button is normally open (0) or normally closed (1)
	uint32_t lastPressTime;//time the button was last pressed
    uint32_t lastChangeTime;//time the button was last pressed
    uint32_t lastSwitchTime;//time the button was last pressed
    uint32_t lastReleaseTime;//time the button was last pressed
    
    uint32_t timer; //time the button was last pressed
    bool heldPrint; //whether a timed hold has printed or not
	char Name[8];	//name of the button (used for printing back and passes to the menu screen)
	
public:
    uint16_t Pressed();//get (1) on press, elapsed of hold, 0 otherwise
	bool Released();	//returns 1 on the cycle the button is released
	bool Changed();	//returns 1 on the cycle the button status changes
	bool checkHold();		//returns 1 if button is being pressed when called, 0 if not (no debouncing)
    bool Held(uint16_t duration);//returns 1 if button has been pressed for duration, 0 if not
	bool checkSwitch();		//returns condition of switch w/debouncing
	
	void PrintName();		//print name of button (mostly for debugging)
	const char* GetName();  //returns name of button
	
    //make a button or switch [input pin, what button normally reads, name of button]
	void Set(int _pin, bool _norm, const char* _name);
	void AutoNorm();

	//constructor
	ButtonClass(){};
	//make a button or switch [input pin, what button normally reads, name of button]
    ButtonClass(int _pin, bool _norm, const char* _name);
};//end ButtonClass


#endif