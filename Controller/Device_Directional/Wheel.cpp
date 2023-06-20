#include "Wheel.h"

int Wheel::readSensors() {
  int sensor1 = digitalRead(innerHallPin) == LOW ? 1 : 0;
  int sensor2 = digitalRead(middleHallPin) == LOW ? 2 : 0;
  int sensor3 = digitalRead(outerHallPin) == LOW ? 3 : 0;

  return sensor1 + sensor2 + sensor3;
}

int8_t Wheel::calculateInputMagnitude() {
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

int8_t Wheel::calculateInputDirection(int sensorState) {
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

int8_t Wheel::calculateRollingAverage(int8_t magnitude) {
  previousIndex = (previousIndex + 1) % AVERAGE_LENGTH;

  previousMagnitudes[previousIndex] = magnitude;

  int16_t magnitudeSum = 0;
  for (int i = 0; i < AVERAGE_LENGTH; i++) {
    magnitudeSum += previousMagnitudes[i];
  }

  return magnitudeSum / AVERAGE_LENGTH;
}

void Wheel::setup(bool isLeft, int inner, int middle, int outer, bool isIncrementing, Joystick_* controller) {
  innerHallPin = inner;
  middleHallPin = middle;
  outerHallPin = outer;

  Wheel::isLeft = isLeft;

  this->controller = controller;

  pinMode(innerHallPin, INPUT);
  pinMode(middleHallPin, INPUT);
  pinMode(outerHallPin, INPUT);

  if (isIncrementing) {
    direction = 1;
  } else {
    direction = -1;
  }
}

void Wheel::update() {
  controllerUpdated = false;

  // sensorState is an int from 0-3.
  // 0 = noMagnets, 1 = sensor1, 2 = sensor2, 3 = sensor3.
  int sensorState = readSensors();

  if (sensorState != 0 && lastSensorState != sensorState) {
    int8_t inputMagnitude = calculateInputMagnitude();

    int8_t averageMagnitude = calculateRollingAverage(inputMagnitude);

    int8_t direction = calculateInputDirection(sensorState) * Wheel::direction;

    lastSensorState = sensorState;
    lastTriggerTime = millis();

    isStopped = false;

#if USE_UNITY_SCALE
    averageMagnitude = axisMax - averageMagnitude;
#endif

    if (isLeft) {
      controller->sliderRight(direction * averageMagnitude);
    } else {
      controller->sliderLeft(direction * averageMagnitude);
    }
    controllerUpdated = true;

#if PRINT_DEBUG

    String name = Wheel::isLeft ? "Left" : "Right";

    Serial.print(" " + name + "SensorState:");
    Serial.print(sensorState);
    Serial.print(" " + name + "Y:");
    Serial.print(direction * averageMagnitude);
    //Serial.print(" " + name + "direction:");
    //Serial.print(direction);
    //Serial.print(" " + name + "Average_Magnitude:");
    //Serial.print(averageMagnitude);
    //Serial.print(" " + name + "Magnitude:");
    //Serial.print(inputMagnitude);
#endif
  }

  if (isStopped == false && lastTriggerTime + stopDelay < millis()) {
    isStopped = true;
    // we do this to add the magnitude of 0 to the array previousMagnitudes
    calculateRollingAverage(0);
    if (isLeft) {
      controller->sliderRight(0);
    } else {
      controller->sliderLeft(0);
    }
    String name = Wheel::isLeft ? "Left" : "Right";

    Serial.print(" " + name + "Y:");
    Serial.print(0);
    controllerUpdated = true;

#if PRINT_DEBUG
    Serial.println("Stopped");
#endif
  }
}
