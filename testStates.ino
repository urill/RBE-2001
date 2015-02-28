State testHoldRod_0(){
  elevatorSM.Set(elevatorUpState);
  gripperSM.Set(gripperOpenState);
  info("insert rod");
  testSM.Set(testTurnLeft_0);
}

State testTurnLeft_0(){
  //info("Waiting for Go button");
  if (goButton.fell()){
    gripperSM.Set(gripperHoldState);
    testSM.Set(testTurnLeft_01);
  }
  if (testSM.Timeout(100)){
  lcd.clear();
  lcd.print(F("Line sensor"));
  lcd.setCursor(0,1);
  lcd.print(analogRead(PIN_LINE_SENSOR_L));
  lcd.print(F(" "));
  lcd.print(analogRead(PIN_LINE_SENSOR_R));
  lcd.print(F(" "));
  lcd.print(analogRead(PIN_LINE_SENSOR_CROSS));
  testSM.Set(testTurnLeft_0);
  }
}

State testTurnLeft_01(){
  if (elevatorSM.Finished && gripperSM.Finished){
    testSM.Set(testTurnLeft_1);
  }
}

State testTurnLeft_1(){
  info("following line");
  stopOnVSwitch = false;
  stopOnCrossLine = true;
  moveSM.Set(lineFollowState);
  testSM.Set(testTurnLeft_2);
}

State testTurnLeft_2(){
  if (moveSM.Finished) {
    info("Turning Left");
    moveSM.Set(turnLeftState);
    testSM.Set(testTurnLeft_3);
    //testSM.Finish();
  }
}

State testTurnLeft_3(){
  if (moveSM.Finished) {
    info("following line");
    stopOnVSwitch = false;
    stopOnCrossLine = true;
    moveSM.Set(lineFollowState);
    testSM.Set(testTurnLeft_4);
  }
}

State testTurnLeft_4(){
  if (moveSM.Finished){
    info("approaching");
    stopOnVSwitch = true;
    stopOnCrossLine = false;
    stopOnBumperSwitch = true;
    moveSM.Set(lineFollowState);
    testSM.Set(testTurnLeft_5);
  }
}

State testTurnLeft_5(){
  if (moveSM.Finished && testSM.Timeout(3000)){
    gripperSM.Set(gripperOpenState);
    testSM.Set(testTurnLeft_6);
  }
}

State testTurnLeft_6(){
  if (gripperSM.Finished){
    elevatorSM.Set(elevatorDownState);
    testSM.Set(testTurnLeft_7);
  }
}

State testTurnLeft_7(){
  if (elevatorSM.Finished && testSM.Timeout(5000)){
    testSM.Set(testHoldRod_0);
  }
}

State sensorReadoutState1(){
  if (testSM.Timeout(100)){
  lcd.clear();
  lcd.print(F("Line sensor"));
  lcd.setCursor(0,1);
  lcd.print(analogRead(PIN_LINE_SENSOR_L));
  lcd.print(F(" "));
  lcd.print(analogRead(PIN_LINE_SENSOR_R));
  lcd.print(F(" "));
  lcd.print(analogRead(PIN_LINE_SENSOR_CROSS));
  testSM.Set(sensorReadoutState1);
  }
}

State sensorReadoutState2(){
  if (testSM.Timeout(100)){
    /*
  lcd.clear();
  lcd.print(F("Elevator Pot"));
  lcd.setCursor(0,1);
  lcd.print(analogRead(PIN_ELEVATOR_POT));
  */
  Serial.println(analogRead(PIN_LINE_SENSOR_L));
  Serial.println(analogRead(PIN_LINE_SENSOR_R));
  Serial.println("");
  testSM.Set(sensorReadoutState2);


  }
}

State sensorReadoutState3(){
  if (testSM.Timeout(100)){
  Serial.println(analogRead(PIN_ELEVATOR_POT));
  testSM.Set(sensorReadoutState3);
  }
}

State testFollowLine_1(){
  info("following line");
  stopOnVSwitch = false;
  stopOnCrossLine = true;
  moveSM.Set(lineFollowState);
  testSM.Set(testFollowLine_2);
}

State testFollowLine_2(){
  if (moveSM.Finished) {
    info("turning around");
    moveSM.Set(turnAroundState);
    testSM.Set(testFollowLine_3);
  }
}

State testFollowLine_3(){
  if (moveSM.Finished) {
    testSM.Set(testFollowLine_1);
  }
}


State blahState_h(){
  info("following line");
  stopOnVSwitch = true;
  moveSM.Set(lineFollowState);
  elevatorSM.Set(elevatorDownState);
  gripperSM.Set(gripperOpenState);
}

State blahState_b(){
  if (moveSM.Finished) {
    testSM.Set(testGripState_h,testGripState_b);
  }
}

State testGripState_h(){
  info("gripper hold");
  gripperSM.Set(gripperHoldState);
}

State testGripState_b(){
  if (gripperSM.Finished){
    testSM.Set(testElevatorStateU_h,testElevatorStateU_b);
  }
}

State testElevatorStateU_h(){
  info("elevator up");
  elevatorSM.Set(elevatorUpState);
}

State testElevatorStateU_b(){
  if (elevatorSM.Finished) {
    gripperSM.Set(gripperOpenState);
    testSM.Finish();
  }
}

/*
State testElevatorStateD_h(){
  info("elevator down")
  elevatorSM.Set(elevatorDownState);
}

State testElevatorStateD_b(){
  if (elevatorSM.Finished) {
    testSM.Set(testGripState_h,testGripState_b);
  }
}
*/
