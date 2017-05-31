#include <LiquidCrystalFast.h>
#include <FlexCAN.h>

// initialize the library with the numbers of the interface pins
LiquidCrystalFast lcd( 6,  7,  8, 9, 10,11,12);
         // LCD pins: RS  RW  EN  D4 D5 D6 D7
int led = 13;

//creat CANbus
FlexCAN CANbus(500000);
static CAN_message_t msg,rxmsg;
static uint8_t hex[17] = "0123456789abcdef";
int txCount,rxCount;
unsigned int txTimer,rxTimer;

//mask for CANbus filtering
CAN_filter_t deviceMask{ 0, 0, 48 };

int c =0;
bool line=0;


// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16,2);
  pinMode(led, OUTPUT); 
	
	//start CANbus, filtering out things that do not match deviceMask
  CANbus.begin(deviceMask);
	
  lcd.setCursor(0,0);
     lcd.print("waiting");
} 

void loop(void)
{
//variable to store message in
  CAN_message_t message;
  
	if (CANbus.available())
	{
		CANbus.read(message);

		lcd.setCursor(0,0);
		for (int i=0 ; i<8; i++)
		{
			bool num=0;
			if (isdigit(message.buf[i] )){ message.buf[i] -= '0';num=1;}
			lcd.setCursor(c,line);
			if (num){ lcd.print(message.buf[i]); c++;}
			else { lcd.print((char)message.buf[i]); c++;}
			line = (c>15)?!line:line;
			c = (c>15)?0:c;
		}
	lcd.setCursor(0,0);
	}//end if available
}//end loop
