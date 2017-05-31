#include "Controller.h"

SticksClass sticks[NUMSTICKS];	//array of analog sticks
StickVals LSUDvals, LSLRvals, RSLRvals, RSUDvals;	//analog stick value storage
KnobClass knobs[NUMKNOBS];	//array of analog knobs
KnobVals KNOB1vals, KNOB2vals, KNOB3vals, KNOB4vals;//analog knob value storage

ButtonClass buttons[NUMBUTTONS];	//array of button objects (allows us to step through them in loops)
ButtonClass switches[NUMSWITCHES];	//array of switch objects (allows us to step through them in loops)

void ControllerClass::MakeAnalogs()
{
	/*
	makes sticks for current version of controller
	*/
	sticks[_st::LSUD].Init("LS U/D",21,2,0,&LSUDvals);
	sticks[_st::LSLR].Init("LS L/R",14,2,1,&LSLRvals);
	sticks[_st::RSUD].Init("RS U/D",18,2,1,&RSUDvals);
	sticks[_st::RSLR].Init("RS L/R",19,2,0,&RSLRvals);
	
	//2-5,11,12 lcd
	//6,8,9,10,13,14,18,19,21,23,24,25,26,27,28,29,30,31,32
	//6sw,10b,4st
	/*
	makes knobs for current version of controller
	knob 1 -> 4	 ::	left -> right
	*/
	knobs[0].Init("KNOB 1",20,&KNOB1vals);
	knobs[1].Init("KNOB 2",15,&KNOB2vals);
	knobs[2].Init("KNOB 3",16,&KNOB3vals);
	knobs[3].Init("KNOB 4",17,&KNOB4vals);
	
};//end MakeAnalogs
void ControllerClass::MakeDigitals()
{
	/*
	makes buttons for current version of controller
	matches with PlayStation button labels
	*/
	buttons[_b::TRIANGLE].Set( 32,0,"Tri");
	buttons[_b::START].Set( 31,0,"Start");
	buttons[_b::SQUARE].Set( 30,0,"[_]");
	buttons[_b::OH].Set( 29,0,"(O)");
	buttons[_b::UP].Set( 28,0,"/\\");
	buttons[_b::LEFT].Set( 27,0,"<<");
	buttons[_b::DOWN].Set( 26,0,"\\/");
	buttons[_b::EX].Set( 25,0,"X");
	buttons[_b::RIGHT].Set( 24,0,">>");
	buttons[_b::SELECT].Set( 8,0,"Sel");
	buttons[_b::NONE].Set( 33, 0, "NONE!!" );
	
	/*
	makes switches for current version of controller
	R1/L1 are face switches toward the center
	R2/L2 are face switches toward the outside
	RF/LF -right front and left front switches
	*/
	switches[_sw::R1].Set( 23,0,"R1");
	switches[_sw::R2].Set( 6,0,"R2");
	switches[_sw::RF].Set( 9,0,"RF");
	switches[_sw::L1].Set( 10,0,"L1");
	switches[_sw::L2].Set( 22,0,"L2");
	switches[_sw::LF].Set( 13,0,"LF");
	switches[_sw::NOSW].Set( 33,0,"NONE!!" );
};//end MakeDigitals()


/****
Menu button section
****/
bool ControllerClass::GetEnter(){return GetButtonPress(Enter);};
bool ControllerClass::GetBack(){return GetButtonPress(Back);};
bool ControllerClass::GetNext(){return GetButtonPress(Next);};
bool ControllerClass::GetPrev(){return GetButtonPress(Prev);};
void ControllerClass::SetMenuButtons(ButtonClass* _prev,ButtonClass* _next,ButtonClass* _back,ButtonClass* _enter)
{
	Prev = _prev;
	Next = _next;
	Back = _back;
	Enter = _enter;
};


