#include "ArmClass.h"

enum ArmErrFlags{
	axisOutOfBounds=1,
	calInProgress=2,
	calAborted=4,
	notCalibrated=8,
	invalidGoal=16,
	invalidPos=32,
	invalidRaw=64,
	noCommand=128,
	disabled=256,
	partiallyDisabled=512,
	notInit=0x8000,
}

ArmClass::ArmClass()
{
	error = notInit;	//set not-Init flag, clear all others
	error |= notCalibrated;	//set not-calibrated flag
}

void ArmClass::Disable()
{
	//For each PID, disable it
	for (byte i = 0;i<sizeof(PID)/sizeof(PIDClass);i++)
	{
		PID[i].Disable();
	}
	error |= disabled;	//error, Arm is disabled
    status.enabled = false; //status not enabled
    
    error ^= partiallyDisabled;  //clear partiallyDisabled flag
}

uint16_t ArmClass::Disable(uint8_t axis)
{
	//For selected PID, disable it
	if (0 < axis <= sizeof(PID)/sizeof(PIDClass)){
		PID[axis].Disable();
		error |= partiallyDisabled;	//partially disabled flag
	}
	else
		error |= axisOutOfBounds;	//error, tried to disable axis that does not exist
    
    //check to see if all axes are disabled, set Disabled flag, clear partiallyDisabled flag
    bool allDisabled = 1;
    for (byte axis = 0; axis<sizeof(PID)/sizeof(PIDClass);axis++)
    {
        if PID[axis].Enabled()
        {
            allDisabled = 0;
            break;
        }
    }
    if allDisabled
    {
        error ^= partiallyDisabled;
        error |= disabled;
    }
}

void ArmClass::Enable()
{
	//For each PID, enable it
	for (byte i = 0;i<sizeof(PID)/sizeof(PIDClass);i++)
	{
		PID[i].Enable();
	}
	error ^= partiallyDisabled;
    status.enabled = true;
}

uint16_t ArmClass::Enable(uint8_t axis)
{
	//For selected PID, enable it
	if (0 < axis <= sizeof(PID)/sizeof(PIDClass)){
		PID[axis].Enable();
	}
	else
		error |= axisOutOfBounds;	//error, tried to disable axis that does not exist
		
	//check to see if all axes are enabled, clear partiallyDisabled flag
    bool allEnabled = 1;
    for (byte axis = 0; axis<sizeof(PID)/sizeof(PIDClass);axis++)
    {
        if !PID[axis].Enabled()
        {
            allEnabled = 0;
            break;
        }
    }
    if allEnabled
    {
        error ^= partiallyDisabled;
        error ^= disabled;
        status.enabled = true;
    }
}

uint32_t ArmClass::Calibrate()
{
	if (!calibrating){
		calibrating = 1;
		error |= notCalibrated;	//set not-calibrated flag
	}
	else{
		error |= calInProgress;	//error, calibration already in progress
		return error;
	}
    
    error ^= calAborted;
}

uint32_t ArmClass::AbortCalibrate()
{
	if (calibrating>0){
		calibrating=0;
		error ^= calInProgress;	//clear cal in progress flag
	}
	else
		//TODO: do other things because this is bad
		error |= calAborted;	//error, calibration aborted
		return error;
}

bool ArmClass::SetPID(ArmPIDVals rx)
{
	PIDvals = rx;
	//For each PID, set it's gains
	for (byte i =0; i<sizeof(PID)/sizeof(PIDClass); i++)
	{
		PID[i].SetVals({PIDvals.kP[i],PIDvals.kI[i],PIDvals.kD[i]});
	}
	
	//TODO: some kind of error checking or something to return?
	return error;
}

ArmPositionStruct ArmClass::GetGoal()
{
	//do updating and compiling of packet
	return goal;
}

ArmPositionStruct ArmClass::GetPosition()
{
	//do updating and compiling of packet
	return pos;
}

ArmRawStruct ArmClass::GetRaw()
{
	//do updating and compiling of packet
	return raw;
}

FromArmPacket ArmClass::GetStatus()
{
	//do updating and compiling of packet
	return status;
}

void ArmClass::Hold()
{
	
}

uint32_t ArmClass::Move()
{
	return error;
}

uint32_t ArmClass::Operate()
{
	//check status
		
	//check if calibrated
	if (!IsCalibrated()){
		Calibrate();	//run calibration sequence
		error |= notCalibrated;	//error, not calibrated
	}
	else{
		//do moving and such
	}
	
	return error;
}

uint32_t ArmClass::SetGoal(ArmPositionStruct rx)
{
	//check if goal is valid \ do math and stuff
	if (goal is valid)
		goal = rx;
	else
		error |= invalidGoal;	//error, goal not valid
	return error;
}

uint32_t ArmClass::SetPosition(ArmPositionStruct rx)
{
	//check if position is valid \ do math and stuff
	if (position is valid)
		pos = rx;
	else
		error |= invalidPos;
	return error;
}

uint32_t ArmClass::SetRaw(ArmRawStruct rx)
{
	//check if raw position is valid \ do math and stuff
	if (raw is valid)
		raw = rx;
	else
		error |= invalidRaw;
	return error;
}

bool ArmClass::SetStatus(ToArmPacket rx)
{
	if (status != rx)
	{
		status = rx;
		return true;
	}
	else
	{
		return false;
	}
}

uint32_t ArmClass::Do(CommandStruct rx)
{
	switch(rx.cmdNum){
		case CommandType::ArmSoftStop:
			Disable(0);
			Disable(1);
			Disable(2);
			Disable(3);
			Disable(4);
			Disable(5);
		case CommandType::ArmHardStop:
			Disable();
			break;
		case CommandType::ArmStow:
			
			break;
		case CommandType::ArmUnstow:
			
			break;
		default:
			error |= noCommand;
	return error;
}

uint32_t ArmClass::_Calibrate()
{
	//set calibration step to 1 if calibration is not in progress
	calibrating = (calibrating!=0)?calibrating:1;
	if (calibrating) error |= calInProgress;
	return calibrating;
}

uint32_t ResetError()
{
	//reset error
	error = 0;
	//check for persistent errors
	return error;
}

uint32_t Init()
{
	error ^= notInit;	//clear not-init flag
	return error;
}

bool IsCalibrated()
{
	return !(error&notCalibrated);
}