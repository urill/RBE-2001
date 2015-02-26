State elevatorDownState(){

}

State elevatorUpState(){

}

State gripperOpenState(){
  gripper.write(GRIPPER_OPEN_POSITION);
  gripperSM.Set(gripperWaitState);
}

State gripperHoldState(){
  gripper.write(GRIPPER_HOLD_POSITION);
  gripperSM.Set(gripperWaitState);
}

State gripperCloseState(){
  gripper.write(GRIPPER_CLOSE_POSITION);
  gripperSM.Set(gripperWaitState);
}

State gripperWaitState(){
  if (gripperSM.Timeout(GRIPPER_ACTION_TIMEOUT)){
    gripperSM.Finish();
  }
}
