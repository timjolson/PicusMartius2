#ifndef THRUSTERPID_CPP
#define THRUSTERPID_CPP

#include "ThrusterPID.h"
#include "PID.h"

//using Proportional controller to ramp thruster speeds up and down
//TODO: instead of generic value, set up millis() to full speed from stop, then recalculate
//P value or anything else

/*****	        range	kp		ki		kd		respTime*/
PIDstruct thPID={ 0,	0.0,    0.0,    0.0,    50, };


#endif