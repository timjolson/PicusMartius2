#include <Arduino.h>
#include <EEPROM.h>

#include "EEPROMCustom.h"
#define EEPROMOFFSET 20	//where to start reading/writing
uint16_t _EEPROMaddress;	//current address to write to
bool EEPROMinited = 0;

Stream* EEPROMstream;			//output stream for feedback

//eg: SetEEPROMStream(&Serial1);
void SetEEPROMStream(Stream* _ostream){
    if (!EEPROMinited){
        EEPROMstream = _ostream;		//set where to print EEPROM feedback
        _EEPROMaddress = EEPROMOFFSET;		//start at predefined address
        
        EEPROMinited = 1;
        
        #if EEPROMDEBUG
        EEPROMstream->println(_EEPROMaddress);
        Serial.println(_EEPROMaddress);
        #endif
    }
}

uint16_t EEPROMstart(){ return EEPROMOFFSET; };
uint16_t EEPROMnext(){ return EEPROMinited?_EEPROMaddress:EEPROMOFFSET; };
uint16_t EEPROMrestart(){ _EEPROMaddress = EEPROMOFFSET; return _EEPROMaddress; };