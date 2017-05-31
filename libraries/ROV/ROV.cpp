#include <Arduino.h>

#include "PID.h"
#include "TSensor.h"
#include "CSensor.h"
#include "Thruster.h"
#include "Adafruit_PWMServoDriver.h"
#include "ROV.h"
#include "ROVEthernet.h"

ROVClass ROV;

// constructor
ROVClass::ROVClass()
{
    
}

//create and set buttons and things on startup
void ROVClass::Init()
{
    TopLeftFront.Init("TLF",1);
    TopRightFront.Init("TRF",1);
    TopLeftRear.Init("TLR",0);
    TopRightRear.Init("TRR",1);
    BottomLeftFront.Init("BLF",1);
    BottomRightFront.Init("BRF",1);
    BottomLeftRear.Init("BLR",0);
    BottomRightRear.Init("BRR",0);
    
    /*Temperature Sensor*/
    Vicor1.Init("Vicor1");
    Vicor2.Init("Vicor2");
    
    /*Current Sensor*/
    MainCSens.Init(A10);
    PnCSens.Init(A1);
    
    coolingPump.begin();        //start pwm pin control
    coolingPump.setPWMFreq(1000);//set frequency
    PumpOff();                  //initialize pump off
}

void ROVClass::Move(int _drA, int _drS, int _yS, int _rS, int _pS, int _dS)
{
    //Command ROV from single function
    
    Drive(_drA, _drS, _yS);
    Tilt(_rS, _pS, _dS);

}

void ROVClass::Drive(int _angle, int _speed, int _yawspeed)
{
    //Driving Control
    if (surfaceStat<2){
        //put angle in 0->360 degrees TODO:remove unneccessary change in python
        //if (_angle<0) _angle+=360;
        float c = cos(_angle*3.14159/180);  //get cosine of direction
        float s = sin(_angle*3.14159/180);  //get sine of direction

        float amp = abs(_speed);            //get amplitude/magnitude
        yawspeed = _yawspeed;
        float rot = _yawspeed*(255/50);     //get yaw speed, re-map from +/-50% to 0->255
        //TODO:check mapping to get max yaw
        
        float speeds[4]={0};

        //translation terms
        speeds[0] -= c*amp;
        speeds[1] += c*amp;
        speeds[2] += c*amp;
        speeds[3] += c*amp;

        //forward/back terms
        speeds[0] -= s*amp;
        speeds[1] -= s*amp;
        speeds[2] -= s*amp;
        speeds[3] += s*amp;

        //rotation terms
        speeds[0] -= rot;
        speeds[1] += rot;
        speeds[2] -= rot;
        speeds[3] -= rot;
        
        //yaw PID terms
        float yP = yawPID.GetPid()*255;
        speeds[0] -= yP;
        speeds[1] += yP;
        speeds[2] -= yP;
        speeds[3] -= yP;
        
        /* KEEP VECTOR RATIOS */
        float biggest = 255;
        for (int i=0;i<4;i++)
            biggest = max(biggest,fabs(speeds[i]));
        for (int i=0;i<4;i++)
            speeds[i] /= biggest;
        
        //Translation Motors
        BottomLeftFront.Move(speeds[0]*255);   //Set motor speeds, scaled to 0->255
        BottomRightFront.Move(speeds[1]*255);
        //BottomLeftRear.Move(speeds[2]*255);
        //BottomRightRear.Move(speeds[3]*255);
    }
    else StopAll();

}

void ROVClass::Tilt(int _rollspeed, int _pitchspeed, int _depth)
{
    //Orientation and depth control
    if (surfaceStat==0){
        rollspeed = _rollspeed;
        pitchspeed = _pitchspeed;
        depthspeed = -_depth*(255/100); //re-map from % to 0->255
        float speeds[4]={0};    //motor speed storage
        
        //roll terms
        speeds[0] += rollspeed;
        speeds[1] += rollspeed;
        speeds[2] += rollspeed;
        speeds[3] += rollspeed;

        //pitch terms
        speeds[0] -= pitchspeed;
        speeds[1] += pitchspeed;
        speeds[2] += pitchspeed;
        speeds[3] -= pitchspeed;

        //depth terms
        speeds[0] -= depthspeed;
        speeds[1] += depthspeed;
        speeds[2] -= depthspeed;
        speeds[3] += depthspeed;

        //TODO: add PID control terms
        //roll terms
        float rP = rollPID.GetPid()*255;
        speeds[0] += rP;
        speeds[1] += rP;
        speeds[2] += rP;
        speeds[3] += rP;
        //pitch terms
        float pP = pitchPID.GetPid()*255;
        speeds[0] -= pP;
        speeds[1] += pP;
        speeds[2] += pP;
        speeds[3] -= pP;
        //depth terms
        float dP = depthPID.GetPid()*255;
        speeds[0] -= dP;
        speeds[1] += dP;
        speeds[2] -= dP;
        speeds[3] += dP;
        
        /* KEEP VECTOR RATIOS */
        float biggest = 127;
        for (int i=0;i<4;i++)
            biggest = max(biggest,abs(speeds[i]));
        for (int i=0;i<4;i++)
            speeds[i] /= biggest;

        //Orientation Motors
        TopLeftFront.Move(speeds[0]*255);   //Set motor speeds, scaled to 0->255
        TopRightFront.Move(speeds[1]*255);
        TopLeftRear.Move(speeds[2]*255);
        TopRightRear.Move(speeds[3]*255);
    }
    else StopVerticals();
}