/****
General button section
****/
bool ControllerClass::GetButtonPress(ButtonClass *_button){return _button->checkpressed();};
bool ControllerClass::GetButtonChange(ButtonClass *_button){return _button->checkchanged();};
bool ControllerClass::GetButtonRelease(ButtonClass *_button){return _button->checkreleased();};
void ControllerClass::UNSetButton(byte n){buttonSlot[n] = &buttons[_b::NONE];};
uint16_t ControllerClass::GetButtonsBinary()
{
	b_buttons = 0;	//start all zeros
	for (int i =0; i < NUMBUTTONS; i++)
	{//for all buttons
		if (buttonSlot[i] != Back)//todo: use Back member instead of hard coded button
		{
			if (buttonSlot[i] != &buttons[_b::NONE])//if button is assigned
			{
				bool cb = buttonSlot[i]->checkpressed();
				b_buttons |= cb & B00000001;//add bit if pressed, or 0
				if ( i < NUMBUTTONS-1 ) //for all but last button
					b_buttons = b_buttons << 1;
			}//end assigned
			//else //leaves bit as zero
		}
	}//end all buttons
	return b_buttons;
};//end GetButtonsBinary
uint16_t ControllerClass::GetSwitchesBinary()
{
	b_switches = 0;	//start all zeros
	for (int i =0; i < NUMSWITCHES; i++)
	{//for all switches
		if (switchSlot[i] != &switches[_sw::NOSW])//if button is assigned
		{
			uint16_t cs = switchSlot[i]->checkSwitch();
			b_switches |= cs & B00000001;//add bit if pressed, or 0
			if ( i < NUMSWITCHES-1 ) //for all but last switch
				b_switches = b_switches << 1;	//shift for next bit
		}//end assigned
		//else //leaves bit as zero
	}//end all switches
	return b_switches;
};//end GetSwitchesBinary
ButtonClass* ControllerClass::SetButton(byte _buttonN, ButtonClass* _button)
{
	if (_buttonN < NUMBUTTONS){			//valid button number
		buttonSlot[_buttonN] = _button;	//set controller slot to use the button
		if (_button != &buttons[_b::NONE])	//if _button is not blank
			Serial.println("GOOD BUTTON");
		
		for (int i=0;i < NUMBUTTONS; i++)//for all slots
		{
			if (i != _buttonN)//except the slot we're setting
			{
				if (buttonSlot[i] == _button)//if slot matches button we're assigning
				{
					buttonSlot[i] = &buttons[_b::NONE];//un-assign button from slot
				}//end matching assignment
			}//end different slots
		}//end for all slots
		
		return buttonSlot[_buttonN];	//return newly assigned button
		
	}//end for valid button number
	
	else Serial.println("INVALID BUTTON");
	return &buttons[_b::NONE];	//return blank button
};//end SetButton
ButtonClass* ControllerClass::SetSwitch(byte _switchN, ButtonClass* _switch)
{
	if (_switchN < NUMSWITCHES){			//valid switch number
		switchSlot[_switchN] = _switch;	//set controller slot to use the switch
		if (_switch != &switches[_b::NONE])	//if _switch is not blank
			Serial.println("GOOD SWITCH");
		
		for (int i=0;i < NUMSWITCHES; i++)//for all slots
		{
			if (i != _switchN)//except the slot we're setting
			{
				if (switchSlot[i] == _switch)//if slot matches switch we're assigning
				{
					switchSlot[i] = &switches[_b::NONE];//un-assign switch from slot
				}//end matching assignment
			}//end different slots
		}//end for all slots
		
		return switchSlot[_switchN];	//return newly assigned switch
		
	}//end for valid switch number
	
	else Serial.println("INVALID SWITCH");
	return &switches[_b::NONE];	//return blank switch
};//end SetSwitch
ButtonClass* ControllerClass::WhatButtonPressed()
{
	delay(800);
	for (int i =0; i<NUMBUTTONS; i++)
	{//check all buttons
		if (&buttons[i] != Back)
		{//other than back button, we can assign any other
			if (buttons[i].checkHold())
			{
				return &buttons[i];//return the button that's pressed
			}//end if(checkHold)
		}//end if != Back
	}//end check all buttons
	return &buttons[_b::NONE];//return dummy button if nothing was pressed
};//end WhatButtonPressed
ButtonClass* ControllerClass::WhatSwitchFlipped()
{
	GetSwitchesBinary();//get initial reading for comparison
	delay(800);
	for (int i=0; i<NUMSWITCHES; i++)
	{//check all switches
		if (switches[i].checkchanged())
		{//if it was flipped
			return &switches[i];//return the switch thats flipped
		}//end if(checkchanged)
	}//end check all switches
	return &switches[_sw::NOSW];//return dummy switch if nothing pressed
};//end WhatSwitchFlipped

