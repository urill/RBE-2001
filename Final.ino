#include <Servo.h>
#include <PID_v1.h>
#include <SM.h>
#include <LiquidCrystal.h>
#include <Bounce2.h>
#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>
#include "LineNavi.h"
#include "DiffDrive.h"
#include "BluetoothDefinitions.h"

#define PIN_GO_BUTTON 2
#define PIN_LEFT_WHEEL 4
#define PIN_RIGHT_WHEEL 5
#define PIN_ELEVATOR_MOTOR 6
#define PIN_GRIPPER 7
#define PIN_LINE_SENSOR_L A0
#define PIN_LINE_SENSOR_R A1
#define PIN_LINE_SENSOR_CROSS A2
#define PIN_ELEVATOR_POT A3
#define PIN_V_SWITCH 22
#define PIN_ELEVATOR_UPPER 23
#define PIN_ELEVATOR_LOWER 24
#define PIN_BUMPER_L 25
#define PIN_BUMPER_R 26
#define PIN_RADIATION_LED 27

#define TURN_SPEED 28
#define TURN_FORWARD_SPEED 10
#define LINE_FOLLOW_SPEED 20

#define LINE_FOLLOW_KP 0.017
#define LINE_FOLLOW_KI 0.55
#define LINE_FOLLOW_KD 0.000015

#define LINE_FOLLOW_SAMPLING_TIMEOUT 50
#define LINE_FOLLOW_CROSSING_THRESHOLD 700
#define LINE_FOLLOW_TURNING_THRESHOLD 700 //was 900
#define LINE_SENSOR_RIGHT_OFFSET 0 //was -60

#define LINE_FOLLOW_CROSSING_IGNORE_TIME 300
#define TURN_90_MIN_TIME 1100
#define TURN_180_MIN_TIME 1800 //chaanged with fresh battery origonally 2400

#define TURN_180_FORWARD 4

#define TURN_STOP_DELAY 500

#define RETRACT_SPEED 30
#define RETRACT_TIME 800
#define BACKUP_VSWITCH_MIN_TIME 1500
#define RETRACT_STEER -5

#define TURN_TIMEOUT 1700
#define TURN_AROUND_TIMEOUT 4800
#define LINE_FOLLOW_TIMEOUT 10000

#define GRIPPER_OPEN_POSITION 90
#define GRIPPER_HOLD_POSITION 165
#define GRIPPER_CLOSE_POSITION 170
#define GRIPPER_ACTION_TIMEOUT 500

#define POT_LOWER_VALUE 130
#define POT_UPPER_VALUE 206
#define ELEVATOR_UP_THROTTLE 1200
#define ELEVATOR_DOWN_THROTTLE 1700
#define ELEVATOR_UP_APPROACHING_THROTTLE 1300
#define ELEVATOR_DOWN_APPROACHING_THROTTLE 1600
#define ELEVATOR_UP_APPROACHING_THRESHOLD 170
#define ELEVATOR_DOWN_APPROACHING_THRESHOLD 160
#define ELEVATOR_MOVE_TIMEOUT 5000
#define ELEVATOR_UP_HOLD_THROTTLE 1460



LiquidCrystal lcd(40,41,42,43,44,45);

double lineFollowSensorDifference, lineFollowSteer;
PID lineFollowPID(&lineFollowSensorDifference, &lineFollowSteer, 0 ,0,0,0, DIRECT); //LINE_FOLLOW_KP,LINE_FOLLOW_KI,LINE_FOLLOW_KD,

Servo leftWheel;
Servo rightWheel;
Servo elevatorMotor;
Servo gripper;

LineNavi navi;
DiffDrive drive;

Bounce goButton = Bounce();

SM sm(mainWaitingForStartState);
SM moveSM(stopState);
SM elevatorSM(Nop);
SM gripperSM(Nop);
SM navigationSM(Nop);
SM bluetoothReceiveSM(bluetoothReceiveState);
SM bluetoothSendSM(bluetoothSendWaitStartState);

