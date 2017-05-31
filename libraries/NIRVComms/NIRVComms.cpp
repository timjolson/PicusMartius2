#include "NIRVComms.h"
#include "Variables.h"
#include "Robot1.h"
#include "Tracker.h"
#include "PID.h"
#include <Arduino.h>

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

extern bool PRINTBACK;
extern bool GO;
extern RobotClass NIRV;
extern unsigned long last_command_time;
extern uint64_t CurrentTime;

void Interpret(const char* inCHAR, int* inINT, Stream *serial){
	byte n=0;	byte c=0;	byte action=0;	byte currCase;
	byte _case[_numActions]={0};	byte option[_numCases]={0};
	
	for ( ; inCHAR[c]!=0; c++){//read until no more characters
		if (inCHAR[c]=='Z' || inCHAR[c]=='z'){
        //stop
			GO = 0;
			action++;
		}//end if Z/z
        
        else if (inCHAR[c]=='J'){
			_case[action]=currCase=2;//case 2
            for (int value=0;value<8;value++,n++){
                val[currCase][0]=inINT[n];
			}//end for
			action++;
		}//end if MV
       
		else if (inCHAR[c]=='r') {
			//reset board
			#define RESTART_ADDR       0xE000ED0C
			#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
			#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))
			WRITE_RESTART(0x5FA0004);
		}
        
        else if (inCHAR[c]=='X'){}
        
		else{
            serial->print(byte(inCHAR[c]));
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
				case 1://move
                    
				break;
                case 2://controller data
                    Serial.println("Processed");
                    NIRV.SetControlData(val[currCase]);
                    last_command_time = CurrentTime;
                break;
                default:
				Serial.print("\n\nComms error! No case " + currCase);
				Serial.println(" exists...\n");
				break;
			}//end switch
		}//end  !ran currCase
	}//end for all actions
}//end Interpret()
