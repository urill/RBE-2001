State mainWaitingForStartState(){
  info(F("Production State"));
  elevatorSM.Set(elevatorDownState);
  gripperSM.Set(gripperOpenState);
  sm.Set(mainWaitingForStartState_b);
}

State mainWaitingForStartState_b(){
  if (goButton.fell()){
    sm.Set(mainMovingToSpentReactorState_h,mainMovingToSpentReactorState_b);
    //bluetoothSendSM.Set(bluetoothSendHBState);
  }

  if (digitalRead(PIN_BUMPER_L) == LOW){
    sm.Set(mainShowSensorInfoState);
  }

  if (digitalRead(PIN_BUMPER_R) == LOW){
    sm.Set(mainShowBluetoothInfoState);
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
    lcd.print(analogRead(PIN_ELEVATOR_POT));
    sm.Set(mainShowSensorInfoState);
  }

  if (goButton.fell()){
    sm.Set(mainWaitingForStartState);
  }
}

State mainShowBluetoothInfoState(){
  if (sm.Timeout(100)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(bluetoothSpentAvailability, HEX);
    lcd.setCursor(5,0);
    lcd.print(bluetoothNewAvailability, HEX);
    lcd.setCursor(0,1);
    lcd.print(bluetoothStopFlag);
    lcd.print(bluetoothResumeFlag);
    sm.Set(mainShowBluetoothInfoState);
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
    setRadiationLevel(CARRYING_SPENT_FUEL_ROD);
    elevatorSM.Set(elevatorUpState);
    sm.Set(mainExtractingRodFromReactorState_3);
  }
}

State mainExtractingRodFromReactorState_3(){
  if (elevatorSM.Finished){
    sm.Set(mainRetractAndTurnAroundAtSpentReactorState_1);
  }
}

State mainRetractAndTurnAroundAtSpentReactorState_1(){
  info("Retracting");
  moveSM.Set(retractState);
  sm.Set(mainRetractAndTurnAroundAtSpentReactorState_2);
}

State mainRetractAndTurnAroundAtSpentReactorState_2(){
  if (moveSM.Finished) {
    info("Turning around");
    moveSM.Set(turnAroundState);
    sm.Set(mainRetractAndTurnAroundAtSpentReactorState_3);
  }
}

State mainRetractAndTurnAroundAtSpentReactorState_3(){
  if (moveSM.Finished) {
    sm.Set(mainMovingToSpentStorageState);
  }
}

byte getClosestSpentStorage(){
    if (currentPosition == REACTOR_A){
      if (bluetoothSpentAvailability ^ SPENT_4_MASK) return SPENT_4;
      if (bluetoothSpentAvailability ^ SPENT_3_MASK) return SPENT_3;
      if (bluetoothSpentAvailability ^ SPENT_2_MASK) return SPENT_2;
      if (bluetoothSpentAvailability ^ SPENT_1_MASK) return SPENT_1;
    } else if (currentPosition == REACTOR_B){
      if (bluetoothSpentAvailability ^ SPENT_1_MASK) return SPENT_1;
      if (bluetoothSpentAvailability ^ SPENT_2_MASK) return SPENT_2;
      if (bluetoothSpentAvailability ^ SPENT_3_MASK) return SPENT_3;
      if (bluetoothSpentAvailability ^ SPENT_4_MASK) return SPENT_4;
    } else {
      severe("unexpected position");
    }

    info("no storage available");
    return 0;
}

byte getClosestNewStorage(){
    if (currentPosition == SPENT_1){
      if (bluetoothNewAvailability & NEW_4_MASK) return NEW_1;
    } else if (currentPosition == SPENT_2){
      if (bluetoothNewAvailability & NEW_3_MASK) return NEW_2;
    } else if (currentPosition == SPENT_3){
      if (bluetoothNewAvailability & NEW_2_MASK) return NEW_2;
    } else if (currentPosition == SPENT_4){
      if (bluetoothNewAvailability & NEW_1_MASK) return NEW_1;
    } else {
      severe("u/p");
      lcd.print(currentPosition);
    }
    if (bluetoothNewAvailability & NEW_1_MASK) return NEW_1;
    if (bluetoothNewAvailability & NEW_2_MASK) return NEW_2;
    if (bluetoothNewAvailability & NEW_3_MASK) return NEW_3;
    if (bluetoothNewAvailability & NEW_4_MASK) return NEW_4;
    info("no storage available");
    return 0;
}

State mainMovingToSpentStorageState(){
   if (moveSM.Finished) {
    byte nextPosition = getClosestSpentStorage();
    if (nextPosition != 0){
      navi.setNavigation(currentPosition, nextPosition);
      currentPosition = nextPosition;
      sm.Set(mainMovingToSpentStorageState_2);
      info("Moving to Spent");
    }
  }
}

