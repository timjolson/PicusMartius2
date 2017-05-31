#ifndef customEEPROM_H
#define customEEPROM_H

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROMDEBUG 0		//print back eeprom debug data
extern uint16_t _EEPROMaddress;		//current address to write to
extern Stream* EEPROMstream;		//output stream for feedback
extern bool EEPROMinited;
/*
EEPROM structure reading and writing library.
Structs to be saved MUST HAVE loc integer (holds location for memory access).

To use, the structs must be created in the project before inclusion of this header,
then added into EEPROMinit()

To make Arduino preserve EEPROM on upload cycle, see www.engbedded.com/fusecalc.
'Fuses' must be set in avr/boards.txt
CAUTION: Fuses, boards.txt, etc. modifications are hazardous

*/

void SetEEPROMStream(Stream* _ostream);//set where to print EEPROM feedback

template <typename T>				//can put in any struct
uint16_t EEPROMinit( T* _toGet ){	//initially read struct in from EEPROM
	if (!EEPROMinited)
    {
        #if EEPROMDEBUG
        //EEPROMstream->println("EEPROM not initialized");
        Serial.println("EEPROM not initialized");
        #endif
        return 0;
    }
    else
    {
        T tempEEPROM;							//place to store data
        EEPROM.get(_EEPROMaddress, tempEEPROM);		//get the data
        memcpy (_toGet, &tempEEPROM, sizeof(tempEEPROM));	//copy to argument location
        _toGet->loc = _EEPROMaddress;			//repair address in argument struct (must be done)
        _EEPROMaddress += sizeof(T);			//update to next read address
        #if EEPROMDEBUG
        EEPROMstream->print("Next Init Addr: ");
        EEPROMstream->println(_EEPROMaddress);
        //Serial.print("Next Init Addr: ");
        //Serial.println(_EEPROMaddress);
        #endif
        return _EEPROMaddress;					//return
    }
}

template <typename T>				//can put in any struct
uint16_t EEPROMread( T* _toGet ){	//initially read struct in from EEPROM
	if (!EEPROMinited)
    {
        #if EEPROMDEBUG
        //EEPROMstream->println("EEPROM not initialized");
        Serial.println("EEPROM not initialized");
        #endif
    }
    else
    {
        if (_toGet->loc + sizeof(_toGet) > EEPROM.length())
        {
            #if EEPROMDEBUG
            EEPROMstream->println("Attempted reading outside EEPROM range");
            //Serial.println("Attempted reading outside EEPROM range");
            #endif
        }
        else
        {
            T temp;							//place to store data
            uint16_t _return = _toGet->loc;		//will return address of struct just initialized
            EEPROM.get(_toGet->loc, temp);		//get the data
            memcpy (_toGet, &temp, sizeof(temp));//copy to argument location
            _toGet->loc = _return;			    //repair address in argument struct (must be done)
            #if EEPROMDEBUG
            EEPROMstream->print("Read Addr: ");
            EEPROMstream->println(_toGet->loc);
            //Serial.print("Read Addr: ");
            //Serial.println(_toGet->loc);
            #endif
            return _return;					//return
        }
    }
    return 0;
}

template <typename S>			//lets us put in whatever we want
bool EEPROMwrite( S _toWrite){		//write struct to EEPROM
    if (!EEPROMinited)
    {
        #if EEPROMDEBUG
        //EEPROMstream->println("EEPROM not initialized");
        Serial.println("EEPROM not initialized");
        #endif
        return 0;
    }
    else
    {
        if (_toWrite.loc + sizeof(_toWrite) > EEPROM.length())
        {
            #if EEPROMDEBUG
            EEPROMstream->println("EEPROM out of space");
            //Serial.println("EEPROM out of space");
            #endif
            return 0;
        }
        else
        {
            //write data (uses update so only writes changes)
            EEPROM.put(_toWrite.loc, _toWrite);	//write the input starting at the struct's loc (its address)
            //read back from eeprom
            S tempEEPROM;							//place to store whats read
            EEPROM.get(_toWrite.loc, tempEEPROM);	//read what was written

            //check if update worked
            //if what was written and read back does not match what was supposed to be written, error
            if (memcmp(&tempEEPROM,&_toWrite,sizeof(_toWrite))!=0)
            {
                #if EEPROMDEBUG
                EEPROMstream->println("EEPROM updating problem");
                //Serial.println("EEPROM updating problem");
                #endif
                return 0;
            }
            else{
                #if EEPROMDEBUG
                EEPROMstream->println("EEPROM updated");
                //Serial.println("EEPROM updated");
                #endif
                return 1;
            }
        }
    }
}

uint16_t EEPROMstart();
uint16_t EEPROMnext();
uint16_t EEPROMrestart();

#endif