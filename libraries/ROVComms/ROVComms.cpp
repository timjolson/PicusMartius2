#include "ROVComms.h"
#include "ROV.h"
#include "Variables.h"
#include "Thruster.h"
#include "CSensor.h"
#include "IMU.h"
#include "PSensor.h"
#include "ROVEthernet.h"
#include "TSensor.h"
#include "VideoFeed.h"
#include "PID.h"
#include <Arduino.h>

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
	
	char buff[100];
	
	for ( ; inCHAR[c]!=0; c++){//read until no more characters
		if (inCHAR[c]=='Z' || inCHAR[c]=='z'){
        //stop
			GO = 0;
			action++;
		}//end if Z/z
        
		if (inCHAR[c]=='x'){
            //get x axis value from joystick
			_case[action]=currCase=1;
			val[currCase][0]=inINT[n];
			SETOPT1;
			n++;
                	
			if (inCHAR[c]=='y'){
                //get y axis value from joystick
				_case[action]=currCase=1;
				val[currCase][1]=inINT[n];
				SETOPT2;
				n++;
				c++;
                		
				if (inCHAR[c]=='z'){
                    //get z axis value from joystick
					_case[action]=currCase=1;
					val[currCase][2]=inINT[n];
					SETOPT3;
					n++;
				}
			}
		action++;
		}
        
		else if (inCHAR[c]=='a'){
            //get angle from controller
			_case[action]=currCase=2;//case 2
			val[currCase][0]=inINT[n];
			SETOPT1;
			n++;
			c++;

            if (inCHAR[c]=='r'){
                //get amplitude/radius from controller
                _case[action]=currCase=2;//case 2
                val[currCase][1]=inINT[n];
                SETOPT2;
                n++;
                c++;

                if (inCHAR[c]=='w'){
                    //get yaw from controller
                    _case[action]=currCase=2;//case 2
                    val[currCase][2]=inINT[n];
                    SETOPT3;
                    n++;
				}
			}
			action++;
		}
        
        else if (inCHAR[c]=='C'){
            //change feed 1 to next camera
            Cam1.SetFeed(inINT[n]);
			sprintf(buff, "C%d,%d",Cam1.GetFeed(),Cam2.GetFeed());
			SendEthernet(buff);
            //Serial.println(buff);
			action++;
            n++;
		}//end if M
        
        else if (inCHAR[c]=='c'){
            //change feed 2 to next camera
            Cam2.SetFeed(inINT[n]);
			sprintf(buff, "C%d,%d",Cam1.GetFeed(),Cam2.GetFeed());
			SendEthernet(buff);
            //Serial.println(buff);
			action++;
            n++;
		}//end if M
        
    /**** New as of 6/16 ****/
    /************************/
        else if (inCHAR[c]=='O'){
            if(inINT[n])
                ROV.EnableOrientation();
            else
                ROV.DisableOrientation();
            action++;
            n++;
        }
        else if (inCHAR[c]=='D'){
            c++;
            if (inCHAR[c]=='p'){
                ROV.depthPID.SetKp(inINT[n] / 1000.0);
            }
            else if (inCHAR[c]=='i'){
                ROV.depthPID.SetKi(inINT[n] / 1000.0);
            }
            else if (inCHAR[c]=='d'){
                ROV.depthPID.SetKd(inINT[n] / 1000.0);
            }
            else{
                if(inINT[n])
                    ROV.EnableDepth();
                else
                    ROV.DisableDepth();
            }
            action++;
            n++;
        }
        else if (inCHAR[c]=='Y'){
            c++;
            if (inCHAR[c]=='p'){
                ROV.yawPID.SetKp(inINT[n] / 1000.0);
            }
            else if (inCHAR[c]=='i'){
                ROV.yawPID.SetKi(inINT[n] / 1000.0);
            }
            else if (inCHAR[c]=='d'){
                ROV.yawPID.SetKd(inINT[n] / 1000.0);
            }
            else{
                if(inINT[n])
                    ROV.EnableYaw();
                else
                    ROV.DisableYaw();
            }
            action++;
            n++;
        }
        else if (inCHAR[c]=='T'){
            c++;
            if (inCHAR[c]=='p'){
                ROV.rollPID.SetKp(inINT[n] / 1000.0);
                ROV.pitchPID.SetKp(inINT[n] / 1000.0);
            }
            else if (inCHAR[c]=='i'){
                ROV.rollPID.SetKi(inINT[n] / 1000.0);
                ROV.pitchPID.SetKi(inINT[n] / 1000.0);
            }
            else if (inCHAR[c]=='d'){
                ROV.rollPID.SetKd(inINT[n] / 1000.0);
                ROV.pitchPID.SetKd(inINT[n] / 1000.0);
            }
            action++;
            n++;
        }
        else if (inCHAR[c]=='%'){
            ROV.Save();
            action++;
        }
        else if (inCHAR[c]=='&'){
            ROV.CalibrateOrientation();
            action++;
        }
        else if (inCHAR[c]=='M'){
            ROV.ResetAlarms();
            ROV.CanDive();
            action++;
        }
        else if (inCHAR[c]=='m'){
            ROV.CanDive();
        }
        else if (inCHAR[c]=='s'){
            ROV.IsAtSurface();
            action++;
        }
        else if (inCHAR[c]=='S'){
            ROV.IsAboveSurface();
            action++;
        }
        else if (inCHAR[c]=='P'){
            if(inINT[n]){
				SendEthernet("P1");
                ROV.PumpOn();
			}
            else
			{
                ROV.PumpOff();
				SendEthernet("P0");
			}
            action++;
            n++;
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
				case 1://tilt
                    //orientation control from joystick
                    ROV.Tilt(val[currCase][0]-128,val[currCase][1]-128,val[currCase][2]-50);
				break;
                case 2://move
                    //driving control from controller
                    ROV.Drive(val[currCase][0],val[currCase][1],val[currCase][2]-50);
                break;
                default:
                    serial->print("\n\nComms error! No case " + currCase);
                    serial->println(" exists...\n");
                break;
			}//end switch
		}//end  !ran currCase
	}//end for all actions
}//end Interpret()
