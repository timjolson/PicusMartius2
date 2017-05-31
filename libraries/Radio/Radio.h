#ifndef RADIO_h
#define RADIO_h

#include <Arduino.h>
#include "Variables.h"

//todo: comments

class RadioClass{
	Stream* stream;		//stream for rx/tx
	char Name[8];		//name of radio
	uint8_t header, footer, textheader, cmdheader;	//unique bytes to send in tx depending on instructions
	//Data rxdata;	//place to keep last good rx data
	byte data[DATASIZEBYTE];//received control data
	char command[16];		//received command
	char text[16];			//received text
	uint16_t err;			//errors in rx or tx
	uint16_t timeout, sendTimer, printTimer;	//timeout in millis for disable
	uint32_t lastHeartBeat, lastSent, lastPrint;//time of last good rx
	bool haveData;		//have new data?
	bool haveCmd;		//have new command?
	bool haveText;		//have new text?
	
	public:
		void PrintData();
		
		void SetStream(Stream* _stream);		//set stream for rx/tx
		void SetHeader(uint8_t _header);		//set general header byte
		void SetCmdHeader(uint8_t _cmdheader);	//set command header byte
		void SetTextHeader(uint8_t _textheader);//set text header byte
		void SetFooter(uint8_t _footer);		//set footer byte
		void SetTimeout(int _millis);			//set timeout (millis)
		void SendCommand(const char* _sendcmd);	//send command char's over radio
		void SendData(bool* _sendData, uint8_t _size);		//send controller data over radio
		void SendText(const char* _sendtext);	//send text over radio
		void Receive();							//watches for rx, reads into data[]
		
		byte* GetData();
		bool HaveData();						//returns whether or not we have new data
		const char* GetCmd();					//returns received command
		bool HaveCmd();							//returns whether or not we have new command
		const char* GetText();					//returns received text
		bool HaveText();						//returns whether or not we have new text
		
		void PrintName(){Serial.print(Name);};
		const char* GetName(){return Name;};
		
		uint16_t Err(){return err;};	//returns error status
		bool Safe();					//returns 0 if timed out, 1 otherwise
		
		//constructor
		RadioClass(uint8_t _header,uint8_t _footer,uint8_t _textheader,uint8_t _cmdheader,Stream* _stream);
		RadioClass(){};
};


#endif