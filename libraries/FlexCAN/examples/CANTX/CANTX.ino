#include <FlexCAN.h>
#include <Arduino.h>
//#include "CAN_include.h"
#include <Metro.h>

#define PRINTRX 1
#define PRINTTX 1

CAN_filter_t deviceMask{ 0, 0, 48 };

Metro sysTimer = Metro(1);// milliseconds

char txchar[10]={0};

FlexCAN CANbus(500000);
static CAN_message_t txmsg,rxmsg;
static uint8_t hex[17] = "0123456789abcdef";

int txCount,rxCount;
unsigned int txTimer,rxTimer;

int led = 13;

// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
}

void CAN_send(void);
bool CAN_check(void);
void CAN_init(CAN_filter_t &);


void setup()
{
	Serial.begin(9600);
	
	CAN_init( deviceMask /*mask*/);
	
	Serial.println("ready");

}

void loop()
{
	//find out if we have a message waiting
	bool CAN_avail = CAN_check(); //run CAN_check() every loop, it runs the CAN timers
	
	if (CAN_avail)
	{
		//print whole message on Serial
		Serial.println(rxmsg.id);
		Serial.write(rxmsg.buf[0]);Serial.write('\n');
		Serial.write(rxmsg.buf[1]);Serial.write('\n');
		Serial.write(rxmsg.buf[2]);Serial.write('\n');
		Serial.write(rxmsg.buf[3]);Serial.write('\n');
		Serial.write(rxmsg.buf[4]);Serial.write('\n');
		Serial.write(rxmsg.buf[5]);Serial.write('\n');
		Serial.write(rxmsg.buf[6]);Serial.write('\n');
		Serial.write(rxmsg.buf[7]);Serial.write('\n');
		Serial.println("end buffers\n");
	}
	
	txCount=0;
	//if we get something from Serial, write it to a message
	while (Serial.available()&&txCount<10)
	{
		Serial.readBytes( &txchar[txCount] , 1);
		txCount++;
		//Serial.print("c");//Serial.println(txchar[txCount - 1]);
	}
	while (Serial.available())Serial.read();
	
	if (txchar[0] !=0){
		
	for (int i =0; i<8; i++)
	{
		txmsg.buf[i]=(txchar[i]==0)?32:txchar[i];
		txchar[i]=0;
	}
}

	CAN_send();

}

// -------------------------------------------------------------
/********
opens CANbus for use, only receiving messages matching mask

********/
void CAN_init(CAN_filter_t &mask)
{
  CANbus.begin(mask);
  //pinMode(led, OUTPUT);
  //digitalWrite(led, 1);
  
  delay(1000);
  sysTimer.reset();
}


// -------------------------------------------------------------
/********
runs CAN timers, reads into rxmsg any received messages

optional: print message id and all received buffers

********/
bool CAN_check(void)
{
  // service software timers based on Metro tick
  if ( sysTimer.check() ) {
    if ( txTimer ) {
      --txTimer;
    }
    if ( rxTimer ) {
      --rxTimer;
    }
  }


  #ifdef PRINTRX
  // if not time-delayed, read CAN messages and print 1st byte
  if ( !rxTimer ) {
    while ( CANbus.read(rxmsg) ) {
      //hexDump( sizeof(rxmsg), (uint8_t *)&rxmsg );
      	Serial.write(rxmsg.id);Serial.write('\n');
        Serial.write(rxmsg.buf[0]);Serial.write('\n');
        Serial.write(rxmsg.buf[1]);Serial.write('\n');
        Serial.write(rxmsg.buf[2]);Serial.write('\n');
        Serial.write(rxmsg.buf[3]);Serial.write('\n');
        Serial.write(rxmsg.buf[4]);Serial.write('\n');
        Serial.write(rxmsg.buf[5]);Serial.write('\n');
        Serial.write(rxmsg.buf[6]);Serial.write('\n');
        Serial.write(rxmsg.buf[7]);Serial.write('\n');
        Serial.println("end received buffers");Serial.write('\n');
    }
  }
  #endif //end ifdef PRINTRX

  return (rxmsg.buf[0] != 0);

}//end CAN_check


// -------------------------------------------------------------
/********
delays transmit time, writes txmsg to CANbus

optional: print message id and all transmitted buffers

********/
void CAN_send(void)
{
  // insert a time delay between transmissions
  if ( !txTimer ) {
    
    txTimer = 100;//milliseconds
    txmsg.len = 8;
    txmsg.id = 0x222;//0x222

/******************/
//write txmsg to CANbus if buf[0] has data
  if (txmsg.buf[0] != 0)
  {
  	CANbus.write(txmsg);
    #ifdef PRINTTX
    //print buffers
    digitalWrite(led, 1);
    Serial.write(txmsg.id);Serial.write('\n');
    Serial.write(txmsg.buf[0]);Serial.write('\n');
    Serial.write(txmsg.buf[1]);Serial.write('\n');
    Serial.write(txmsg.buf[2]);Serial.write('\n');
    Serial.write(txmsg.buf[3]);Serial.write('\n');
    Serial.write(txmsg.buf[4]);Serial.write('\n');
    Serial.write(txmsg.buf[5]);Serial.write('\n');
    Serial.write(txmsg.buf[6]);Serial.write('\n');
    Serial.write(txmsg.buf[7]);Serial.write('\n');
    Serial.println("end transmitted buffers");Serial.write('\n');
  #endif //end ifdef PRINTTX
  txmsg.buf[0]=0;
  }
/******************/


    //digitalWrite(led, 0);
    // time delay to force some rx data queue use
    rxTimer = 3;//milliseconds
  }

}