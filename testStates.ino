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

State blahState_h(){
  moveSM.Set(lineFollowState);
}

State blahState_b(){
  if (moveSM.Finished) {
    //testSM.Set(blahState2_h,blahState2_b);
  }
}

State testElevatorStateU_h(){
  //Serial.println("in u h");
  Serial.println("Elevator Up");
  elevatorSM.Set(elevatorUpState);
  gripperSM.Set(gripperHoldState);
}

State testElevatorStateU_b(){
  //Serial.println("in u b");
  if (elevatorSM.Finished && testSM.Timeout(4000)) {
    testSM.Set(testElevatorStateD_h,testElevatorStateD_b);
  }
}

State testElevatorStateD_h(){
  //Serial.println("in d h");
  Serial.println("Elevator Down");
  elevatorSM.Set(elevatorDownState);
  gripperSM.Set(gripperOpenState);
}

State testElevatorStateD_b(){
  //Serial.println("in d b");
  if (elevatorSM.Finished && testSM.Timeout(4000)) {
    gripperSM.Set(gripperHoldState);
    testSM.Set(testElevatorStateU_h,testElevatorStateU_b);
  }
}
