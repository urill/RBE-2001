
State blahState_h(){
  moveSM.Set(turnRightState);
  Serial.println("turn right");
}

State blahState_b(){
  if (moveSM.Finished && moveSM.Timeout(4000)) {
    testSM.Set(blahState2_h,blahState2_b);
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
