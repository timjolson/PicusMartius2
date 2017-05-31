#ifndef GYRO_H
#define GYRO_H

#include "i2c_t3.h"

class Gyro
{
public:
	Gyro();
	Gyro(char);
	float getZAngle();
	float getAVelocity();
	void reset();
	void reset(float,float,float);
	void update();

private:
	void _init();
	float _getAngleValue(int16_t val);
	int _readX();
	int _readY();
	int _readZ();
	void _itgWrite(char address, char registerAddress, char data);
	unsigned char _itgRead(char address, char registerAddress);
	void _estimateXYZ();

	char WHO_AM_I;  // I2C Address
	float _xRate;	// retrieved rates of change
	float _yRate;
	float _zRate;
	float _x;
	float _y;
	float _z;
	uint32_t _lastupdate;
	
	const char SMPLRT_DIV= 0x15;	 //
	const char DLPF_FS = 0x16;		//
	const char GYRO_XOUT_H = 0x1D;  //
	const char GYRO_XOUT_L = 0x1E;  //
	const char GYRO_YOUT_H = 0x1F;  //
	const char GYRO_YOUT_L = 0x20;  //
	const char GYRO_ZOUT_H = 0x21;  //
	const char GYRO_ZOUT_L = 0x22;  //
	const char DLPF_CFG_0 = 1<<0;	//
	const char DLPF_CFG_1 = 1<<1;	//
	const char DLPF_CFG_2 = 1<<2;	//
	const char DLPF_FS_SEL_0 = 1<<3;//
	const char DLPF_FS_SEL_1 = 1<<4;//
	const char _itgAddress = 0x69;	//
};

#endif
