#include <Joystick.h>
#include "Wheel.h"

#define HALL1 0
#define HALL2 1
#define HALL3 2
#define HALL4 3
#define HALL5 4
#define HALL6 5

Joystick_ controller;

Wheel leftWheel;
Wheel rightWheel;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  controller.useManualSend(true);
  controller.use8bit(true);
  controller.begin();

  leftWheel.setup(true, HALL1, HALL2, HALL3, true, &controller);
  rightWheel.setup(false, HALL4, HALL5, HALL6, true, &controller);
}

void loop() {
  leftWheel.update();
  rightWheel.update();
  // wheeltwo.loop;

  if (leftWheel.controllerUpdated || rightWheel.controllerUpdated) {
    controller.send_now();
    Serial.println();
  }
}

//========MOCK SENSORS========
#if MOCK_SENSORS
unsigned long lastMockTriggerTime;
int lastMockState = 1;

int readMockSensors() {
  int output = 0;

  // calculates a sinus curve that goes from 50 to 200
  int mockFrequency = ((sin(millis() / 1200.0f) / 2) + 0.5f) * 150 + 50;

  // calulates a sinus curve and takes the sign of it;
  int direction = sign(sin(millis() / 1000.0f) * 100);

  if (lastMockTriggerTime + mockFrequency < millis()) {
    lastMockTriggerTime = millis();

    output = lastMockState + direction;

    if (output == 4) {
      output = 1;
    }

    if (output == 0) {
      output = 3;
    }

    lastMockState = output;
  }

  return output;
}

int sign(int number) {
  return (number > 0) - (number < 0);
}
#endif
