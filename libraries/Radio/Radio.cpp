#include "Radio.h"
#include <Arduino.h>

#define RADIODEBUG 1

//todo: comments
#define HEADER B01010101
#define FOOTER B10110100
#define TEXTHEADER B11010011
#define CMDHEADER B10011001

RadioClass XBEE=RadioClass(HEADER,FOOTER,TEXTHEADER,CMDHEADER,&Serial1);

extern uint32_t CurrentTime;	//gets updated at beginning of every cycle with millis()

void RadioClass::SetStream(Stream* _stream)
{
	stream = _stream;	//set stream for rx/tx
	SendText("Radio Stream Test");
};//end SetStream()

bool RadioClass::HaveData(){
	if (haveData)
		return DATASIZEBYTE;
	return 0;
};
bool RadioClass::HaveCmd(){return haveCmd;};
bool RadioClass::HaveText(){return haveText;};
byte* RadioClass::GetData(){
	if (haveData){
		haveData =0;	//no longer new data
		return data;
	}
	return 0;
};//end GetData()
const char* RadioClass::GetCmd(){
	if (haveCmd){
		haveCmd =0;	// no longer new command
		return command;
	}
	return '\0';
};//end GetCmd()
const char* RadioClass::GetText(){
	if (haveText){
		haveText =0;//no longer new text
		return text;
	}
	return '\0';
};//end GetText

void RadioClass::SendData(bool* _sendData, uint8_t _size)
{
	if (CurrentTime - lastSent >= sendTimer)
	{
		#if RADIODEBUG
		if (CurrentTime - lastPrint >= printTimer)
		{
			Serial.println("SENDING::");
			for (int i =0; i < DATASIZEBYTE; i++)
			{
				Serial.print(_sendData[i]);
			}
		}
		lastPrint = CurrentTime;
		#endif
		
		//error check of size
		//if (_size <= NUMBUTTONS+NUMSWITCHES+NUMKNOBS*8+NUMSTICKS*10)
		if (_size <= sizeof(data))	//compare to size of storage
		{
			//add header
			stream->write(header);
			//add header
			stream->write(header);
			//add size
			stream->write(_size);
			//add data
			byte* d = (byte*)_sendData;
			for (int i = 0; i < _size; i++)
			{
				stream->write(d[i]);
			}	
			//add footer
			stream->write(footer);
		}
		else
		{
			//todo: send data size error
			err|= B00000100;
		}
		lastSent = CurrentTime;
	}//end sendTimer
};//end SendData

void RadioClass::SendText(const char* _sendtext)
{
	if (sizeof(_sendtext) <= 16)	//length of lcd screen row
	{	
		//add header
		stream->write(header);
		//add text header
		stream->write(textheader);
		//add text
		stream->write(_sendtext);
		//Serial.println(_sendtext);
		//new line
		//stream->write("\n");
	}
	else
	{
		err|= B00000010;
		//todo: send text size error
	}
};//end SendText

void RadioClass::SendCommand(const char* _sendcmd)
{
	uint8_t size = sizeof(_sendcmd);
	//error check size
	if (size <= (16))	//commands should not be this big anyway
	{
		//add header
		stream->write(header);
		//add command header
		stream->write(cmdheader);
		//add command char's
		for (int i=0; i< size; i++)
		{
			stream->write(_sendcmd[i]);
		}
		//add footer
		stream->write(footer);
	}
	else
	{
		err|=B00000001;
		//todo: send command size error
	}
};//end SendCommand

void RadioClass::Receive()
{
	byte rx[DATASIZEBYTE]={0};	//place to read headers/footers //todo: change #
	
	//read stream
	if (stream->available() > (int)(2*sizeof(header)))	//data available could at least contain two headers
	{
		stream->readBytes(&rx[0], 1);
		//look for first header
		if (rx[0] == header)
		{
			stream->readBytes(&rx[0], 1);
			//look for text header
			if (rx[0] == textheader)
			{
				haveText =1;
				memset(text,0,sizeof(text));
				//print what comes in
				stream->readBytes(text,(int)stream->available());	//get received text
				
				#if RADIODEBUG
				Serial.println("--- TEXT RECEIVED ---");
				Serial.print(text);
				Serial.println("\n--- END RECEIVED ---");
				#endif
			}//end text printing
			
			//look for command header
			if (rx[0] == cmdheader)
			{
				haveCmd =1;
	  			memset(command,0,sizeof(command));
				stream->readBytes(command,(int)stream->available());
				#if RADIODEBUG
				Serial.println("--- CMD RECEIVED ---");
				Serial.print(command);
				Serial.println("\n--- END RECEIVED ---");
				#endif
				//todo:debug
				
			}//end command receive
			
			//look for normal header again
			else if (rx[0] == header)
			{
				//how much data are we getting?
				uint8_t size =0;	
				stream->readBytes(&size, 1);	//get size of transmission
				
				//will transmission fit in rxdata?
				if (size <= sizeof(rx))
				{
					
					//pull until we reach size of transmission
					for (int i = 0; i <= size; i++)//include all data bytes plus footer byte
					{
						stream->readBytes(&rx[i], 1);
					}//end pulling
					
					//check for footer
					if (rx[size] != footer)	//no footer
					{
						//todo: set error
						err|= B00001000;
						Serial.println("Last Byte Does Not Match Footer");
						Serial.println("----- RX Data -----");
						for (int i=0; i<size; i++)
							//print received 'footer'
							Serial.print(rx[i]);
						stream->println("----- RX Data -----");
						
					}//end no footer
					else	//yes, footer
					{						
						for (int i=0; i < size; i++ )
						{
							data[i]=rx[i];	//copy to data[] for use
						}
						
						#if RADIODEBUG
						if (CurrentTime - lastPrint >= printTimer)
						{
							Serial.println("RECEIVING::");
							for (int i =0; i < DATASIZEBYTE; i++)
							{
								Serial.print(data[i]);
							}
						}
						lastPrint = CurrentTime;
						#endif
						
						haveData =1;
						lastHeartBeat = CurrentTime;
						err = 0;
					}//end successful receive
				}//end size error check
				else	//size will not fit rxdata[]
				{
					//todo: set error
					err|=B00010000;
					//-receive size error
				}//end failed size check
			}//end second header
			else	//no second header
			{
				//todo: set error
				err|=B00100000;
				//-no second header error
			}//end failed second header
		}//end first header
		else //no first header
		{
			if (stream->peek() != header)	//we'll junk everything until we find a header
			{
				stream->readBytes(&rx[0],1);	//junk peeked byte
			}
			//todo: set error
			err|=B01000000;
			//-no first header found
		}//end failed first header
	}//end stream.available()
	
};//end Receive()

bool RadioClass::Safe()
{
	//if timeout has been reached since last successful radio contact
	if (CurrentTime - lastHeartBeat > timeout)
	{
		haveData =0;
		err |= B10000000;
		return 0;	//fails safety
	}
	return 1;		//passes safety
};//end Safe()

void RadioClass::SetTimeout(int _millis)
{
	timeout = abs(_millis);	//ha! no negative numbers
};//end SetTimeout

RadioClass::RadioClass(uint8_t _header,uint8_t _footer,uint8_t _textheader,uint8_t _cmdheader, Stream* _stream)
{
	header = _header;
	footer = _footer;
	textheader = _textheader;
	cmdheader = _cmdheader;
	stream = _stream;
	sendTimer = 140;
	printTimer = 800;
	lastHeartBeat = 0;			//set lastHeartBeat so not junk data
	timeout = 250;				//250 millis between rx before controller disables
	err =0;						//start with no error
};//end Constructor