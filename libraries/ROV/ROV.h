#ifndef ROV_H
#define ROV_H

#include <Arduino.h>

#include "PID.h"
#include "TSensor.h"
#include "CSensor.h"
#include "Thruster.h"
#include "Adafruit_PWMServoDriver.h"

class ROVClass
{
	float roll;   //roll angle measured from flat; left roll from pilots perspective is positive
    float rollOffset;
    float pitch;  //pitch angle measured from flat; nose up is positive
    float pitchOffset;
    float yaw;    //yaw angle measured +CCC / -CW from initial calibration
    float yawOffset;
    float depth;  //depth measurement
    int rollspeed;  //speed to roll
    int pitchspeed; //speed to pitch
    int yawspeed;   //speed to yaw
    int depthspeed; //speed to change depth
    byte surfaceStat;
    float currentDraw;
    
	unsigned long last_command_time;
	char buff[];
	
    bool pumpStat;  //is pump on(1) or off(0)
    //note: pump is on pin 15
    
    //Create Thrusters, define their pins
    //signal pin, direction pin, current sensor pin
    Thruster TopLeftFront=Thruster(11,23,A9);
    Thruster TopRightFront=Thruster(8,24,A3);
    Thruster TopLeftRear=Thruster(12,22,A8);
    Thruster TopRightRear=Thruster(7,29,A2);
    Thruster BottomLeftFront=Thruster(5,28,A6);
    Thruster BottomRightFront=Thruster(6,25,A5);
    Thruster BottomLeftRear=Thruster(2,27,A7);
    Thruster BottomRightRear=Thruster(3,26,A4);
    
    //Create Regulator Temperature Sensors (analog read pin)
    TSensor Vicor1=TSensor(A11);
    TSensor Vicor2=TSensor(A12);
    
    //Create Current Sensors (analog read pin)
    CSensor PnCSens;
    CSensor MainCSens;
    
    void SetPitch(float angle);   //update pitch angle
    void SetRoll(float angle);    //update roll angle
    void SetYaw(float angle);     //update yaw angle
    void SetDepth(float depth);   //update depth measurement
    
    Adafruit_PWMServoDriver coolingPump = Adafruit_PWMServoDriver();    //lets us use pwm pins from expander (pump)
    
    void StopAll();    //Stop all motors
    void StopVerticals();//Stop vertical thrusters
    
public:
	//constructor
	ROVClass();
    
    void PumpOff(){coolingPump.setPWM(0,0,0);pumpStat=0;}; //turn cooling pump off
    void PumpOn(){coolingPump.setPWM(0,0,4095);pumpStat=1;};//turn cooling pump on
    bool GetPump(){return pumpStat;};   //get status of the pump
    
    //Command ROV from one function
    void Move(int driveAngle, int driveSpeed, int yawSpeed, int rollSpeed, int pitchSpeed, int depthSpeed);
    //Drive/mecanum/steer ROV
    void Drive(int angle, int speed, int yaw);
    //Orientation control of ROV
    void Tilt(int roll, int pitch, int depth);
    
    //PIDs for stabilization, etc.
    CalcPid rollPID, pitchPID, yawPID, depthPID;
    
    void CalibrateOrientation();
    
    //Update Orientation Angles TODO:call and input IMU data
    void SetOrientation(float roll, float pitch, float yaw, float depth);
    
    void EnableOrientation(){
        rollPID.Enable();rollPID.SetSp(0);
        pitchPID.Enable();pitchPID.SetSp(0);
        };
    void DisableOrientation(){  rollPID.Disable();  pitchPID.Disable();   };
    void EnableDepth(){
        depthPID.Enable();depthPID.SetSp(depthPID.GetDisp());
        };
    void DisableDepth(){    depthPID.Disable(); };
    void EnableYaw(){
        yawPID.Enable();yawPID.SetSp(yawPID.GetDisp());
        };
    void DisableYaw(){  yawPID.Disable();   };
    
	void SendSettings();
	
	void Init();    //Initialize (creates motors, sensors, sets PID values, enables pump/pwm control)
    
    void IsAtSurface(){surfaceStat=1;};
    void IsAboveSurface(){surfaceStat=2;};
    void CanDive(){surfaceStat=0;};
    
    void ResetAlarms();
    
    const char* Monitor(); //Monitor sensors, etc.
    const char* getState(); //returns the string containing ROV state data to send to the pi
	unsigned long GetLastCmdT(){return last_command_time;};
	void SetLastCmdT(unsigned long in){last_command_time=in;};
    void Save();    //save PID and calibrations
};

extern ROVClass ROV;

#endif
