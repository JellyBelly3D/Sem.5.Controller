#ifndef Wheel_h
#define Wheel_h

#define MOCK_SENSORS false
#define PRINT_DEBUG true
#define USE_UNITY_SCALE false

#include <Arduino.h>
#include <Joystick.h>

class Wheel {
public:
  bool controllerUpdated = false;

private:
  // Configuration
  int innerHallPin;
  int middleHallPin;
  int outerHallPin;
  int direction;
  Joystick_* controller;
  bool isLeft;

  // Constants
  const int8_t magnetCount = 45;
  const float wheelcircumference = 200;                              // in centimeters
  const float distancePerMagnet = wheelcircumference / magnetCount;  // in centimeters
  const float maxSpeed = 400;                                        // in centimeters per second
  const uint16_t stopDelay = 100;                                    // in milliseconds
  const int8_t axisMax = 127;

  // State
  uint32_t lastTriggerTime;
  int lastSensorState = 0;
  bool isStopped = true;
#define AVERAGE_LENGTH 1
  int8_t previousMagnitudes[AVERAGE_LENGTH];
  int previousIndex = 0;

public:
  void setup(bool isLeft, int inner, int middle, int outer, bool isIncrementing, Joystick_* controller);
  void update();

private:
  int readSensors();
  int8_t calculateInputMagnitude();
  int8_t calculateInputDirection(int sensorState);
  int8_t calculateRollingAverage(int8_t magnitude);
};
#endif