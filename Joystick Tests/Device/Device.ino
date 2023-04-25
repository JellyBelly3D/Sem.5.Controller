#include <Joystick.h>

#define HALL_PIN 0


Joystick_ controller;

const int16_t wheelcircumference = 2000;  // in millimeters
const int8_t magnetCount = 9;
const int16_t distancePerMagnet = wheelcircumference / magnetCount;  // in millimeters
const int16_t maxSpeed = 10;
const int8_t axisMax = 127;

uint32_t lastTriggerTime;
bool lastHallState = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);

  lastTriggerTime = millis();

  controller.useManualSend(true);
  controller.use8bit(true);
  controller.begin();
}

void loop() {
  bool currentHallState = digitalRead(HALL_PIN) == LOW ? true : false;
  digitalWrite(LED_BUILTIN, LOW);
  //bool currentHallState = mockHallSensor() == LOW ? true : false;

  if (lastHallState != currentHallState) {

    lastHallState = currentHallState;

    if (currentHallState == true) {

      uint16_t timeSinceLastMagnet = millis() - lastTriggerTime;

      lastTriggerTime = millis();

      uint16_t speed = distancePerMagnet / timeSinceLastMagnet;  // millimeters per millisecond

      uint16_t normalizedSpeed = normalizeSpeed(speed);

      controller.Y(normalizedSpeed);

      Serial.print("Y:");
      Serial.println(normalizedSpeed);

      controller.send_now();
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}

uint16_t normalizeSpeed(uint16_t speed) {
  return (speed * axisMax) / maxSpeed;
}


const unsigned long magnetRate = 250;
unsigned long previousMagnetTime;
PinStatus mockHallSensor() {
  if (millis() >= previousMagnetTime + magnetRate * (sin(millis() / 1000) / 2 + 0.5)) {
    previousMagnetTime = millis();
    return LOW;
  }
  return HIGH;
}
