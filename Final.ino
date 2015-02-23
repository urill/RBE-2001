#include "SM.h"

SM sm(waitingForStartState);
SM moveSM(stopState);
//LineNavi navi();

void setup() {

}

void loop() {
  EXEC(sm);
  EXEC(moveSM);
}
