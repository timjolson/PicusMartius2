#include <Arduino.h>
#include <EEPROM.h>
#include "EEPROMCustom.h"
#include "PID.h"


CalcPid pid;

PIDstruct ps = {1,4.0,5.0,6.0,7,};

void setup(){
	
	//Necessary to initialize EEPROM addresses
	//also used to set EEPROM feedback stream when
	//EEPROMDEBUG in header is 1
	SetEEPROMStream(&Serial);
	pid.MEMInit();
	
	//pid.SetValues(0,4.0,5.0,6.0,7);
	//pid.SetValues(&ps);
	Serial.begin(115200);
	
	while (!Serial){};
	delay(30);
	
	Serial.println("\n\n*******************\n\n");
	Serial.println("Starting Value");
	
	Serial.println(pid.GetKp());
	
	pid.Save();
	
	Serial.println("after Save");
	Serial.println(pid.GetKp());
	
	pid.SetGains(3.0, 9.0, 8.0);
	
	Serial.println("after SetGains");
	Serial.println(pid.GetKp());
	
	pid.ResetGains();
	
	Serial.println("after ResetGains");
	Serial.println(pid.GetKp());
	
	pid.SetKp(2.0);
	pid.SetKd(2.0);
	
	Serial.println("after SetKp, SetKd");
	Serial.println(pid.GetKp());
	
	pid.Save();
	
	pid.ResetGains();
	
	Serial.println("after Save and ResetGains");
	Serial.println(pid.GetKp());
	
	
}

void loop(){


}