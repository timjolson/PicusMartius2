#include "Screen.h"
#include <LiquidCrystal.h>

#define PRINTSCREEN 1
#define DEBUGSCREEN 0

extern LiquidCrystal lcd;
#define PROMPT1LOC 0,0	//top left cursor position
#define PROMPT2LOC 8,0	//mid-way top row cursor position
#define DISPLAYLOC 0,1	//bottom left cursor position

extern uint32_t CurrentTime;

void ScreenClass::Init(uint16_t _interv){
	interval = _interv;	//set update interval
	lcd.begin(16,2);	//lcd screen is 2 lines, 16 columns
	lcd.clear();		//clear lcd screen
	strcpy(prompt1,"Init-ed    ");
}//end Init()

void ScreenClass::Update(const char* _p1, const char* _p2, const char* _disp){
	//if interval since last screen update has passed
	if (CurrentTime - lastUpdate >= interval)
	{
		lastUpdate = CurrentTime;				//keep track of time
		if (strcmp(prompt1,_p1)||strcmp(prompt2,_p2)){	//prompts (first row) have changed
			#if DEBUGSCREEN
			Serial.print("len");Serial.println(strlen(_p1));
			#endif
			strncpy(prompt1,_p1,sizeof(prompt1));
			#if DEBUGSCREEN
			Serial.print("len");Serial.println(strlen(_p2));
			#endif
			strcpy(prompt2,_p2);
			
			lcd.clear();						//clear screen
			lcd.clear();
			lcd.setCursor(PROMPT1LOC);			//move to first write location
			lcd.print(prompt1);					//print whatever is located at p1
			lcd.setCursor(PROMPT2LOC);			//move to second write location
			lcd.print(prompt2);					//print whatever is located at p2
			
			#if PRINTSCREEN
			Serial.print(prompt1);//print to serial monitor for debugging
			Serial.println(prompt2);
			#endif
			
		}
		
		if(strcmp(display,_disp)){	//display (second row) has changed
			strcpy(display,_disp);
			
			lcd.setCursor(DISPLAYLOC);			//move to display location
			lcd.print("                ");		//clear entire row
			lcd.setCursor(DISPLAYLOC);			//move to display location
			lcd.print(display);					//print whatever is located at display
			
			#if PRINTSCREEN
			Serial.println(display);//print to serial monitor for debugging
			#endif
		}
	}
}//end Update()
