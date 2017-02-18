#ifndef ArmClass_H
#define ArmClass_H

#include <vector>

class ArmClass {
	public:
		
		ArmClass();//Constructor
		uint32_t Init();//Initializer
		
		bool SetStatus(ToArmPacket);
		FromArmPacket GetStatus();
		
		uint32_t SetGoal(ArmPositionStruct);
		ArmPositionStruct GetGoal();
		uint32_t SetRaw(ArmRawStruct);
		ArmRawStruct GetRaw();
		uint32_t SetPosition(ArmPositionStruct);
		ArmPositionStruct GetPosition();
		
		void Enable();
		void Disable();
		void Disable(uint8_t axis);
		
		uint32_t Operate();
		
		bool SetPID(ArmPIDVals);
		
		uint32_t Calibrate();
		uint32_t AbortCalibrate();
		bool IsCalibrated();
		
		uint32_t Do(CommandStruct);
		
	private:
		
		_Calibrate();
		uint32_t Move();
		void Hold();
		
		ArmPIDVals PIDvals;
		PIDClass[7] PID;
		AnalogEncoderClass[6] encoder;
		
		ArmPositionStruct pos;
		ArmRawStruct raw;
		
		ArmRawStruct goal;
		ArmRawStruct position;
		
		uint32_t calibrating;
		StatusStruct status;
		
		uint32_t error;
		uint32_t ResetError();
		
		vector<CommandStruct> commandlist;
		
};//end ArmClass

ArmClass Arm();

#endif