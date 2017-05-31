#include "Variables.h"
#include <Arduino.h>

bool GO = 1;    //robot safe to do things or robot is allowed to drive

uint32_t CurrentTime =0;//current time in millis
uint64_t CurrentTimeMicros =0;//current time in micros
uint32_t CycleTime = 0;	//time between cycle starts in millis
uint32_t CycleTimeMicros =0; //time between cycle starts in micros
uint32_t LastTime = 0;	//millis of previous cycle start
uint32_t LastTimeMicros=0; //micros of previous cycle start

uint8_t NextServoNum = 0;   //index for next 'Servo' object

short heading;          //robot heading angle

bool on_target;         //whether or not the robot is on target

//printing (mostly obselete)
bool PRINTM=0;
bool PRINTG=0;
bool PRINTJ1=0;
//calibration countdowns (mostly obselete)
short calibratingA=0;
short calibratingG=0;
short calibratingJ=0;
short calibratingM=0;

//print communication debugs
bool COMPRINTBACK=0;

