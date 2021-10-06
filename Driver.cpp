#include "Arduino.h"
#include "Driver.h"

Driver::Driver(byte enR, byte inR1, byte inR2, byte enL, byte inL1, byte inL2, byte error){
    _enR = enR;
    _inR1 = inR1;
    _inR2 = inR2;
    _enL = enL;
    _inL1 = inL1;
    _inL2 = inL2;
    _error = error;
}

void Driver::begin(){
    pinMode(_enR, OUTPUT);
    pinMode(_inR1, OUTPUT);
    pinMode(_inR2, OUTPUT);
    pinMode(_enL, OUTPUT);
    pinMode(_inL1, OUTPUT);
    pinMode(_inL2, OUTPUT);

    digitalWrite(_inR1, LOW);
    digitalWrite(_inR2, HIGH);
    digitalWrite(_inL1, LOW);
    digitalWrite(_inL2, HIGH);
}

void Driver::moveForward(int speed){
  analogWrite(_enR, speed);
  analogWrite(_enL, speed+_error);
}

void Driver::turnRight(int speed){
  analogWrite(_enR, (int)speed/2);
  analogWrite(_enL, speed+_error);
}

void Driver::turnLeft(int speed){
  analogWrite(_enR, speed);
  analogWrite(_enL, (int)(speed+_error)/2);
}

void Driver::turnRight90(){
  turnRight();
  delay(735);
}

void Driver::turnLeft90(){
  turnLeft();
  delay(1075);
}

void Driver::hold(){
  analogWrite(_enR, 0);
  analogWrite(_enL, 0);
}

void Driver::readIR(){
  _frontSensorVal = digitalRead(_frontSensor);
  _backSensorVal = digitalRead(_backSensor);
  _rightSensorVal = digitalRead(_rightSensor);
  _leftSensorVal = digitalRead(_leftSensor);
}

void Driver::lineFollow(byte frontSensor, byte backSensor, byte rightSensor, byte leftSensor, byte sensorPower){

  _frontSensor = frontSensor;
  _backSensor = backSensor;
  _rightSensor = rightSensor;
  _leftSensor = leftSensor;
  _sensorPower = sensorPower;

  pinMode(_frontSensor, INPUT);
  pinMode(_backSensor, INPUT);
  pinMode(_rightSensor, INPUT);
  pinMode(_leftSensor, INPUT);
  pinMode(_sensorPower, OUTPUT);

  digitalWrite(_sensorPower, HIGH);

  // black 1
  // white 0
  Serial.begin(9600);
  while (1){
    readIR();
    if ((_frontSensorVal == 0 or _backSensorVal == 0) && _rightSensorVal == 1 && _leftSensorVal == 1){
      Serial.println("Forward");
      moveForward();
    }
    else if ((_frontSensorVal == 0 or _backSensorVal == 0) && _rightSensorVal == 0 && _leftSensorVal == 1){
      Serial.println("Turn right");
      turnRight();
    }
    else if ((_frontSensorVal == 0 or _backSensorVal == 0) && _rightSensorVal == 1 && _leftSensorVal == 0){
      Serial.println("Turn left");
      turnLeft();
    }
    else if (_frontSensorVal == 1 && _backSensorVal == 1){   //due to error in front sensor calibration. otherwise not needed
      Serial.println("Forward");
      moveForward();
    }
   else if ((_frontSensorVal == 0 or _backSensorVal == 0) && _rightSensorVal == 0 && _leftSensorVal == 0){
      Serial.println("Hold");
      hold();
      //break;
    }
  }
  digitalWrite(_sensorPower, LOW);
}
