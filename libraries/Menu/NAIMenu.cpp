#include "NAIMenu.h"		//contains MenuClass
#include "Screen.h"		//runs lcd
#include "Controller.h" //runs inputs
#include "Radio.h"		//runs receiving/transmitting
#include "NAIPages_Lists.h"//configurable, default menu hierarchy
#include "Button.h"
#include "Sticks.h"
#include "Knobs.h"

#define MENUDEBUG 0	//turn on serial printing for debugging

ScreenClass LCD;	//screen object, runs lcd screen
extern uint32_t CurrentTime;	//gets updated at beginning of every cycle with millis()
extern ControllerClass Controller;	//contains all assigned sticks, buttons, knobs, and more
extern SticksClass sticks[NUMSTICKS];	//array of actual analog sticks on controller
extern KnobClass knobs[NUMKNOBS];	//array of analog knobs on controller
extern ButtonClass buttons[NUMBUTTONS];	//array of button objects
extern ButtonClass switches[NUMSWITCHES];	//array of switch objects
extern RadioClass XBEE;

void MenuClass::Init(){
	LCD.Init(100);//start lcd menu screen, update interval (millis)
	
	SetAck(&ack);		//set ack to be menu acknowledge page
	ack.SetAsAck();		//make page itself a special page
	SetAccept(&accept);	//set accept to be menu accept page
	accept.SetAsAcc();	//make page itself a special page
	SetHome(&home);		//set homepage of menu to home
	
	CreatePresetLists();//make common lists of options
	
	home.Setup("NAI 2.0","    HOME",&ack);	//home page has ack as parent (ack will perform as root)
	home.AddOption("ENABLE?",1,&enabled);	//add an option to home page
	{//enable
		enabled.Setup("NAI 2.0","   READY");	//prompts on sub-page
		enabled.AddOption("RUNNING",&ack);		//add option to page
	}//end enable
	home.AddOption("OPERATION MODE?",&style);		//add another option to home page
	{//style
		style.Setup("OPERATIO","N  MODE?");
		style.AddOption("RC",5,&accept);
		style.AddOption("VideoGame",6,&accept);
		style.AddOption("Arm Control",7,&accept);
		style.AddOption("Mecanum",8,&accept);
		style.AddOption("Tank",9,&accept);
	}//end style
	home.AddOption("CONFIG?",&config);
	{//config
		config.Setup("CONFIG","   WHAT?");
		config.AddOption("STICKS",&configsticks);
		{//configsticks
			configsticks.Setup("WHICH","  AXIS?");
			configsticks.AddOption("X AXIS",&xstick);
			xstick.Setup("X AXIS","     SET",&configsticks);
			xstick.AddOption(stickoptions);
			configsticks.AddOption("Y AXIS",&ystick);
			ystick.Setup("Y AXIS","     SET",&configsticks);
			ystick.AddOption(stickoptions);
			configsticks.AddOption("W AXIS",&wstick);
			wstick.Setup("W AXIS","     SET",&configsticks);
			wstick.AddOption(stickoptions);
			configsticks.AddOption("Z AXIS",&zstick);
			zstick.Setup("Z AXIS","     SET",&configsticks);
			zstick.AddOption(stickoptions);
		}//end configsticks
		config.AddOption("DRIVE",&configdrive);
		{//config drive
			configdrive.Setup("DRIVE","   SET");
			configdrive.AddOption("MECANUM",70,&accept);
			configdrive.AddOption("TANK",71,&accept);
		}//end configdrive
		config.AddOption("BUTTONS",&configbuttons);
		{//configbuttons
			configbuttons.Setup("WHICH"," BUTTON?");
			configbuttons.AddOption("SHOOT",40,&ack);
			configbuttons.AddOption("CRAZY",41,&ack);
			//... to command 55
		}//end configbuttons
		config.AddOption("KNOBS",&configknobs);
		{//configknobs
			configknobs.Setup("WHICH","   KNOB?");
			configknobs.AddOption("SPEED",&configspeed);
				configspeed.Setup("SPEED","     SET");
				configspeed.AddOption(knoboptions);
			configknobs.AddOption("ROTATION",&configrotation);
				configrotation.Setup("ROTATION","     SET");
				configrotation.AddOption(knoboptions);
			configknobs.AddOption("OTHER1",&configother1);
				configother1.Setup("","");
				configother1.AddOption(knoboptions);
			configknobs.AddOption("OTHER2",&configother2);
				configother2.Setup("","");
				configother2.AddOption(knoboptions);
		}//end configknobs
		config.AddOption("SWITCHES",&configswitches);
		{//configswitches
			configswitches.Setup("WHICH"," SWITCH?");
			configswitches.AddOption("HALFSPEED",60,&ack);
			configswitches.AddOption("ARM/DRIV",61,&ack);
		}
	}//end config
	home.AddOption("CALIBRATE?",&calibrate);
	{//calibrate
		calibrate.Setup("CALIBRAT","E  WHAT?");
		calibrate.AddOption("STICKS",&calsticks);
			calsticks.Setup("CALIBRAT","E STICKS");
			calsticks.AddOption("READY?",11,&ack);
		calibrate.AddOption("KNOBS",&calknobs);
			calknobs.Setup("CALIBRAT","E  KNOBS");
			calknobs.AddOption("READY?",15,&ack);
		calibrate.AddOption("DRIVE",&caldrive);
			caldrive.Setup("CALIBRAT","E MOTORS");
			caldrive.AddOption("READY?",&calm);
			calm.Setup("ARE YOU ","   SURE?");
			calm.AddOption("Yes",16,&ack);
		calibrate.AddOption("ARM",&calarm);
			calarm.Setup("CALIBRAT","E  WHAT?");
			calarm.AddOption("WHOLE ARM",20,&ack);
			calarm.AddOption("LOWER ARM",21,&ack);
			calarm.AddOption("UPPER ARM",22,&ack);
			calarm.AddOption("WRIST",23,&ack);
			calarm.AddOption("GRIPPER",24,&ack);
		calibrate.AddOption("GYRO",&calgyro);
			calgyro.Setup("CALIBRAT","E  GYROS");
			calgyro.AddOption("READY?",28,&accept);
	}//end calibrate
	
	//menu option to reset controller - accessible from home page
	home.AddOption("RESET CONTROLLER",&resetcontroller);
	resetcontroller.Setup("RESET CO","NTROLLER");
	resetcontroller.AddOption("ARE YOU SURE?",999,&ack);
	
	//starts navigation at home page
	GoToPage(&home);
};//end Init()

