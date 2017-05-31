#include <Arduino.h>
#include "IMU.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"
#include "Variables.h"

IMUClass IMU;

static const short _refresh = 10;
static short _refresh_shift = 0;
static bool wire_started = 0;

void IMUClass::Init(const char* _name)
{
    //PRINT=1;
    Calibrated=0;
    if (bno.begin()){
        Serial.println("IMU Started");
        wire_started = 1;
        //delay(1000);
        bno.setExtCrystalUse(true);
        
        displaySensorDetails();
    }
    else
    {
        Serial.println("NO IMU Comms");
    }
    
    //offset I2C readings to free up system
    refresh = _refresh;
    refresh_shift = _refresh_shift;
    _refresh_shift += 2;

	strcpy(Name,_name);			//display name for IMU
};

void IMUClass::Operate()
{
    if (Calibrated) Read(); //read data
    else _calibrate();      //calibrate
}

void IMUClass::Read()
{
    //apply refresh rate
    if ( CurrentTime - refresh_shift > LastTime )
    {
        LastTime = CurrentTime;
        
        sensors_event_t event;
        bno.getEvent(&event);
        
        raw[0] = event.orientation.x;   //set axes
        raw[1] = event.orientation.y;
        raw[2] = event.orientation.z;
    }
}

float IMUClass::GetAxis(int axis)
{
    if ((axis<3)&&(axis>=0))    //error check request
        return raw[axis];
    else{
        if (PRINT) Serial.println("Gyro axis error"); //out of range
        return 0.0;
    }
};

void IMUClass::_calibrate()
{
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);
    
    Calibrated = !!accel;
    
}

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void IMUClass::displaySensorDetails()
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
*/
/**************************************************************************/
void IMUClass::displaySensorStatus(void)
{
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void IMUClass::displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}