SM testSM(testHoldRod_0);
//SM testSM(testTurnLeft_0);
//SM testSM(sensorReadoutState1);
//SM testSM(testFollowLine_1);
//SM testSM(blahState_h,blahState_b);
//SM testSM(testElevatorStateU_h,testElevatorStateU_b);

boolean reactorAReplaced = false;
boolean reactorBReplaced = false;

boolean stopOnCrossLine;
boolean stopOnVSwitch;
boolean stopOnBumperSwitch;

byte currentPosition = NEW_1;

boolean errorFlag = false;

unsigned long lastLineFollowStarted;

ReactorProtocol pcol(byte(BLUETOOTH_ADDRESS));
BluetoothClient bt;
BluetoothMaster btmaster;

boolean bluetoothStopFlag;
boolean bluetoothResumeFlag;
byte bluetoothSpentAvailability;
byte bluetoothNewAvailability;
byte radiationLevel;

boolean bluetoothConnected;

void setup() {
  pinMode(PIN_V_SWITCH, INPUT_PULLUP);
  pinMode(PIN_GO_BUTTON, INPUT_PULLUP);
  pinMode(PIN_ELEVATOR_UPPER, INPUT_PULLUP);
  pinMode(PIN_ELEVATOR_LOWER, INPUT_PULLUP);
  pinMode(PIN_BUMPER_L, INPUT_PULLUP);
  pinMode(PIN_BUMPER_R, INPUT_PULLUP);
  pinMode(PIN_LINE_SENSOR_L,INPUT);
  pinMode(PIN_LINE_SENSOR_R,INPUT);
  pinMode(PIN_LINE_SENSOR_CROSS,INPUT);
  pinMode(PIN_ELEVATOR_POT,INPUT);
  pinMode(PIN_LEFT_WHEEL,OUTPUT);
  pinMode(PIN_RIGHT_WHEEL,OUTPUT);
  pinMode(PIN_ELEVATOR_MOTOR,OUTPUT);
  pinMode(PIN_GRIPPER,OUTPUT);
  pinMode(PIN_RADIATION_LED, OUTPUT);

  setRadiationLevel(0);

  goButton.attach(PIN_GO_BUTTON);
  goButton.interval(5);

  Serial.begin(115200);
  Serial1.begin(115200);

  lcd.begin(16,2);
  leftWheel.attach(PIN_LEFT_WHEEL);
  rightWheel.attach(PIN_RIGHT_WHEEL);
  elevatorMotor.attach(PIN_ELEVATOR_MOTOR,1000,2000);
  gripper.attach(PIN_GRIPPER);
  lineFollowPID.SetOutputLimits(-100, 100);
  lineFollowPID.SetSampleTime(1);
  lineFollowPID.SetTunings(LINE_FOLLOW_KP,LINE_FOLLOW_KI,LINE_FOLLOW_KD);
  lineFollowPID.SetMode(AUTOMATIC);
  lcd.print("Setup Finished");
}

void loop() {
  goButton.update();
  EXEC(sm);
  //EXEC(testSM);
  EXEC(moveSM);
  EXEC(elevatorSM);
  EXEC(gripperSM);
  EXEC(bluetoothReceiveSM);
  EXEC(bluetoothSendSM);
}

void verbose(String s){
  #ifdef VERBOSE
  if (errorFlag) return;
  lcd.clear();
  lcd.print(s);
  #endif
}

void info(String s){
  if (errorFlag) return;
  lcd.clear();
  lcd.print(s);
}

void severe(String s){
  /*
  if (!errorFlag) {
    lcd.clear();
  }
  */
  lcd.clear();
  errorFlag = true;
  lcd.print(s);
}

void debug(String s){
  if (errorFlag) return;
  lcd.clear();
  lcd.print(s);
}

void setRadiationLevel(byte level){
  radiationLevel = level;
  if (level == CARRYING_SPENT_FUEL_ROD ||
    level == CARRYING_NEW_FUEL_ROD){
      digitalWrite(PIN_RADIATION_LED,HIGH);
  } else {
    digitalWrite(PIN_RADIATION_LED,LOW);
  }
}
