#include "SmartLynx.h"

bool SPI_Started = 0;

void SmartLynx::gpioClear(byte in){
    digitalWrite(in, LOW);
}
void SmartLynx::gpioSet(byte in){
    digitalWrite(in, HIGH);
}
uint32_t SmartLynx::WriteByte(byte send){
    gpioClear(SPI_CS);
    SPDR = send;
    uint32_t temp = 0;
    uint16_t start = micros();
    while ((micros() - start) < SPI_TIMEOUT)
    {
        if (SPSR & _BV(SPIF))
            break;
        //uint32_t temp = SPI.transfer(send);
        //TODO remove uneeded stuff
    }
    temp = SPDR?SPDR:0;
    gpioSet(SPI_CS);
    return temp;
}
uint32_t SmartLynx::WriteData(uint32_t data, byte byteCount){ 
    uint32_t temp =0;
    
    for (int index=byteCount; index > 0; index-- )
    {
        // right shift data according to loop index   
        temp |= WriteByte( data >> (8 * (index-1)) );
    }
    return temp;
}
uint32_t SmartLynx::ReadData(byte data){
    uint32_t temp =0;
    temp = WriteData(SMARTLYNX_GET_PARAM | data, 4);
    return temp;
}
int32_t SmartLynx::ReadPos(){
    int32_t temp=0;
    uint32_t i;
    uint32_t spiRxData, spiTxData;
    uint32_t spiTxBursts[4];
    uint32_t spiRxBursts[4];
    uint8_t maxArgumentNbBytes = 3;

    spiTxBursts[0] = ((uint8_t)SMARTLYNX_GET_PARAM )| (SMARTLYNX_ABS_POS);
    spiTxBursts[1] = SMARTLYNX_NOP;
    spiTxBursts[2] = SMARTLYNX_NOP;
    spiTxBursts[3] = SMARTLYNX_NOP;

    for (i = 3-maxArgumentNbBytes; i < 4; i++)
    {
        //TODO remove uneeded stuff
        gpioClear(SPI_CS);
        //SPDR = spiTxBursts[i];
        //while (!(SPSR & (1<<SPIF)));
        spiRxBursts[i] = WriteByte(spiTxBursts[i]);
        //spiRxBursts[i] = SPDR?SPDR:0;
        gpioSet(SPI_CS);
    }

    spiRxData = (spiRxBursts[0] << 24)|
              (spiRxBursts[1] << 16)|
              (spiRxBursts[2] << 8) |
              spiRxBursts[3];

    if (spiRxData & 0x00200000)  //if flagged as negative
    {
        spiRxData = spiRxData^0x3FFFFF;  //invert
        spiRxData = spiRxData & 0x3FFFFF;  //limit to 22 bits
        spiRxData += 1;                    //add one
        temp = -spiRxData;
    }
    else
    {
        temp = (int32_t) spiRxData;
    }
    pos = temp;
    return temp;
}
uint32_t SmartLynx::ReadStat(uint32_t filter){
    uint32_t temp = ReadData(SMARTLYNX_STATUS);
    temp &= filter;
    if (filter == SMARTLYNX_MOTOR_STAT)
        return temp & SMARTLYNX_MOTOR_STAT;
    else
        return !!temp;
}
void SmartLynx::SetSpeed(float speed){
    float data;
    // set speed command  
    WriteByte(SMARTLYNX_SET_PARAM | SMARTLYNX_FS_SPD);  
    data = speed * stepsPerInch;  // -> steps/s
    data *= 67.1;
    // set speed  
    WriteData((uint32_t)data, 2);
}
void SmartLynx::Run(float speed){
    float data;
    // data conversion for smart lynx  
    data = speed * stepsPerInch; // -> steps/s
    data *= 67.1;  // -> onboard conversion
    // run command  
    WriteByte(SMARTLYNX_RUN | ((speed>0)?reverse:!reverse));
    // set speed  
    WriteData((uint32_t)abs(data), 3);
}

SmartLynx::SmartLynx(byte cs, byte _step, byte stby, byte flag, byte busy, byte _log){   
    Setup(cs, _step, stby, flag, busy, _log);
}

