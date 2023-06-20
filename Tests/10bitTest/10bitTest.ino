#include <Joystick.h>

Joystick_ controller;

void setup() {
  controller.useManualSend(true);
  controller.begin();

}

void loop() {
  float sinus = sin(millis()/5000.0f);

  float input = sinus>0?1-sinus:1-sinus-2;

  input = (input+1)/2 * 1023;

  controller.Y(input);
  controller.send_now();

}