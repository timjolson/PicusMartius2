#include <Arduino.h>
#include "Callable.h"

#ifndef AIRCYLINER_H
#define AIRCYLINER_H

#define THRESHOLD_PERCENT 10
#define CALIBRATE_UPDATE_MAX_WAIT_MILLIS 3500

class AirCylinder : public Callable {
  private:
    void _setLastPosition(int position);
    int _getCurrentPosition();
    int _getRawPosition();
    void _log(char* msg);
    void _error(char* msg);
    void _setPinModes();
    void _closeCylinder();
    void _closeCylinder(bool isSlow);
    void _openCylinder();
    void _openCylinder(bool isSlow);
    void _holdPosition();

    /* Variables */
    byte _valvePin;
    byte _pressureHoldingPin;
    byte _potPin;
    int _potMin;
    int _potMax;
    int _holdingPosition;
    int _lastPosition;
    boolean _isInverse;
    boolean _isLogEnabled;
    boolean _hasOStream;
    Stream* _ostream;

  public:
    /* Constructors */
    AirCylinder();
    AirCylinder(byte valvePin, byte pressureHoldingPin, byte potPin);
    AirCylinder(byte valvePin, byte pressureHoldingPin, byte potPin, boolean isInverse);

    /* Setters */
    void setValvePin(byte pin);
    void setPressureHoldingPin(byte pin);
    void setPotPin(byte pin);
    void setisInverse(boolean isInverse);
    void setHoldingPosition(int position);
    void setOStream(Stream* ostream);
    void setPotMin(int min);
    void setPotMax(int max);
    void setCalibrationData(int min, int max);

    /* Getters */
    byte getValvePin();
    byte getPressureHoldingPin();
    byte getPotPin();
    boolean isInverse();
    int getPotMin();
    int getPotMax();
    int getCurrentPosition();
    int getRawPosition();
    int getHoldingPosition();
    int getLastPosition();
    boolean isLogEnabled();

    /* Functions */
    void stablize();
    bool waitForHoldingPosition(int position);
    void enableLog();
    void disableLog();
    void calibrateMax();
    void calibrateMin();
    void moveToMin();
    void moveToMax();
    virtual bool call(int methodNum, int val);
    
    static const int METHOD_WAITFORHOLDINGPOSITION = 1;
    static const int METHOD_MOVESERVO = 2;
};

/* Constructors */
AirCylinder::AirCylinder(){
  AirCylinder(-1, -1, -1);
}


AirCylinder::AirCylinder(byte valvePin, byte pressureHoldingPin, byte potPin) {
  AirCylinder(-1, -1, -1, false);  
}


AirCylinder::AirCylinder(byte valvePin, byte pressureHoldingPin, byte potPin, boolean isInverse) {
  _valvePin = valvePin;
  _pressureHoldingPin = pressureHoldingPin;
  _potPin = potPin;
  _isInverse = isInverse;

  _potMin = -1;
  _potMax = -1;
  _holdingPosition = 100.0f;
  _hasOStream = false;
  _isLogEnabled = false;
  _lastPosition = -1;

  _setPinModes();
}


/* Public Setters */
void AirCylinder::setValvePin(byte pin) { _valvePin = pin; }
void AirCylinder::setPressureHoldingPin(byte pin) { _pressureHoldingPin; }
void AirCylinder::setPotPin(byte pin) { _potPin = pin; }
void AirCylinder::setisInverse(boolean isInverse) { _isInverse = isInverse; }
void AirCylinder::setHoldingPosition(int position) { _holdingPosition = position; }
void AirCylinder::enableLog() { _isLogEnabled = true; }
void AirCylinder::disableLog() { _isLogEnabled = false; }
void AirCylinder::setOStream(Stream *ostream) {
  _hasOStream = true;
  _ostream = ostream;
}
void AirCylinder::setPotMin(int min) { _potMin = min; }
void AirCylinder::setPotMax(int max) { _potMax = max; }
void AirCylinder::setCalibrationData(int min, int max) {
  setPotMin(min);
  setPotMax(max);
}


/* Private Setters */
void AirCylinder::_setLastPosition(int position) { _lastPosition = position; }


