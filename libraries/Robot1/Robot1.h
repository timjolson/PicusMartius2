#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "TeensyRCData.h"
#include "Motor1.h"
#include "PID.h"

class RobotClass
{
	uint16_t r_mode;	//current robot mode settings
	Stream* stream;		//where to print debugging data
    
    int heading;
    
	short jdir;
    
	int servoNum = 0;
    
	int Read_Serial();
	void Calibrate1();
	int maximum(int);
	int maximum2(int);
	
	void Tankdrive_v1();
    
    // pin, min, max
    Motor Front_Left = Motor(23, 1000, 2000);
    Motor Front_Right = Motor(22, 1000, 2000);
    Motor Rear_Left = Motor(21, 1000, 2000);
    Motor Rear_Right = Motor(20, 1000, 2000);
    
    void driveRCmecanum();
    
public:
	//constructor
	RobotClass();
    CalcPd PDg;
    void driveRCmecanum(int);
    void SetHeading(int);
    void UpdateHeading(int _h){heading = _h;};
    void turnRCmecanum(int);
    bool CheckTarget();

	//mode setting
	uint16_t M_GetMode();			//get current mode settings
	uint16_t M_SetMode(byte _rxmode);//set current mode
	bool M_GetEnabled();			//get enable/disable status
	void M_Enable();				//set mode to enabled
	void M_Disable();				//set mode to disabled
	void M_DriveOn(byte _drivemode);//enable and set drive mode
	void M_DriveOff();				//disable driving mode
	void M_ArmOn();					//enable arm control mode
	void M_ArmOff();				//disable arm control mode
	void M_SetOpsMode(byte _mode);	//set operating mode /arm vs drive vs whatever else
	void M_GyroOn();				//enable gyro rotational correction
	void M_GyroOff();				//disable gyro rotational correction
	void M_Auto();					//set autonomous mode
	void M_Manual();				//set manual control mode
	bool M_GetDriveMode();			//get drive mode (mecanum or tank)
	//r_mode = (Enable)(Drive)(Control Type)(Drive Mode)(Arm)(Gyro)(Auto)(empty)
	
	//general controller section
	void Init();						//create and set buttons and things on startup
	
	bool* GetTXdata();
	byte GetTXsize();
	
	void SetControlData(int* _data);

	// call in .ino setup routine
	void setupRC(int[]);

	void Drive();
    void Drive(bool tank);
    void Stop();

	void Move(int speed);

	void CalibrateRC();
	
	short cosDir(short, short, short, byte);
	short sinDir(short, short, short, byte);
};

#endif
