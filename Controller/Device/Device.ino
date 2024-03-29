#include <Joystick.h>

#define HALL_PIN 0


Joystick_ controller;

const int8_t magnetCount = 9;
const float wheelcircumference = 200;                              // in centimeters
const float distancePerMagnet = wheelcircumference / magnetCount;  // in centimeters
const float maxSpeed = 800;                                        // in centimeters per second
const int8_t stopDelay = 100;                                       // in milliseconds
const int8_t axisMax = 127;

bool isStopped = true;
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
  //digitalWrite(LED_BUILTIN, currentHallState?HIGH:LOW);
  //bool currentHallState = mockHallSensor() == LOW ? true : false;

  if (lastHallState != currentHallState) {

    lastHallState = currentHallState;

    if (currentHallState == true) {

      uint16_t timeSinceLastMagnet = millis() - lastTriggerTime;

      lastTriggerTime = millis();

      float speed = distancePerMagnet / (timeSinceLastMagnet / 1000.0f);  // millimeters per millisecond

      int8_t normalizedSpeed = normalizeSpeed(speed);
      //      digitalWrite(LED_BUILTIN, HIGH);

      controller.Y(normalizedSpeed);

      Serial.print(" DistancePerMagnet:");
      Serial.print(distancePerMagnet);
      Serial.print(" TimeSinceLastMagnet:");
      Serial.print(timeSinceLastMagnet);
      Serial.print(" Y:");
      Serial.print(normalizedSpeed);
      Serial.print(" Speed:");
      Serial.println(speed);

      isStopped = false;

      controller.send_now();
    }
  }


  if (isStopped == false && lastTriggerTime + stopDelay < millis()) {
    isStopped = true;
    controller.Y(0);
    controller.send_now();
    Serial.println("Stopped");
  }
}

int8_t normalizeSpeed(float speed) {
  speed = speed > maxSpeed ? maxSpeed : speed;
  return (speed * (float)axisMax) / maxSpeed;
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
