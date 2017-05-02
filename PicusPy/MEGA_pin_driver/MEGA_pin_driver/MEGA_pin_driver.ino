char* buf;
int pinNum;
int val;
int pinNums[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24, 27, 28, 31};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(10);

  for (int i=0; i<16; i++)
  {
    pinMode(pinNums[i],1);
    digitalWrite(pinNums[i],LOW);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()){
    Serial.readBytes(buf, 1);
    if (buf[0]=='&'){
//      Serial.println("1&1");
      Serial.readBytes(buf,1);
      if (buf[0]=='&'){
//        Serial.println("2&2");
        pinNum = Serial.parseInt();
//pinNum = Serial.read();
//val = Serial.read();
        val = Serial.parseInt();
        switch (pinNum){
          case 2: case 3:
          case 5: case 6:
          case 7: case 8:
          case 9: case 10:
          case 11: case 12:
            analogWrite(pinNum, val);
          break;
          case 24: case 27:
          case 28: case 31:
            digitalWrite(pinNum, int(val/250));
          break;
          default:
          break;
        }
//        Serial.print(pinNum);
//        Serial.print('!');
//        Serial.print(val);
      } // end if &
    } // end if &
  }
}

