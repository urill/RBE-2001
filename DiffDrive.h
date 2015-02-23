#include "Arduino.h"
#include <Servo.h>

class DiffDrive{
public:
  DiffDrive(int pinLeft, int pinRight);
  void go(int speed,int steer);
  void stop();
private:
  Servo leftWheel;
  Servo rightWheel;
};
