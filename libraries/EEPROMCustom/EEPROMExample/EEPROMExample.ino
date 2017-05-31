#include <Arduino.h>
#include <EEPROM.h>
#include "EEPROMCustom.h"

/* Struct can be inside a header */
struct ExampleStruct
{
	/* include whatever you need */
    short _short;
	float _float;
    
    /* loc is required */
	uint16_t loc;   //location in EEPROM
};

/* Struct of default values */
ExampleStruct Estr = { 
    10,
    1.0,
    
    /* loc can be initialized to anything, as long as structure is
    eeprom initialized before use. Omitting value results in loc = 0 */
    /*0,*/
};

/* Struct of alternate values */
ExampleStruct Estr2 = { 2, 2.0, };

/* Place holding/actively used struct to load EEPROM data into */
ExampleStruct RefStr;

/* Function to print an ExampleStruct */
void Print(struct ExampleStruct s){
    Serial.println("Structure contains:");
    Serial.println(s._short);
    Serial.println(s._float);
    
    Serial.println("Location determined by previous EEPROM structs:");
    Serial.println(s.loc);
}

void setup(){
    // Enable In/Output
    Serial.begin(115200);
    
    // Initialize EEPROMCustom Library
    SetEEPROMStream(&Serial);
    
    // Print struct before loading from EEPROM
    Serial.println("/**** Before Init ****/");
    Print(RefStr);
    
    // Load EEPROM data into RefStr
    EEPROMinit(&RefStr);
    
    // Print struct after loading from EEPROM
    Serial.println("/**** After Init ****/");
    Print(RefStr);
}

void loop(){
    
    // If keyboard input
    if (Serial.available()){
        // Get the character
        char input = Serial.read();
        
        if (toupper(input) == 'S'){     // If 's' or 'S'
            /* Save struct data to EEPROM */
            /* Uses Update method to preserve cell life */
            EEPROMwrite(RefStr);
            Serial.println("Structs Saved");
        }
        else if (input == 'L'){         // If 'L'
            /* Load values into struct */
            int back = RefStr.loc;      // backup EEPROM location
            RefStr = Estr;              // set values (EEPROM location included)
            RefStr.loc = back;          // reset EEPROM location
            Serial.println("\n\nStruct Loaded:");
            Print(RefStr);
        }
        else if (input == 'l'){
            /* Load values into struct */
            int back = RefStr.loc;
            RefStr = Estr2;
            RefStr.loc = back;
            Serial.println("\n\nStruct Loaded:");
            Print(RefStr);
        }
        else if (toupper(input) == 'R'){         // If 'r' or 'R'
            /* Re-load struct from EEPROM */
            EEPROMread(&RefStr);
            Serial.println("\n\nStruct RE-Loaded:");
            Print(RefStr);
        }
        else    //junk otherwise
            Serial.read();
    }
}
