#include "TSensor.h"

/****All Temperatures read and stored in *C  *****/
const float ALARMTEMP = 120.0;      //temperature to raise alarm and shutdown systems
const float WARNTEMP = 90.0;        //temperature to warn operators
const float milliVoltsPerDegree = 19.5; //conversion factor for sensor
const float milliVoltsPerSig = 5000 / 1024.0;//conversion factor for 5 VDC line at 10-bit resolution
const short offsetZeroC = 400;      //millivolts sensor reads when at 0*C

void TSensor::Init(const char* name)
{
    strcpy(Name,name);  //name for printing
    
    temperature = 0;    //clear temperature
    
    //set readings, avg to zero
    for (int i=0;i<samples;i++)
        value[i]=0;
    
    avg = 0.0;
    
    printTimer = 0;  //setting to zero disables regular printing,\
                    otherwise set = milliseconds between prints
}

TSensor::TSensor(int _pin)
{
    pin = _pin;         //analog pin to read sensor
    pinMode(pin,INPUT); //set as input
    
    samples = 10;       //default number of samples
    value = new int[samples];//create storage for samples
}
TSensor::TSensor(int _pin, int n)
{
    pin = _pin;         //analog pin to read sensor
    pinMode(pin,INPUT); //set as input
    
    samples = n;        //set sample size
    value = new int[samples];//create storage for samples
}

//Monitor() Runs sensor averaging, printing, and alarm checking
float TSensor::Monitor()
{
    temperature = UpdateAverage(abs(analogRead(pin))); //get updated average reading
    temperature *= milliVoltsPerSig;    //convert to millivolts
    temperature -= offsetZeroC;         //shift for 0 deg C
    temperature /= milliVoltsPerDegree; //convert to degrees
    
    uint64_t Time = millis();   //get current processor time
    if (PRINT&&printTimer){     //if printing is enabled and printTimer has a value
        if (Time - lastPrint > printTimer)//if specified interval has elapsed
        {
            //print name of sensor and temperature
            Serial.print(Name);
            Serial.print("\t");
            Serial.println(temperature);
            lastPrint = Time;   //update timer
        }
    }
    
    /****** ALARMS GO HERE ******/
    if (temperature > ALARMTEMP){       //Over alarm temperature
        Serial.println(".....ALARM.....");
        Serial.println(".....ALARM.....");
        Serial.println(Name);
        Serial.println("Temperature High");
        Serial.print(temperature);
        Serial.print("degrees C \n.....ALARM.....");
        Serial.println("..SHUTDOWN IMMEDIATELY..");
    }
    else if (temperature > WARNTEMP){   //Over warning temperature
        Serial.println(".....WARNING.....");
        Serial.println(Name);
        Serial.println("Temperature High");
        Serial.print(temperature);
        Serial.print("degrees C \n.....WARNING.....");
    }
    
    return temperature;     //return averaged temperature
}

//UpdateAverage(input) adds "input" to the moving average and re-evaluates
float TSensor::UpdateAverage(float input)
{
    avg = 0.0;  //reset average
    
    //shift everything down in array, add to avg
    //TODO: make more efficient by changing starting and adding point instead\
    of moving everything each update
    for (int i=0;i<samples-1;i++){  //for all samples but last
        avg += value[i+1];      //add them to average
        value[i]=value[i+1];    //move them down a slot
    }
    value[samples-1] = input;  //add new value to storage
    
    avg += value[samples-1];    //add new value to average
    avg /= samples;             //divide by number of samples
    
    return avg;         //return new average
}