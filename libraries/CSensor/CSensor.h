#ifndef CSensor_h
#define CSensor_h
#include <Arduino.h>

class CSensor {
    int pin;    //analog sensor pin
    int samples;
    
    float avg;  //average of readings
    int* value;//store readings

    float currentDraw;  //current being drawn
    
    float UpdateAverage(float input);//add reading, update average
    
    uint64_t lastRead;    //last time sensor was read
    bool Calibrated;        //is sensor calibrated

public:
    short calibrating;  //is sensor calibrating, also serves as countdown
    bool PRINT;         //print sensor data on/off
    
    void Calibrate();   //begin calibration sequence

    float Monitor();    //monitor sensor
    
    float GetCurrentDraw(){return currentDraw;};//get current being drawn

    void Init(int _pin);//set analog reading pin
    
    //constructor
    CSensor();
    CSensor(int nSamples);
    
};

#endif
