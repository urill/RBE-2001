#include "LineNavi.h"
#include "Arduino.h"

LineNavi::LineNavi(){
  nextIndex = 0;
  int blah = ROUTES[0][0][0];
}

void LineNavi::setNavigation(byte from, byte to){
  this->from = from;
  this->to = to;
  nextIndex = 0;
}

byte LineNavi::getNextAction(){
  if (nextIndex > NAVI_ARRAY_MAX) return DONE;
  byte result = ROUTES[from][to][nextIndex];
  nextIndex++;
  return result;
}
