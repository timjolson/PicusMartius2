#include <Arduino.h>
#include "Gyros.h"
#include <i2c_t3.h>

extern uint32_t CurrentTimeMicros;

int nullgyro[10] = {0};
const short _refresh = 0;
short _refresh_shift = 0;
bool wire_started;

//******** DEFINES
const char SMPLRT_DIV= 0x15;
const char DLPF_FS = 0x16;
const char GYRO_XOUT_H = 0x1D;
const char GYRO_XOUT_L = 0x1E;
const char GYRO_YOUT_H = 0x1F;
const char GYRO_YOUT_L = 0x20;
const char GYRO_ZOUT_H = 0x21;
const char GYRO_ZOUT_L = 0x22;


//This is a list of settings that can be loaded into the registers.
//DLPF, Full Scale Register Bits
//FS_SEL must be set to 3 for proper operation
//Set DLPF_CFG to 3 for 1kHz Fint and 42 Hz Low Pass Filter
/*
const char DLPF_CFG_0 = 1<<0;
const char DLPF_CFG_1 = 1<<1;
const char DLPF_CFG_2 = 1<<2;
*/
const char DLPF_CFG_0 = 0;
const char DLPF_CFG_1 = 0;
const char DLPF_CFG_2 = 0;

const char DLPF_FS_SEL_0 = 1<<3;
const char DLPF_FS_SEL_1 = 1<<4;

void GyroClass::SetAddr(char address)
{
    addr = address;
};

void GyroClass::Init(const char* _name, int _max_rate, int _min_rate)
{
    //PRINT=1;
    Calibrated=0;
    if (!wire_started){
        Wire.begin();
        wire_started = 1;
    }

    offset[0] = 0;
/*     offset[1] = 0;
 *     offset[2] = 0;
 */
    refresh = _refresh;
    refresh_shift = _refresh_shift;
    _refresh_shift += 2;

    char id = itgRead(0x00);
    
    if (PRINT){
        Serial.print("ID: ");
        Serial.println(id, HEX);
    }

    itgWrite(DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
    // 9 - Set the sample rate to 100 hz
    itgWrite(SMPLRT_DIV, 0);
    
	strcpy(Name,_name);			//display name for gyro
    
    max_rate = _max_rate;
    min_rate = _min_rate;
};

int* GyroClass::Operate()
{
    unsigned int cycleT = micros() - LastTime;
    
    if (!Calibrated){
        _calibrate(cycleT);
        return nullgyro;
    }
    else{
        //if (cycleT > refresh){
            LastTime = micros();
            Read();
            
            for (int i=0; i<1;i++){
                if (/*(abs(raw[i]) < 2)||*/(abs(raw[i]) > max_rate)) gyro[i] =0;
                else if (cycleT > cycleTavg[i][0]) gyro[i] = 0;
                else gyro[i] = raw[i];
                // +/- 2000 deg/sec
            }
            return gyro;
        //}
    }
}

void GyroClass::Read()
{
    raw[0] = 0;
    raw[0] = itgRead(GYRO_ZOUT_H)<<8;
    raw[0] |= itgRead(GYRO_ZOUT_L);
    raw[0] -= offset[0];
    
/*     raw[1]=0;
 *     raw[1] = itgRead(GYRO_XOUT_H)<<8;
 *     raw[1] |= itgRead(GYRO_XOUT_L);
 *     raw[1] -= offset[1];
 *     
 *     raw[2]=0;
 *     raw[2] = itgRead(GYRO_YOUT_H)<<8;
 *     raw[2] |= itgRead(GYRO_YOUT_L);
 *     raw[2] -= offset[2];
 */
}

short GyroClass::GetRawInput(int axis)
{
    if ((axis<3)&&(axis>=0)) 
        return raw[axis];
    else{
        if (PRINT) Serial.println("Gyro axis error");
        return 0;
    }
};

void GyroClass::itgWrite(char registerAddress, char data)
{
  //Initiate a communication sequence with the desired i2c device
  Wire.beginTransmission(addr);
  //Tell the I2C address which register we are writing to
  Wire.write(registerAddress);
  //Send the value to write to the specified register
  Wire.write(data);
  //End the communication sequence
  Wire.endTransmission(I2C_STOP, 200);
}


unsigned char GyroClass::itgRead(char registerAddress)
{
  //This variable will hold the contents read from the i2c device.
  unsigned char data=0;

  //Send the register address to be read.
  Wire.beginTransmission(addr);
  //Send the Register Address
  Wire.write(registerAddress);
  //End the communication sequence.
  Wire.endTransmission(I2C_STOP, 200);

  //Ask the I2C device for data
  Wire.beginTransmission(addr);
  Wire.requestFrom(addr, 1, I2C_STOP, 200);

  //Wait for a response from the I2C device
  if(Wire.available()){
    //Save the data sent from the I2C device
    data = Wire.read();
  }

  //End the communication sequence.
  Wire.endTransmission(I2C_STOP, 200);

  //Return the data read during the operation
  return data;
}

void GyroClass::_calibrate(unsigned int cycleT)
{
    for (int i=0;i<1;i++)
    {
        raw[i]=0;
        _cyclenum[i]++;
        
        Read();
        
        cycleAvg[i][_cyclenum[i]]=raw[i];
        cycleTavg[i][_cyclenum[i]]=cycleT;
        
        if (_cyclenum[i] == 100){
            double sum=0;
            double Tsum=0;
            while (_cyclenum[i]>1){
                sum+=(cycleAvg[i][_cyclenum[i]]+cycleAvg[i][_cyclenum[i]-1])/2;
                Tsum+=(cycleTavg[i][_cyclenum[i]]+cycleTavg[i][_cyclenum[i]-1])/2;
                _cyclenum[i]-=2;
            }
            
            sum/=50;
            Tsum/=50;
            if (abs(sum) > min_rate){
                Serial.print("Gyro ");
                Serial.print(Name);
                Serial.println(" Cal Error:: ");
                Serial.print("Axis ");
                Serial.print(i);
                Serial.print(" Avg: ");
                Serial.println(sum);
                offset[i]=0;
                Calibrated=0;
            }
            else{
                offset[i] = sum;
                Calibrated=1;
                cycleAvg[i][0]=sum;
                sum=0;
                _cyclenum[i]=0;
            }
            
            cycleTavg[i][0]=1.2*Tsum;
            Tsum=0;
        }
    }
}

/*
short cosDir(short indir, short adjust, short norm, byte db)
{
      float output=0;
      output=indir-adjust;      //add adjustment angle
      if (abs(output)>(1800)) output-=3600 * (output/abs(output));  // +/- 180*
      output=cos(output*PI/1800);       //get cos of new angle
      output *= norm;                   //normalize
      if (abs(output)>norm) output-= norm * (output/abs(output));   //limit
      if (abs(output)<db) output = 0;        //deadband

      return output;
}

short sinDir(short indir, short adjust, short norm, byte db)
{
      float output=0;
      output=indir-adjust;      //add adjustment angle
      if (abs(output)>(1800)) output-=3600 * (output/abs(output));  // +/- 180*
      output=sin(output*PI/1800);       //get sin of new angle
      output *= norm;                   //normalize
      if (abs(output)>norm) output-= norm * (output/abs(output));   //limit
      if (abs(output)<db) output = 0;        //deadband

      return output;
}
*/