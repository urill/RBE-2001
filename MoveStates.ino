void setLineFollowStopCondition(boolean crossLine, boolean vSwitch, boolean bumper){
  stopOnCrossLine = crossLine;
  stopOnVSwitch = vSwitch;
  stopOnBumperSwitch = bumper;
}

State turnLeftState(){
  verbose(F("Turn L"));
  drive.go(TURN_FORWARD_SPEED,-TURN_SPEED);
  moveSM.Set(turnLeftWaitState);
}

State turnRightState(){
  verbose(F("Turn R"));
  drive.go(TURN_FORWARD_SPEED,TURN_SPEED);
  moveSM.Set(turnRightWaitState);
}

State turnLeftWaitState(){
  if (moveSM.Timeout(TURN_TIMEOUT)){
    moveSM.Set(stopState);
    severe(F("Turn L timeout"));
  }

  int leftSensor = analogRead(PIN_LINE_SENSOR_L);
  int rightSensor = analogRead(PIN_LINE_SENSOR_R) + LINE_SENSOR_RIGHT_OFFSET;

  if (moveSM.Timeout(TURN_90_MIN_TIME) &&
  ( leftSensor > LINE_FOLLOW_TURNING_THRESHOLD ||
    rightSensor > LINE_FOLLOW_TURNING_THRESHOLD)){
    moveSM.Set(stopState);
  }

}

State turnRightWaitState(){
  if (moveSM.Timeout(TURN_TIMEOUT)){
    moveSM.Set(stopState);
    severe(F("Turn R timeout"));
  }

  int leftSensor = analogRead(PIN_LINE_SENSOR_L);
  int rightSensor = analogRead(PIN_LINE_SENSOR_R) + LINE_SENSOR_RIGHT_OFFSET;

  if (moveSM.Timeout(TURN_90_MIN_TIME) &&
  ( leftSensor > LINE_FOLLOW_TURNING_THRESHOLD ||
    rightSensor > LINE_FOLLOW_TURNING_THRESHOLD)){
    moveSM.Set(stopState);
  }

}

State turnAroundState(){
  verbose(F("Turn 180"));
  drive.go(0,TURN_SPEED);
  moveSM.Set(turnAroundWaitState);
}

State turnAroundWaitState(){
  if (moveSM.Timeout(TURN_AROUND_TIMEOUT)){
    moveSM.Set(stopState);
    severe(F("Turn around timeout"));
  }

  int leftSensor = analogRead(PIN_LINE_SENSOR_L);
  int rightSensor = analogRead(PIN_LINE_SENSOR_R) + LINE_SENSOR_RIGHT_OFFSET;

  if (moveSM.Timeout(TURN_180_MIN_TIME) &&
  (leftSensor > LINE_FOLLOW_TURNING_THRESHOLD ||
    rightSensor > LINE_FOLLOW_TURNING_THRESHOLD)){
    moveSM.Set(stopState);
  }

}

boolean processCrossingLine(){
  int crossSensor = analogRead(PIN_LINE_SENSOR_CROSS);
  if (millis() - lastLineFollowStarted > LINE_FOLLOW_CROSSING_IGNORE_TIME &&
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

boolean processBumperSwitch(){
  if (digitalRead(PIN_BUMPER_L) == LOW || digitalRead(PIN_BUMPER_R) == LOW){
    moveSM.Set(stopState);
    return true;
  }
  return false;
}

State lineFollowState(){
  verbose(F("Following Line"));
  lastLineFollowStarted = millis();
  lineFollowPID.Reset();
  if (stopOnVSwitch || stopOnBumperSwitch) {
    lineFollowPID.SetTunings(LINE_FOLLOW_SLOW_KP,LINE_FOLLOW_SLOW_KI,LINE_FOLLOW_SLOW_KD);
  } else {
    lineFollowPID.SetTunings(LINE_FOLLOW_KP,LINE_FOLLOW_KI,LINE_FOLLOW_KD);
  }
  moveSM.Set(lineFollowState_b);
}

State lineFollowState_b(){
  if (stopOnBumperSwitch && processBumperSwitch()) return;
  if (stopOnCrossLine && processCrossingLine()) return;
  if (stopOnVSwitch && processVSwitch()) return;
  int leftSensor = analogRead(PIN_LINE_SENSOR_L);
  int rightSensor = analogRead(PIN_LINE_SENSOR_R) + LINE_SENSOR_RIGHT_OFFSET;
  lineFollowSensorDifference = (double) (leftSensor - rightSensor);
  lineFollowPID.Compute();
  if (stopOnVSwitch || stopOnBumperSwitch) {
    drive.go(LINE_FOLLOW_APPROACHING_SPEED,lineFollowSteer);
  } else {
    drive.go(LINE_FOLLOW_SPEED,lineFollowSteer);
  }
  moveSM.Set(lineFollowWaitState);
}

State lineFollowWaitState(){
  if (stopOnBumperSwitch && processBumperSwitch()) return;
  if (stopOnCrossLine && processCrossingLine()) return;
  if (stopOnVSwitch && processVSwitch()) return;
  if (moveSM.Timeout(LINE_FOLLOW_SAMPLING_TIMEOUT)){
    moveSM.Set(lineFollowState_b);
  }
}

State stopState(){
  verbose(F("Stop"));
  drive.stop();
  moveSM.Finish();
}

State retractState(){
  verbose("retract");
  drive.go(-RETRACT_SPEED,0);
  moveSM.Set(retractWaitState);
}

State retractWaitState(){
  if(moveSM.Timeout(RETRACT_TIME)){
    moveSM.Set(stopState);
  }
}

State insertState(){
  verbose("retract");
  drive.go(RETRACT_SPEED,0);
  moveSM.Set(retractWaitState);
}

State insertWaitState(){
  if(moveSM.Timeout(RETRACT_TIME)){
    moveSM.Set(stopState);
  }
}
