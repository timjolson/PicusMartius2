#include "Gyro.h"

Gyro::Gyro()
{
	Gyro(0x00);
}

Gyro::Gyro(char a)
{
	WHO_AM_I = a;
	_init();
	_lastupdate = micros();
}
float Gyro::getZAngle() {return _z; }
float Gyro::getAVelocity() {return _readZ(); }

void Gyro::reset() {
	reset(0,0,0);
}

void Gyro::reset(float x,float y,float z) {
	_x = x;
	_y = y;
	_z = z;
}

void Gyro::update()
{
	float x = _getAngleValue(_readX());
	float y = _getAngleValue(_readY());
	float z = _getAngleValue(_readZ());
	if (true) // test that the angles are reasonable based upon micros() - _lastupdate and magnitude
		  _x = x;
	if (true)
		  _y = y;
	if (true)
		  _z = z;
	_lastupdate = micros();
}

void Gyro::_init() {
	//gyro I2C
	Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_29_30, I2C_PULLUP_INT, I2C_RATE_400);
	char id=0;
	id = _itgRead(_itgAddress, 0x00);
	//Serial.print("ID: ");
	//Serial.println(id, HEX);
	_itgWrite(_itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));
	_itgWrite(_itgAddress, SMPLRT_DIV, 9);
}

float Gyro::_getAngleValue(int16_t val) {
	//float t = (float)val;
	return (((float)val * 2000) / 32767);
}

int Gyro::_readX() {
	int data=0;
	data = _itgRead(_itgAddress, GYRO_XOUT_H)<<8;
	data |= _itgRead(_itgAddress, GYRO_XOUT_L);

	return data;
}
int Gyro::_readY() {
	int data=0;
	data = _itgRead(_itgAddress, GYRO_YOUT_H)<<8;
	data |= _itgRead(_itgAddress, GYRO_YOUT_L);

	return data;
}
int Gyro::_readZ() {
	int data=0;
	data = _itgRead(_itgAddress, GYRO_ZOUT_H)<<8;
	data |= _itgRead(_itgAddress, GYRO_ZOUT_L);

	return data;
}

void Gyro::_itgWrite(char address, char registerAddress, char data) {
	Wire1.beginTransmission(address);
	Wire1.write(registerAddress);
	Wire1.write(data);
	Wire1.endTransmission();
}

unsigned char Gyro::_itgRead(char address, char registerAddress) {
	unsigned char data=0;
	Wire1.beginTransmission(address);
	Wire1.write(registerAddress);
	Wire1.endTransmission();
	Wire1.beginTransmission(address);
	Wire1.requestFrom(address, 1);
	if(Wire1.available()){
		data = Wire1.read();
	}
	Wire1.endTransmission();
	return data;
}

void Gyro::_estimateXYZ() {
	// -2000 to 2000 degrees/second 
	//reset(_x,_y,_z);
}
