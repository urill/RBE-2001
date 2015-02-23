#include <Servo.h>
#include "SM.h"
#include "LineNavi.h"
#include "DiffDrive.h"

#define PIN_LEFT_WHEEL 4
#define PIN_RIGHT_WHEEL 5

SM sm(waitingForStartState);
SM moveSM(stopState);

LineNavi navi();
DiffDrive drive(PIN_LEFT_WHEEL,PIN_RIGHT_WHEEL);

void setup() {

}

void loop() {
  EXEC(sm);
  EXEC(moveSM);
}
