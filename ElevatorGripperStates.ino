State elevatorDownState(){
  if (digitalRead(PIN_ELEVATOR_LOWER) == LOW ||
    elevatorSM.Timeout(ELEVATOR_MOVE_TIMEOUT)){
      elevatorSM.Set(elevatorStopState);
      return;
  }

  if (analogRead(PIN_ELEVATOR_POT) > ELEVATOR_DOWN_APPROACHING_THRESHOLD){
    elevatorMotor.writeMicroseconds(ELEVATOR_DOWN_THROTTLE);
  } else {
    elevatorMotor.writeMicroseconds(ELEVATOR_DOWN_APPROACHING_THROTTLE);
  }
}

State elevatorUpState(){
  if (digitalRead(PIN_ELEVATOR_UPPER) == LOW ||
    elevatorSM.Timeout(ELEVATOR_MOVE_TIMEOUT)){
      //elevatorSM.Set(elevatorStopState);
      elevatorMotor.writeMicroseconds(ELEVATOR_UP_HOLD_THROTTLE);
      elevatorSM.Finish();
      return;
  }

  if (analogRead(PIN_ELEVATOR_POT) < ELEVATOR_UP_APPROACHING_THRESHOLD){
    elevatorMotor.writeMicroseconds(ELEVATOR_UP_THROTTLE);
  } else {
    elevatorMotor.writeMicroseconds(ELEVATOR_UP_APPROACHING_THROTTLE);
  }
}

State elevatorStopState(){
  elevatorMotor.writeMicroseconds(1500);
  elevatorSM.Finish();
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