void MenuClass::CreatePresetLists(){
	//generate stick list
	
	for (int i =30, j =0; j< NUMSTICKS; i++, j++)
	{
		stickoptions.AddOption(sticks[j].GetName(),i,&accept);
	}
	
	//generate knob list
	for (int i =34, j =0; j < NUMKNOBS; i++, j++)
	{
		knoboptions.AddOption(knobs[j].GetName(),i,&accept);
	}
	
	//generate button list (commands 40-55)
	for (int i =40, j =0; j < NUMBUTTONS; i++, j++)
	{
		buttonoptions.AddOption(buttons[j].GetName(),i,&accept);
	}
	
};//end CreatePresetLists()

void MenuClass::DoCommand(int command){
	//executes custom actions
	switch(command){
		case 0:
		break;
		case 1://enable
			Controller.M_Enable();
		break;
	//style
		case 5://rc drive control
			#if MENUDEBUG
			Serial.println("Damn Right");
			#endif
			PromptFor("RC CONTROL",1000);
			XBEE.SendCommand("RC");
			Controller.LoadDefaultSticks(_modelist::RC);
			Controller.M_SetOpsMode(_modelist::DRIVE);
		break;
		case 6://videogame drive control
			#if MENUDEBUG
			Serial.println("Andrew = Douche");
			#endif
			PromptFor("VID.GAME CONTROL",1000);
			XBEE.SendText("VG");
			Controller.LoadDefaultSticks(_modelist::VG);
			Controller.M_SetOpsMode(_modelist::DRIVE);
		break;
		case 7://arm control
			#if MENUDEBUG
			Serial.println("Controlling Arm");
			#endif
			PromptFor("ARM CONTROL",1000);
			Controller.M_SetOpsMode(_modelist::ARM);
		break;
		case 8://mecanum drive
			Controller.M_DriveOn(_modelist::MECANUM);
			PromptFor("MECANUM MODE",1000);
		break;
		case 9://tank drive
			Controller.M_DriveOn(_modelist::TANK);
			PromptFor("TANK MODE",1000);
		break;
	//calibrate
		case 11://calibrate sticks
			//start stick calibrating
			Controller.CalibrateSticks();
			CountDown(10);
		break;
		case 15://calibrate knobs
			//start knob calibrating
			Controller.CalibrateKnobs();
			CountDown(10);
		break;
		case 16://calibrate drive motors
			//todo: add speed controller calibrate command
		break;
		case 20://calibrate whole arm
			XBEE.SendCommand("%*");//todo: update command
		break;
		case 21://calibrate lower arm
			XBEE.SendCommand("%L");//todo: update command
		break;
		case 22://calibrate upper arm
			XBEE.SendCommand("%U");//todo: update command
		break;
		case 23://calibrate wrist
			XBEE.SendCommand("%W");//todo: update command
		break;
		case 24://calibrate gripper
			XBEE.SendCommand("%G");//todo: update command
		break;
		case 28://calibrate gyros
			//todo: add calibrate gyros command
		break;
	//config
		//config sticks
		case 30://LS U/D
		case 31://LS L/R
		case 32://RS U/D
		case 33://RS L/R
			if (currentPage == &xstick)
			{//setting x axis
				Controller.SetStick(_ax::X,&sticks[command - 30]);
			}
			else if (currentPage == &ystick)
			{//setting y axis
				Controller.SetStick(_ax::Y,&sticks[command - 30]);
			}
			else if (currentPage == &wstick)
			{//setting w axis
				Controller.SetStick(_ax::W,&sticks[command - 30]);
			}
			else if (currentPage == &zstick)
			{//setting z axis
				Controller.SetStick(_ax::Z,&sticks[command - 30]);
			}
		break;
	//config knobs
		case 34:	case 35:	case 36:	case 37://knobs 1-4
			if (currentPage == &configspeed){
				#if MENUDEBUG
				Serial.println("configspeed");
				#endif
				PromptFor("SPEED KNOB", 1200);
				Controller.SetKnob(0/*speed knob*/,&knobs[command-34]);
			}
			else if (currentPage == &configrotation){
				#if MENUDEBUG
				Serial.println("configrotation");
				#endif
				PromptFor("ROTATION KNOB", 1200);
				Controller.SetKnob(1/*rotation knob*/,&knobs[command-34]);
			}
		break;
	//config buttons
		case 40: case 41: case 42: case 43: case 44: 
		case 45: case 46: case 47: case 48: case 49: 
		case 50: case 51: case 52: case 53: case 54: 
		case 55:
			DisplayFor(
				/*watch buttons and assign whats pressed*/
				Controller.SetButton((command-40),Controller.WhatButtonPressed())\
					->GetName(),1000);/*display on screen what button assigned*/
			//GoToPage(currentPage);	//redraw page
		break;
	//config switches
		case 60: case 61: case 62: case 63: case 64:
		case 65: case 66: case 67: case 68: case 69:
			DisplayFor(
				/*watch switches and assigned whats flipped*/
				Controller.SetSwitch((command-60),Controller.WhatSwitchFlipped())\
					->GetName(),1000);/*display on screen what switch assigned*/
			//GoToPage(currentPage);	//redraw page
		break;
	//config drive
		case 70://config mecanum
			//todo: reversing of motors, axes
		break;
		case 71://config tank
			//todo: reversing of motors, axes
		break;
		case 999://restart controller
			#define RESTART_ADDR       0xE000ED0C
			#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
			#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))
			WRITE_RESTART(0x5FA0004);
		break;
		default:
			#if MENUDEBUG
			Serial.println("no command");
			#endif
		break;
	}
};//end DoCommand()

