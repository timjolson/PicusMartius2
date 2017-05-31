/*******

This sketch allows you to cycle through PWM expander pins. Entering '+' into the serial monitor will turn the previous pin off
and the next pin on. The currently ON pin number will print to the monitor.


*******/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

int pinNum = 15;

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

void setup() {
  Serial.begin(115200);
  Serial.println("16 channel PWM zero!");

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  
  pwm.begin();
  pwm.setPWMFreq(1000);  // This is the maximum PWM frequency
    
  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!

  // for every pwm pin
  // write the output to zero
    
    
    for (int i=0; i<16; i++){
      pwm.setPWM(i, 0, 0);
    }
    
}

void loop() {
    if (millis()%2000==0){
        Serial.println("PWM Expander Running");
    }
    delay(1);
    
    if (Serial.available())
    {
      if (Serial.peek() == '+')
      {
        Serial.read();
        pwm.setPWM(pinNum,0,0);
        pinNum ++;
        if (pinNum>15)
          pinNum=0;
        Serial.println(pinNum);
        pwm.setPWM(pinNum,0,2000);
      }
    }
    
}
