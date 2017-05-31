#ifndef PID_h
#define PID_h
#include <Arduino.h>
#include "EEPROMCustom.h"

struct PIDstruct
{
	short range;	//range to be considered in place
	float Kp;		//proportional
	float Ki;		//integral
	float Kd;		//derivative
    uint8_t responseTime;// millis between calculations
    uint16_t loc;   //location in EEPROM
};

class CalcPid {
	long prev_error;
	uint32_t time_on_target;
	int32_t last_setpoint;
	int32_t Setpoint, pos_disp;
	uint32_t lastCalculation;
	uint16_t responseTime;
	uint8_t responseShift;
	long error, integral;
	bool target, check, print_target;
	float control;
	
	uint16_t target_time_req;
    bool smooth;
    
    bool enabled;
	
public:	
	PIDstruct pidvalues;
	void Disable();
	void Enable();
    
	float GetPid();
	
    void SetGains(float kp, float ki, float kd){pidvalues.Kp = kp;pidvalues.Ki=ki;pidvalues.Kd=kd;};
	void ResetGains(){EEPROMread(&pidvalues);};
    float GetKp(){return pidvalues.Kp;};
    float GetKi(){return pidvalues.Ki;};
    float GetKd(){return pidvalues.Kd;};
    void SetKp(float kp){pidvalues.Kp=kp;};
	void SetKi(float ki){pidvalues.Ki=ki;};
	void SetKd(float kd){pidvalues.Kd=kd;};
	void Save(){EEPROMwrite(pidvalues);};
    
    bool Target();
	void SetSp(int32_t _sp){Setpoint=_sp;};
	
	void SetDisp(int32_t _disp){pos_disp=_disp;};
    int32_t GetDisp(){return pos_disp;};
	
    void SetCtrl(){control=0;};
    float GetControl(){return control;};
    void SetAsSmooth(){smooth=1;};
	void SetAsSmooth(bool in){smooth=in;};
	
	int32_t GetError(){return Setpoint - pos_disp;};
	
	void SetValues(struct PIDstruct*);
    void SetValues(short range, float Kp, float Ki, float Kd, uint8_t response);
	
    void MEMInit();
    
	//constructor
	CalcPid();
	
};//end CalcPid

#endif
