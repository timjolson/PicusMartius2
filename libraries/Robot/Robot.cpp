#include "Robot.h"

#define FORWARD 200
#define REVERSE 0
#define NEUTRAL 100
#define OFFSET 100

#define TANK_FORWARD 100
#define TANK_REVERSE -100

bool TANKDRIVE = true;

int left_speed;
int right_speed;

int front_left = 0;
int rear_left = 0;
int front_right = 0;
int rear_right = 0;

int rightY = 1500;
int rightX = 1500;
int leftX = 1500;

int rightYmax = 1500;
int rightXmax= 1500;
int leftXmax = 1500;

int rightYmin = 1500;
int rightXmin = 1500;
int leftXmin = 1500;

bool not_calibrated = true;

bool mecanum_debugging_printout = false;
bool tankdrive_debugging_printout = true;

// for Read_Serial()
unsigned int integerValue=0;  // Max value is 65535
char incomingByte;

// motor objects
Motor Front_Left = Motor(23, 1000, 2000);
Motor Front_Right = Motor(22, 1000, 2000);
Motor Rear_Left = Motor(21, 1000, 2000);
Motor Rear_Right = Motor(20, 1000, 2000);

// constructor
RobotClass::RobotClass()
{
    // initialize motor objects
    Front_Left.Init("Front_Left", false);
    Front_Right.Init("Front_Right", false);
    Rear_Left.Init("Rear_Left", false);
    Rear_Right.Init("Rear_Right", false);
}

// set pins for the RC radio
void RobotClass::setupRC(int input_pins[])
{
    setupRCData(input_pins);
}

void RobotClass::SetControlData(byte* _data)
{

}

/*
//mode settings
uint16_t RobotClass::M_GetMode(){return 0;};			//get current mode settings
uint16_t RobotClass::M_SetMode(byte _rxmode){return 0;};//set current mode
bool RobotClass::M_GetEnabled(){return 0;};			//get enable/disable status
void RobotClass::M_Enable(){};				//set mode to enabled
void RobotClass::M_Disable(){};				//set mode to disabled
void RobotClass::M_DriveOn(byte _drivemode){};//enable and set drive mode
void RobotClass::M_DriveOff(){};				//disable driving mode
void RobotClass::M_ArmOn(){};					//enable arm control mode
void RobotClass::M_ArmOff(){};				//disable arm control mode
void RobotClass::M_SetOpsMode(byte _mode){};	//set operating mode /arm vs drive vs whatever else
void RobotClass::M_GyroOn(){};				//enable gyro rotational correction
void RobotClass::M_GyroOff(){};				//disable gyro rotational correction
void RobotClass::M_Auto(){};					//set autonomous mode
void RobotClass::M_Manual(){};				//set manual control mode
bool RobotClass::M_GetDriveMode(){return 0;};			//get drive mode (mecanum or tank)
//r_mode = (Enable)(Drive)(Control Type)(Drive Mode)(Arm)(Gyro)(Auto)(empty)
*/

//create and set buttons and things on startup
void RobotClass::Init(){
	
}

//bool* RobotClass::GetTXdata(){return *TXdata;};
byte RobotClass::GetTXsize(){
    return DATASIZEBYTE;
}

/*
short RobotClass::cosDir(short indir, short adjust, short norm, byte db)
{
    float output=0;
    output=indir-adjust;				//add adjustment angle
  
    if (abs(output)>(1800))
        output-=3600 * (output/abs(output));	// +/- 180*
    output=cos(output*PI/1800);			//get cos of new angle
    output *= norm;						//normalize
  
    if (abs(output)>norm) 
        output-= norm * (output/abs(output));	//limit
    if (abs(output)<db) 
        output = 0;				//deadband

    return output;
}
*/

