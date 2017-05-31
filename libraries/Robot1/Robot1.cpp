#include "Robot1.h"
#include "PID.h"

extern PIDstruct gyroPID;
extern unsigned long last_command_time;
extern uint32_t CurrentTimeMicros;

const int FORWARD = 200;
const int REVERSE = 0;
const int NEUTRAL = 100;
const int OFFSET = 100;

const int TANK_FORWARD = 100;
const int TANK_REVERSE = -100;

bool TANKDRIVE = false;

int left_speed;
int right_speed;

int front_left = 0;
int rear_left = 0;
int front_right = 0;
int rear_right = 0;

int rightY = 500;
int rightX = 500;
int leftX = 500;

int rightYmax = 1000;
int rightXmax= 1000;
int leftXmax = 1000;

int rightYmin = 0;
int rightXmin = 0;
int leftXmin = 0;

bool not_calibrated = true;

const bool mecanum_debugging_printout = false;
const bool tankdrive_debugging_printout = true;

// for Read_Serial()
unsigned int integerValue=0;  // Max value is 65535
char incomingByte;

// motor objects

// constructor
RobotClass::RobotClass()
{
    // initialize motor objects
    Front_Left.Init("Front_Left", true);//reversed
    Front_Right.Init("Front_Right", false);
    Rear_Left.Init("Rear_Left", true);//reversed
    Rear_Right.Init("Rear_Right", false);
    
    PDg.init(&gyroPID);
}

// set pins for the RC radio
void RobotClass::setupRC(int input_pins[])
{
    setupRCData(input_pins);
}

void RobotClass::SetControlData(int* _data)
{
    rcValue[0] = _data[0]; //rotate (rx)
    rcValue[1] = _data[1]; //drive (ly)
    rcValue[2] = _data[2]; //drive (ry)
    rcValue[3] = _data[3]; //drive (lx)
    rcValue[4] = _data[4]; //dispenser
    rcValue[5] = _data[5]; //conveyor
    rcValue[6] = _data[6]; //sort
    rcValue[7] = _data[7]; //linear actuator
}

//create and set buttons and things on startup
void RobotClass::Init(){
	
}

void RobotClass::turnRCmecanum(int _rot)
{
    last_command_time = CurrentTimeMicros;
    PDg.Disable();
    //Serial.print("cmd");Serial.println(_rot);
    //left x
    _rot = constrain(_rot,-10,10);
    rcValue[3] = constrain(_rot*5,-500,500)+1500;
    //Serial.print("rcv");Serial.println(rcValue[3]);
    rcValue[1] = 1500;
    rcValue[0] = 1500;
}

void RobotClass::SetHeading(int _angle)
{
    last_command_time = CurrentTimeMicros;
    PDg.Enable();
    //left x
    if (_angle < -180000) _angle += 360000;
    else if (_angle > 180000) _angle -=360000;
    PDg.SetSp(_angle / 10);
}

void RobotClass::driveRCmecanum(int _angle)
{
    last_command_time = CurrentTimeMicros;
    SetHeading(heading*10);
    
    //Serial.print("HEADING TO HOLD");Serial.println(heading*10);

    _angle /=100;
    //right y
    rcValue[1] = constrain((8*5)*sin(_angle * PI/180),-500,500)+1500;
    //right x
    rcValue[0] = constrain((12*5)*cos(_angle * PI/180),-500,500)+1500;
}

bool RobotClass::CheckTarget()
{
    return PDg.Target();
}

void RobotClass::Stop()
{
    rcValue[0]= rcValue[1]= rcValue[2]= rcValue[3]= 1500;
}

