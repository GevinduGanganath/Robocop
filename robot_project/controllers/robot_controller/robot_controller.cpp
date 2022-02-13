// File: robot_controller.cpp
// Date: 
// Description: EN2532 Robot Design & Competition 
// Author: Robocop
// Modifications:

// Webots include files
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/PositionSensor.hpp>
#include <iostream>

//constants
#define TIME_STEP 32
#define MAX_SPEED 8 
#define NORM_SPEED 5
#define IR_TRESH 425
#define Kp 1.2
#define Kd 0.2
#define HALF_WALL_GAP 0.375

// All the webots classes are defined in the "webots" namespace
using namespace webots;
using namespace std;

//Objects
Robot *robot;
Motor *motor_l;
Motor *motor_r;
DistanceSensor *IR[6];
PositionSensor *encoder_l;
PositionSensor *encoder_r;
DistanceSensor *DS[3];  // left, front, right

char IR_names[6][4] = {"IR1", "IR2", "IR3", "IR4", "IR5", "IR6"};
char DS_names[3][5] = {"DS_L", "DS_F", "DS_R"};
double encoder_l_reading, encoder_r_reading;

int previous_error = 0;
int error;

//function protos
void delay(float time);
float PID_CV();
void line_follow();
void go_ahead();
void turn_right(short int degree=0);
void turn_left(short int degree=0);
void turn_back();
void stop();
void goto_next_square(float delay);
void wall_follow();

int main(int argc, char **argv) {
  //Robot instance.
  robot = new Robot();
  //Motors
  motor_l = robot->getMotor("L_motor");
  motor_r = robot->getMotor("R_motor");  
  motor_l->setPosition(INFINITY);
  motor_r->setPosition(INFINITY);
  motor_l->setVelocity(0.0);
  motor_r->setVelocity(0.0);
  
  //Encoders
  encoder_l = robot->getPositionSensor("L_encoder");
  encoder_l->enable(TIME_STEP);
  encoder_r = robot->getPositionSensor("R_encoder");
  encoder_r->enable(TIME_STEP);
  
  //IR sensors
  for (int i=0; i<6; i++) {
    IR[i] = robot->getDistanceSensor(IR_names[i]);
    IR[i]->enable(TIME_STEP);
  } 
  
  //Ultrasonic sensors
  for (int i=0; i<3; i++){
    DS[i] = robot->getDistanceSensor(DS_names[i]);
    DS[i]->enable(TIME_STEP);
  } 
 
  while (robot->step(TIME_STEP) != -1){   // main code here
    cout << "--------------Robocop---------------" << endl; 
    line_follow();
    wall_follow();
 };
  delete robot;
  return 0;
}

void delay(float time){
  float current_time = robot->getTime();
  while (robot->step(TIME_STEP) != -1){
    if (robot->getTime() > (current_time + time-0.032)){
      break;
    }
  };
  return;
}


float PID_CV(){
  int IR_values[6];
  float control_val;

  for (int j=0; j<6; j++) {
      double IR_reading = IR[j]->getValue();
      if (IR_reading > IR_TRESH) {
        IR_values[j] = 0;
      }
      else {
        IR_values[j] = -(-5+2*j);
      }
    }
   
  error = 0;
  for (int i=0; i<6; i++){
    //cout << IR_values[i] << " " ;
    error += IR_values[i];
  }
      
  if (error%2 == 0){
    error /= 2;
  }
  // cout << error << endl;
  control_val = Kp*error + Kd*(error-previous_error);
  previous_error = error;
  return control_val;
}

void line_follow(){
  cout << "[INFO] line following mode activated" << endl; 
  // CV negative turn left, otherwise turn right
  while (robot->step(TIME_STEP) != -1){
    float CV = PID_CV();
    if (CV<0) {
      motor_l->setVelocity(NORM_SPEED+CV);
      motor_r->setVelocity(NORM_SPEED);
    }
    else if (CV>0) {
      motor_l->setVelocity(NORM_SPEED);
      motor_r->setVelocity(NORM_SPEED-CV);
    }
    else {
      motor_l->setVelocity(NORM_SPEED);
      motor_r->setVelocity(NORM_SPEED);
    }
    if ((DS[0]->getValue() < HALF_WALL_GAP) and (DS[2]->getValue() < HALF_WALL_GAP)){
      break;
    }
  };
  return;
}

