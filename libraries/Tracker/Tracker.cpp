#include <Arduino.h>
#include "Variables.h"
#include "Tracker.h"

extern uint32_t CurrentTime;

void TrackerClass::Operate()
{
    unsigned int cycleT = CurrentTime - LastTime;
    
    if (0){
        Serial.print("g1:");Serial.print(g1.IsCalibrated());
        Serial.print("\tg2:");Serial.println(g2.IsCalibrated());
    }
    
    /*
    if (!g1.IsCalibrated() || !g2.IsCalibrated())
    {
        Calibrated=0;
    }
    else
        if (Calibrated==0){
            Serial.print("Tracker ");
            Serial.print(Name);
            Serial.print(" Calibrated\n");
            Calibrated=1;
        }
    */
    GyroAdd(cycleT);
    
    LastTime = CurrentTime;
}

void TrackerClass::Reset()
{
    for (int i=0;i<3;i++)
        pos[i]=0;
    Calibrate();
};

int TrackerClass::GetPos(int axis)
{
    return pos[axis];
}

void TrackerClass::SetPos(int _pos)
{
    integralG[0] = _pos * 10;
};

void TrackerClass::GyroAdd(int cycleT)
{
    
    if (0) Serial.print("add\t");
    int avg=0; int count =0;
    /*
    rate[0] = g1.Operate();
    if (reverse[0]) rate[0][0] *= -1;   //update for 3 axes
    if (rate[0][0]){
        if (rate[0][0] > 0)
            avg += (900/490.0)*rate[0][0];
        else
            avg += (900/490.0)*rate[0][0];
        count ++;
    }
    
    
    rate[1] = g2.Operate();
    if (reverse[1]) rate[1][0] *= -1;      //update for 3 axes
    if (rate[1][0]){
        if (rate[1][0] > 0)
            avg += (900/382.0)*rate[1][0];
        else
            avg += (900/438.0)*rate[1][0];
        count++;
    }
    
    
    if (0){ 
        Serial.print("\trates:");
        Serial.print(rate[0][0]);
        Serial.print("\t");
        Serial.print(rate[1][0]);
    }
    
    if (count){
        if (avg > 0)
            avg *= 37.36;
        else if (avg < 0)
            avg *= 39.71;

        avg = avg/count;
        //integrate gyros
        integralG[0] += float(((int)cycleT * avg)/10000); //add WRT time
    }
    if (0){
        Serial.print("\tavg:");
        Serial.print(avg);
    }
    */
    if (integralG[0]>(18000)) integralG[0] -= 18000;  // +/- 360*
    else if (integralG[0]<(-18000)) integralG[0] += 18000;  // +/- 360*
    
    pos[0] = integralG[0]/10;
    if (0){
        Serial.print("\tpos:");
        Serial.println(pos[0]);
    }
    rate[0]=0;
}

void TrackerClass::Calibrate()
{
    g1.Calibrate();
    g2.Calibrate();
    
    Serial.print(Name);Serial.println(" Calibrating...");
}

void TrackerClass::Init(const char* _name)
{
    strcpy(Name,_name);
    Calibrated=0;
    
    g1.SetAddr(0x69);
    g2.SetAddr(0x68);
    g1.Init("g1",2000,4);
    g2.Init("g2",2000,5);
    delay(50);
    
    Serial.print(Name);Serial.println(" Initialized...");
}