// takes in raw rc values and outputs
void RobotClass::driveRCmecanum()
{
    int drive = map(rcValue[1], 1000, 2000, 500, -500);
    int strafe = map(rcValue[0], 1000, 2000, 500, -500);
    int rotate = map(rcValue[3], 1000, 2000, -500, 500);
    
    PDg.SetDisp(heading);
    float val = PDg.GetPd();
    /*
Serial.print(heading);Serial.print("\t");
Serial.print(PDg.GetError());Serial.print("\t");
Serial.println(val);
    */
    rotate -= val*80;
    
    //Serial.print("rot");Serial.println(rotate);
    // set the speeds for the four motors of the robot
    front_left  = drive + strafe - rotate;
    rear_left   = drive - strafe - rotate;
    front_right = drive - strafe + rotate;
    rear_right  = drive + strafe + rotate;
    
    //get scale for drive motors to keep vector ratios
	//intact when max output is reached/exceeded
	
	////first get biggest speed
	int biggest = 1000;
	if (abs(front_left)>biggest)biggest=abs(front_left);
	if (abs(rear_left)>biggest)biggest=abs(rear_left);
	if (abs(front_right)>biggest)biggest=abs(front_right);
	if (abs(rear_right)>biggest)biggest=abs(rear_right);

	////then set outputs to ratio of biggest speed
	int front_left_temp = map(abs(front_left),0,biggest,0,1000);
	int rear_left_temp = map(abs(rear_left),0,biggest,0,1000);
	int front_right_temp = map(abs(front_right),0,biggest,0,1000);
	int rear_right_temp = map(abs(rear_right),0,biggest,0,1000);

    ////put signs back in
    front_left=(front_left_temp)*(front_left/abs(front_left));
    rear_left=(rear_left_temp)*(rear_left/abs(rear_left));
    front_right=(front_right_temp)*(front_right/abs(front_right));
    rear_right=(rear_right_temp)*(rear_right/abs(rear_right));
    
    //write to motors
    Front_Left.Move(front_left);
    Rear_Left.Move(rear_left);
    Front_Right.Move(front_right);
    Rear_Right.Move(rear_right);
}

void RobotClass::Drive()
{
    if (TANKDRIVE)
    {
        Tankdrive_v1();
    }
    else
    {
        driveRCmecanum();
    }
}

// function called by .ino file
void RobotClass::Drive(bool tank)
{
    if(tank)
    {
        Tankdrive_v1();
    }
    else
    {
        driveRCmecanum();
    }
}

// calibrate RC drive
void RobotClass::CalibrateRC()
{
    // chech for signal to start calibration
    /*
    if(Read_Serial()==1)
    {
        Calibrate1();
    }
    */
    
    if(rcValue[5] > 1900)
    {
        Calibrate1();
    }
}

void RobotClass::Tankdrive_v1()
{
    // map values from the radio
    int y = map(rcValue[1], rightYmax, rightYmin, TANK_FORWARD, TANK_REVERSE);
    int x = map(rcValue[0], rightXmax, rightXmin, TANK_FORWARD, TANK_REVERSE);

    if(0)
    //if(tankdrive_debugging_printout)
    { 
        Serial.print(x);
        Serial.print("\t");
        Serial.println(y);
    }
    
    int right;
    int left;
    
    // first Compute the angle in deg
    
    // First hypotenuse
    float z = sqrt(x*x + y*y);
    // angle in radians
    float rad = acos(abs(x)/z);
    // and in degrees
    float angle = rad*180/PI;
    
    // Now angle indicates the measure of turn
    // Along a straight line, with an angle o, the turn coefficient is same
    // this applies for angles between 0-90, with angle 0 the coeff is -1
    // with angle 45, the coefficient is 0 and with angle 90, it is 1
    float tcoeff = -1 + (angle/90)*2;
    float turn = tcoeff * abs(abs(y) - abs(x));
    turn = (turn*100)/100;

    // And max of y or x is the movement
    int move = max(abs(y),abs(x));

    // First and third quadrant
    if( (x >= 0 && y >= 0) || (x < 0 &&  y < 0) )
    {
        left = move;
        right = turn;
    } 
    else 
    {
        right = move;
        left = turn;
    }

    // Reverse polarity
    if(y < 0) 
    {
        left = 0 - left;
        right = 0 - right;
    }
    
    left_speed = left;
    right_speed = right;
    if(0)
    //if(tankdrive_debugging_printout)
    {
        Serial.print(left_speed);
        Serial.print("\t");
        Serial.println(right_speed);
    }

    //left_speed = map(left_speed, TANK_FORWARD, TANK_REVERSE, 2000, 1000);
    //right_speed = map(right_speed, TANK_FORWARD, TANK_REVERSE, 2000, 1000);

    Front_Left.Move(left_speed);
    Rear_Left.Move(left_speed);
    Front_Right.Move(right_speed);
    Rear_Right.Move(right_speed);
}