/****
Overall controller section
****/
void ControllerClass::SetOutput(Stream* _ostream){stream=_ostream;};
void ControllerClass::Init()
{
	SetOutput(&Serial);
	MakeDigitals();
	MakeAnalogs();
	
	//TODO read in mode and config settings from eeprom
	SetSticks(&sticks[_st::RSLR],&sticks[_st::RSUD],&sticks[_st::LSLR],&sticks[_st::LSUD]);
	
	for (int i =0; i < NUMBUTTONS; i++){
		SetButton(i,&buttons[i]);	//start all non-menu buttons unassigned
	}
	for (int i =0; i < NUMSWITCHES; i++){
		SetSwitch(i,&switches[i]);	//start all switches unassigned
	}
	
	SetMenuButtons(&buttons[_b::UP],&buttons[_b::DOWN],&buttons[_b::SELECT],&buttons[_b::START]);
	SetKnobs(&knobs[0],&knobs[1],&knobs[2],&knobs[3]);
	
};//end Init()
void ControllerClass::LoadDefaultSticks(byte _default)
{
	if (_default==_modelist::RC)
	{
		SetSticks(&sticks[_st::RSLR],&sticks[_st::RSUD],&sticks[_st::LSLR],&sticks[_st::LSUD]);
		M_DriveOn(_modelist::RC);
	}
	else if (_default==_modelist::VG)
	{
		SetSticks(&sticks[_st::LSLR],&sticks[_st::LSUD],&sticks[_st::RSLR],&sticks[_st::RSUD]);
		M_DriveOn(_modelist::VG);
	}
};//end LoadDefaultSticks
void ControllerClass::Read()
{
	if (millis() - LastRead > 15)	//read everything every 15 millis
	{
		bool* array;
		array = (bool*) malloc (DATASIZEBYTE);
		memset(array,0,sizeof(&array));	//set array to 0's
		
		array[0] |= c_mode;				//add in operation mode data
		
		//stick data
		array[0] = array[0] << (NUMSTICKS*10);//shift for stick data
		array[0] |= GetSticksBinary();		//reads and adds stick data
		
		//knob data
		array[0] = array[0] << (NUMKNOBS*8);//shift for knob data
		array[0] |= GetKnobsBinary();		//reads and adds knob data
		//todo: test getknobsbinary and reverse
		
		//button data
		array[0] = array[0] << (NUMBUTTONS);	//shift for button data
		array[0] |= GetButtonsBinary();		//reads and adds button data
		
		//switch data
		array[0] = array[0] << (NUMSWITCHES);	//shift for switch data
		array[0] |= GetSwitchesBinary();	//reads and adds switch data
		
		memcpy(&TXdata,&array,DATASIZEBYTE);
		
		LastRead = millis();
		haveTXdata =1;
	}
	
};//end Read()
bool* ControllerClass::GetTXdata(){
	if (haveTXdata){
		haveTXdata =0;
		return *TXdata;
	}
	return 0;
};
byte ControllerClass::GetTXsize(){return DATASIZEBYTE;};

/****
Mode section
****/
//byte c_mode = (Enable)(Drive)(Control Type)(Drive Mode)(Arm)(Gyro)(Auto)(empty)
void ControllerClass::M_Enable(){c_mode |= B10000000;};			//set mode to enabled
void ControllerClass::M_Disable(){c_mode=c_mode & B01111111;};	//set mode to disabled
void ControllerClass::M_DriveOn(byte _drivemode){			//enable and set drive mode 0=mecanum, 1=tank
	M_ArmOff();
	c_mode |= B0100000;		//enable drive mode
	if (_drivemode==_modelist::MECANUM) c_mode |= B0001000;		//mecanum mode
	else if (_drivemode==_modelist::TANK) c_mode = c_mode & B1110111;//tank mode
};//end DriveOn
void ControllerClass::M_DriveOff(){c_mode = c_mode & B10111111;};		//disable driving mode
void ControllerClass::M_ArmOn(){M_DriveOff();c_mode |= B00001000;};		//enable arm control mode
void ControllerClass::M_ArmOff(){c_mode=c_mode & B11110111;};			//disable arm control mode
void ControllerClass::M_GyroOn(){c_mode |= B00000100;};					//enable gyro rotational correction
void ControllerClass::M_GyroOff(){c_mode=c_mode & B11111011;};			//disable gyro rotational correction
void ControllerClass::M_Auto(){c_mode |= B00000010;};					//set autonomous mode
void ControllerClass::M_Manual(){c_mode=c_mode & B11111101;};			//set manual control mode
bool ControllerClass::M_GetEnabled(){return bool((c_mode & B10000000) >> 7);};//get enable/disable status
byte ControllerClass::M_GetMode(){return c_mode;};						//get current mode settings
bool ControllerClass::M_GetDriveMode(){return bool((c_mode & B00010000) >> 4);};//get mecanum/tank status
void ControllerClass::M_SetOpsMode(byte _mode)
{
	if (_mode == _modelist::ARM)
		M_ArmOn();						//enable arm, disable drive
	else if (_mode == _modelist::DRIVE)
		M_DriveOn(M_GetDriveMode());	//enable drive, disable arm, keep current drive mode
};//end SetOpsMode


