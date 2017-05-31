#include "Robot.h"

RobotClass Robot;

void RobotClass::SetControlData(byte* _data)
{
	//remove byte rounding bits from _data
	//byte _data[sizeof(_data1)];
	//memcpy(_data,&_data1,sizeof(_data1));
// 	for (int i =0; i < sizeof(_data1); i++)
// 	{
// 		_data[i] = _data1[i];
// 	}
	
	_data[0] = _data[0] >> (DATASIZEBYTE*8 - DATASIZEBOOL);
	
	//switches
	_data[0] = _data[0] >> 1;	//junk dummy switch
	for (int i = NUMSWITCHES-1; i >=0; i--)
	{
		switches[i] = _data[0] & B00000001;	//get bit
		_data[0] = _data[0] >> 1;						//shift
	}
	
	//buttons
	_data[0] = _data[0] >> 1; //junk dummy button
	for (int i = NUMBUTTONS-1; i >= 0; i--)
	{
		buttons[i] = _data[0] & B00000001;	//get bit
		_data[0] = _data[0] >> 1;						//shift
	}
	
	//knobs
	for (int i = NUMKNOBS; i>=0; i--)
	{
		knobs[i] = _data[0] &B11111111;	//get byte
		_data[0] = _data[0] >> 8;					//shift
	}
	
	//sticks
	for (int i = NUMSTICKS; i>=0; i--)
	{
		sticks[i] = _data[0] & B11111111;	//get byte
		_data[0] = _data[0] >> 8;	//shift for last 2 bits
		sticks[i] |= (_data[0] & B00000011) << 8;	//add last 2 bits
		_data[0] = _data[0] >> 2;	//shift away last 2 bits
	}
}

//mode setting
uint16_t RobotClass::M_GetMode(){return 0;};			//get current mode settings
uint16_t RobotClass::M_SetMode(byte _rxmode){return 0;};//set current mode
bool RobotClass::M_GetEnabled(){return 0;};			//get enable/disable status
void RobotClass::M_Enable(){};				//set mode to enabled
void RobotClass::M_Disable(){};				//set mode to disabled
void RobotClass::M_DriveOn(byte _drivemode){};//enable and set drive mode
void RobotClass::M_DriveOff(){};				//disable driving mode
void RobotClass::M_ArmOn(){};					//enable arm control mode
void RobotClass::M_ArmOff(){};				//disable arm control mode
void RobotClass::M_SetOpsMode(byte _mode){};	//set operating mode /arm vs drive vs whatever else
void RobotClass::M_GyroOn(){};				//enable gyro rotational correction
void RobotClass::M_GyroOff(){};				//disable gyro rotational correction
void RobotClass::M_Auto(){};					//set autonomous mode
void RobotClass::M_Manual(){};				//set manual control mode
bool RobotClass::M_GetDriveMode(){return 0;};			//get drive mode (mecanum or tank)
//r_mode = (Enable)(Drive)(Control Type)(Drive Mode)(Arm)(Gyro)(Auto)(empty)

//general controller section
void RobotClass::Init(){};						//create and set buttons and things on startup

bool* RobotClass::GetTXdata(){return *TXdata;};
byte RobotClass::GetTXsize(){return DATASIZEBYTE;};