/*
short RobotClass::sinDir(short indir, short adjust, short norm, byte db)
{
    float output=0;
    output=indir-adjust;		//add adjustment angle
    
	if (abs(output)>(1800))
	    output-=3600 * (output/abs(output));  // +/- 180*
    
	output=sin(output*PI/1800);		//get sin of new angle
    output *= norm;					//normalize
    
	if (abs(output)>norm)
	    output-= norm * (output/abs(output));   //limit
    if (abs(output)<db)
	    output = 0;				//deadband

    return output;
}
*/

// takes in raw rc values and outputs
void RobotClass::driveRCmecanum()
{
    // map rc values to the chosed scale
    rightY = map(rcValue[0], rightYmax, rightYmin, FORWARD, REVERSE);
    rightX = map(rcValue[1], rightXmax, rightXmin, FORWARD, REVERSE);
    leftX = map(rcValue[3], leftXmax, leftXmin, FORWARD, REVERSE);

    // calculated values
    int drive = (FORWARD - rightY) - NEUTRAL;
    int strafe = rightX - NEUTRAL;
    int rotate = leftX - NEUTRAL;

    // set the speeds for the four motors of the robot
    front_left  = drive + strafe + rotate + NEUTRAL;
    rear_left   = drive - strafe + rotate + NEUTRAL;
    front_right = drive - strafe - rotate + NEUTRAL;
    rear_right  = drive + strafe - rotate + NEUTRAL;
/*
    Front_Left.Move(maximum(front_left)-OFFSET);
    Rear_Left.Move(maximum(rear_left)-OFFSET);
    Front_Right.Move(maximum(front_right)-OFFSET);
    Rear_Right.Move(maximum(rear_right)-OFFSET);
*/
    // print outs for debugging
    if(mecanum_debugging_printout)
    {

        Serial.print(maximum(front_left)-OFFSET);
        Serial.print("\t");
        Serial.print(maximum(rear_left)-OFFSET);
        Serial.print("\t");
        Serial.print(maximum(front_right)-OFFSET);
        Serial.print("\t");
        Serial.print(maximum(rear_right)-OFFSET );
        Serial.print("        ");

        Serial.print(rightY);
        Serial.print("\t");
        Serial.print(rightX);
        Serial.print("\t");
        Serial.print(leftX);
        Serial.print("        ");
    }
}

void Move(int test){

}
// reads values in from Serial Monitor in Arduino IDE
int RobotClass::Read_Serial()
{
    if (Serial.available() > 0) {   // something came across serial
          integerValue = 0;         // throw away previous integerValue
              while(1) 
              {            // force into a loop until 'n' is received
                incomingByte = Serial.read();
                if (incomingByte == '\n') break;   // exit the while(1), we're done receiving
                if (incomingByte == -1) continue;  // if no characters are in the buffer read() returns -1
                integerValue *= 10;  // shift left 1 decimal place
                // convert ASCII to integer, add, and shift left 1 decimal place
                integerValue = ((incomingByte - 48) + integerValue);
              }
          //Serial.println(integerValue);   // Do something with the value
      }
    return integerValue;
}

// calibrate values from the RC remove
void RobotClass::Calibrate1()
{
    while(not_calibrated)
    {
        // calibrate rightX
        if(rcValue[0] > rightXmax)
        {
            rightXmax = rcValue[0];
        }

        if(rcValue[0] < rightXmin)
        {
            rightXmin = rcValue[0];
        }

        // calibrate rightY
        if(rcValue[1] > rightYmax)
        {
            rightYmax = rcValue[1] ;
        }
        
        if(rcValue[1] < rightYmin)
        {
            rightYmin = rcValue[1];
        }

        // calibrate leftX
        if(rcValue[3] > leftXmax)
        {
            leftXmax = rcValue[3];
        }
        
        if(rcValue[3] < leftXmin)
        {
            leftXmin = rcValue[3];
        }
        
        // check for signal to stop calibration
        if(Read_Serial()==4)
        {
            // set value to exit while loop
            not_calibrated = false;

            // add timeout value
        }
    }
}