void ROVClass::StopAll()
{
    //write all motors stopped
    TopLeftFront.Move(0);
    TopRightFront.Move(0);
    TopLeftRear.Move(0);
    TopRightRear.Move(0);
    BottomLeftFront.Move(0);
    BottomRightFront.Move(0);
    BottomLeftRear.Move(0);
    BottomRightRear.Move(0);
}

void ROVClass::StopVerticals()
{
    //write all vertical motors stopped
    TopLeftFront.Move(0);
    TopRightFront.Move(0);
    TopLeftRear.Move(0);
    TopRightRear.Move(0);
}

void ROVClass::CalibrateOrientation(){
    //TODO: add IMU Cal Sequence
    rollOffset = roll;
    pitchOffset = pitch;
    yawOffset = yaw;
}

void ROVClass::SetOrientation(float _roll, float _pitch, float _yaw, float _depth)
{
    SetRoll(_roll - rollOffset);
    SetPitch(_pitch - pitchOffset);
    SetYaw(_yaw - yawOffset);
    SetDepth(_depth);
}

void ROVClass::SetPitch(float _angle)
{
    pitch = _angle;
    pitchPID.SetDisp(_angle);
}

void ROVClass::SetRoll(float _angle)
{
    roll = _angle;
    rollPID.SetDisp(_angle);
}

void ROVClass::SetYaw(float _angle)
{
    yaw = _angle;
    yawPID.SetDisp(_angle);
}

void ROVClass::SetDepth(float _depth)
{
    depth = _depth;
    depthPID.SetDisp(_depth);
}

const char* ROVClass::Monitor()
{
    //monitor regulator temperatures
    Vicor1.Monitor();
    Vicor2.Monitor();
    
    currentDraw = 0;
    currentDraw += 
        TopLeftFront.Monitor() +
        TopRightFront.Monitor() +
        TopLeftRear.Monitor() +
        TopRightRear.Monitor() +
        BottomLeftFront.Monitor() +
        BottomRightFront.Monitor() +
        BottomLeftRear.Monitor() +
        BottomRightRear.Monitor() +
        PnCSens.Monitor() +
        MainCSens.Monitor()
    ;
    
    sprintf(buff, "T%d,%dc%d", (int)(Vicor1.GetTemperature()*1000), (int)(Vicor2.GetTemperature()*1000),
        (int)(currentDraw*10));
    
    return buff;
}

const char* ROVClass::getState()
{
    sprintf(buff, "X%dY%dZ%d", (int)roll, (int)pitch, (int)yaw);
    
    return buff;
}

void ROVClass::SendSettings()
{
    sprintf(buff, "Dp%dDi%dDd%d", (int)depthPID.GetKp()*1000.0, (int)depthPID.GetKi()*1000.0, (int)depthPID.GetKd()*1000.0);
	SendEthernet(buff);
	sprintf(buff, "Yp%dYi%dYd%d", (int)yawPID.GetKp()*1000.0, (int)yawPID.GetKi()*1000.0, (int)yawPID.GetKd()*1000.0);
	SendEthernet(buff);
	sprintf(buff, "Tp%dTi%dTd%d", (int)rollPID.GetKp()*1000.0, (int)rollPID.GetKi()*1000.0, (int)rollPID.GetKd()*1000.0);
	SendEthernet(buff);
}

void ROVClass::ResetAlarms()
{
    TopLeftFront.Enable();
    TopRightFront.Enable();
    TopLeftRear.Enable();
    TopRightRear.Enable();
    BottomLeftFront.Enable();
    BottomRightFront.Enable();
    BottomLeftRear.Enable();
    BottomRightRear.Enable();
}

void ROVClass::Save()
{
    rollPID.Save();
    pitchPID.Save();
    yawPID.Save();
    depthPID.Save();
}