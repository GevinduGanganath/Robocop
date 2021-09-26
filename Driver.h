#pragma once
#include "Arduino.h"

class Driver
{
private:
    short int _enR;
    short int _inR1; 
    short int _inR2; 
    short int _enL; 
    short int _inL1; 
    short int _inL2;
    short int _error;
    
public:
    Driver(int enR, int inR1, int inR2, int enL, int inL1, int inL2, int error);
    void begin();
    void moveForward(int speed);
    void turnRight(int speed);
    void turnLeft(int speed);
    void hold();
};
