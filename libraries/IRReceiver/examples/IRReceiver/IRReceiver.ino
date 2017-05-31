/*
  FILE: IRReceiver.ino
  AUTHORS: Steven Brice
  Description: Print readings from an IR Receiver / Remote
*/
#include <IRremote.h>

#define IR_PIN 5

IRrecv irrecv(IR_PIN);
IRsend irsend;

decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if(irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
}
