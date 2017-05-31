void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(19200);
}

char inChar; // Where to store the character read
char inData[20]; // Allocate some space for the string
byte _index = 0; // Index into array; where to store the character

int16_t mouseData[2];
int count = 0;
void loop() {
//  if(Serial1.available()) {
//    Serial.println((char)Serial1.read());
//  }
  if(readMice(mouseData)) {
    Serial.print(mouseData[0]);
    Serial.print(",");
    Serial.println(mouseData[1]);
  }
  //if(mouseData[0] != 0 || mouseData[1] != 0) {
//    Serial.print(mouseData[0]);
//    Serial.print(",");
//    Serial.println(mouseData[1]);
  //}
  //mouseData[0] = 0;
  //mouseData[1] = 0;
}

bool readMice(int16_t *dest) {
  HardwareSerial &input = Serial1;
  if (input.available() >= 6) {
    // Convenience macro to read the next byte
    #define READ_BYTE() ((uint8_t)input.read())
    
    uint8_t byte1, byte2;
    uint8_t expectedParity = 0, parity;

    byte1 = READ_BYTE();
//    if (nextByte != 0xFF) {
    if (byte1 != 0xFF) {
      Serial.print("[");
      Serial.print((int)byte1);
      Serial.print("]");
      return false;
    } else {
      Serial.println(); // Print blank line to clear old stuff
    }
    int16_t tempData[2] = {0};
    
    byte1 = READ_BYTE();
    byte2 = READ_BYTE();
    tempData[0] = ((int16_t)byte1 << 8) | (int16_t)byte2; // Merge together
    expectedParity = (calculateParity(byte1) << 3) | (calculateParity(byte2) << 2);
    
    byte1 = READ_BYTE();
    byte2 = READ_BYTE();
    tempData[1] = ((int16_t)byte1 << 8) | (int16_t)byte2; // Merge together
    expectedParity |= (calculateParity(byte1) << 1) | (calculateParity(byte2));
    
    parity = READ_BYTE();
    if (parity == expectedParity) {
      dest[0] = tempData[0];
      dest[1] = tempData[1];
      return true;
    }
    Serial.print("[BAD2]");
    Serial.print(tempData[0]);Serial.print(",");Serial.print(tempData[1]);Serial.print("!");Serial.println(parity);
    return false;
    
  }
  return false;
}

uint8_t calculateParity(uint8_t val) {
  uint8_t parity = 0;
  // While val still has bits set
  for (; val > 0; val >>= 1) {
    // Flip our parity bit if the lowest bit is set.
    parity ^= (val & 0x1);
    // Shift off the lowest bit after every iteration.
  }
  return parity;
}


