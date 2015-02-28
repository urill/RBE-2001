#include "LineNavi.h"
#include "Arduino.h"

void LineNavi::setNavigation(byte from, byte to){
  this->from = from;
  this->to = to;
  nextIndex = 0;
}

byte LineNavi::getNextAction(){
  if (nextIndex > NAVI_ARRAY_MAX) return DONE;
  byte result = pgm_read_byte(&ROUTES[from][to][nextIndex]);
  nextIndex++;
  return result;
}
