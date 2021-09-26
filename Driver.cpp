#include "Arduino.h"
#include "Driver.h"

// error parameter add to remove the wheel unbalance. Not working well

Driver::Driver(int enR, int inR1, int inR2, int enL, int inL1, int inL2, int error){
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
  analogWrite(_enR, 0);
  analogWrite(_enL, speed+_error);
}

void Driver::turnLeft(int speed){
  analogWrite(_enR, speed);
  analogWrite(_enL, 0);
}

void Driver::hold(){
  analogWrite(_enR, 0);
  analogWrite(_enL, 0);
}