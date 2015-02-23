#include <Servo.h>
#include "SM.h"
#include "LineNavi.h"
#include "DiffDrive.h"

#define PIN_LEFT_WHEEL 4
#define PIN_RIGHT_WHEEL 5

#define TURN_SPEED 50
#define LINE_FOLLOW_SPEED 50
#define TURN_TIMEOUT 1000
#define TURN_AROUND_TIMEOUT 2000
#define LINE_FOLLOW_TIMEOUT 2000


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
