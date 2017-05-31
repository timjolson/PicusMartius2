#ifndef Controller_h
#define Controller_h
#include <Arduino.h>
#include "Sticks.h"	//read of controller sticks
#include "Button.h"	//digital input (buttons/switches)
#include "Knobs.h"	//read of controller knobs

class ControllerClass{
	Stream* stream;		//where to print debugging data
	uint32_t LastRead;	//last controller read time
	
	ButtonClass* Prev;	//menu navigation button - previous option
	ButtonClass* Next;	//menu navigation button - next option
	ButtonClass* Back;	//menu navigation button - back out/cancel
	ButtonClass* Enter;	//menu navigation button - choose displayed option
	
	byte c_mode;		//current controller mode (also serves as binary for transmission)
	bool* TXdata[DATASIZEBOOL+8];
	bool haveTXdata;
	
	//binaries for transmitting
	uint16_t b_buttons;	//binary for buttons (16 max)
	uint8_t b_switches;	//binary for switches (8 max)
	uint64_t b_sticks;	//binary for sticks
	uint32_t b_knobs;	//binary for knobs
	
public:
	//inputs on the controller
	ButtonClass* buttonSlot[NUMBUTTONS];	//array of general purpose buttons for custom commands
	ButtonClass* switchSlot[NUMSWITCHES];	//array of general purpose switches
	SticksClass* stickSlot[NUMSTICKS];	//array of analog sticks on controller
	KnobClass* knobSlot[NUMKNOBS];		//array of analog knobs
	
	//transmitting data
	uint64_t GetSticksBinary();		//get binary data for all axes to transmit
	uint32_t GetKnobsBinary();		//get binary data for all knobs to transmit
	uint16_t GetButtonsBinary();	//get binary data for all buttons to transmit
	uint16_t GetSwitchesBinary();	//get binary data for all switches to transmit
	
	//menu operations
	void SetMenuButtons(ButtonClass* _prev,ButtonClass* _next,ButtonClass* _back,ButtonClass* _enter);	//set which buttons operate the menu (Previous, Next, Back, Enter)
	bool GetEnter();	//returns GetButtonPress() of whatever button is set to Enter
	bool GetBack();		//returns GetButtonPress() of whatever button is set to Back
	bool GetPrev();		//returns GetButtonPress() of whatever button is set to Prev
	bool GetNext();		//returns GetButtonPress() of whatever button is set to Next
	
	//stick operations
	void MakeAnalogs();					//set properties for sticks and knobs
	short GetStickVal(byte _axisN);			//get current value of a stick (stickSlot[n])
	bool IsStickCalibrated(byte _axisN);		//get whether a stick is calibrated or not (stickSlot[n])
	void SetSticks(SticksClass*,SticksClass*,SticksClass*,SticksClass*);//assign stick objects to stickSlot[n]
	void SetStick(byte _axisN,SticksClass* _stick);	//assign (1) stick object to stickSlot[n]
	void CalibrateSticks();				//start calibrating sticks (call once, UseSticks() will continue calibration)
	void UseSticks();					//read and/or calibrate sticks
	
	//knob operations
	short GetKnobVal(byte _knobN);				//get current value of a knob (knobSlot[n])
	bool IsKnobCalibrated(byte _knobN);		//get whether a stick is calibrated or not (stickSlot[n])
	void SetKnobs(KnobClass*,KnobClass*,KnobClass*,KnobClass*);	//assign knob objects to controller knobSlot[n]
	void SetKnob(byte _knobN,KnobClass* _knob);		//assign (1) knob object to knobSlot[n]
	void CalibrateKnobs();				//start calibrating knobs (call once, UseKnobs() will continue calibration)
	void UseKnobs();					//read and/or calibrate knobs
	
	//button and switch operations
	void MakeDigitals();					//set properties for buttons and switches
	ButtonClass* SetButton(byte _buttonN,ButtonClass* _button);		//set a physically defined button to a specific function (not menu related)
	ButtonClass* SetSwitch(byte _switchN,ButtonClass* _switch);		//set a physically defined switch to a specific function
	void UNSetButton(byte n);					//reset a function to having no assigned button (not menu related)
	bool GetButtonPress(ButtonClass* _button);	//get 1 on the cycle a button is (debounced)pressed
	bool GetSwitchOn(ButtonClass* _switch);		//get 1 on the cycle a switch is (debounced) 'ON'
	bool GetButtonChange(ButtonClass* _button);	//get 1 on the cycle a button status is (debounced)changed
	bool GetSwitchChange(ButtonClass* _switch);	//get 1 on the cycle a switch status is (debounced)changed
	bool GetButtonRelease(ButtonClass* _button);//get 1 on the cycle a button is (debounced)released
/***	!!WARNING!! WhatButtonPressed() currently uses a delay of 600 millis	***/
	ButtonClass* WhatButtonPressed();	//returns (first in enumeration) button that gets pressed
	ButtonClass* WhatSwitchFlipped();	//returns (first in enumeration) switch that gets flipped
	
	//mode setting
	byte M_GetMode();				//get current mode settings
	bool M_GetEnabled();			//get enable/disable status
	void M_Enable();				//set mode to enabled
	void M_Disable();				//set mode to disabled
	void M_DriveOn(byte _drivemode);//enable and set drive mode
	void M_DriveOff();				//disable driving mode
	void M_ArmOn();					//enable arm control mode
	void M_ArmOff();				//disable arm control mode
	void M_SetOpsMode(byte _mode);	//set operating mode /arm vs drive vs whatever else
	void M_GyroOn();				//enable gyro rotational correction
	void M_GyroOff();				//disable gyro rotational correction
	void M_Auto();					//set autonomous mode
	void M_Manual();				//set manual control mode
	bool M_GetDriveMode();			//get drive mode (mecanum or tank)
	//c_mode = (Enable)(Drive)(Control Type)(Drive Mode)(Arm)(Gyro)(Auto)(empty)
	
	//general controller section
	void Read();						//calculate data from controller sub-objects
	void Init();						//create and set buttons and things on startup
	void SetOutput(Stream* _ostream);	//set stream to output debug info
	void LoadDefaultSticks(byte _default);//load default stick configuration
	bool* GetTXdata();
	byte GetTXsize();	
	
	//constructor
	ControllerClass(){};
};


#endif