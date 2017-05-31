#include <Arduino.h>
#include <EEPROM.h>
#include "EEPROMCustom.h"
#include "PID.h"

uint32_t CurrentTime;

CalcPid pid;

PIDstruct ps = {1,3.0,4.0,5.0,15,};
bool one, two, three, four, five, six, seven, eight, nine, ten = 0;

void setup(){
	
	pid.SetValues(1,0.02,0.0,0.00,50);
	//pid.SetValues(&ps);
	
	Serial.begin(115200);
	
	while (!Serial){};
	delay(14);
	Serial.println("Setting position to 10.0, Setpoint to 10.0");
	pid.SetDisp(10.0);
	pid.SetSp(10.0);
	delay(200);
}

void loop(){
	CurrentTime = millis();
	float val = pid.GetPid();
	
	if (micros()%(int)(0.01*1000000)<200){
		Serial.print("PID:");
		Serial.println(val);
	}
	
	if ((CurrentTime>5000)&&!one){
		Serial.println("*******Setpoint 0.0");
		pid.SetSp(0.0);
		one = 1;
	}
	if ((CurrentTime>8000)&&!two){
		Serial.println("*******Position 0.0");
		pid.SetDisp(0.0);
		pid.SetCtrl();		//with real feedback system, control value would decrease as system approached target
		two=1;
	}
	if ((CurrentTime>10000)&&!three){
		Serial.println("*******Disabled");
		pid.Disable();
		three =1;
	}
	if ((CurrentTime>12000)&&!four){
		Serial.println("*******Enabled");
		pid.Enable();
		four =1;
	}
	if ((CurrentTime>15000)&&!five){
		Serial.println("*******Setpoint 5.0");
		pid.SetSp(5.0);
		five=1;
	}
	if ((CurrentTime>20000)&&!six){
		Serial.println("*******Smoothing On, Setpoint 2.0");
		pid.SetAsSmooth();
		pid.SetSp(2.0);
		pid.SetDisp(5.0);
		six=1;
	}
	if ((CurrentTime>22500)&&!seven){
		Serial.println("*******Position 2.0");
		pid.SetDisp(2.0);
		seven=1;
	}
	if ((CurrentTime>27000)&&!eight){
		Serial.println("*******Smoothing Off");
		pid.SetAsSmooth(0);
		eight=1;
	}
	if ((CurrentTime>30000)&&!nine){
		Serial.println("*******Setpoint 1.9");
		pid.SetSp(1.9);
		nine=1;
	}
	if ((CurrentTime>32000&&!ten)){
		Serial.println("*******Position1.9");
		pid.SetDisp(1.9);
		ten=1;
	}
	if ((CurrentTime>35000))
		delay(10000);
		
	if (pid.Target())
		Serial.println("*******###########");
	
	delay(2);
}