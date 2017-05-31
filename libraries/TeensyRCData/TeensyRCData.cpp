/*
-----------------------------------------------------------------------------------------
NIU ROBOTICS LIBRARY:   TeensyRCData
-----------------------------------------------------------------------------------------

FILENAME:   TeensyRCData.cpp

AUTHORS:    Nathan Freitag

DATE:       2015-11-13

PURPOSE:    This library class is designed to reading in RC data values from an
            RC transmitter and receiver. This library is Teensy specific and uses
            pin interupts.

SPECS:      Max number of channels:     8

USEAGE:     To initialize library (call in setup function in the main .ino file):

                setupRCData(pinNumbers); // where pinNumbers is an 8 elemet array with the pins wanted

            to get the data from the pin at position i (determined by the order given as setup):

                x = rcValue[i]; // the value will be stored in x (min = 900, max = 2100)    
            
NOTES:      Writing still in progress. Testing not complete.
*/

#include "TeensyRCData.h"

#define RC_MIN 900      // minimum valid value allowed to be read
#define RC_MAX 2100     // maximum valid value allowed to be read

// array to store the values of the channels - auto updated by the interupts
// must be volitile to make sure that the compiler allows the value to be changed at all times
volatile uint16_t rcValue[8] = {1502, 1502, 1502, 1502, 1502, 1502, 1502, 1502}; // interval [1000;2000]
uint8_t rcChannel[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint32_t rcEdgeTime[8];

// protoyptes for all the interup/service fuctions
void RCServiceR0();
void RCServiceF0();
void RCServiceR1();
void RCServiceF1();
void RCServiceR2();
void RCServiceF2();
void RCServiceR3();
void RCServiceF3();
void RCServiceR4();
void RCServiceF4();
void RCServiceR5();
void RCServiceF5();
void RCServiceR6();
void RCServiceF6();
void RCServiceR7();
void RCServiceF7();
void RCServiceR8();
void RCServiceF8();

// initializer function - call in the setup fuction in the driver (.ino) file
void setupRCData(int pins[]) 
{
    // set the pins
    for (int i = 0; i < 6; i++)
    {
        rcChannel[i] = pins[i];
    }

    if (rcChannel[0]) 
    {
        pinMode(rcChannel[0],INPUT);
        attachInterrupt(rcChannel[0], RCServiceR0, RISING);
    }
    if (rcChannel[1]) 
    {
        pinMode(rcChannel[1],INPUT);
        attachInterrupt(rcChannel[1], RCServiceR1, RISING);
    }
    if (rcChannel[2]) 
    {
        pinMode(rcChannel[2],INPUT);
        attachInterrupt(rcChannel[2], RCServiceR2, RISING);
    }
    if (rcChannel[3]) 
    {
        pinMode(rcChannel[3],INPUT);
        attachInterrupt(rcChannel[3], RCServiceR3, RISING);
    }
    if (rcChannel[4]) 
    {
        pinMode(rcChannel[4],INPUT);
        attachInterrupt(rcChannel[4], RCServiceR4, RISING);
    }
    if (rcChannel[5]) 
    {
        pinMode(rcChannel[5],INPUT);
        attachInterrupt(rcChannel[5], RCServiceR5, RISING);
    }
    if (rcChannel[6]) 
    {
        pinMode(rcChannel[6],INPUT);
        attachInterrupt(rcChannel[6], RCServiceR6, RISING);
    }
    if (rcChannel[7]) 
    {
        pinMode(rcChannel[7],INPUT);
        attachInterrupt(rcChannel[7], RCServiceR7, RISING);
    }
    if (rcChannel[8]) 
    {
        pinMode(rcChannel[8],INPUT);
        attachInterrupt(rcChannel[8], RCServiceR8, RISING);
    }
}

void RCServiceR0() 
{
    rcEdgeTime[0]= micros();
    attachInterrupt(rcChannel[0], RCServiceF0, FALLING);
}

void RCServiceF0() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[0], RCServiceR0, RISING);
    sei();
    current = current - rcEdgeTime[0];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[0] = current;
    }
}

void RCServiceR1()
{
    rcEdgeTime[1]= micros();
    attachInterrupt(rcChannel[1], RCServiceF1, FALLING);
}

void RCServiceF1() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[1], RCServiceR1, RISING);
    sei();
    current = current - rcEdgeTime[1];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[1] = current;
    }
}

void RCServiceR2() 
{
    rcEdgeTime[2]= micros();
    attachInterrupt(rcChannel[2], RCServiceF2, FALLING);
}

void RCServiceF2() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[2], RCServiceR2, RISING);
    sei();
    current = current - rcEdgeTime[2];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[2] = current;
    }
}

void RCServiceR3() 
{
    rcEdgeTime[3]= micros();
    attachInterrupt(rcChannel[3], RCServiceF3, FALLING);
}

void RCServiceF3() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[3], RCServiceR3, RISING);
    sei();
    current = current - rcEdgeTime[3];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[3] = current;
    }
}

void RCServiceR4() 
{
    rcEdgeTime[4]= micros();
    attachInterrupt(rcChannel[4], RCServiceF4, FALLING);
}

void RCServiceF4() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[4], RCServiceR4, RISING);
    sei();
    current = current - rcEdgeTime[4];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[4] = current;
    }
}

void RCServiceR5() 
{
    rcEdgeTime[5]= micros();
    attachInterrupt(rcChannel[5], RCServiceF5, FALLING);
}

void RCServiceF5() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[5], RCServiceR5, RISING);
    sei();
    current = current - rcEdgeTime[5];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[5] = current;
    }
}

void RCServiceR6() 
{
    rcEdgeTime[6]= micros();
    attachInterrupt(rcChannel[6], RCServiceF6, FALLING);
}

void RCServiceF6() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[6], RCServiceR6, RISING);
    sei();
    current = current - rcEdgeTime[6];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[6] = current;
    }
}

void RCServiceR7() 
{
    rcEdgeTime[7]= micros();
    attachInterrupt(rcChannel[7], RCServiceF7, FALLING);
}

void RCServiceF7() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[7], RCServiceR7, RISING);
    sei();
    current = current - rcEdgeTime[7];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[7] = current;
    }
}

void RCServiceR8() 
{
    rcEdgeTime[8]= micros();
    attachInterrupt(rcChannel[8], RCServiceF8, FALLING);
}

void RCServiceF8() 
{
    uint32_t current= micros();
    attachInterrupt(rcChannel[8], RCServiceR8, RISING);
    sei();
    current = current - rcEdgeTime[8];
    if (current >= RC_MIN && current <= RC_MAX) 
    {
        rcValue[8] = current;
    }
}

