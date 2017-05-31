#ifndef PARSING_H
#define PARSING_H

#include "SmartLynx.h"

const bool PRINTBACK = 0;

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

extern SmartLynx Drawer;

struct TOOLstruct
{
    uint16_t loc;
    char name[10];
    byte drawer;
    uint16_t type;
    uint16_t size;
};


const int _numChars =40;	//max number of characters per command line
const int _numValues =40;	//max number of values per command line
const int _numActions =40;	//max number of things that can occur per command line
const int _numCases =8;		//max number of different command cases
int val[_numCases][_numValues];

bool isNumeric(char arg){
	return (((int) arg)>=48 && ((int) arg)<=57);
};//returns if a character is a number

void Interpret(const char* inCHAR, int* inINT, Stream *serial){
	byte n=0;	byte c=0;	byte action=0;	byte currCase;
	byte _case[_numActions]={0};	byte option[_numCases]={0};
	
	for ( ; inCHAR[c]!=0; c++){//read until no more characters
		if (inCHAR[c]=='x'){
            Drawer.GoToMin();
            serial->println("Closing...");
		}
		else if (inCHAR[c]=='o'){
			//_case[action]=currCase=2;//case 2
            Drawer.GoToMax();
            serial->println("Opening...");
		}
        
        else if (inCHAR[c]=='G'){
            Drawer.GoToInches(float(inINT[n])/10.0);
            serial->print("Going to: ");
            serial->println(inINT[n]/10.0);
            n++;
        }
        else if (inCHAR[c]=='g'){
            Drawer.MoveInches(float(inINT[n])/10.0);
            serial->print("Moving: ");
            serial->println(inINT[n]/10.0);
            n++;
        }
        
        else if (inCHAR[c]=='m'){
            c++;
            if (inCHAR[c]=='i'){
                c++;
                if (inCHAR[c]=='n'){
                    c++;
                    serial->println("Min Set");
                    Drawer.SetMin();
                }
            }
            else if (inCHAR[c]=='a'){
                c++;
                if (inCHAR[c]=='x'){
                    c++;
                    serial->println("Max Set");
                    Drawer.SetMax();
                }
            }
        }
        
        else if (inCHAR[c]=='e'){
            Drawer.Enable();
            serial->println("Enabling...");
        }
        else if (inCHAR[c]=='d'){
            Drawer.Disable();
            serial->println("Disabling...");
        }
        
        else if (inCHAR[c]=='r'){
            Drawer.ResetPos();
            serial->println("Reset Pos...");
        }
        else if (inCHAR[c]=='R'){
            Drawer.SetPosMax();
            serial->println("Reset Pos...");
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
				case 1:
				break;
                default:
                    serial->print("\n\nCommand error! No case ");
                    serial->print(currCase);
                    serial->println(" exists...\n");
                break;
            }//end switch
		}//end  !ran currCase
	}//end for all actions
}//end Interpret()

void getSerial(Stream *serial){
	if (serial->available()){
		int recN[_numValues]={0};
		char recC[_numChars]={0};
		byte c=0;
		byte n=0;
		boolean isNegative[_numValues]={0};
		for(int i=0; (serial->available()&&(i<15)); i++)
		{
			if (isNumeric(serial->peek()))
			{//get numeric bytes
				recN[n]=serial->parseInt();
				if (isNegative[n])recN[n] *=-1;
				if (PRINTBACK){
					serial->print("recN=");
					serial->println(recN[n]);
				}//end PRINTBACK
				n++;
			}//end numeric
			else
			{//get non-numeric
				char p = serial->read();
				if (p=='-')isNegative[n]=1;
				else if ((p!=',')&&(p!='!')){
					recC[c]=p;
					if (PRINTBACK){
						serial->print("recC=");
						serial->println(recC[c]);
					}//end PRINTBACK
					c++;
				}
			}//end non-numeric
		}//end for serial.available
		Interpret(recC,recN,serial);
	}//end if available
}//end getSerial

void getSerial(const char* string){
	if (string[0]!='\0'){
		int recN[_numValues]={0};
		char recC[_numChars]={0};
		byte c=0;
		byte n=0;
		boolean isNegative[_numValues]={0};
		for(int i=0; (string[i]!='\0'&&(i<20)); i++)
		{
			if (isNumeric(string[i]))
			{//get numeric bytes
                char number[10];
                int j=i;
                int digits=0;
                for ( ; isNumeric(string[j])&&(j<20); j++, digits++)
                {   // as long is there are numerals
                    number[digits] = string[j];    //store it to temporary spot
                }
                number[digits] = '\0'; //set last character to null
                i+=digits-1;

                recN[n]=atoi(number); //convert to number

                if (isNegative[n])recN[n] *=-1;
                if (PRINTBACK){
                    Serial.print("recN=");
                    Serial.println(recN[n]);
                }//end PRINTBACK
                n++;
			}//end numeric
			else
			{//get non-numeric
				char p = string[i];
				if (p=='-')isNegative[n]=1;
				else if ((p!=',')&&(p!='!')){
					recC[c]=p;
					if (PRINTBACK){
						Serial.print("recC=");
						Serial.println(recC[c]);
					}//end PRINTBACK
					c++;
				}
			}//end non-numeric
		}//end for serial.available
		Interpret(recC,recN,&Serial);
	}//end if characters remain
}//end getSerial


#endif
