
State waitingForStartState_h(){
  info(F("Ready"));
}

State waitingForStartState_b(){
  if (digitalRead(PIN_GO_BUTTON) == LOW){
    sm.Set(movingToLoadedReactorState_h,movingToLoadedReactorState_b);
  }
}

boolean processNavigate(){
  byte nextMove = navi.getNextAction();
  switch (nextMove){
    case FOLLOW_LINE:
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

State movingToLoadedReactorState_h(){
  boolean stopOnCrossLine = true;
  boolean stopOnVSwitch = false;
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

State movingToLoadedReactorState_b(){
  if (moveSM.Finished) {
    boolean done = processNavigate();
    if (done){
      sm.Set(alignToReactorState_h);
    }
  }
}

State alignToReactorState_h(){
  boolean stopOnCrossLine = false;
  boolean stopOnVSwitch = true;
  moveSM.Set(lineFollowState);
}

State alignToReactorState_b(){
  if (moveSM.Finished) {
    sm.Set(extractingRodFromReactorState_1);
  }
}

State extractingRodFromReactorState_1(){
  gripperSM.Set(gripperHoldState);
}

State extractingRodFromReactorState_2(){
  if (gripperSM.Finished){
    gripperSM.Set(gripperHoldState);
  }
}

State movingToSpentStorageState(){

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