/****
Sticks section
****/
short ControllerClass::GetStickVal(byte _axisN){ return stickSlot[_axisN]->Use(); };
bool ControllerClass::IsStickCalibrated(byte _axisN){ return stickSlot[_axisN]->IsCalibrated();};
void ControllerClass::SetSticks(SticksClass* _stick1,SticksClass* _stick2,SticksClass* _stick3,SticksClass* _stick4)
{
	stickSlot[0]=_stick1;
	stickSlot[1]=_stick2;
	stickSlot[2]=_stick3;
	stickSlot[3]=_stick4;
};
void ControllerClass::SetStick(byte _axisN,SticksClass* _stick)
{
	//todo: when stick assigned, remove from other assignments
	if (_axisN < NUMSTICKS)	//valid stick number
		stickSlot[_axisN] = _stick;	//set
	else Serial.println("INVALID STICK");
};
void ControllerClass::CalibrateSticks()
{
	for (int i =0; i < NUMSTICKS; i++)
		stickSlot[i]->Calibrate();
};//end CalibrateSticks
void ControllerClass::UseSticks()
{
	for (int i =0; i < NUMSTICKS; i++)
	{
		stickSlot[i]->Use();
	}
};//end UseSticks
uint64_t ControllerClass::GetSticksBinary()
{
	uint64_t temp = 0;
	uint16_t print[4]={0}; 
	
	for (int i =0; i< NUMSTICKS; i++)
	{
		//either get value or continue calibrating
		short cs = stickSlot[i]->Use();
		print[i]=0;
		
		//get high byte data
		temp |= (cs >> 8) & B00000011;		//add 2 bits from high byte
		print[i] |= (cs >> 8) & B00000011;	//add 2 bits from high byte
		
		//open up low byte
		temp = temp << 8;			//shift over 1 byte
		print[i] = print[i] << 8;	//shift over 1 byte
		
		//get low byte data
		temp |= (cs & B11111111);		//add 8 bits from low byte
		print[i] |= (cs & B11111111);	//add 8 bits from low byte
		
		//prep for next axis data
		if (i<NUMSTICKS-1)			//do this for all but last axis
			temp = temp << 8;		//shift for next high byte bits
		
	}//end for all sticks   
	
	b_sticks = temp;
	return temp;
};//end GetSticksBinary()


/****
Knobs section
****/
short ControllerClass::GetKnobVal(byte _knobN){ return knobSlot[_knobN]->Use(); };
bool ControllerClass::IsKnobCalibrated(byte _knobN){ return knobSlot[_knobN]->IsCalibrated();};
void ControllerClass::SetKnobs(KnobClass* _k1,KnobClass* _k2,KnobClass* _k3,KnobClass* _k4)
{
	knobSlot[0]=_k1;
	knobSlot[1]=_k2;
	knobSlot[2]=_k3;
	knobSlot[3]=_k4;
};
void ControllerClass::SetKnob(byte _knobN,KnobClass* _knob)
{
	//todo: when knob assigned, remove from other assignments
	if (_knobN < NUMKNOBS)	//valid knob number
		knobSlot[_knobN] = _knob;	//set
	else Serial.println("INVALID KNOB");
};
void ControllerClass::CalibrateKnobs()
{
	for (int i =0; i < NUMKNOBS; i++)
		knobSlot[i]->Calibrate();
};//end CalibrateKnobs
void ControllerClass::UseKnobs()
{
	for (int i =0; i < NUMKNOBS; i++)
		knobSlot[i]->Use();
};
uint32_t ControllerClass::GetKnobsBinary()
{
	uint32_t temp = 0;
	for (int i =0; i< NUMKNOBS; i++)
	{
		//either get value or continue calibrating
		short ck = knobSlot[i]->Use();
		
		//get byte data
		temp |= ck & B11111111;		//add 8 bits
		//	Serial.print(temp);Serial.print("\t");
		
		//prep for next byte data
		if (i<NUMKNOBS-1)			//do this for all but last knob
			temp = temp << 8;		//shift for next byte
	}//end for all sticks
	b_knobs = temp;
	return temp;
};//end GetKnobsBinary
