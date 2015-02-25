State turnLeftState(){
  drive.go(0,-TURN_SPEED);
  moveSM.Set(turnWaitState);
}

State turnRightState(){
  drive.go(0,TURN_SPEED);
  moveSM.Set(turnWaitState);
}

State turnWaitState(){
  if (moveSM.Timeout(TURN_TIMEOUT)){
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
  }
}

boolean processCrossingLine(){
  int crossSensor = analogRead(PIN_LINE_SENSOR_CROSS);
  if (crossSensor < LINE_FOLLOW_CROSSING_THRESHOLD){
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
  lineFollowSensorDifference = (double) (rightSensor - leftSensor);
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
