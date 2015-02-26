#include "LineNaviRoutes.h"
/*
                       spent rods

                     +6    +5    +4    +3
                     |     |     |     |
                   +---+ +---+ +---+ +---+
                     |     |     |     |
              +      |     |     |     |      +
              |      |     |     |     |      |
reactor A +---------------------------------------+ reactor B
          1   |      |     |     |     |      |   2
              +      |     |     |     |      +
                     |     |     |     |
                   +---+ +---+ +---+ +---+
                     |     |     |     |
                     +7    +8    +9    +10

                        new rods

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
  byte getNextAction();
  void setNavigation(byte from, byte to);
private:
  byte nextIndex;
  byte from;
  byte to;
};