SmartLynx::SmartLynx(byte cs, byte _step, byte stby, byte flag, byte busy){
    Setup(cs, _step, stby, flag, busy, 0);
}

void SmartLynx::Setup(byte cs, byte _step, byte stby, byte flag, byte busy, byte _log){
    SPI_CS = cs;
    STEP = _step;
    STBY = stby;
    FLAG = flag;
    BUSY = busy;
    LOGIC = _log;
    SPI_TIMEOUT = 300;
}

void SmartLynx::Setup(byte cs, byte _step, byte stby, byte flag, byte busy){
    Setup(cs, _step, stby, flag, busy, 0);
}

void SmartLynx::Init(const char* _name, float pitchDiameter, bool rev){
    strcpy(Name, _name);
    pitchDiam = pitchDiameter;
    reverse = rev;
    open = 0; //TODO
    
    SetMax(15.5);
    SetMin(0.0);
    
    pinMode(STBY, OUTPUT);
    gpioClear(STBY);
    pinMode(SPI_CS, OUTPUT);
    pinMode(FLAG, INPUT);
    pinMode(BUSY, INPUT);
    pinMode(STEP, OUTPUT);
    pinMode(LOGIC, OUTPUT);
    //pinMode(53, OUTPUT);    //need for hardware and SPI to work
    
    gpioSet(STBY);
    gpioSet(LOGIC);
    
    if (!SPI_Started){
        SPI.begin();
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE3);
        SPI.setClockDivider(SPI_CLOCK_DIV4);
        
        SPI_Started = 1;
        Serial.println("SPI");
    }
    
    Enabled=0;
    enable_attempts=0;
    
    acc = 400;
    dec = 1000;
    maxS = 16;      // in/sec
    minS = 0;
    Enable();
    
}

void SmartLynx::Enable(){
    Disable();
    gpioSet(STBY);      //driver will take config and commands, enable motor movement  
    gpioSet(LOGIC);     //allow programming/config
    
    SetStepSize(SMARTLYNX_STEP_ONE_32ND);   //half step mode
    
    ReadStat(SMARTLYNX_NOP);            //clear status flags
    
    ResetPos();
    
    _SetAcceleration(acc);
    _SetDeceleration(dec);
    SetMaxSpeed(maxS);      // in/sec
    
    enable_attempts++;
    
    //TODO: debug
    ReadData(SMARTLYNX_STEP_MODE);
    if (!((ReadData(SMARTLYNX_STEP_MODE)&B00000111) == (SMARTLYNX_STEP_ONE_32ND))){
        if (enable_attempts < 3){
            Disable();
            Enable();
        }
        else{
            //failed more than twice
            Serial.print(Name);
            Serial.print(" Enable Failure\n");
        }
    }
    else{
        Enabled = 1;
        enable_attempts = 0;
    }
}
void SmartLynx::Disable(){
    gpioClear(STBY);
    if (!enable_attempts) Enabled=0;
}

