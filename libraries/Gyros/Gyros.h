#ifndef GYROS_H
#define GYROS_H

#include <Arduino.h>

class GyroClass {
   
    bool Calibrated;
    int cycleAvg[1][101];
    int cycleTavg[1][101];
    byte _cyclenum[1];
    
    short offset[1];
    void _calibrate(unsigned int cycleT);
    void Read();
    
    short raw[1];
    int gyro[1];
	
	short max_rate;
    short min_rate;
    
    short refresh;
    short refresh_shift;
	
	uint64_t LastTime;
	char Name[10];
	
	char addr;
    
    void itgWrite(char registerAddress, char data);
    unsigned char itgRead(char registerAddress);
	
public:
	bool PRINT;
	
    void Calibrate(){Calibrated=0;};
    bool IsCalibrated(){return Calibrated;};

	int* Operate();
	
	short GetRawInput(int axis);
    
	void Init(const char* Name, int max_rate, int min_rate);
	void SetAddr(char address);
    
	//constructor
	GyroClass(char address){ addr = address;};
    GyroClass(){ addr = 0x69; };
	
};//end GyroClass Class


#endif