
State mainWaitingForStartState(){
  info(F("Production State"));
  elevatorSM.Set(elevatorDownState);
  gripperSM.Set(gripperOpenState);
  sm.Set(mainWaitingForStartState_b);
}

State mainWaitingForStartState_b(){
  if (goButton.fell()){
    sm.Set(mainMovingToSpentReactorState_h,mainMovingToSpentReactorState_b);
  }

  if (digitalRead(PIN_BUMPER_L) == LOW){
    sm.Set(mainShowSensorInfoState);
  }
}

State mainShowSensorInfoState(){
  if (sm.Timeout(100)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(analogRead(PIN_LINE_SENSOR_L));
    lcd.setCursor(4,0);
    lcd.print(analogRead(PIN_LINE_SENSOR_R));
    lcd.setCursor(8,0);
    lcd.print(analogRead(PIN_LINE_SENSOR_CROSS));
    lcd.setCursor(0,1);
    lcd.print(digitalRead(PIN_BUMPER_L));
    lcd.print(digitalRead(PIN_BUMPER_R));
    lcd.print(digitalRead(PIN_V_SWITCH));
    lcd.print(digitalRead(PIN_ELEVATOR_UPPER));
    lcd.print(digitalRead(PIN_ELEVATOR_LOWER));
    lcd.setCursor(6,1);
    lcd.print(PIN_ELEVATOR_POT);
    sm.Set(mainShowSensorInfoState);
  }

  if (goButton.fell()){
    sm.Set(mainWaitingForStartState);
  }
}

boolean processNavigate(){
  byte nextMove = navi.getNextAction();
  //lcd.print(nextMove);
  switch (nextMove){
    case FOLLOW_LINE:
      setLineFollowStopCondition(1,0,0);
      moveSM.Set(lineFollowState);
      break;
    case TURN_90_CCW:
      moveSM.Set(turnLeftState);
      break;
    case TURN_90_CW:
      moveSM.Set(turnRightState);
      break;
    case DONE:
      moveSM.Set(stopState);
      return true;
      break;
    default:
      severe(F("Unexpected navigation"));
  }
  return false;
}


State mainMovingToSpentReactorState_h(){
  elevatorSM.Set(elevatorDownState);
  gripperSM.Set(gripperOpenState);
  if (!reactorAReplaced){
    navi.setNavigation(currentPosition, REACTOR_A);
    info(F("Heading R A"));
    currentPosition = REACTOR_A;
    reactorAReplaced = true;
    return;
  }
  else if (!reactorBReplaced){
    navi.setNavigation(currentPosition, REACTOR_B);
    info(F("Heading R B"));
    currentPosition = REACTOR_B;
    reactorBReplaced = true;
    return;
  }
}

State mainMovingToSpentReactorState_b(){
  if (moveSM.Finished) {
    boolean done = processNavigate();
    if (done){
      sm.Set(mainAlignToReactorState_h,mainAlignToSpentReactorState_b);
    }
  }
}

/*
Align to the reactor.
*/
State mainAlignToReactorState_h(){
  setLineFollowStopCondition(0,1,1);
  moveSM.Set(lineFollowState);
}

State mainAlignToSpentReactorState_b(){
  if (moveSM.Finished) {
    sm.Set(mainExtractingRodFromReactorState_1);
  }
}

State mainAlignToEmptyReactorState_b(){
  if (moveSM.Finished) {
    //sm.Set(mainExtractingRodFromReactorState_1);
    //TODO
  }
}


/*
Extract the rod from the reactor
*/
State mainExtractingRodFromReactorState_1(){
  gripperSM.Set(gripperHoldState);
  sm.Set(mainExtractingRodFromReactorState_2);
}

State mainExtractingRodFromReactorState_2(){
  if (gripperSM.Finished){
    elevatorSM.Set(elevatorUpState);
    sm.Set(mainExtractingRodFromReactorState_3);
  }
}

State mainExtractingRodFromReactorState_3(){
  if (elevatorSM.Finished){
    sm.Set(mainRetractAndTurnAround_1);
  }
}

State mainRetractAndTurnAround_1(){
  info("Retracting");
  moveSM.Set(retractState);
  sm.Set(mainRetractAndTurnAround_2);
}

State mainRetractAndTurnAround_2(){
  if (moveSM.Finished) {
    info("Turning around");
    moveSM.Set(turnAroundState);
    sm.Finish(); //TODO
  }
}

State mainMovingToSpentStorageState(){
  
}

State insertingRodToSpentStorageState(){

}

State movingToNewStorageState(){

}

State extractingRodFromNewStorageState(){

}

State movingToEmptyReactorState(){

}

State insertingRodToReactorState(){

}
