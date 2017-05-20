char* buf;
int pinNum;
int val;
int pinNums[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 24, 27, 28, 31};
int potPins[] = {0, 2, 4, 6, 1, 3};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(10);

  for (int i=0; i<16; i++)
  {
    pinMode(pinNums[i],OUTPUT);
    digitalWrite(pinNums[i],LOW);
  }

  for (int i=0; i<6; i++){
    pinMode(potPins[i], INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()){
    Serial.readBytes(buf, 1);
    if (buf[0]=='&'){
      Serial.readBytes(buf,1);
      if (buf[0]=='&'){
        pinNum = Serial.parseInt();
        val = Serial.parseInt();
        switch (pinNum){
          case 2: case 3:
          case 5: case 6:
          case 7: case 8:
          case 9: case 10:
          case 11: case 12:
          case 33: case 35:
          case 37: case 39:
          case 41: case 43:
            analogWrite(pinNum, val);
          break;
          case 24: case 27:
          case 28: case 31:
            digitalWrite(pinNum, int(val/250));
          break;
          default:
          break;
        }
      } // end if &
      else if (buf[0]=='$'){
        int val[] = {0,0,0,0,0,0,0,0,0,0};
        for (int i = 0; i<6; i++){
          val[i] = analogRead(potPins[i]);
        }
        char tx[] = {0,0,0,0,0,0,0,0,0,0,0,0};
        tx[0] = '&';
        tx[1] = '$';
        int p = 0;
        for (int i = 0; i<10; i+=2, p++){
          tx[i+2] = ',';
          tx[i+3] = map(val[p]-300, 0, 524, 0, 255);
        }
//        tx[12] = '\n';
        Serial.print(tx);
      }
    } // end if &
  }
}

