#ifndef TSensor_h
#define TSensor_h
#include <Arduino.h>

class TSensor {
    int pin;    //analog pin to read sensor
    char Name[10];  //name for printing
    
    int samples;    //number of samples for averaging
    float avg;      //average of readings
    int* value;     //stored samples
    float UpdateAverage(float input);//add "input" reading to the average
    
    float temperature;  //calculated temperature
    
    uint64_t lastRead;  //last time sensor was read
    uint64_t lastPrint; //last time data was printed
    
    bool Calibrated;    //is the sensor calibrated

public:
    short calibrating;  // !0 => sensor is calibrating - also serves as countdown
    bool PRINT;         //whether or not to print the sensor's data
    short printTimer;   //millis() between data prints
    
    void Calibrate();   //call once to begin calibration sequence

    float Monitor();    //monitor the sensor, update average, etc.
    
    float GetTemperature(){return temperature;};//get the averaged temperature
    
    void Init(const char* name);//Name the sensor for clarified feedback
    
    //constructor
    TSensor(int _pin, int n);//analog pin for the sensor, "n" samples to use for averaging
    TSensor(int _pin);      //default sample size = 10
};

#endif
