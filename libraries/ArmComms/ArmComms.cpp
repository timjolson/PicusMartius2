#include "ArmComms.h"
#include "PID.h"
#include "EEPROMCustom.h"
#include "PropCyl.h"
#include <Adafruit_PWMServoDriver.h>


extern CalcPid PDg;
extern Adafruit_PWMServoDriver pwm;
extern PropCyl cyl_lower, cyl_upper, cyl_wrist, cyl_extra;
extern CalcPid lowerPID, upperPID, wristPID;
extern JOINT wristJoint, upperJoint, lowerJoint;
extern PIDstruct PID_lower,PID_upper,PID_wrist;
extern bool COMPRINTBACK;

//MACROS - DO NOT EDIT
#define SETOPT1 option[currCase]|=1
#define SETOPT2 option[currCase]|=2
#define SETOPT3 option[currCase]|=4
#define SETOPT4 option[currCase]|=8
#define SETOPT5 option[currCase]|=16
#define SETOPT6 option[currCase]|=32
#define SETOPT7 option[currCase]|=64
#define SETOPT8 option[currCase]|=128
#define OPT1 ((1&option[currCase])==1)
#define OPT2 ((2&option[currCase])==2)
#define OPT3 ((4&option[currCase])==4)
#define OPT4 ((8&option[currCase])==8)
#define OPT5 ((16&option[currCase])==16)
#define OPT6 ((32&option[currCase])==32)
#define OPT7 ((64&option[currCase])==64)
#define OPT8 ((128&option[currCase])==128)

