#include "ThrusterPID.h"
#include "Thruster.h"

//Thruster speeds are reversed ( 0 = full speed* , 255 = stopped )
/*See note below about full speed*/

/**** FOR SAFETY OF CIRCUITS, DO NOT CHANGE UNLESS H-BRIDGE FREQUENCY IS CHANGED ******/
const short ThrusterMax = 5;        //cannot write value lower than this (top speed)

extern PIDstruct thPID;             //thruster PID is common through all 8, kept in header
const short ThrusterStop = 255;     //stopped value (lowest speed)
const short ThrusterDeadband = 13;  //deadband for motor to not do anything
const short CurrentLowerLimit = 1;  //milliAmps to consider motor not connected
const short CurrentUpperLimit = 2500;//milliAmps to consider motor stalled

Thruster::Thruster(int _sigPin, int _dirPin, int _currPin)
{
	sigPin = _sigPin;		//signal(pwm) output pin
    currPin = _currPin;     //current sensor pin
    dirPin = _dirPin;       //direction pin
    
    /***FREQUENCY ADJUSTMENTS****/
    //default 490Hz
    
    //31kHz
    /*PWM TIMER SETTINGS*/
    //TCCR1B = (TCCR1B & 0b11111000) | 0x01;
    //TCCR3B = (TCCR3B & 0b11111000) | 0x01;
    //TCCR4B = (TCCR4B & 0b11111000) | 0x01;
    
}

void Thruster::Init(const char* _name, bool _reverse){
	strcpy(Name,_name);			//display name for motor
	reverse = _reverse;		    //if the motor uses reverse operation
    
    PID.MEMInit();				//sets up EEPROM saving/loading, loads
    PID.SetValues(&thPID);      //set the ramping PID values to common
    PID.SetAsSmooth();          //make PID ramp smoothly up and down

    //Set pin modes
    pinMode(sigPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(currPin, INPUT);
    
	digitalWrite(sigPin, HIGH);  //stop motor on poweron
	digitalWrite(dirPin, HIGH);  //set direction pin consistent
    
    cSens.Init(currPin);        //set current sensor pin
    
	deadband = ThrusterDeadband;    //set deadband
    
    overCurrent = 0;            //overcurrent flag
    underCurrent = 0;           //undercurrent flag
}

void Thruster::Write(short _commandspeed)
{    
    speed = _commandspeed;  //set speed to the commandspeed
    
    //if command is to stop
    if (speed == 0) 
        digitalWrite(sigPin, HIGH); //stop motor (== pwm 255)
    else{
        _commandspeed = map(_commandspeed, 0, 255, ThrusterStop, ThrusterMax);    //map speed to control value range
        
        if (_commandspeed < ThrusterMax )  //double check for electronics safety
            _commandspeed = ThrusterMax;
        
        analogWrite(sigPin, _commandspeed);    //write the output speed
    }
}

void Thruster::Move(short _commandspeed)
{    
    if (!(overCurrent || underCurrent)) //if neither flag has been set
    {
        if (enabled){
            speed = 255*PID.GetPid(); //get speed magnitude, scale to 0->255 range
            
            //apply deadband
            if(abs(_commandspeed) < deadband)
            {
                PID.SetSp(0);   //set PID to stopped
            }
            else
            {
                if (speed < 0) //if negative command
                    digitalWrite(dirPin, !reverse); //set direction backwards
                else            //positive command
                    digitalWrite(dirPin, reverse);  //set direction forwards
                
                PID.SetSp(_commandspeed); //set PID to desired speed
                
            }
            
            PID.SetDisp(speed);  //update current speed output to ramping function
            Write(abs(speed));   //write magnitude of speed to motor
        }
    }
    else    //over or under current flagged
    {
        //hard stop everything
        speed = 0;  //feedback speed = 0
        PID.Disable();//set PID to go to 0
        Write(0);   //ouput to motor = 0
    }
}

float Thruster::Monitor()
{
    currentDraw = cSens.Monitor();  //monitor sensor, update average, etc.
    
    if (!(overCurrent || underCurrent))//no current flags
    {
        //TODO: check current vs pwm equation
        
        if (speed > deadband)   //use deadband
        {
            if ( currentDraw*1000.0 < CurrentLowerLimit ) //if draw is below lower limit
            {
                digitalWrite(sigPin, HIGH); //stop motor
                Serial.print(Name);
                Serial.print(" :::  Under Current ::: ");
                Serial.println(currentDraw);
                
                //underCurrent = 1; //set flag (requires power cycle to restore)
            }
        }
        
        if ( currentDraw*1000.0 > CurrentUpperLimit ) //if draw is above upper limit
        {
            digitalWrite(sigPin, HIGH); //stop motor
            Serial.print(Name);
            Serial.print(" :::  Over Current ::: ");
            Serial.println(currentDraw);
            
            overCurrent = 1;    //set flag (requires power cycle to restore)
        }
    }
    
    return currentDraw;
}
