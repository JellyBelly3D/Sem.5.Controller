#include <Joystick.h>

#define HALL_PIN1 0
#define HALL_PIN2 1
#define HALL_PIN3 2

#define MOCK_SENSORS false
#define PRINT_DEBUG true

Joystick_ controller;

const int8_t magnetCount = 9;
const float wheelcircumference = 200;                              // in centimeters
const float distancePerMagnet = wheelcircumference / magnetCount;  // in centimeters
const float maxSpeed = 400;                                        // in centimeters per second
const uint16_t stopDelay = 10000;                                  // in milliseconds
const int8_t axisMax = 127;

bool isStopped = true;
uint32_t lastTriggerTime;
int lastSensorState = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HALL_PIN1, INPUT);
  pinMode(HALL_PIN2, INPUT);
  pinMode(HALL_PIN3, INPUT);

  lastTriggerTime = millis();

  controller.useManualSend(true);
  controller.use8bit(true);
  controller.begin();
}

void loop() {
#if MOCK_SENSORS
  int sensorState = readMockSensors();
#else
  // sensorState is an int from 0-3.
  // 0 = noMagnets, 1 = sensor1, 2 = sensor2, 3 = sensor3.
  int sensorState = readSensors();
#endif

  if (sensorState != 0 && lastSensorState != sensorState) {
    int8_t inputMagnitude = calculateInputMagnitude();

    int8_t direction = calculateInputDirection(sensorState);

    lastSensorState = sensorState;
    lastTriggerTime = millis();

    isStopped = false;

    controller.Y(direction * inputMagnitude);
    controller.send_now();

#if PRINT_DEBUG
    Serial.print(" SensorState:");
    Serial.print(sensorState);
    Serial.print(" Y:");
    Serial.print(direction * inputMagnitude);
    Serial.print(" Speed:");
    Serial.println(inputMagnitude);
#endif
  }

  if (isStopped == false && lastTriggerTime + stopDelay < millis()) {
    isStopped = true;

    controller.Y(0);
    controller.send_now();

#if PRINT_DEBUG
    Serial.println("Stopped");
#endif
  }
}

int readSensors() {
  int sensor1 = digitalRead(HALL_PIN1) == LOW ? 1 : 0;
  int sensor2 = digitalRead(HALL_PIN2) == LOW ? 2 : 0;
  int sensor3 = digitalRead(HALL_PIN3) == LOW ? 3 : 0;

  return sensor1 + sensor2 + sensor3;
}

int8_t calculateInputMagnitude() {
  uint16_t timeSinceLastTrigger = millis() - lastTriggerTime;

  // Calculate speed in cm/s
  float speed = distancePerMagnet / (timeSinceLastTrigger / 1000.0f);  // unit: cm/s

  // Clamp speed to MaxSpeed
  speed = speed > maxSpeed ? maxSpeed : speed;

  // Normalize speed to between 0 and 1
  float normalizedSpeed = speed / maxSpeed;

  // Scale normalized speed to be between 0 and 127
  int8_t axisMagnitude = normalizedSpeed * axisMax;

  return axisMagnitude;
}

int8_t calculateInputDirection(int sensorState) {

  int forward = 1;
  int backward = -1;

  int direction = forward;

  // Direction is determined based on the order that the sensorStates come in
  // going from 3 -> 2 would indicate a positive direction while
  // going from 3 -> 1 would indicate a positive direction.
  //
  // negative <- | -> positive
  // -----3-----3-----3-----3-
  // ---2-----2-----2-----2---
  // -1-----1-----1-----1-----

  switch (lastSensorState) {
    case 1:
      direction = sensorState == 2 ? forward : backward;
      break;

    case 2:
      direction = sensorState == 3 ? forward : backward;
      break;

    case 3:
      direction = sensorState == 1 ? forward : backward;
      break;

    default:
      direction = forward;
      break;
  }

  return direction;
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