void go_ahead(){
  motor_l->setVelocity(NORM_SPEED);
  motor_r->setVelocity(NORM_SPEED);
  return;
}

void stop(){
  motor_l->setVelocity(0);
  motor_r->setVelocity(0);
  return;
}

void turn_right(short int degree){
  if (degree == 90){
    encoder_l_reading = encoder_l->getValue();
    while (robot->step(TIME_STEP) != -1){
      motor_l->setVelocity(NORM_SPEED);
      motor_r->setVelocity(-NORM_SPEED);
      if (encoder_l->getValue() > (encoder_l_reading + 2.75)){
        stop();
        break;
      }
    };
  }
  else{
    motor_l->setVelocity(NORM_SPEED);
    motor_r->setVelocity(0);
  }
  return;
}

void turn_left(short int degree){
  if (degree == 90){
    encoder_r_reading = encoder_r->getValue();
    while (robot->step(TIME_STEP) != -1){
      motor_r->setVelocity(NORM_SPEED);
      motor_l->setVelocity(-NORM_SPEED);
      if (encoder_r->getValue() > (encoder_r_reading + 2.75)){
        stop();
        break;
      }
    };
  } 
  else{
    motor_r->setVelocity(NORM_SPEED);
    motor_l->setVelocity(0);
  }
  return;
}

void turn_back(){
  encoder_l_reading = encoder_l->getValue();
  while (robot->step(TIME_STEP) != -1){
    motor_r->setVelocity(-NORM_SPEED);
    motor_l->setVelocity(NORM_SPEED);
    if (encoder_l->getValue() > (encoder_l_reading + 5.5)){
      stop();
      break;
    }
  };
  return;
}

void goto_next_square(float delay){
  go_ahead();
  encoder_l_reading = encoder_l->getValue();
  while (robot->step(TIME_STEP) != -1){
    double DS_readings[3] = {};
    for (int i=0; i<3; i++){
      DS_readings[i] = DS[i]->getValue();
    }
    if ((encoder_l->getValue() > (encoder_l_reading + delay)) or (DS_readings[1]<(HALF_WALL_GAP/2.25))){
      break;
    }
  };
  stop();
  return;
}


void wall_follow(){
  // left, front, right
  cout << "[INFO] wall following mode activated" << endl; 
  while (robot->step(TIME_STEP) != -1){
    double DS_readings[3] = {};
    for (int i=0; i<3; i++){
      DS_readings[i] = DS[i]->getValue();
    }
    //cout << "[INFO]" << "  LS : " << DS_readings[0] << " FS : " << DS_readings[1] << " RS : " << DS_readings[2] << endl;

    if (DS_readings[2] > HALF_WALL_GAP*1){
      cout << "[INFO] wall following -> turn right" << endl;
      goto_next_square(5);
      turn_right(90);
      goto_next_square(6.5);
    }
    // else if (DS_readings[1] > HALF_WALL_GAP*1.1){
      // cout << "[INFO] go ahead" << endl;
      // ;
    // }
    else if ((DS_readings[0] > HALF_WALL_GAP*1) and (DS_readings[1] < HALF_WALL_GAP/3)){
      cout << "[INFO] wall following -> turn left" << endl;
      goto_next_square(5);
      turn_left(90);
      goto_next_square(6);
    }
    else if ((DS_readings[0] < HALF_WALL_GAP/1.5) and (DS_readings[1] < HALF_WALL_GAP/1.5) and (DS_readings[2] < HALF_WALL_GAP/1.5)){
      cout << "[INFO] wall following -> turn back" << endl;
      goto_next_square(5);
      turn_back();
      goto_next_square(5);
    }
    if (DS[0]->getValue() < HALF_WALL_GAP/4){
      motor_r->setVelocity(NORM_SPEED*0.85);
      motor_l->setVelocity(NORM_SPEED);
   }
    else if (DS[2]->getValue() < HALF_WALL_GAP/4){
      motor_r->setVelocity(NORM_SPEED);
      motor_l->setVelocity(NORM_SPEED*0.85);
   }
    else{
      motor_r->setVelocity(NORM_SPEED);
      motor_l->setVelocity(NORM_SPEED);
   }
 };
}
