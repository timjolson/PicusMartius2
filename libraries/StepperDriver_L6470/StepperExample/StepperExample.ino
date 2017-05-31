#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include "EEPROMCustom.h"
#include "cppLib.h"
#include "SmartLynx.h"
#include "Parsing.h"

bool dir = 0;

// cs, step, stby, flag, busy, logicV
SmartLynx Drawer = SmartLynx(53,48,49,47,46,45);
//SmartLynx Drawer2 = SmartLynx(22,23,24,25,26,27);

void setup() {
    Serial.begin(115200);  //communication to monitor
    
    // Name, pitch diameter(mm), reverse?
    Drawer.Init("Drawer1", 38.2, 1);
    //Drawer2.Init("Drawer2", 19.1, 1);
    
    Drawer.SetAcceleration(400);
    Drawer.SetDeceleration(1000);
    Drawer.SetMaxSpeed(14);
    
    //Drawer2.SetMaxSpeed(8);
    
    Serial.println("ready");
}

void loop(){
    getSerial(&Serial);
    getSerial("o");
    delay(10000);
    getSerial("x");
    delay(10000);
  
}
/*
void sweeploop(){
    uint64_t start = millis();
    while(millis()-start < 12000){
       Serial.print("pos\t");Serial.println(Drawer2.GetPosInches());
       Drawer2.GoToInches(15.0);
       delay(3000);
       
       Serial.print("pos\t");Serial.println(Drawer2.GetPosInches());
       Drawer2.GoToInches(0.0);
       delay(3000);
    }
}

void speedloop(){
    uint64_t start = millis();
    while(millis()-start < 7500){
        Serial.print("pos\t");Serial.println(Drawer.GetPosInches());
        Drawer.Run(-3.5);
        delay(1000);
        Serial.print("pos\t");Serial.println(Drawer.GetPosInches());
        Drawer.SoftStop();
        delay(2000);
    }
}

void moveloop(){
     Serial.print("pos\t");Serial.println(Drawer.GetPosInches());
     Drawer.GoToInches(0.0);
     delay(3000);
    
     Serial.print("pos\t");Serial.print(Drawer.GetPosInches());
     Drawer.MoveInches(.5);
     delay(3000);
  
     Serial.print("pos\t");Serial.print(Drawer.GetPosInches());
     Drawer.MoveInches(-1.0);
     delay(3000);
  
     Serial.print("pos\t");Serial.print(Drawer.GetPosInches());
     Drawer.MoveInches(1.25);
     delay(3000);
  
     Serial.print("pos\t");Serial.print(Drawer.GetPosInches());
     Drawer.MoveInches(-1.5);
     delay(3000);
  
     Serial.print("pos\t");Serial.print(Drawer.GetPosInches());
     Drawer.MoveInches(2);
     delay(3000);
  
     Serial.print("pos\t");Serial.print(Drawer.GetPosInches());
     Drawer.MoveInches(-3);
     delay(3000);
  
     Serial.print("pos\t");Serial.print(Drawer.GetPosInches());
     Drawer.MoveInches(5.25);
     delay(3000);
   
}

void steploop() {
    uint64_t start = millis();
    while(millis()-start < 10000){
        Serial.println(Drawer2.GetPos());
        Drawer2.SetStepDir(dir);    //set to forward stepping
        Drawer2.SingleStep();      //step
        
        if (millis()%2000 < 50)
        {
            Drawer2.ResetPos();
            Serial.println("reset and reverse");
            dir = !dir;
        }
        
        delay(50);
    }
}

void onesteploop() {
    uint64_t start = millis();
    while(millis()-start < 6000){
        Serial.print("stat1:");
        Serial.println(Drawer.ReadStat(SMARTLYNX_MOTOR_DIR));
        Drawer.SetStepDir(dir);    //set to forward stepping
        Drawer.SingleStep();      //step
        Serial.print("stat2:");
        Serial.println(Drawer.ReadStat(SMARTLYNX_MOTOR_DIR));
        delay(500);
        
        Serial.print("stat3:");
        Serial.println(Drawer.ReadStat(SMARTLYNX_MOTOR_DIR));
        Drawer.SetStepDir(!dir);
        Drawer.SingleStep();      //step
        Serial.print("stat4:");
        Serial.println(Drawer.ReadStat(SMARTLYNX_MOTOR_DIR));
        delay(1500);
    }
}
*/
