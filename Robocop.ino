#include "Driver.h"

Driver Driver(5, 6, 7, 8, 9, 10);

void setup(){
  Driver.begin();
  
  Driver.moveForward();
  delay(1000);
  Driver.turnLeft();
  delay(300);
  Driver.moveForward();
  delay(1000);
  Driver.turnRight();
  delay(300);
  Driver.moveForward();
  delay(1000);
  Driver.hold();
 
  Driver.hold();
  
  }

void loop(){
  }
