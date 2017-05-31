/****
Stepper Motor Driver Library - L6470 Embedded on PHI Robotics' SmartLynx 1.0

Updated: 6 Jun 2016
By: Tim Olson, Engineering Intern, SK Handtool, LLC

****/

/**** HARDWARE SET UP

Wire Connections:
    For single driver board:
        GND -> Arduino/Common Ground
        STCK -> Mega2560 Pin 48
        SPI_CS -> Chip select pin for the board [assigned in Init()]
        SPI_MOSI -> Mega2560 MOSI Pin 51
        SPI_MISO -> Mega2560 MISO Pin 50
        VDD -> Mega2560 3.3V or digital pin assigned in Init()
        SPI_SCK -> Mega2560 SCLK/SPI Clock Pin 52
        BUSY -> digital pin assigned in Init()
        FLAG -> digital pin assigned in Init()
        STBY/RST -> digital pin assigned in Init()
    
    For daisy chain:
        BUSY, FLAG, GND, STBY, VDD can be wired together for different functionality.
        SPI* run from first board to second, etc. through JP2 on driver.
        STCK should run to each driver individually, as it is the "single step" pin.
        
        **NOTE** To daisy chain, the jumpers must be set according the the SmartLynx Manual

Motor<>Driver Connections:
NEMA17 Openbuilds Stepper
    GND -> Common ground / power supply ground
    BAT+ -> 24VDC depending upon motor
    
    A+ -> Motor Coil A Green
    A- -> Motor Coil A Yellow
    B+ -> Motor Coil B Red
    B- -> Motor Coil B Blue

NEMA23 Openbuilds Stepper
    GND -> Common ground / power supply ground
    BAT+ -> 24VDC depending upon motor
    
    A+ -> Motor Coil A Red
    A- -> Motor Coil C Green
    B+ -> Motor Coil B Yellow
    B- -> Motor Coil D Blue

END HARDWARE SETUP
****/

#ifndef SMARTLYNX_H
#define SMARTLYNX_H
#include <Arduino.h>
#include <SPI.h>
#include "L6470_Regs.h"

class SmartLynx {
    char Name[10];
    
    bool open;
    uint32_t pos;
    uint32_t pos_last;
    float posInches;
    
    uint32_t lastChange;
    bool Enabled;
    byte enable_attempts;
    
    bool reverse;
    bool extend;
    float pitchDiam;
    float degreePerStep;
    float stepPerRev;
    float inchPerRev;
    float stepsPerInch;
    
    float minS;
    float maxS;
    int acc;
    int dec;
    float min;
    float max;
    byte stepSize;
    
    byte SPI_CS;
    byte STBY;
    byte LOGIC;
    byte STEP;
    byte BUSY;
    byte FLAG;
    
    short SPI_TIMEOUT; //micros
    
    void gpioClear(byte pin);
    void gpioSet(byte pin);
    uint32_t WriteByte(byte send);
    uint32_t WriteData(uint32_t data, byte byteCount);
    uint32_t ReadData(byte data);
    int32_t ReadPos();
    
    void SetSpeed(float speed);
    
    void _SetAcceleration(uint32_t acc);
    void _SetDeceleration(uint32_t dec);
    
    void ClearFlags();

public:
    
    SmartLynx(){};
    SmartLynx(byte cs, byte step, byte stby, byte flag, byte busy, byte log);
    SmartLynx(byte cs, byte step, byte stby, byte flag, byte busy);
    
    void Setup(byte cs, byte step, byte stby, byte flag, byte busy, byte log);
    void Setup(byte cs, byte step, byte stby, byte flag, byte busy);
    void Init(const char* name, float pitchDiameter,bool reverse);
    
    void Enable();
    void Disable();
    
    void SetMax();
    void SetMax(float max);
    void SetMin();
    void SetMin(float min);
    
    void SetMaxSpeed(float speed);
    void SetMinSpeed(float speed);
    void SetAcceleration(uint32_t acc);
    void SetDeceleration(uint32_t dec);
    void SetStepSize(byte stepSize);
    void SetStepDir(bool dir);
    
    void Move(int32_t stepCount);
    void MoveInches(float inches);
    void GoToInches(float inches);
    void GoToMax();
    void GoToMin();
    void Run(float speed);
    void SingleStep();
    void SoftStop();
    void HardStop();
    
    int32_t GetPos();
    float GetPosInches();
    void ResetPos();
    void SetPosMax();
    
    uint32_t ReadStat(uint32_t filter);
    bool FlagMonitor();
    byte getFlagPin();
    
    bool Moving();
    
};

#endif //ifndef
