byte* buf;
bool isInited = false;
int numDigitals = 14; //0->13
int numAnalogs = 6; //14->19
const int numPins = numDigitals+numAnalogs;

struct read_uno
{
  int vals[6] = {0}; //feedback values (-1 = OUTPUT mode)
//  uint32_t timestamp = 0; // micro seconds since last time_reset
//  uint8_t time_reset = 0; // number of timestamp rollovers
};

struct pins_uno
{
  byte modes[20] = {INPUT}; // input=0 or output=1 or pwm=-1 for all pins
  byte def_vals[20] = {0};// default value for all pins
  bool curr_digs[20] = {0};// current value for all digital pins
  byte curr_pwm[20] = {0};// current value for all pwm pins
  
  uint16_t timeout = 100; // millis timeout to reset to default values
  uint16_t refresh = 5000; // micros between data transmit
};

int pins_size = sizeof(pins_uno);
pins_uno stat;
int read_size = sizeof(read_uno);
read_uno feedback;
// uint64_t start_time = micros();
uint64_t last_tx = micros();

void update_struct(byte* rx){
  memcpy(&rx, &stat, pins_size);
  write_pins(true);
}

void write_pins(bool timeout){
  if (!timeout)
    for (int p=0; p<20; p++){
      if (stat.modes[p]==OUTPUT)
        digitalWrite(p,!!stat.curr_digs[p]);
      else if (stat.modes[p]==byte(-1))
        analogWrite(p, stat.curr_pwm[p]);
      else
        Serial.print("!mode");
    }
  else
    for (int p=0; p<20; p++){
      if (stat.modes[p]==OUTPUT)
        digitalWrite(p,!!stat.def_vals[p]);
      else if (stat.modes[p]==byte(-1))
        analogWrite(p, stat.def_vals[p]);
      else
        Serial.print("!mode");
    }
}

void check(){
  Serial.readBytes(buf, pins_size);
  if (Serial.read()=='~'){ // final character is good
    update_struct(buf);  // do pin settings
    isInited = true;    // we are initialized
  }
  else
    Serial.print("!#"); // tell master that init failed
}

void read_pins(){
  for (int p = 0; p<numAnalogs; p++){
    if (stat.modes[p+numDigitals]==INPUT){
      feedback.vals[p] = analogRead(p+numDigitals);
    }
  }
}

void setup(){
  Serial.begin(115200);
  Serial.setTimeout(5);

  // wait for # for setup procedure
  while (Serial.read()!='#'){}

  // verify with another #, or restart while (!init)
  if (Serial.read()!='#') // no second #
    Serial.print("!#"); // tell master that init failed
  else // received both #
    check();

  if (!isInited) setup(); // if init was attempted but failed, wait for init again
}

void loop(){
  //Serial.write((uint8_t*) settings, struct_size);

  while (Serial.read()!='&'){}
  if (Serial.read()=='&'){
    check();
    write_pins()
//    uint64_t dt = micros() - start_time; 
//    if (dt>4294966000){ //4294967295
//      start_time = micros();
//      feedback.time_reset++;
//    }
    
//    feedback.timestamp = micros() - start_time;
    read_pins();
    
    if ((micros() - last_tx) > stat.refresh){
      Serial.write((uint8_t*) &feedback, read_size);
      last_tx = micros();
    }
  }
}

