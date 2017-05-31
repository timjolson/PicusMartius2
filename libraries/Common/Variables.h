#ifndef Variables_h
#define Variables_h

#include <Arduino.h>

extern uint32_t CurrentTime;//current time in millis
extern uint64_t CurrentTimeMicros;//current time in micros
extern uint32_t CycleTime;	//time between cycle starts in millis
extern uint32_t CycleTimeMicros;
extern uint32_t LastTime;	//millis of previous cycle start
extern uint32_t LastTimeMicros;
extern uint8_t NextServoNum;
extern bool COMPRINTBACK;
extern bool GO;

//enumerators
enum _motor { mUP=1, mDOWN=0, mIN=0, mOUT=1}; //enumerated default motor positions
enum _st {LSUD,LSLR,RSUD,RSLR};		//enumerated list of sticks
enum _b { TRIANGLE,START,SQUARE,OH,UP,LEFT,DOWN,EX,RIGHT,SELECT,NONE };	//enumerated list of buttons
enum _sw { R1,R2,RF,L1,L2,LF,NOSW };	//enumerated list of switches
enum _ax { X, Y, W, Z };		//enumerated list of axes
enum _modelist {	//whole controller modes for specific application
	RC,			//radio control style
	VG,			//video game control style
	MECANUM,	//mecanum drive
	TANK,		//tank drive
	DRIVE,		//drive the bot
	ARM			//operate the arm
};

//command options
const bool UPLOADINSTRUCT=0;	//act as passthrough at startup

//Drive
const short PWMmax = 4095;		//max PWM for expander chip
const short PWMmin = 0;			//min PWM for expander chip
const short PWMneutral = 2047;	//neutral PWM for expander chip

//Servo
const short ServoMax = 180;		//max servo value
const short ServoMin = 0;		//min servo value
const short ServoNeutral = 90;	//neutral servo value
const uint8_t MAXSERVOS = 10;	//max number of servos in project

//controller config
const uint8_t NUMBUTTONS = 11;	//size of button array (real quantity +1)
const uint8_t NUMSWITCHES = 7;	//size of switch array (real quantity +1)
const uint8_t NUMSTICKS = 4;	//size of stick array
const uint8_t NUMKNOBS = 4;		//size of knobs array
//radio info from controller config
const uint8_t DATASIZEBOOL=(NUMBUTTONS+NUMSWITCHES+NUMKNOBS*8+NUMSTICKS*10);
const uint8_t DATASIZEBYTE=(DATASIZEBOOL/8+1);

typedef struct JOINTVALUES
{
	int min;	//minimum
	int max;	//maximum
	int range;	//range of values
}JOINTVALUES;
typedef struct JOINT
{
	uint16_t loc;			//location in eeprom
	JOINTVALUES raw;	//position values
	JOINTVALUES encoder;//encoder values
	JOINTVALUES angle;	//angle mapping for joint values
}JOINT;

#endif