#include <Servo.h>
#include <PID_v1.h>
#include <SM.h>
#include <LiquidCrystal.h>
#include "LineNavi.h"
#include "DiffDrive.h"

#define PIN_GO_BUTTON 2
#define PIN_LEFT_WHEEL 4
#define PIN_RIGHT_WHEEL 5
#define PIN_ELEVATOR_MOTOR 6
#define PIN_GRIPPER 7
#define PIN_LINE_SENSOR_L A0
#define PIN_LINE_SENSOR_R A1
#define PIN_LINE_SENSOR_CROSS A2
#define PIN_V_SWITCH A3

#define TURN_SPEED 50
#define LINE_FOLLOW_SPEED 50

#define TURN_TIMEOUT 1000
#define TURN_AROUND_TIMEOUT 2000
#define LINE_FOLLOW_TIMEOUT 10000

#define LINE_FOLLOW_KP 1
#define LINE_FOLLOW_KI 0
#define LINE_FOLLOW_KD 0
#define LINE_FOLLOW_SAMPLING_TIMEOUT 50
#define LINE_FOLLOW_CROSSING_THRESHOLD 500

#define GRIPPER_OPEN_POSITION 90
#define GRIPPER_HOLD_POSITION 80
#define GRIPPER_CLOSE_POSITION 70

LiquidCrystal lcd(40,41,42,43,44,45);

double lineFollowSensorDifference, lineFollowSteer;
PID lineFollowPID(&lineFollowSensorDifference, &lineFollowSteer, 0 ,LINE_FOLLOW_KP,LINE_FOLLOW_KI,LINE_FOLLOW_KD, DIRECT);

Servo leftWheel;
Servo rightWheel;
Servo elevatorMotor;
Servo gripper;

LineNavi navi();
DiffDrive drive;


SM sm(waitingForStartState_h,waitingForStartState_b);
SM moveSM(stopState);
SM elevatorSM();
SM gripperSM();
SM navigationSM();

SM testSM(blahState_h,blahState_b);

boolean reactorAReplaced = false;
boolean reactorBReplaced = false;

boolean stopOnCrossLine;
boolean stopOnVSwitch;

byte currentPosition = NEW_1;

void setup() {
  //Serial.begin(115200);
  lcd.begin(16,2);
  leftWheel.attach(PIN_LEFT_WHEEL);
  rightWheel.attach(PIN_RIGHT_WHEEL);
  lineFollowPID.SetOutputLimits(-100, 100);
  lineFollowPID.SetMode(MANUAL);
}

void loop() {
  EXEC(sm);
  EXEC(moveSM);
  EXEC(testSM);
}

void verbose(String s){
  lcd.clear();
  lcd.print(s);
}

void info(String s){
  lcd.clear();
  lcd.print(s);
}

void severe(String s){
  lcd.clear();
  lcd.print(s);
}
