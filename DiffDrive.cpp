#include "DiffDrive.h"
DiffDrive::DiffDrive(int pinLeft, int pinRight){
  leftWheel.attach(pinLeft,1000,2000);
  rightWheel.attach(pinRight,1000,2000);
}

void DiffDrive::go(int speed,int steer){
  int _speed = map(speed,-100,100,0,180);
  int _steer = map(steer,-100,100,0,90);
  int _leftOutput = constrain(_speed + _steer,0,180);
  int _rightOutput = constrain(_speed - _steer,0,180);
  leftWheel.write(_leftOutput);
  rightWheel.write(_rightOutput);
}

void DiffDrive::stop(){
  this->go(0,0);
}