State mainMovingToSpentStorageState_2(){
  if (moveSM.Finished) {
    boolean done = processNavigate();
    if (done){
      sm.Set(mainAlignToSpentStorageState);
    }
  }
}

State mainAlignToSpentStorageState(){
  info("Aligning to spent");
  setLineFollowStopCondition(0,1,1);
  moveSM.Set(lineFollowState);
  sm.Set(mainAlignToSpentStorageState_2);
}

State mainAlignToSpentStorageState_2(){
  if (moveSM.Finished) {
    sm.Set(mainInsertingRodToSpentStorageState);
  }
}

State mainInsertingRodToSpentStorageState(){
  if (moveSM.Finished) {
    gripperSM.Set(gripperOpenState);
    sm.Set(mainReleaseRodToSpentStorageState);
  }
}
State mainReleaseRodToSpentStorageState(){
  if(gripperSM.Finished){
    elevatorSM.Set(elevatorDownState);
    sm.Set(mainBackupAtSpentStorageState);
  }
}
State mainBackupAtSpentStorageState(){
  if(elevatorSM.Finished){
    moveSM.Set(retractState);
    sm.Set(mainElevatorUpAtSpentStorageState);

  }
}
State mainElevatorUpAtSpentStorageState(){
  if(moveSM.Finished){
   gripperSM.Set(gripperCloseState);
   elevatorSM.Set(elevatorUpState);
   sm.Set(mainPushInRodAtSpentStorageState);
  }
}

State mainPushInRodAtSpentStorageState(){
  if(elevatorSM.Finished){
    info(F("pushing"));
    setLineFollowStopCondition(0,1,1);
    moveSM.Set(lineFollowState);
    sm.Set(mainRetractAndTurnAroundAtSpentStorageState_1);
  }
}

State mainRetractAndTurnAroundAtSpentStorageState_1(){
  if(moveSM.Finished){
    setRadiationLevel(0);
    info("Retracting");
    moveSM.Set(retractState);
    sm.Set(mainRetractAndTurnAroundAtSpentStorageState_2);
  }
}

State mainRetractAndTurnAroundAtSpentStorageState_2(){
  if (moveSM.Finished) {
    info("Turning around");
    moveSM.Set(turnAroundState);
    sm.Set(mainRetractAndTurnAroundAtSpentStorageState_3);
  }
}

State mainRetractAndTurnAroundAtSpentStorageState_3(){
  if (moveSM.Finished) {
    sm.Set(mainMovingToNewStorageState);
  }
}

State mainMovingToNewStorageState(){
   if (moveSM.Finished) {
    byte nextPosition = getClosestNewStorage();
    if (nextPosition != 0){
      navi.setNavigation(currentPosition, nextPosition);
      currentPosition = nextPosition;
      sm.Set(mainMovingToNewStorageState_2);
      elevatorSM.Set(elevatorUpState);
      gripperSM.Set(gripperOpenState);
      info("Moving to New");
    }
  }
}

State mainMovingToNewStorageState_2(){
  if (moveSM.Finished && elevatorSM.Finished && gripperSM.Finished) {
    boolean done = processNavigate();
    if (done){
      sm.Set(mainAlignToNewStorageState);
    }
  }
}

State mainAlignToNewStorageState(){
  info("Aligning to new");
  setLineFollowStopCondition(0,1,1);
  moveSM.Set(lineFollowState);
  sm.Set(mainAlignToNewStorageState_2);
}

State mainAlignToNewStorageState_2(){
  if (moveSM.Finished) {
    sm.Set(extractingRodFromNewStorageState);
  }
}

State extractingRodFromNewStorageState(){
  gripperSM.Set(gripperHoldState);
  setRadiationLevel(CARRYING_NEW_FUEL_ROD);
  sm.Set(extractingRodFromNewStorageState_2);
}

State extractingRodFromNewStorageState_2(){
  if(gripperSM.Finished){
    sm.Set(mainRetractAndTurnAroundAtNewStorageState_1);
  }
}

State mainRetractAndTurnAroundAtNewStorageState_1(){
  if(moveSM.Finished){
    info("Retracting");
    moveSM.Set(retractState);
    sm.Set(mainRetractAndTurnAroundAtNewStorageState_2);
  }
}

State mainRetractAndTurnAroundAtNewStorageState_2(){
  if (moveSM.Finished) {
    info("Turning around");
    moveSM.Set(turnAroundState);
    sm.Set(mainRetractAndTurnAroundAtNewStorageState_3);
  }
}

State mainRetractAndTurnAroundAtNewStorageState_3(){
  if (moveSM.Finished) {
    sm.Set(movingToEmptyReactorState);
  }
}

State movingToEmptyReactorState(){//TODO
}

State insertingRodToReactorState(){

}