void MenuClass::PrintPage(){
	/*
	lcd.clear();
	//lcd.setCursor(PROMPT1LOC);
	lcd.print(currentPage->prompt1);
	lcd.setCursor(PROMPT2LOC);
	lcd.print(currentPage->prompt2);
	*/
	
	//Serial.print(currentPage->prompt1);
	//Serial.println(currentPage->prompt2);
	
	PrintOption();
};//end PrintPage()

void MenuClass::PrintOption(){
	/*
	lcd.setCursor(DISPLAYLOC);
	lcd.print(currentPage->_list.op[displayOption].display);
	*/
	
	//Serial.println(currentPage->_list.op[displayOption].display);
};//end PrintOption()

void MenuClass::GoToPage(pages* _new)
{
	if (Update()){//if its okay to navigate
		displayOption = 0;	//start at first option in list
		currentPage = _new;	//change page
		PrintPage();		//display it
	}
};//end GoToPage()

void MenuClass::Prev(){
	if (Update()){//if its okay to navigate
		//change to previous display option in list, loops to bottom if at the top
		displayOption = displayOption?(displayOption - 1):(currentPage->numops -1);
		PrintOption();
	}
};//end Prev

void MenuClass::Next(){
	if (Update()){//if its okay to navigate
		//change to next display option in list, loops to top if at the bottom
		displayOption = (displayOption==currentPage->numops-1)?0:(displayOption + 1);
		PrintOption();
	}
};//end Next()

