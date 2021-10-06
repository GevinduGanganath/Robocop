/*
 * Library for motor drivers, without using encorders.
 * 
 * default speed and error set to 50 and 22 respectively.
 * values are subjected to be change on the motor conditions. 
 * for simulations error = 0 and speed in the range 0-255.
 */

#pragma once
#include "Arduino.h"

#define sample_error 12
#define sample_speed 40

// for speed 50 error = 22

class Driver
{
private:
    byte _enR;
    byte _inR1; 
    byte _inR2; 
    byte _enL; 
    byte _inL1; 
    byte _inL2;
    byte _error;

    byte _frontSensor; 
    byte _backSensor; 
    byte _rightSensor; 
    byte _leftSensor;
    byte _sensorPower;

    volatile bool _frontSensorVal;
    volatile bool _backSensorVal;
    volatile bool _leftSensorVal;
    volatile bool _rightSensorVal;

    void readIR();
    
public:
    Driver(byte enR, byte inR1, byte inR2, byte enL, byte inL1, byte inL2, byte error = sample_error); 
    void begin();
    void moveForward(int speed = sample_speed);
    void turnRight(int speed = sample_speed);
    void turnLeft(int speed = sample_speed);
    void turnRight90();
    void turnLeft90();
    void hold();

    void lineFollow(byte frontSensor, byte backSensor, byte rightSensor, byte leftSensor, byte sensorPower);
};
