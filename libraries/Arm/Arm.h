#include <Arduino.h>
#include <Wire.h>
#include "Queue.h"
#include "Callable.h"


#ifndef ARM_H
#define ARM_H

#define QUEUE_SIZE 125
#define MASTER_WIRE_ADDRESS 1

class Arm : public Callable {
  private:
    AirCylinder* _lowerArm;
    AirCylinder* _upperArm;
    AirCylinder* _wrist;
    byte _servoPin;
    byte _vaccPin;
    byte _coneSwitchPin;
    int _servoPos;
    byte _currentRoutine;
    boolean _doingRoutine;
    Queue<Callable,QUEUE_SIZE> functionQueue;

    void _setFingers(int servoPos) { _servoPos = servoPos; }
    void _doRoutine();
    void _pickUpCone();
    void _scoreConeLow();
    void _scoreConeHigh();
    void _scoreConeeSideways();
    void _storeCone();
    void _rest();
    void _pickUpConeFromBot();
    void _stabilize();
    bool _isHoldingCone();
    void _routineComplete();
    void _sendRoutineComplete();
    bool _moveForward(int distance);
    bool _waitForTimePeriod(int time);

  public:
    Arm(AirCylinder *lowerArm, AirCylinder *upperArm, AirCylinder *wrist, byte servoPin, byte vaccPin, byte coneSwitchPin);
    void stabilize();
    void openFingers()             {  _setFingers(Arm::SERVO_OPEN);   }
    void closeFingers()            {  _setFingers(Arm::SERVO_CLOSE);  }
    void setFingers(int servoPos) {  _setFingers(servoPos);    }
    void setVaccum(byte status);
    bool isHoldingCone(int val);
    void doRoutine();
    void doRoutine(byte);
    void calibrateRoutine(byte);
    void clearQueue() {
      functionQueue.clear();
      setVaccum(VACC_OFF);
    }
    virtual bool call(int methodNum, int val);
    
    static char charToWrite;

    static const int SERVO_OPEN = 0;
    static const int SERVO_CLOSE = 250;
    static const int SERVO_PUSH_OFF_CONE = 200;

    static const byte VACC_OFF = 0;
    static const byte VACC_ON = 1;

    // Routines
    static const byte PICKUP_CONE = 0;
    static const byte SCORE_CONE_LOW = 1;
    static const byte SCORE_CONE_HIGH = 2;
    static const byte SCORE_CONE_SIDEWAYS = 3;
    static const byte STORE_CONE = 4;
    static const byte RESTING = 5;
    
    // Method IDs
    static const int METHOD_WAITFORHOLDINGPOSITION = 1;
    static const int METHOD_MOVESERVO = 2;
    static const int METHOD_VACC_ON = 3;
    static const int METHOD_VACC_OFF = 4;
    static const int METHOD_CHECK_IF_HOLDING_CONE = 5;
    static const int METHOD_ROUTINE_DONE = 6;
    static const int METHOD_MOVE_FORWARD = 7;
    static const int METHOD_WAIT = 8;

    // Specific calls for methods
    static const int PICKUP_CONE_FROM_GROUND = 1;
    static const int PICKUP_CONE_FROM_BOT = 2;
    static const int CONE_IS_DROPPED_SCORE_LOW = 3;

    static const int MOVE_SHORT = 10;
    static const int MOVE_FAR = 11;
    static const int MOVE_FAR_WAITTIME = (1000 * 1.5);
    static const int MOVE_SHORT_WAITTIME = (800 * 1.5);
};

char Arm::charToWrite = 'o';

Arm::Arm(AirCylinder *lowerArm, AirCylinder *upperArm, AirCylinder *wrist, byte servoPin, byte vaccPin, byte coneSwitchPin) {
  // Set a reference to all the arm cylinders
  _lowerArm = lowerArm;
  _upperArm = upperArm;
  _wrist = wrist;

  // Set the pin the servos are on
  _servoPin = servoPin;
  _vaccPin = vaccPin;
  _coneSwitchPin = coneSwitchPin;

  _doingRoutine = false;
  
  pinMode(_servoPin, OUTPUT);
  pinMode(_vaccPin, OUTPUT);
  pinMode(_coneSwitchPin, INPUT);
}

void Arm::_sendRoutineComplete() {
  Arm::charToWrite = '=';
  Serial.println("Complete");
}

