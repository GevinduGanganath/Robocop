#include "Driver.h"
#define forwardSpeed 60
 
Driver Driver(5, 6, 7, 8, 9, 10, 27);

void setup(){
  Driver.begin();
  
  Driver.moveForward(forwardSpeed);
  delay(1000);
  Driver.turnRight(forwardSpeed);
  delay(100);
  Driver.moveForward(forwardSpeed);
  delay(1000);
  Driver.turnLeft(forwardSpeed);
  delay(100);
  Driver.moveForward(forwardSpeed);
  delay(1000);
  Driver.hold();
  }

void loop(){
  }
