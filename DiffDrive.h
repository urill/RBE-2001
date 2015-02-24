#include "Arduino.h"
#include <Servo.h>

class DiffDrive{
public:
  void go(int speed,int steer);
  void stop();
};
