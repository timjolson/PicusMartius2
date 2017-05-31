#include "Comms.h"
#include <Arduino.h>
#include "Variables.h"

extern void Interpret(const char* inCHAR, int* inINT, Stream *serial);

int val[_numCases][_numValues];

bool isNumeric(char arg){
	return (((int) arg)>=48 && ((int) arg)<=57);
};//returns if a character is a number

void getSerial(Stream *serial){
	if (serial->available()){
		int recN[_numValues]={0};
		char recC[_numChars]={0};
		byte c=0;
		byte n=0;
		boolean isNegative[_numValues]={0};
		int i = 0;
		delayMicroseconds(10);
		while(serial->available()&&(i<15))
		{
			if (isNumeric(serial->peek()))
			{//get numeric bytes
				recN[n]=serial->parseInt();
				if (isNegative[n])recN[n] *=-1;
				if (COMPRINTBACK){
					serial->print("recN=");
					serial->println(recN[n]);
				}//end COMPRINTBACK
				n++;
			}//end numeric
			else
			{//get non-numeric
				char p = serial->read();
				if (p=='-')isNegative[n]=1;
				else if ((p!=',')&&(p!='!')){
					recC[c]=p;
					if (COMPRINTBACK){
						serial->print("recC=");
						serial->println(recC[c]);
					}//end COMPRINTBACK
					c++;
				}
			}//end non-numeric
			i++;
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
				if (COMPRINTBACK){
					Serial.print("recN=");
					Serial.println(recN[n]);
				}//end COMPRINTBACK
				n++;
			}//end numeric
			else
			{//get non-numeric
				char p = string[i];
				if (p=='-')isNegative[n]=1;
				else if ((p!=',')&&(p!='!')){
					recC[c]=p;
					if (COMPRINTBACK){
						Serial.print("recC=");
						Serial.println(recC[c]);
					}//end COMPRINTBACK
					c++;
				}
			}//end non-numeric
		}//end for serial.available
		Interpret(recC,recN,&Serial);
	}//end if characters remain
}//end getSerial
