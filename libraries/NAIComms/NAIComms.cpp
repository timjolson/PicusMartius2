#include "NAIComms.h"
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
extern RobotClass NAI;
extern CalcPd PDg;
extern TrackerClass Tracker;
extern unsigned long last_command_time;

void Interpret(const char* inCHAR, int* inINT, Stream *serial){
	byte n=0;	byte c=0;	byte action=0;	byte currCase;
	byte _case[_numActions]={0};	byte option[_numCases]={0};
	int val[_numCases][_numValues];
	
	for ( ; inCHAR[c]!=0; c++){//read until no more characters
		if (inCHAR[c]=='Z' || inCHAR[c]=='z'){
        //stop
			GO = 0;
			action++;
		}//end if Z/z
		
        else if (inCHAR[c]=='G' || inCHAR[c]=='g'){
        //stop
			GO = 1;
            last_command_time = micros();
			action++;
		}//end if G/g
        
        else if (inCHAR[c]=='M'){
			_case[action]=currCase=1;//case 1
			byte option_last=0;
			c++;
			for (bool __exit=0; !__exit; ){
				if (inCHAR[c]=='V'){ val[currCase][0]=inINT[n];SETOPT1;n++;	}//move
				if (option_last!=option[currCase]) c++;
				else	__exit=1;
				option_last=option[currCase];
			}//end for
			action++;
		}//end if MV
        
         else if (inCHAR[c]=='R'){
			_case[action]=currCase=2;//case 2
			byte option_last=0;
			c++;
			for (bool __exit=0; !__exit; ){
				if (inCHAR[c]=='R'){ val[currCase][0]=inINT[n];SETOPT1;n++;	}//spin
                if (inCHAR[c]=='L'){ val[currCase][1]=inINT[n];SETOPT2;n++;	}//spin
                if (inCHAR[c]=='A'){ val[currCase][2]=inINT[n];SETOPT3;n++; }//set angle
                if (inCHAR[c]=='r'){ val[currCase][3]=inINT[n];SETOPT4;n++; }//set relative angle
				if (option_last!=option[currCase]) c++;
				else	__exit=1;
				option_last=option[currCase];
			}//end for
			action++;
		}//end if MV
        
        else if (inCHAR[c]=='U'){
            if (inCHAR[c]=='H'){ Tracker.SetPos(inINT[n]);NAI.UpdateHeading(inINT[n]);n++;c++;}//update heading
            action++;
		}
        else if (inCHAR[c]=='C'){
            if (inCHAR[c]=='G'){ Tracker.SetCal(1);Serial.println("Flip calibrated");c++;}
            action++;
        }
        
		else if (inCHAR[c]=='r') {
			//reset board
			#define RESTART_ADDR       0xE000ED0C
			#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
			#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))
			WRITE_RESTART(0x5FA0004);
		}
        
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
                    //Serial.print("INPUT = ");Serial.println(val[currCase][0]);
                    //Serial.print("INPUT = ");Serial.println(val[currCase][0]*180/PI);
                    NAI.driveRCmecanum(val[currCase][0]*180/PI);
				break;
                case 2://turn
                    if OPT1{ //RR
                        //Serial.print("comms1");Serial.println(val[currCase][0]);
                        NAI.turnRCmecanum(val[currCase][0]);
                    }
                    if OPT2{ //RL
                        //Serial.print("comms2");Serial.println(((int)-1) * val[currCase][1]);
                        NAI.turnRCmecanum(-1 * val[currCase][1]);
                    }
                    else if OPT3{ //RA
                        //Serial.print("heading:");Serial.println(val[currCase][2]);
                        NAI.SetHeading(val[currCase][2]*180/PI);
                    }
                    else if OPT4{ //Rr
                        //Serial.print("add:");Serial.print(val[currCase][3]*180/PI);
                        //Serial.print(" to " );Serial.println(Tracker.GetPos(0)*10);
                        NAI.SetHeading(val[currCase][3]*180/PI + Tracker.GetPos(0)*10);
                    }
                break;
                default:
				Serial.print("\n\nComms error! No case " + currCase);
				Serial.println(" exists...\n");
				break;
			}//end switch
		}//end  !ran currCase
	}//end for all actions
}//end Interpret()
