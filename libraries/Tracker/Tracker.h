#ifndef TRACKER_H
#define TRACKER_H

#include <Arduino.h>
#include "Gyros.h"

class TrackerClass
{
    GyroClass g1;
    GyroClass g2;
    
    bool Calibrated;
    
    void GyroAdd(int cycleT);
    void _calibrate(int);
    
    short pos[3];
    int* rate[3];
    bool reverse[3];
    int integralG[3];
    
    char Name[10];
    
    uint32_t LastTime;
    
public:
	bool PRINT;
	void Read();

    void Calibrate();

	void Operate();
    void Reset();
	
	int GetPos(int axis);
    void SetPos(int _pos);
    void SetCal(bool _cal){Calibrated=_cal;};
    
	void Init(const char* Name);
	
	//constructor
	TrackerClass(){};
    
};

#endif