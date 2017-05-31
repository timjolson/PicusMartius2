#ifndef Screen_h
#define Screen_h
#include <Arduino.h>

#define SCREENDEBUG 1	//enable serial printing of screen items
#define SIZEOFPROMPT 8
#define SIZEOFDISPLAY 16

/*
Screen Class allows for watching for updated variables to print to an lcd screen
*/
class ScreenClass
{
	uint16_t interval;	//time interval to look for updates\rewrite screen
	char prompt1[SIZEOFPROMPT];		//text to display first on screen
	char prompt2[SIZEOFPROMPT];		//text to display half way across first line of screen
	uint32_t lastUpdate;//time screen last changed
protected:
	char display[SIZEOFDISPLAY];		//text to display on bottom line of screen
public:
	void Init(uint16_t);				//initialize screen, using LiquidCrystal library, pass in update interval in millis
	void Update(const char* _p1,const char* _p2,const char* _disp);		//watches for changes in display text, rewrites screen when applicable
	ScreenClass(){lastUpdate = 0;strcpy(prompt1,"No  Init");};
};


#endif