bool Arm::_moveForward(int distance) {
  if(distance == MOVE_FAR) {
    Arm::charToWrite = 'K'; // Move Far
  } else {
    Arm::charToWrite = 'k'; // Move Short
  }
  return true;
}

bool Arm::_waitForTimePeriod(int time) {
  if((int)(millis()/100) > time) {
    return true;
  }
}

void Arm::doRoutine(byte routine) {
  _currentRoutine = routine;
  _doRoutine();
}

void Arm::stabilize() {
  functionQueue.popAndRun();
  _stabilize();
}

void Arm::_stabilize() {
  _lowerArm->stablize();
  _upperArm->stablize();
  _wrist->stablize();

  analogWrite(_servoPin, _servoPos);
}

void Arm::setVaccum(byte status) {
  if(status == Arm::VACC_ON) {
    digitalWrite(_vaccPin, HIGH);
  } else {
    digitalWrite(_vaccPin, LOW);
  }
}

bool Arm::isHoldingCone(int val) {
  //Add Steps back into the front of the queue

  if(val == CONE_IS_DROPPED_SCORE_LOW) {
    if(digitalRead(_coneSwitchPin) == LOW) {
      return true;
    }
    functionQueue.pushFront(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 4);

  } else if(val == PICKUP_CONE_FROM_GROUND) {
    if(digitalRead(_coneSwitchPin) == HIGH) {
      return true; // A cone is on 
    }
    functionQueue.pushFront(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 2);

    int delayTime = (int)((500 + millis()) / 100);
    functionQueue.pushFront(this, METHOD_WAIT, delayTime);

    functionQueue.pushFront(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 30);


  } else if(val == PICKUP_CONE_FROM_BOT) {
    if(digitalRead(_coneSwitchPin) == HIGH) {
      return true; // A cone is on 
    }

    functionQueue.push(this, METHOD_VACC_ON, 0);
    functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 30);
    functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 80);
    functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 30);
    functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 38);
    functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 91);
  }

  return false;
}


void Arm::_doRoutine() {
  switch(_currentRoutine) {
    case PICKUP_CONE:
      _pickUpCone();
      break;
    case SCORE_CONE_LOW:
      _scoreConeLow();
      break;
    case SCORE_CONE_HIGH:
      _scoreConeHigh();
      break;
    case SCORE_CONE_SIDEWAYS:
      _scoreConeeSideways();
      break;
    case STORE_CONE:
      _storeCone();
      break;
    case RESTING:
      _rest();
      break;
    default:
      _stabilize();
      break;
  }
}

void Arm::_routineComplete() {
  functionQueue.push(this, METHOD_ROUTINE_DONE, 0);
}

void Arm::_pickUpCone() { // Pickup from Ground
  // Lower - 100%
  // Upper - 80%
  // Wrist - 80%
  // Lower - 0%

  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 80);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 90);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 75);
  functionQueue.push(this, METHOD_VACC_ON, 0);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 5);
  functionQueue.push(this, METHOD_MOVESERVO, SERVO_CLOSE);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 30);

  functionQueue.push(this, METHOD_CHECK_IF_HOLDING_CONE, PICKUP_CONE_FROM_GROUND);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 100);

  _storeCone();

  // Routine is Done
//  _routineComplete();
}

void Arm::_pickUpConeFromBot() {
  functionQueue.push(this, METHOD_VACC_ON, 0);
//  functionQueue.push(this, METHOD_MOVESERVO, SERVO_CLOSE);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 90);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 0);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 65);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 41);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 30);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 80);
//  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 30);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 42);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 86);

  functionQueue.push(this, METHOD_CHECK_IF_HOLDING_CONE, PICKUP_CONE_FROM_BOT);
  
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 95);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 80);
}

void Arm::_scoreConeLow() {
  _pickUpConeFromBot();

  // Lower - 100%
  // Upper - 85%
  // Lower - 45%
  // Wrist - 60%
  // Lower - 30%

  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 90);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 95);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 73);
  functionQueue.push(this, METHOD_VACC_ON, 0);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 86);
  functionQueue.push(this, METHOD_MOVESERVO, SERVO_CLOSE);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 20);
  functionQueue.push(this, METHOD_VACC_OFF, 0);

  int delayTime = (int)((500 + millis()) / 100);
  functionQueue.push(this, METHOD_WAIT, delayTime);

  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 3);

  delayTime = (int)((1500 + millis()) / 100);
  functionQueue.push(this, METHOD_WAIT, delayTime);

  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 10);
