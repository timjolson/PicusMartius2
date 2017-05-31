#include <Arduino.h>
#include "Button.h"

uint32_t CurrentTime;
//                          pin, default reading, name
ButtonClass b1 = ButtonClass(2,         0,          "A");
ButtonClass b2 = ButtonClass(3,         0,          "B");

void setup(){
    
    Serial.begin(115200);
    b2.AutoNorm();
}

void onebuttonloop(){
    
    CurrentTime = millis();
    
    Serial.print("Changed, Hold, Released, Switch, Held_ms(1), Held_ms(2), Press\n   ");
    Serial.print(b1.Changed());
    Serial.print("      ");
    Serial.print(b1.checkHold());
    Serial.print("      ");
    Serial.print(b1.Released());
    Serial.print("         ");
    Serial.print(b1.checkSwitch());
    Serial.print("         ");
    Serial.print(b1.Held(1000));
    Serial.print("         ");
    Serial.print((b1.Pressed()>2000)?1:0);
    Serial.print("         ");
    Serial.print(b1.Pressed());
    Serial.print("\n\n");
    
    delay(200);
}


void loop(){
    
    CurrentTime = millis();
    
    Serial.print("Changed, Hold, Released, Switch, Held_ms(1), Held_ms(2), Press\n   ");
    b1.Changed();
    Serial.print(b2.Changed());
    Serial.print("      ");
    b1.checkHold();
    Serial.print(b2.checkHold());
    Serial.print("      ");
    b1.Released();
    Serial.print(b2.Released());
    Serial.print("         ");
    b1.checkSwitch();
    Serial.print(b2.checkSwitch());
    Serial.print("         ");
    b1.Held(1000);
    Serial.print(b2.Held(1000));
    Serial.print("         ");
    b1.Pressed();
    Serial.print((b2.Pressed()>2000)?1:0);
    Serial.print("         ");
    b1.Pressed();
    Serial.print(b2.Pressed());
    Serial.print("\n\n");
    
    delay(200);
}