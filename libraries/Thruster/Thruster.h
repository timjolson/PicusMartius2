#ifndef Thruster_h
#define Thruster_h
#include <Arduino.h>
#include "PID.h"
#include "CSensor.h"

class Thruster {
    short speed;        //speed actually being written to H-bridge
    short deadband;     //deadband to not tell motor to drive
    bool reverse;       //whether the motor is reversed or not (left vs right handed prop)
    
    int sigPin;         //pin to write speed to ( 255 max , min is decided by circuit and frequency ) TODO:calculate
    int dirPin;         //pin to change H-bridge direction
    int currPin;        //analog pin to read current sensor
    
    char Name[10];      //name for printing

    bool overCurrent;   //if the motor overcurrents
    bool underCurrent;  //if the motor undercurrents
    
    float currentDraw;  //current being drawn (averaged/smoothed)
    
    uint64_t lastRead;  //last time sensor was read
    bool Calibrated;    //is sensor calibrated

    CSensor cSens;//current sensor object
    CalcPid PID;        //PID to control ramping
    
    void Write(short val);//write speed to motor
    
    bool enabled;       //whether the thruster is allowed to move or not

public:
    void Enable(){enabled = 1; overCurrent = underCurrent = 0;PID.Enable();PID.SetDisp(0);PID.SetSp(0);};
    void Disable(){enabled = 1; PID.Disable();PID.SetDisp(0);PID.SetSp(0);};

    short calibrating;  //is sensor calibrating, also serves as countdown
    bool PRINT;         //print sensor's data on/off
    short printTimer;   //millis() between prints
    
    void Calibrate();   //begin calibration sequence TODO:active calibration for max speed
    void Step(byte startstep);  //go to next step in calibration

    void Move(short speed);//tell motor a speed to drive (will ramp up and down)
    float Monitor();     //monitor current sensor
    
    short GetSpeed(){return speed;};    //get speed currently being written to H-bridge

    // set the name and direction
    void Init(const char* Name, bool _reverse);
    
    //constructor
    Thruster(int signalPin,int dirPin, int currPin);
    
};

#endif