// function to correct any values that are too large
int RobotClass::maximum(int motor)
{
    if(motor > FORWARD)
    {
        return motor = FORWARD;
    }
    else if (motor < REVERSE)
    {
        return motor = REVERSE;
    }
    else
    {
        return motor;
    }
}

int RobotClass::maximum2(int motor)
{
    if(motor > TANK_FORWARD)
    {
        return motor = TANK_FORWARD;
    }
    else if (motor < TANK_REVERSE)
    {
        return motor = TANK_REVERSE;
    }
    else
    {
        return motor;
    }
}

// function called by .ino file
void RobotClass::Drive()
{
    if(TANKDRIVE)
    {
        Tankdrive_v1();

        /*
        int throttle = map(rcValue[0], rightYmax, rightYmin, 100, -100);
        int spin = map(rcValue[3], leftXmax, leftXmin, 100, -100);
       
        front_left = 
        rear_left = 
        front_right = 
        rear_right =
    */

        //#if TANKDRIVE
        //speed += (ysign*js1.y)+(wsign*js1.w)+(wsign);
        ////speed = (ysign*y.GetPd())+(wsign*w.GetPd())+(gsign*g.GetPd());
        ////speed = js1.Amp()*((ysign*y.GetPd())+(wsign*w.GetPd())+(gsign*g.GetPd()));
        //#else
        /*
        short usecosdir = cosDir(jdir, (short) pos[2] * js1.g, 500, 0);
        short usesindir = sinDir(jdir, (short) pos[2] * js1.g, 500, 0);
        speed += xsign * usecosdir * js1.amp;
        speed += ysign * usesindir * js1.amp;
        speed += wsign * (1.5- js1.amp) * js1.w;
        speed -= (2 - js1.amp)*(wsign);
        */

        //#endif
    }
    else
    {
        driveRCmecanum();
    }

    for(int i = 0; i < 5; i++)
    {
        Serial.print(rcValue[i]);
        Serial.print("\t");
    }

    Serial.print(rightYmax);
    Serial.print("\t");
    Serial.print(rightXmax);
    Serial.print("\t");
    Serial.print(leftXmax);
    Serial.print("        ");
    
    Serial.print(rightYmin);
    Serial.print("\t");
    Serial.print(rightXmin);
    Serial.print("\t");
    Serial.print(leftXmin);
    Serial.print("        ");

    Serial.println();
}

// calibrate RC drive
void RobotClass::CalibrateRC()
{
    // chech for signal to start calibration
    if(Read_Serial()==1)
    {
        Calibrate1();
    }
}

void RobotClass::Tankdrive_v1()
{
    // map values from the radio
    int y = map(rcValue[0], rightYmax, rightYmin, TANK_REVERSE, TANK_FORWARD);
    int x = map(rcValue[1], rightXmax, rightXmin, TANK_FORWARD, TANK_REVERSE);

    y = maximum2(y);
    x = maximum2(x);

    if(tankdrive_debugging_printout)
    { 
        Serial.print(x);
        Serial.print("\t");
        Serial.print(y);
        Serial.print("\t");
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
    // Along a straight line, with an angle o, the turn co-efficient is same
    // this applies for angles between 0-90, with angle 0 the co-eff is -1
    // with angle 45, the co-efficient is 0 and with angle 90, it is 1
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

    if(tankdrive_debugging_printout)
    {
        Serial.print(left_speed);
        Serial.print("\t");
        Serial.print(right_speed);
        Serial.print("\t");
    }

    //left_speed = map(left_speed, TANK_FORWARD, TANK_REVERSE, 2000, 1000);
    //right_speed = map(right_speed, TANK_FORWARD, TANK_REVERSE, 2000, 1000);

    Front_Left.Move(left_speed);
    Rear_Left.Move(left_speed);
    Front_Right.Move(right_speed);
    Rear_Right.Move(right_speed);
}