void MenuClass::Back(){
	if (Update()){//if its okay to navigate
		if (currentPage->parent->specialPage != 1)//parent is real
		{
			GoToPage(currentPage->parent);//go back
		}
		else GoToPage(currentPage);//parent not real, redraw same page
	}
};//end Back()

void MenuClass::Enter(){
		if (currentPage->_list.op[displayOption].child->specialPage == 1) //child page is ack
		{
			DoCommand(currentPage->_list.op[displayOption].cmd);//execute custom action
			if (Update())//if its okay to navigate
			{
				ShowAck();	//show we did something
				GoToPage(currentPage);	//redraw page
			}
		}
		else if (currentPage->_list.op[displayOption].child->specialPage == 2) //child page is accept
		{
			DoCommand(currentPage->_list.op[displayOption].cmd);//execute custom action
			ShowAccept();	//show we did something, say we want to go to parent page when possible
		}
		else //child page is real
		{
			if (currentPage->_list.op[displayOption].child==&enabled){
				DoCommand(currentPage->_list.op[displayOption].cmd);
			}
			GoToPage(currentPage->_list.op[displayOption].child); //go to selected page
		}
};//end Enter()

void MenuClass::ShowAck(){
	/*Serial.println("ack");*/
};//end ShowAck()
	
void MenuClass::ShowAccept(){
	gotoParent=1;	//we want to go to parent page when possible
	/*Serial.println("accept");*/
};//end ShowAccept()

void MenuClass::SetHome(struct pages* _home)
{
	homepage=_home;
};//end SetHome()

void MenuClass::GoHome()
{
	GoToPage(homepage);
};//end GoHome()

void MenuClass::DisplayFor(const char* text, uint16_t time){
	if ((displayChanged + promptChanged) ==0){//if we don't have temporary displays
		displayTime = time;		//set duration of display
		lastChange = CurrentTime;	//set time to start from
		
		displayChanged = 1;		//we do have a temporary display
		strcpy(lastDisplay,currentPage->_list.op[displayOption].display);	//save so we can replace the text when display is done
		
		strcpy(currentPage->_list.op[displayOption].display,text);	//change display text for screen class to monitor
		
		changedPage = currentPage;
	}
	else //screen is pre-occupied
		Serial.println("other display change in progress");
};//end DisplayFor()

void MenuClass::PromptFor(const char* text, uint16_t time){
	if ((displayChanged + promptChanged) ==0){//if we don't have temporary displays
		displayTime = time;		//set duration of display
		lastChange = CurrentTime;	//set time to start from
		
		promptChanged = 1;		//we do have a temporary display
		strcpy(lastPrompt1,currentPage->prompt1);//save so we can replace the text when display is done
		strcpy(lastPrompt2,currentPage->prompt2);//save so we can replace the text when display is done
		strcpy(lastDisplay,currentPage->_list.op[displayOption].display);
		
		changedPage = currentPage;	//WHERE to replace text when display is done
		
		strcpy(currentPage->prompt2,text+8);	//set second part of display (characters 9+)
		strcpy(currentPage->prompt1,text);		//set first part of display
		
	}
	else //screen is pre-occupied
		Serial.println("other prompt change in progress");
};//end Promptfor()

