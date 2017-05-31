

//this is the code that goes on the foam radio transmitter for NAI
//

const int RedPin=A4;
const int GreenPin=A5;
int greenpressed = 0;

void setup() {
Serial.begin(115200);
pinMode(RedPin, INPUT_PULLUP);
pinMode(GreenPin, INPUT_PULLUP);

if(digitalRead(GreenPin) == LOW){
  greenpressed = 1;
}
}

//This code gets trapped in one of three successive loops depending on
//what state the controller is in, then breaks out of the loop to 
//advance to the next state.
//the three states are in order as follows:
//green button pressed


void loop() {
  //STOP CHARACTER IS '&'
  //GO CHARACTER IS '+"
 
 if(greenpressed){
  while(1){
   Serial.println('&');
       if(digitalRead(GreenPin) == HIGH){//When green button is releaded
    greenpressed = 0;   
    break;                          //return to initial state
  }
 }
   
 } 
  if((millis() % 300) == 0)
  Serial.println('&');
  //Serial.println(digitalRead(GreenPin));
  if(digitalRead(GreenPin) == LOW){//When green button is pressed,
     while(1){       //advance to the next state
        if((millis() % 300) == 0)
          Serial.println('+');
        if(digitalRead(RedPin)==HIGH){//When red button is pressed,
          while(1){             //advance to the next state
            if((millis() % 300) == 0)
            Serial.println('&');//STOP character
            if(digitalRead(GreenPin) == HIGH){//When green button is releaded
              break;                          //return to initial state
            }
          }
         break; 
        }  
     } 
  }
}
