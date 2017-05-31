/*
-----------------------------------------------------------------------------------------
NIU ROBOTICS LIBRARY:   TeensyRCData
-----------------------------------------------------------------------------------------

FILENAME:	TeensyRCData.h

AUTHORS:	Nathan Freitag

NOTES:		This is the header file for the TeensyRCData library.
*/

#ifndef TeensyRCDATA_H
#define TeensyRCDATA_H

#include <Arduino.h>
#include <inttypes.h>

extern volatile uint16_t rcValue[];

// prototype for the 
void setupRCData(int []);

#endif