/* Public Getters */
byte AirCylinder::getValvePin() { return _valvePin; }
byte AirCylinder::getPressureHoldingPin() { return _pressureHoldingPin; }
byte AirCylinder::getPotPin() { return _potPin; }
boolean AirCylinder::isInverse() { return _isInverse; }
int AirCylinder::getPotMin() { return _potMin; }
int AirCylinder::getPotMax() { return _potMax; }
int AirCylinder::getCurrentPosition() { return _getCurrentPosition(); }
int AirCylinder::getHoldingPosition() { return _holdingPosition; }
int AirCylinder::getLastPosition() { return _lastPosition; }
boolean AirCylinder::isLogEnabled() { return _isLogEnabled; }
int AirCylinder::getRawPosition() { return _getRawPosition(); }


/* Private Getters */
/**
  * _getCurrentPosition
  *
  * Returns the current position of the Air Cylinder in a range from 0.00 to 100.0
  *
  */
int AirCylinder::_getCurrentPosition() {
  //if(_potMin == -1 || _potMax == -1) { //Check to make sure the the values have been calibrated
    //_error("NOT CALIBRATED!");
    //return -1;
  //}

  int pos = map(_getRawPosition(), _potMin, _potMax, 0, 100);

  if(_isInverse) { // Controls are reversed
    pos = (100 - pos); //Give the opposite of the current value
  }
  if(_lastPosition == -1) {
    _lastPosition = pos;
  }

  return pos;
}

int AirCylinder::_getRawPosition() {
  return analogRead(_potPin);
}


/**
  * _setPinModes
  *
  * Enables all the pins required to control the Air Cylinder
  *
  */
void AirCylinder::_setPinModes() {
  pinMode(_valvePin, OUTPUT);
  pinMode(_pressureHoldingPin, OUTPUT);
}

/**
  * waitForHoldingPosition
  *
  * Attempts to stablize the Air Cylider at the current Holding Position
  * function returns true when the position is met
  *
  */
bool AirCylinder::waitForHoldingPosition(int position) {
  setHoldingPosition(position);
  if(abs(getCurrentPosition() - position) > THRESHOLD_PERCENT) {
    // Cylinder is not within the threshold
    return false;
  }
  _log("WaitFor: True");
  return true; // Cylinder is within the threshold
}


/**
  * stablize
  *
  * Attempts to stablize the Air Cylider to the current Holding Position
  *
  */
void AirCylinder::stablize(){
  int currentPosition = getCurrentPosition();
  
  int distance = abs(currentPosition - _holdingPosition);
  
  // If the current distance is less than or equal to the threshold don't
  // attempt to move the cylinder. Just use the position holding function
  if(distance <= THRESHOLD_PERCENT) {
    _holdPosition();

  // If the current position is LESS than the required holding position and
  // it's more than the threshold attempt to move the cylinder. If the Cylinder
  // is in NORMAL mode attempt to OPEN. If it's in REVERSED mode attempt to CLOSE it
  } else if(currentPosition < _holdingPosition && distance > THRESHOLD_PERCENT) {
    if(_isInverse) {
      _closeCylinder();
    } else {
      _openCylinder();
    }
  // If the current position is GREATER than the required holding position and
  // it's more than the threshold attempt to move the cylinder. If the Cylinder
  // is in NORMAL mode attempt to CLOSE. If it's in REVERSED mode attempt to OPEN it
  } else if(currentPosition > _holdingPosition && distance > THRESHOLD_PERCENT) {
    if(_isInverse) {
      _openCylinder();
    } else {
      _closeCylinder();
    }
  }

  // Save the last position for later use
  _lastPosition = currentPosition;
}

void AirCylinder::_openCylinder() {
  _openCylinder(false);
}

void AirCylinder::_openCylinder(bool isSlow) {
  if(isSlow) {
    digitalWrite(_pressureHoldingPin, HIGH);  // Allow air through the holding solenoid
    digitalWrite(_valvePin, HIGH);            // Extend the Cylinder
    delay(1);
    digitalWrite(_pressureHoldingPin, LOW);   // Stop air through the holding solenoid
  } else {
    digitalWrite(_pressureHoldingPin, HIGH);   // Allow air through the holding solenoid
    digitalWrite(_valvePin, HIGH);             // Extend the Cylinder
  }
}

