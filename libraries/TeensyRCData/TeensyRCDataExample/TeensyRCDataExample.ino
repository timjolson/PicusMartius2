/*
This is an example driver/test program for the TeensyRCData library
*/

#include "TeensyRCData.h"

int pins[8] = {1, 2, 3, 4, 5, 6, 7, 8};

void setup(){
  Serial.begin(9600);
  
  setupRCData(pins);
}

void loop(){
  
  for(int i = 0; i < 8; i++)
  {
    Serial.print(rcValue[i]);
    Serial.print("    ");
  }
  
  Serial.println();
  
  delay(100);
}
