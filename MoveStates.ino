State turnLeftState(){
  drive.go(TURN_FORWARD_SPEED,-TURN_SPEED);
  moveSM.Set(turnWaitState);
}

State turnRightState(){
  drive.go(TURN_FORWARD_SPEED,TURN_SPEED);
  moveSM.Set(turnWaitState);
}

State turnWaitState(){
  if (moveSM.Timeout(TURN_TIMEOUT)){
    moveSM.Set(stopState);
    severe(F("Turn 90 timeout"));
  }

  int leftSensor = analogRead(PIN_LINE_SENSOR_L);
  int rightSensor = analogRead(PIN_LINE_SENSOR_R);

  if (moveSM.Timeout(TURN_90_MIN_TIME) &&
  (leftSensor > LINE_FOLLOW_TURNING_THRESHOLD ||
    rightSensor > LINE_FOLLOW_TURNING_THRESHOLD)){
    moveSM.Set(stopState);
  }

}

State turnAroundState(){
  drive.go(0,TURN_SPEED);
  moveSM.Set(turnAroundWaitState);
}

State turnAroundWaitState(){
  if (moveSM.Timeout(TURN_AROUND_TIMEOUT)){
    moveSM.Set(stopState);
    severe(F("Turn around timeout"));
  }

  int leftSensor = analogRead(PIN_LINE_SENSOR_L);
  int rightSensor = analogRead(PIN_LINE_SENSOR_R);

  if (moveSM.Timeout(TURN_180_MIN_TIME) &&
  (leftSensor > LINE_FOLLOW_TURNING_THRESHOLD ||
    rightSensor > LINE_FOLLOW_TURNING_THRESHOLD)){
    moveSM.Set(stopState);
  }

}

boolean processCrossingLine(){
  int crossSensor = analogRead(PIN_LINE_SENSOR_CROSS);
  if (moveSM.Timeout(LINE_FOLLOW_CROSSING_IGNORE_TIME) &&
   crossSensor > LINE_FOLLOW_CROSSING_THRESHOLD){
    moveSM.Set(stopState);
    return true;
  }
  return false;
}

boolean processVSwitch(){
  if (digitalRead(PIN_V_SWITCH) == LOW){
    moveSM.Set(stopState);
    return true;
  }
  return false;
}

State lineFollowState(){
  if (stopOnCrossLine && processCrossingLine()) return;
  if (stopOnVSwitch && processVSwitch()) return;
  int leftSensor = analogRead(PIN_LINE_SENSOR_L);
  int rightSensor = analogRead(PIN_LINE_SENSOR_R);
  lineFollowSensorDifference = (double) (leftSensor - rightSensor);
  lineFollowPID.Compute();
  drive.go(LINE_FOLLOW_SPEED,lineFollowSteer);
  moveSM.Set(lineFollowWaitState);
}

State lineFollowWaitState(){
  if (stopOnCrossLine && processCrossingLine()) return;
  if (stopOnVSwitch && processVSwitch()) return;
  if (moveSM.Timeout(LINE_FOLLOW_SAMPLING_TIMEOUT)){
    moveSM.Set(lineFollowState);
  }
}

State stopState(){
  drive.stop();
  moveSM.Finish();
}
