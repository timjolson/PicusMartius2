#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "Variables.h"

//#include "Motor.h"
//#include "Arm.h"
//#include "Tracking.h"
//#include "AI.h"

class RobotClass{
	uint16_t r_mode;	//current robot mode settings
	Stream* stream;		//where to print debugging data
	
	bool* TXdata[DATASIZEBOOL];
	bool haveTXdata;
	
	bool switches[NUMSWITCHES-1];	//dummy switch
	bool buttons[NUMBUTTONS-1];	//dummy button
	byte knobs[NUMKNOBS];
	uint16_t sticks[NUMSTICKS];
	
public:
	//Drive motors
	//	Calibrate motors
	//	Config motors
	//Operate arm
	//	Calibrate arm
	//	Config arm
	//Run tracking
	//	Calibrate sensors
	//	Config sensors
	//Run AI
	//	Set AI options
	
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
	
	void SetControlData(byte* _data);
	
	//constructor
	RobotClass(){};
};

#endif