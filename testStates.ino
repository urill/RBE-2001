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

State blahState_h(){
  moveSM.Set(lineFollowState);
}

State blahState_b(){
  if (moveSM.Finished) {
    //testSM.Set(blahState2_h,blahState2_b);
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
