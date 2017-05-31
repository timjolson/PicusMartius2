#ifndef IMU_H
#define IMU_H
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"

#include <Arduino.h>

class IMUClass {
    
    Adafruit_BNO055 bno;    //create library object
    
    bool Calibrated;        //is IMU calibrated
    
    void Read();            //read IMU data
    void _calibrate();      //run calibration
	
    float raw[3];           //raw axes data
    
    short refresh;          //refresh rate
    short refresh_shift;    //offset to not bog system
	
	uint32_t LastTime;      //last time updated
	char Name[10];          //name for printing
	
public:
	bool PRINT;         //turn on/off printing of data
	
    void Calibrate(){Calibrated=0;};    //begin calibration sequence
    bool IsCalibrated(){return Calibrated;};//return cal status

	void Operate();     //get data, check calibration, etc.
	
	float GetAxis(int axis);//return angle of axis
    
    void displaySensorDetails();
    void displaySensorStatus();
    void displayCalStatus();
    
	void Init(const char* Name);
    
	//constructor
    IMUClass(){ bno = Adafruit_BNO055(55); };
	
};//end IMUClass Class

extern IMUClass IMU;

#endif