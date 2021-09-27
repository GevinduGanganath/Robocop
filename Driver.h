/*
 * Library for motor drivers, without using encorders.
 * 
 * default speed and error set to 50 and 22 respectively.
 * values are subjected to be change on the motor conditions. 
 * for simulations error = 0 and speed in the range 0-255.
 */

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
    Driver(int enR, int inR1, int inR2, int enL, int inL1, int inL2, int error = 22);
    void begin();
    void moveForward(int speed = 50);
    void turnRight(int speed = 50);
    void turnLeft(int speed = 50);
    void turnRight90();
    void turnLeft90();
    void hold();
};