void Interpret(const char* inCHAR, int* inINT, Stream *serial){
	byte n=0;	byte c=0;	byte action=0;	byte currCase;
	byte _case[_numActions]={0};	byte option[_numCases]={0};
	
	for ( ; inCHAR[c]!=0; c++){//read until no more characters
		
		if (inCHAR[c]=='%'){//calibration
			_case[action]=currCase=1;	//uses case 1 action
			byte option_last=0;			//variable to watch for a change in options
			c++;						//read next character after %
			for (bool __exit=0; !__exit; ){
				if (inCHAR[c]=='L') SETOPT1;	//add lower arm
				if (inCHAR[c]=='U') SETOPT2;	//add upper arm
				if (inCHAR[c]=='W') SETOPT3;	//add wrist
				if (inCHAR[c]=='*') SETOPT4;	//edge
				if (option_last!=option[currCase]) c++;	//option has changed, keep reading
				else{ __exit=1;}				//option did not change, allow move to next action
				option_last=option[currCase];	//keep track of option so we can watch for change
			}//end for
			action++;		//move to next action
		}//end if %
		
		else if (inCHAR[c]=='P'){
			//turns prints on
			_case[action]=currCase=2;	//case 2
			byte option_last=0;
			c++;		//read character after P
			for (bool __exit=0; !__exit; ){
				if (inCHAR[c]=='L') SETOPT1;	//print lower arm
				if (inCHAR[c]=='U') SETOPT2;	//print upper arm
				if (inCHAR[c]=='W') SETOPT3;	//print wrist
				if (option_last!=option[currCase]) c++;
				else{ __exit=1; c--; }				//option did not change, allow move to next action
				option_last=option[currCase];
			}//end for
			action++;
		}//end if P
		else if (inCHAR[c]=='p'){
			//turn off prints
			cyl_lower.PRINT=0;cyl_upper.PRINT=0;cyl_wrist.PRINT=0;
		}//end if p
		
		else if ((inCHAR[c]=='l')||(inCHAR[c]=='u')||(inCHAR[c]=='w')){
			//arm setpoints
			_case[action]=currCase=3;//case 3
			byte option_last=0;
			for (bool __exit=0; !__exit; ){
				if (inCHAR[c]=='l'){ val[currCase][0]=inINT[n];SETOPT1;n++;}	//set value to send to lower setpoint
				if (inCHAR[c]=='u'){ val[currCase][1]=inINT[n];SETOPT2;n++;}	//upper
				if (inCHAR[c]=='w'){ val[currCase][2]=inINT[n];SETOPT3;n++;}	//wrist
				if (option_last!=option[currCase]) c++;
				else{ __exit=1; c--; }				//option did not change, allow move to next action
				option_last=option[currCase];
			}//end for
			action++;
		}//end if l or u or w
		
		else if (inCHAR[c]=='+'){
			//next calibration step
			cyl_lower.Step(0);
			cyl_upper.Step(0);
			cyl_wrist.Step(0);
		}//end if +
		else if (inCHAR[c]=='*'){
			//calibrate edge
			cyl_lower.Step(3);
			cyl_upper.Step(3);
			cyl_wrist.Step(3);
		}//end if +
		
		else if (inCHAR[c]=='S'){
			//save specified joint(s) to EEPROM
			_case[action]=currCase=4;//case 4
			byte option_last=0;
			c++;
			for (bool __exit=0; !__exit; ){
				if (inCHAR[c]=='L') SETOPT1;	//save Lower
				if (inCHAR[c]=='U') SETOPT2;	//save upper
				if (inCHAR[c]=='W') SETOPT3;	//save wrist
				if (option_last!=option[currCase]) c++;
				else{ __exit=1; c--; }				//option did not change, allow move to next action
				option_last=option[currCase];
			}//end for
			action++;
		}//end if S
		
        /* RESET Command for Teensy */
		else if (inCHAR[c]=='r') {
			//reset board
			#define RESTART_ADDR       0xE000ED0C
			#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
			#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))
			WRITE_RESTART(0x5FA0004);
		}
		
		else{
            serial->print(inCHAR[c]);
            serial->print("  ::Not a Recognized Command  c::");
            serial->println(c);
        }	// no command for character entered
		
	}//end for inCHAR!=0
	
	action=0;
	bool _ran[_numCases]={0};
	for ( ; (_case[action]!=0)&&(action!=_numActions); action++ )
	{
		currCase=_case[action];
		if (!_ran[currCase]){
			switch (currCase){
				case 1://calibrate
				if OPT1{
					serial->println("\n\nCalibrate Arm Joint:");
					serial->println("Lower");
					if (OPT4){
						cyl_lower.Step(3);//changed from .calibrating=3
					}
					else cyl_lower.calibrating = (cyl_lower.calibrating!=-1)?-1:0;
				}
				if OPT2{
					serial->println("\n\nCalibrate Arm Joint:");
					serial->println("Upper");
					if (OPT4){
						cyl_upper.Step(3);//changed from .calibrating=3
					}
					else cyl_upper.calibrating = (cyl_upper.calibrating!=-1)?-1:0;
				}
				if OPT3{
					serial->println("\n\nCalibrate Arm Joint:");
					serial->println("Wrist");
					if (OPT4){
						cyl_wrist.Step(3);//changed from .calibrating=3
					}
					else cyl_wrist.calibrating = (cyl_wrist.calibrating!=-1)?-1:0;
				}
				if (OPT4 && !OPT1 && !OPT2 && !OPT3){
					serial->println("\n\nCalibrate Whole Arm");
					//edge calibration for all joints
					cyl_lower.Step(3);//changed from .calibrating=3
					cyl_upper.Step(3);//changed from .calibrating=3
					cyl_wrist.Step(3);//changed from .calibrating=3
				}
				_ran[currCase]=1;
				break;
				
				case 2://prints
				if OPT1{
					serial->print("Lower Arm");
					cyl_lower.PRINT=1;
				}
				if OPT2{
					serial->print("Upper Arm");
					cyl_upper.PRINT=1;
				}
				if OPT3{
					serial->print("Wrist Joint");
					cyl_wrist.PRINT=1;
				}
				_ran[currCase]=1;
				serial->print(" Prints on");
				break;
				
				case 3://arm setpoints
				if OPT1{
					serial->print("lower:");
					serial->println(val[currCase][0]);
					cyl_lower.SetSp(val[currCase][0]);
				}
				if OPT2{
					serial->print("upper:");
					serial->println(val[currCase][1]);
					cyl_upper.SetSp(val[currCase][1]);
				}
				if OPT3{
					serial->print("wrist:");
					serial->println(val[currCase][2]);
					cyl_wrist.SetSp(val[currCase][2]);
				}
				_ran[currCase]=1;
				break;
				
				case 4://save arm cal
				if OPT1{
					cyl_lower.Save();
				}
				if OPT2{
					cyl_upper.Save();
				}
				if OPT3{
					cyl_wrist.Save();
				}
				_ran[currCase]=1;
				break;
				
				default:
				serial->print("\n\nComms error! No case " + currCase);//serial->print(currCase);
				serial->println(" exists...\n");
				break;
			}//end switch
		}//end  !ran currCase
	}//end for all actions
}//end Interpret()
