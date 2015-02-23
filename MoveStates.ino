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

State lineFollowState(){
//or found the crossing line
  if (moveSM.Timeout(LINE_FOLLOW_TIMEOUT)){
    moveSM.Set(stopState);
  }
}

State alignToReactorState(){
  //go until the button depressed
}


State stopState(){
  drive.stop();
  moveSM.Finish();
}