void SmartLynx::SetMaxSpeed(float speed){
    float data;  
    // data conversion for smart lynx
    data = speed * stepsPerInch;  // -> steps/s
    data *= 0.065536; // -> onboard conversion
    
    // set maximum speed command  
    WriteByte(SMARTLYNX_SET_PARAM | SMARTLYNX_MAX_SPEED);  
    // set maximum speed of rotation
    WriteData((uint32_t)abs(data), 2);
}
void SmartLynx::SetMinSpeed(float speed){
    float data;  
    // data conversion for smart lynx
    data = speed * stepsPerInch;  // -> steps/s
    data *= 4.1943;  // -> onbaord conversion
    
    // set minimum speed command  
    WriteByte(SMARTLYNX_SET_PARAM | SMARTLYNX_MIN_SPEED);  
    //set minimum speed of rotation   
    WriteData((uint32_t)abs(data), 2); 
}
void SmartLynx::_SetAcceleration(uint32_t acc){
    float data;
    // data conversion for smart lynx  
    data = 0.0687 * acc;
    // set acceleration command  
    WriteByte(SMARTLYNX_SET_PARAM | SMARTLYNX_ACC);  
    // set acceleration value   
    WriteData((uint32_t)data, 2); 
}
void SmartLynx::SetAcceleration(uint32_t _acc){ acc = _acc; }
void SmartLynx::_SetDeceleration(uint32_t dec){
    float data;  
    // data conversion for smart lynx  
    data = 0.0687 * dec;
    // set acceleration command  
    WriteByte(SMARTLYNX_SET_PARAM | SMARTLYNX_DEC);  
    // set acceleration value   
    WriteData((uint32_t)data, 2); 
}
void SmartLynx::SetDeceleration(uint32_t _dec){ dec = _dec; }
void SmartLynx::SetMax(float _max){  max = _max; }
void SmartLynx::SetMax(){ SetMax(GetPosInches()); }
void SmartLynx::SetMin(float _min){ min = _min; }
void SmartLynx::SetMin(){ SetMin(GetPosInches()); }
void SmartLynx::SetPosMax(){
    if (ReadStat(SMARTLYNX_MOTOR_STAT) == 0){
        ResetPos();
        WriteByte(SMARTLYNX_SET_PARAM | SMARTLYNX_ABS_POS);
        WriteData((uint32_t(max * stepsPerInch)), 4);
        ReadPos();
        Serial.print(GetPos());Serial.println(" is the max");
        Serial.print(GetPosInches());Serial.println(" inches is the max");
    }
    else
        Serial.println("motor moving, wait for stop");
}

void SmartLynx::SetStepSize(byte _stepSize){
    switch(_stepSize){
        case SMARTLYNX_STEP_FULL:
            stepSize = 1;
        break;
        case SMARTLYNX_STEP_HALF:
            stepSize = 2;
        break;
        case SMARTLYNX_STEP_QUARTER:
            stepSize = 4;
        break;
        case SMARTLYNX_STEP_ONE_EIGHT:
            stepSize = 8;
        break;
        case SMARTLYNX_STEP_ONE_16TH:
            stepSize = 16;
        break;
        case SMARTLYNX_STEP_ONE_32ND:
            stepSize = 32;
        break;
        case SMARTLYNX_STEP_ONE_64TH:
            stepSize = 64;
        break;
        case SMARTLYNX_STEP_ONE_128TH:
            stepSize = 128;
        break;
        default:
            Serial.println("Step Size Error!");
            stepSize = 128;
            Disable();
        break;
    }//end switch
    
    //update pulley and motor dependent data
    degreePerStep = 1.8 / stepSize;
    stepPerRev = 360 / degreePerStep;  //400
    inchPerRev = (pitchDiam*3.1415926535898)/25.4;  //4.7247
    stepsPerInch = stepPerRev/inchPerRev;  //84.6615
    //Serial.print("deg/step");Serial.println(degreePerStep);
    //Serial.print("step/rev");Serial.println(stepPerRev);
    //Serial.print("inch/Rev");Serial.println(inchPerRev);
    //Serial.print("step/inch");Serial.println(stepsPerInch);
    
    // set step size command  
    WriteByte(SMARTLYNX_SET_PARAM | SMARTLYNX_STEP_MODE);
    
    // set step size  
    WriteByte(_stepSize);
    
}
void SmartLynx::Move(int32_t stepCount){
    bool last_extend = extend;
    
    extend = (stepCount>0)?reverse:!reverse;
    
    if (extend){   //slow mode
        if (last_extend != extend)    //changed direction
            _SetAcceleration(acc/2.5);  //update mode
    }
    else{           //not slow mode
        if (last_extend != extend)    //changed direction
            _SetAcceleration(acc);      //update mode
    }
    
    // move command  
    WriteByte(SMARTLYNX_MOVE | extend);
    // set number of steps to move  
    WriteData(abs(stepCount), 3);
}
void SmartLynx::MoveInches(float inches){
    Serial.print("\tinches:\t");Serial.print((float)inches);
    Serial.print("\tsteps\t");Serial.println((int)(inches*stepsPerInch));
    Move((int)(inches*stepsPerInch));
}
void SmartLynx::GoToInches(float inches){
    float data;
    inches = (inches>max)?max:inches;   //don't go past max
    inches = (inches<min)?min:inches;   //don't go past min
    
    data = inches * stepsPerInch; // -> steps
    
    bool last_extend = extend;
    extend = ((inches - GetPosInches())>0)?reverse:!reverse;
    
    if (extend){   //slow mode
        if (last_extend != extend)    //changed direction
            _SetAcceleration(acc/2.5);  //update mode
    }
    else{           //not slow mode
        if (last_extend != extend)    //changed direction
            _SetAcceleration(acc);      //update mode
    }
    
    WriteByte(SMARTLYNX_GOTO_DIR | extend);  //GoToDir
    WriteData((uint32_t)abs(data),3);
}
void SmartLynx::GoToMax(){
    GoToInches(max);
}
void SmartLynx::GoToMin(){
    GoToInches(min);
}

