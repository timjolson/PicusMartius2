#ifndef MOUSE_H
#define MOUSE_H

#include <SPI.h>
#include <avr/pgmspace.h>
#include <avr/io.h> // for interrupts
#include <avr/interrupt.h>

class Mouse
{
public:
	Mouse();
	int getX();
	int getY();
	void update(); // re-used void loop();
	void reset();  // made a public function from original
	void calibrate();
	void InterruptMOT();
private:
	void setup();
	void adns_com_begin();
	void adns_com_end();
	byte adns_read_reg(byte);
	void adns_write_reg(byte,byte);
	void adns_upload_firmware();
	void performStartup();
	void UpdatePointer();
	void dispRegisters();
	int convTwosComp(int);
	void writeMice();
	uint8_t calculateParity(uint8_t);
	void setupAlternateSPI();
	
	int _dx;
	int _dy;
	bool _position;
};

#endif
