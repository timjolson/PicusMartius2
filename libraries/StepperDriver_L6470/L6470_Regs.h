// smartlynx register addresses
const byte SMARTLYNX_ABS_POS = 0x01;
const byte SMARTLYNX_SPEED = 0x04;
const byte SMARTLYNX_ACC = 0x05;
const byte SMARTLYNX_DEC = 0x06;
const byte SMARTLYNX_MAX_SPEED = 0x07;
const byte SMARTLYNX_MIN_SPEED = 0x08;
const byte SMARTLYNX_FS_SPD = 0x15;
const byte SMARTLYNX_STEP_MODE = 0x16;
const byte SMARTLYNX_ALARM_EN = 0x17;
const byte SMARTLYNX_CONFIG = 0x18;
const byte SMARTLYNX_STATUS = 0x19;

//smartlynx commands
const byte SMARTLYNX_SET_PARAM = 0x00;
const byte SMARTLYNX_GET_PARAM = 0x20; 
const byte SMARTLYNX_RUN = 0x50; 
const byte SMARTLYNX_MOVE = 0x40; 
const byte SMARTLYNX_SOFT_STOP = 0xB0; 
const byte SMARTLYNX_HARD_STOP = 0xB8; 
const byte SMARTLYNX_GET_STATUS = 0xD0;
const byte SMARTLYNX_RESET_POS = 0xD8;
const byte SMARTLYNX_RESET_DEV = 0xC0;
const byte SMARTLYNX_STEPCLOCK = 0x58;
const byte SMARTLYNX_GOTO_DIR = 0x68;

// step size selection
const byte SMARTLYNX_STEP_FULL = 0; 
const byte SMARTLYNX_STEP_HALF = 1; 
const byte SMARTLYNX_STEP_QUARTER = 2; 
const byte SMARTLYNX_STEP_ONE_EIGHT = 3; 
const byte SMARTLYNX_STEP_ONE_16TH = 4; 
const byte SMARTLYNX_STEP_ONE_32ND = 5; 
const byte SMARTLYNX_STEP_ONE_64TH = 6; 
const byte SMARTLYNX_STEP_ONE_128TH = 7;

// flag masks
const byte SMARTLYNX_HIZ = 0x1;         //high impedence state
const byte SMARTLYNX_MOTOR_DIR = 0x10;  //motor direction from read stat
const byte SMARTLYNX_MOTOR_STAT = 0x60; //motor stat from read stat
const byte SMARTLYNX_STEP_LOSS = 0x6000; //step loss flag from read stat
const byte SMARTLYNX_NO_CMD = 0x80;     //no command performed
const byte SMARTLYNX_INVALID_CMD = 0x01;//invalid command received
const byte SMARTLYNX_UNDER_VOLT = 0x200;//undervoltage
const byte SMARTLYNX_TH_WRN = 0x400;    //thermal warning
const byte SMARTLYNX_TH_SD = 0x800;     //thermal shutdown
const byte SMARTLYNX_OVERCURRENT = 0x1000;//over current event

// stepping direction sets
const byte SMARTLYNX_FWD = 0x01;        //motor forward, use with STEP_CLOCK
const byte SMARTLYNX_BCK = 0x00;        //motor backward, use with STEP_CLOCK

// empty command
const byte SMARTLYNX_NOP = 0x00;        //empty command, use to retreive additional bytes
