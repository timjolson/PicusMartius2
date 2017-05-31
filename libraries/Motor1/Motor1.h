#ifndef Motor1_h
#define Motor1_h
#include <Arduino.h>
#include <Servo.h>

class Motor {
    short velocity, acceleration, speed, deadband;
    int sigPin, minPin, maxPin;
    bool reverse, direction;
    short servoNum;
    char Name[10];

    //ButtonClass* maxLim;
    //ButtonClass* minLim;

    uint64_t lastChange;
    bool Calibrated;

    Servo TestServo;



public:
    short calibrating;
    bool PRINT;
    void Calibrate();
    void CalibrateMove();
    void Step(byte startstep);
    void Move(int speed);
    void Operate();
    void Write(int val);
    short GetSpeed(){return speed;};

    // set the name and direction
    void Init(const char* Name, bool _reverse);

    //constructor
    Motor(int signalPin,int minPin,int maxPin);

    //Servo Servo1;

};

#endif