void SmartLynx::SingleStep(){
    // pulse to STEP pin  
    gpioClear(STEP);
    delayMicroseconds(1);
    gpioSet(STEP);
}
void SmartLynx::SetStepDir(bool dir){
    WriteData(SMARTLYNX_STEPCLOCK | (dir?SMARTLYNX_FWD:SMARTLYNX_BCK), 1);
}

void SmartLynx::SoftStop(){
    // soft stop command  
    WriteByte(SMARTLYNX_SOFT_STOP);  
}
void SmartLynx::HardStop(){
    // hard stop command  
    WriteByte(SMARTLYNX_HARD_STOP);
}

int32_t SmartLynx::GetPos(){return ReadPos();}
float SmartLynx::GetPosInches(){return (float)(ReadPos() / stepsPerInch);}
void SmartLynx::ResetPos(){
    WriteByte(SMARTLYNX_RESET_POS);
    ReadPos();
    ReadPos();
}

byte SmartLynx::getFlagPin(){return FLAG;}
void SmartLynx::ClearFlags(){
    ReadData(SMARTLYNX_GET_STATUS);
}
bool SmartLynx::FlagMonitor(){
    bool _return = 0;
    if (Enabled)
    {
        uint32_t temp = ReadData(SMARTLYNX_STATUS);//reads flags
        //uint32_t temp = ReadData(SMARTLYNX_GET_STATUS);//reads and clears flags
        
        if ((temp & SMARTLYNX_TH_WRN)==SMARTLYNX_TH_WRN){
            Serial.print(Name);
            Serial.println(" Thermal Warning");
            _return = 1;
        }
        if ((temp & SMARTLYNX_TH_SD)==0){
            Serial.print(Name);
            Serial.println(" Thermal Shutdown");
            Disable();
            _return = 1;
        }
        if ((temp & SMARTLYNX_HIZ)==SMARTLYNX_HIZ){
            Serial.print(Name);
            Serial.println(" High Impedence");
            _return = 1;
        }
        if ((temp & SMARTLYNX_OVERCURRENT)==0){
            Serial.print(Name);
            Serial.println(" Overcurrent");
            Disable();
            _return = 1;
        }
        if ((temp & SMARTLYNX_STEP_LOSS)==0){
            Serial.print(Name);
            //TODO: check step loss flags while moving, figure out problem
            Serial.println(" Step Loss");
            _return = 1;
        }
        if ((temp & SMARTLYNX_NO_CMD)==SMARTLYNX_NO_CMD){
            Serial.print(Name);
            Serial.println(" No Command Executed");
        }
        if ((temp & SMARTLYNX_INVALID_CMD)==SMARTLYNX_INVALID_CMD){
            Serial.print(Name);
            Serial.println(" Invalid Command");
            _return = 1;
        }
        if ((temp & SMARTLYNX_UNDER_VOLT)==0){
            Serial.print(Name);
            Serial.println(" Undervoltage");
            Disable();
            _return = 1;
        }
        
        ClearFlags();
    }
    return _return;
}
bool SmartLynx::Moving()
{
    if (Enabled)
    {
        if (digitalRead(BUSY)){
            open = 1;
            return 1;
        }
        else if (ReadStat(SMARTLYNX_MOTOR_STAT)){
            open = 1;
            return 1;
        }
        else if (pos > min)
        {
            open = 1;
            return 1;
        }
        return 0;
    }
    else
        return 0;
}