//converts integers to characters to display
const char* TO_CHAR(int val, int base){
	static char buf[33] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
	buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
};

//returns if the screen is available for change (not busy) and 
//updates screen object with what should currently be shown
bool MenuClass::Update(){
	bool _return = 0;	//set default return
	if (countDown >0)	CountDown();	//if we are still counting down, run that function
	if (lastUpdate + updateInterval < CurrentTime){	//is it time to check for update
		if (currentPage != &enabled) Controller.M_Disable(); //set mode disabled if enable screen not active
		lastUpdate = CurrentTime;		//last time updated
		if (displayChanged){	//if display is temporary
			if (lastChange + displayTime < CurrentTime){	//temporary display time has elapsed
				displayChanged = 0;	//no longer temporary text
				strcpy(currentPage->_list.op[displayOption].display,lastDisplay);	//reset to original text
				
				displayTime = 0;
				lastChange = CurrentTime;
				_return = 1;	//good to go
			}
			//else //wait
		}//end displaychanged
		else if (promptChanged){	//if prompt is temporary
			if (lastChange + displayTime < CurrentTime){ //temporary display time has elapsed
				displayOption=0;
				promptChanged = 0;	//no longer temporary text
				strcpy(changedPage->prompt1,lastPrompt1);	//replace text
				strcpy(changedPage->prompt2,lastPrompt2);	//replace text
				//strcpy(changedPage->_list.op[displayOption].display,lastDisplay);
				
				displayTime = 0;
				lastChange = CurrentTime;
				_return = 1;	//good to go
			}
			//else //wait
		}//end promptchanged
		else if (countDown>0){	//if counting down
 			#if MENUDEBUG	//show countdown and converted value
 			Serial.println(countDown);
 			#endif
			strcpy(currentPage->_list.op[displayOption].display,TO_CHAR(countDown,10));	//change display text to how much time is left
			//Serial.println(currentPage->_list.op[displayOption].display);
		}//end counting down
		else if (countDown==0){//if we're done counting and there are no temporary displays
			displayOption=0;
			strcpy(currentPage->_list.op[displayOption].display,lastDisplay);	//reset to original text
			countDown=-1;	//dont count any more
			_return = 1;	//good to go
		}//end if done counting down
	}//end time to check for update
	
	_return = ((displayChanged||promptChanged)&&(_return==0))?0:1;
	//if _return gets set, it passes a 1, if a change or countdown is in progress, leaves as 0
	
	if (_return&&gotoParent)//if we are good to update and are supposed to go to parent page
	{
		gotoParent=0;	//no longer waiting to change pages
		GoToPage(currentPage->parent);	//go to the parent page
	}
	
	LCD.Update(GetPrompt1(),GetPrompt2(),GetDisplay());//update screen object with what should currently be shown
	return _return;
};//end Update()

short MenuClass::CountDown(){//RUN a countdown already started
	if (countDown>0)	//if we're counting down
	{
		uint32_t tempTime = CurrentTime;	//time now
		if ((tempTime-countDownStart)%1000 == 0){//elapsed countdown time is a whole second
			if(cd==0){	//have not updated the count for this second
				countDown -= 1;		//decrement count
				//currentPage->_list.op[displayOption].display = TO_CHAR(countDown,10);	//change display text to how much time is left
				cd=1;		//we have decremented this second
			}
		}
		else cd=0;	//not a whole second, reset decrement ability
	}
	if (countDown==0){	//done counting down
		cd=0;		//reset decrement ability
		//currentPage->_list.op[displayOption].display = lastDisplay;	//reset to original text
	}
	return countDown;	//return count to print on screen
};//end CountDown()

void MenuClass::CountDown(uint16_t time){
	countDown = time+1;			//start counting down from input value (seconds) [+1 to account for time between screen updates]
	countDownStart = millis();	//set start
	strcpy(lastDisplay,currentPage->_list.op[displayOption].display);	//save so we can replace the text when display is done
};//end CountDown(#)