#include <Servo.h>
#include <PID_v1.h>
#include <SM.h>
#include "LineNavi.h"
#include "DiffDrive.h"

#define PIN_LEFT_WHEEL 4
#define PIN_RIGHT_WHEEL 5

#define TURN_SPEED 50
#define LINE_FOLLOW_SPEED 50
#define TURN_TIMEOUT 1000
#define TURN_AROUND_TIMEOUT 2000
#define LINE_FOLLOW_TIMEOUT 2000

#define LINE_FOLLOW_KP 1
#define LINE_FOLLOW_KI 0
#define LINE_FOLLOW_KD 0

double lineFollowSensorDifference, lineFollowSteer;
PID lineFollowPID(&lineFollowSensorDifference, &lineFollowSteer, 0 ,LINE_FOLLOW_KP,LINE_FOLLOW_KI,LINE_FOLLOW_KD, DIRECT);

Servo leftWheel;
Servo rightWheel;

SM sm(waitingForStartState);
SM moveSM(stopState);
SM testSM(blahState_h,blahState_b);

LineNavi navi();

DiffDrive drive;

void setup() {
  Serial.begin(115200);
  leftWheel.attach(PIN_LEFT_WHEEL);
  rightWheel.attach(PIN_RIGHT_WHEEL);
  lineFollowPID.SetSampleTime(50);
  lineFollowPID.SetMode(AUTOMATIC);
}

void loop() {
  EXEC(sm);
  EXEC(moveSM);
  EXEC(testSM);
}

State blahState_h(){
  moveSM.Set(turnRightState);
  Serial.println("turn right");
}

State blahState_b(){
  if (moveSM.Finished && moveSM.Timeout(4000)) {
    testSM.Set(blahState2_h,blahState2_b);
  }
}

State blahState2_h(){
  moveSM.Set(turnLeftState);
  Serial.println("turn left");
}

State blahState2_b(){
  if (moveSM.Finished && moveSM.Timeout(4000)) {
    testSM.Set(blahState_h,blahState_b);
  }
}
