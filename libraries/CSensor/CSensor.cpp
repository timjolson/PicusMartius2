#include "CSensor.h"

const short milliVoltsPerAmp = 185;
const float milliVoltsPerSig = 5000 / 1024.0;

CSensor::CSensor()
{
    samples = 10;
    value = new int[samples];
};

CSensor::CSensor(int n)
{
    samples = n;
    value = new int[samples];
};

void CSensor::Init(int _pin)
{
    pin = _pin;
    currentDraw = 0;
    
    pinMode(pin,INPUT);
    
    for (int i=0;i<samples;i++)
        value[i]=0;
    
    avg = 0.0;
}

float CSensor::Monitor()
{
    currentDraw = UpdateAverage(abs(analogRead(pin) - 512));//shift for center
    currentDraw *= milliVoltsPerSig;    //convert to millivolts
    currentDraw /= milliVoltsPerAmp;    //convert to amps
    
    return currentDraw;
}

float CSensor::UpdateAverage(float input)
{
    avg = 0.0;
    for (int i=0;i<samples-1;i++){
        avg += value[i+1];
        value[i]=value[i+1];
    }
    value[samples-1] = input;
    
    avg += value[samples-1];
    avg /= samples;
    
	return avg;
}