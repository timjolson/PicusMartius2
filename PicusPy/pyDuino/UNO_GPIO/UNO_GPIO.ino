#include <ArduinoBoardManager.h>

ArduinoBoardManager arduino = ArduinoBoardManager();

/* setup with ##ioiioioo~~dddd~~
 * i / o = input or output, for all pins, in order from 0
 * d = pin initial values, for all pins, in order from 0
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

char* buf;
bool isInited = false;
int numDigitals = 0;
int numAnalogs = 0;
int numPins = 0;

// function to set pin modes and initial values
void settings(){
  // wait for initialization
  while (isInited == false){
    
    // initalize all pins as input, write them low
    for (int p = 0; p<numPins; p++){
      pinMode(p, INPUT);
      if (p<numDigitals) digitalWrite(p, LOW);
      else analogWrite(p, 0);
    } // end all pins

    if (Serial.available()){
      // set pinModes
      int count = 0;
      while (Serial.peek()!='~' && count<numPins){ /* ~ := 126 */
        if (Serial.available()>0){
          int stat = Serial.parseInt(); // INPUT or OUTPUT
          pinMode(count, !!stat);
        }
        count++;
      } // end inputs
      
      //set initial values
      int pin=0;
      while (Serial.peek()!='~' && pin<numPins){ /* ~ := 126 */
        if (Serial.available()){
          int val = Serial.parseInt();
          digitalWrite(pin, val);
        }
        pin++;
      } // end digital defaults
      
      //check for final ~ character
      if (Serial.peek()=='~'){ /* ~ := 126 */
        Serial.print("##");
        isInited = true;
      } // end initialized
      else{
        Serial.print("!~");
      } // end not initialized
    }// serial.available()
  }// not isInited

}//end settings()

void setup() {
  // use board type to get GPIO info
  if (!strcmp(arduino.BOARD_NAME,"UNO")){
    numDigitals = 14; // digitals 0->13 := 0->13
    numAnalogs = 6; // analogs 14->19 := A0->A5
  }
  else if (!strcmp(arduino.BOARD_NAME,"MEGA")){
    numDigitals = 14; // digitals 0->13 := 0->13
    numAnalogs = 6; // analogs 14->19 := A0->A5
  }
  
  numPins = numDigitals + numAnalogs;
 
  // Serial connection
  Serial.begin(115200);
  Serial.setTimeout(5);

  // 10-bit analog reads
//  if (!strcmp(arduino.BOARD_NAME, "MEGA"))
//    analogReadResolution(10);


  // wait for # for setup procedure
  while (Serial.read()!='#'){}

  // verify with another #, or restart while (!init)
  if (Serial.read()!='#') Serial.print("!#"); // tell master that init failed
  else settings();  // begin pin settings
  
} // end setup()


void loop() {
  while (Serial.available()){
    if (Serial.peek()=='#'){
      Serial.readBytes(buf,1);
      if (buf[0]=='#'){
        isInited = 0;
        start();
      }
    }
    else{
      Serial.readBytes(buf, 1);
      if (buf[0]=='&'){
        Serial.readBytes(buf,1);
        if (buf[0]=='&'){
          int pinNum = Serial.parseInt();
          int val = Serial.parseInt();
          digitalWrite(pinNum, val);
          Serial.print(pinNum);Serial.print(val);
        } // end if &
        else if (buf[0]=='@'){
          int pinNum = Serial.parseInt();
          int val = Serial.parseInt();
          analogWrite(pinNum, val);
          Serial.print(pinNum);Serial.print(val);
        } // end if @
        else if (buf[0]=='$'){
          int* val;
          for (int i = 0; i<numAnalogs; i++){
            val[i] = analogRead(numDigitals+i);
          }
          String tx = "&$";
          char add[64];
          
          for (int p = 0 ; p<numAnalogs; p++){
            //strcpy(add,tx);
            sprintf(add, "%d,", int(analogRead(p+numDigitals)));
            tx.concat(add);
          }
          Serial.print(tx+"$");
        }
      } // end if first &
    }// end not #
      
  } // end while Serial.available()
} // end loop

