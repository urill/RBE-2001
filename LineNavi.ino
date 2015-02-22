#include "LineNaviRoutes.h"
/*
#define EAST  0
#define NORTH 1
#define WEST  2
#define SOUTH 3
*/
#define NAVI_ARRAY_MAX 6

#define DONE         0
#define TURN_90_CCW  1
#define TURN_180_CCW 2
#define TURN_90_CW   3
#define TURN_180_CW  4
#define FOLLOW_LINE  5

#define REACTOR_A 1
#define REACTOR_B 2
#define SPENT_1 3
#define SPENT_2 4
#define SPENT_3 5
#define SPENT_4 6
#define NEW_1 7
#define NEW_2 8
#define NEW_3 9
#define NEW_4 10

class LineNavi{
public:
  LineNavi();
  byte getNextAction();
  void setNavigation(byte from, byte to);
private:
  byte nextIndex;
  byte from;
  byte to;
};

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
