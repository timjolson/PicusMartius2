#ifndef Comms_h
#define Comms_h

#include <Arduino.h>

boolean isNumeric(char arg);

extern void Interpret(const char* inCHAR, int* inINT, Stream *serial);

void getSerial(Stream* serial);
void getSerial(const char* string);

const int _numChars =40;	//max number of characters per command line
const int _numValues =40;	//max number of values per command line
const int _numActions =40;	//max number of things that can occur per command line
const int _numCases =8;		//max number of different command cases
extern int val[_numCases][_numValues];

#endif