void AirCylinder::_closeCylinder() {
  _closeCylinder(false);
}

void AirCylinder::_closeCylinder(bool isSlow) {
  if(isSlow) {
    digitalWrite(_pressureHoldingPin, HIGH);  // Allow air through the holding solenoid
    digitalWrite(_valvePin, LOW);             // Retract the Cylinder
    delay(1);
    digitalWrite(_pressureHoldingPin, LOW);   // Stop air through the holding solenoid
  } else {
    digitalWrite(_pressureHoldingPin, HIGH);   // Allow air through the holding solenoid
    digitalWrite(_valvePin, LOW);             // Retract the Cylinder
  }
}


void AirCylinder::_holdPosition() {
  int currentPosition = getCurrentPosition();

  // If the current distance is between the threshold and position is
  // greater than or equal 100 attempt to open the Air Cylinder all the
  // way to help stablize. If the cylinder is NORMAL attempt to OPEN
  // and if the cylinder is REVERSED attempt to CLOSE
  if((currentPosition + THRESHOLD_PERCENT) >= 100) {
    if(_isInverse) {
      _closeCylinder();
    } else {
      _openCylinder();
    }
  // If the current distance is between the threshold and position is
  // less than or equal to 0 open the Air Cylinder all the
  // way to help stablize. If the cylinder is NORMAL attempt to CLOSE
  // and if the cylinder is REVERSED attempt to OPEN
  } else if((currentPosition - THRESHOLD_PERCENT) <= 0) {
    if(_isInverse) {
      _openCylinder();
    } else {
      _closeCylinder();
    }
  // If the cylinder is somewhere in the middle attempt to close all
  // the valves to stablize it's movement
  } else {
    digitalWrite(_pressureHoldingPin, LOW);    // Close exhaust valve
  }
}


void AirCylinder::calibrateMax() {
  _log("Calibrating MAX...");
  _openCylinder();
  unsigned long lastUpdateTime = millis();
  int currentRawPosition;

  boolean _isCalibrating = true;
  while(_isCalibrating) {
    _openCylinder();
    currentRawPosition = _getRawPosition();
    if(currentRawPosition > _potMax || _potMax == -1) {
      _potMax = currentRawPosition;
      lastUpdateTime = millis(); //Update the time
    } else if((millis() - lastUpdateTime) > CALIBRATE_UPDATE_MAX_WAIT_MILLIS) {
      _isCalibrating = false; //Stop the loop as the values havent changed after X amount of seconds
    }
  }
  _log("MAX Calibrated!");
}


void AirCylinder::calibrateMin() {
  _log("calibrating MIN");
  _closeCylinder();
  unsigned long lastUpdateTime = millis();
  int currentRawPosition;

  boolean _isCalibrating = true;
  while(_isCalibrating) {
    _closeCylinder();
    currentRawPosition = _getRawPosition();
    if(currentRawPosition < _potMin || _potMin == -1) {
      _potMin = currentRawPosition;
      lastUpdateTime = millis(); //Update the time
    } else if((millis() - lastUpdateTime) > CALIBRATE_UPDATE_MAX_WAIT_MILLIS) {
      _isCalibrating = false; //Stop the loop as the values havent changed after X amount of seconds
    }
  }
  _log("MIN Calibrated");
}


void AirCylinder::moveToMin() {
  _closeCylinder();
}


void AirCylinder::moveToMax() {
  _openCylinder();
}



void AirCylinder::_log(char* msg) {
  if(_isLogEnabled && _hasOStream) { // Make sure the log was enabled and there is an OStream to send the data to
    _ostream->print("[AIRCYL]");
    _ostream->println(msg);
  }
}


void AirCylinder::_error(char* msg) {
 if(_hasOStream) { // Make sure there is an OStream to send the data to
    _ostream->print("[AIRCYL][ERROR]");
    _ostream->println(msg);
  }
}

bool AirCylinder::call(int methodNum, int val) {
  if(methodNum == METHOD_WAITFORHOLDINGPOSITION) {
    return this->waitForHoldingPosition(val);
  }
  //
  return true;
}


#endif