//  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 22);
  functionQueue.push(this, METHOD_MOVESERVO, SERVO_PUSH_OFF_CONE);

  functionQueue.push(this, METHOD_CHECK_IF_HOLDING_CONE, CONE_IS_DROPPED_SCORE_LOW);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 100);

  // Routine is Done
  _routineComplete();
}


void Arm::_scoreConeHigh() {
//  _pickUpConeFromBot();

  // Lower - 100%
  // Wrist - 30%
  // Upper - 100%;
  // Wrist - 0%

  functionQueue.push(this, METHOD_MOVE_FORWARD, MOVE_FAR);
  int delayTime = (int)((MOVE_FAR_WAITTIME + millis()) / 100);
  functionQueue.push(this, METHOD_WAIT, delayTime);

  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 100);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 30);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 100);
  functionQueue.push(this, METHOD_MOVE_FORWARD, MOVE_FAR);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 0);
  functionQueue.push(this, METHOD_VACC_OFF, 0);
  functionQueue.push(this, METHOD_MOVESERVO, SERVO_PUSH_OFF_CONE);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 100);

  // Routine is Done
  _routineComplete();
}


void Arm::_scoreConeeSideways() {
  _pickUpConeFromBot();

  // Lower - 70%
  // Upper - 100%
  // Wrist - 100%

  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 70);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 100);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 100);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION,50);

  functionQueue.push(this, METHOD_MOVE_FORWARD, MOVE_SHORT);
  int delayTime = (int)((MOVE_SHORT_WAITTIME + millis()) / 100);
  functionQueue.push(this, METHOD_WAIT, delayTime);

  functionQueue.push(this, METHOD_VACC_OFF, 0);
  functionQueue.push(this, METHOD_MOVESERVO, SERVO_PUSH_OFF_CONE);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 100);

  // Routine is Done
  _routineComplete();
}

void Arm::_storeCone() {
  // Vacc - ON
  // Wrist - 0%
  // Upper - 50%
  // Wrist - 25%
  // Upper - 33%
  // Lower - 85%
  // Wrist - 40%
  // Vacc - OFF
  // Remove
  functionQueue.push(this, METHOD_VACC_ON, 0);
//  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 44);
//  int delayTime = (int)((200 + millis()) / 100); // Wait 200 milliseconds to continue the rest of the way down
//  functionQueue.push(this, METHOD_WAIT, delayTime); // Wait for a split second

  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 0);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 40);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 30);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 30);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 92);
  functionQueue.push(this, METHOD_VACC_OFF, 0);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 82);
//  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 40);

  functionQueue.push(this, METHOD_VACC_OFF, 0);

  int delayTime = (int)((1000 + millis()) / 100); // Wait 1000 milliseconds to continue
  functionQueue.push(this, METHOD_WAIT, delayTime); // Wait for a split second

  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 48);
  _routineComplete();

  delayTime = (int)((1500 + millis()) / 100); // Wait 1500 milliseconds to continue
  functionQueue.push(this, METHOD_WAIT, delayTime); // Wait for a split second

  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 95);
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 100);

  // Routine is Done
//  _routineComplete();
}

void Arm::_rest() {
  // Lower - 100%
  // Upper - 30%
  // Wrist - 100%
  functionQueue.push(_lowerArm, METHOD_WAITFORHOLDINGPOSITION, 100);
  functionQueue.push(_upperArm, METHOD_WAITFORHOLDINGPOSITION, 60);
  functionQueue.push(_wrist, METHOD_WAITFORHOLDINGPOSITION, 0);

  // Routine is Done
  _routineComplete();
}

bool Arm::call(int methodNum, int val) {
  switch(methodNum) {
    case METHOD_MOVESERVO:
      this->setFingers(val);
      return true;
      break;
    case METHOD_VACC_ON:
      this->setVaccum(Arm::VACC_ON);
      return true;
      break;
    case METHOD_VACC_OFF:
      this->setVaccum(Arm::VACC_OFF);
      return true;
      break;
    case METHOD_CHECK_IF_HOLDING_CONE:
      return this->isHoldingCone(val);
      break;
    case METHOD_MOVE_FORWARD:
      return this->_moveForward(val);
      break;
    case METHOD_ROUTINE_DONE:
      this->_sendRoutineComplete();
      return true;
      break;
    case METHOD_WAIT:
      return this->_waitForTimePeriod(val);
      break;
  }
  return true;
}


#endif
