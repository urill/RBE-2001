#include "DiffDrive.h"
extern Servo leftWheel;
extern Servo rightWheel;

void DiffDrive::go(int speed,int steer){
  int _speed = map(speed,-100,100,1000,2000);
  int _steer = map(steer,-200,200,-1000,1000);
  int _leftOutput = constrain((3000-(_speed + _steer)),1000,2000);
  int _rightOutput = constrain(_speed - _steer,1000,2000);
  leftWheel.writeMicroseconds(_leftOutput);
  rightWheel.writeMicroseconds(_rightOutput);
}

void DiffDrive::stop(){
  this->go(0,0);
}
