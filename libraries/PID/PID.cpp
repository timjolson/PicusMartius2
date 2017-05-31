#include "PID.h"
#include <Arduino.h>

extern uint32_t CurrentTime;
const int ctrl_loop_limit=1000;		//limits control change per loop
uint16_t _responseShift = 0;     //shifts each PID calculator so objects do not run in the same cycles

void CalcPid::MEMInit(){
    EEPROMinit(&pidvalues);
}

void CalcPid::SetValues(struct PIDstruct* S){    
	pidvalues.range = S->range;
    pidvalues.Kp = S->Kp;
    pidvalues.Ki = S->Ki;
    pidvalues.Kd = S->Kd;
    pidvalues.responseTime = S->responseTime;
    responseTime = S->responseTime;
	
    //responseShift += S->responseTime;
    
    target_time_req = 400;
};

void CalcPid::SetValues(short _range, float _Kp, float _Ki, float _Kd, uint8_t _response){
      PIDstruct passValues = {
        _range,
        _Kp,
        _Ki,
        _Kd,
        _response,
    };

    SetValues(&passValues);
}

bool CalcPid::Target()
{
	error= Setpoint - pos_disp;
	if (check){
		if ((abs(error)<pidvalues.range))//within target range
		{
			if (time_on_target==0)//have not set timer
			{
				time_on_target=CurrentTime; //start timer
				print_target = 1;			//we can acknowledge
			}
			else if (CurrentTime - time_on_target>target_time_req)//timer is sufficient
			{
				if (print_target)		//have not acknowledged this time
				{
					target = 1;			//acknowledge
					print_target = 0;	//don't acknowledge again before resetting timer
				}
				else
					target = 0;			//already acknowledged, waiting for next time
			}
			else
				target = 0;		//not on target long enough
		}
		else
		{
			target = 0;			//not on target
			time_on_target = 0;	//allow starting timer again
		}
		
		return target;
	}
	else return 0;
}//end CalcPid::Target

float CalcPid::GetPid()
{
    if (enabled){
        //offset calculations so they don't run in the same cycle
        //only calculate when responseTime is reached (milliseconds)
        if ((CurrentTime - lastCalculation - responseShift)> responseTime)
        {
            error = Setpoint - pos_disp;	//how far off we are now
            integral += error;				//keep track for integral
            integral = constrain(integral, (-1)*ctrl_loop_limit, ctrl_loop_limit);//limit size of integral value
            
            float delta_err = error - prev_error;	//change in error
            //delta_err /= responseTime;		//divide by cycle time(millis)
            prev_error = error;						//store for next run
            
            float temp= pidvalues.Kp*error + pidvalues.Ki*integral + pidvalues.Kd*delta_err;//add cycle's control variable amounts
            //temp = constrain(temp, -0.1, 0.1);			//limit effect per calculation cycle
            
            control += temp;	//add to output value
            control = constrain(control,-100,100);		//limit control value (+- 100%)
            
            //eliminate big value jumps on setpoint change
            if (Setpoint!=last_setpoint){
                if (!smooth) control=0;
                integral=0;
                last_setpoint=Setpoint;
            }
			
            lastCalculation = millis();	//time of calculation
        }
        return control/100.0;	//return control scaled to +- 1.00 for use as multiplier
    }
    else return 0.0;
	
}//end CalcPid::GetPid

void CalcPid::Disable()
{
    SetSp(pos_disp);
    SetCtrl();
    enabled = 0;
	check=0;
}//end Disable

void CalcPid::Enable()
{
    enabled = 1;
	check=1;
    print_target=1;
    SetSp(pos_disp);
    SetCtrl();
}//end Enable

CalcPid::CalcPid()
{
	control=0;
    lastCalculation = 0;
    enabled = 1;
	check =1;
    _responseShift+=5;
    responseShift=_responseShift;
}