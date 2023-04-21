#include <Joystick.h>

Joystick_ piss;

void setup() {
  // put your setup code here, to run once:
  piss.useManualSend(true);
  piss.begin();

}


void loop() {

  piss.X(9);
  piss.Y(5));

  piss.send_now();
}

