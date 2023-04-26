#include <Joystick.h>

#define HALL_PIN1 0
#define HALL_PIN2 1
#define HALL_PIN3 2

#define MOCK_SENSORS false

Joystick_ controller;

const int8_t magnetCount = 9;
const float wheelcircumference = 200;                              // in centimeters
const float distancePerMagnet = wheelcircumference / magnetCount;  // in centimeters
const float maxSpeed = 800;                                        // in centimeters per second
const uint16_t stopDelay = 400;                                      // in milliseconds
const int8_t axisMax = 127;

bool isStopped = true;
uint32_t lastTriggerTime;
int lastSensorState = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HALL_PIN1, INPUT_PULLUP);
  pinMode(HALL_PIN2, INPUT_PULLUP);
  pinMode(HALL_PIN3, INPUT_PULLUP);

  lastTriggerTime = millis();

  controller.useManualSend(true);
  controller.use8bit(true);
  controller.begin();
}

void loop() {
#if MOCK_SENSORS
  int sensorState = readMockSensors();
#else
  int sensorState = readSensors();
#endif

  if (sensorState != 0 && lastSensorState != sensorState) {

    int8_t inputMagnitude = calculateInputMagnitude();

    int8_t direction = calculateInputDirection(sensorState);

    lastSensorState = sensorState;

    controller.Y(direction * inputMagnitude);

    Serial.print(" Y:");
    Serial.print(direction * inputMagnitude);
    Serial.print(" Speed:");
    Serial.println(inputMagnitude);

    isStopped = false;

    controller.send_now();
  }

  if (isStopped == false && lastTriggerTime + stopDelay < millis()) {
    isStopped = true;
    controller.Y(0);
    controller.send_now();
    Serial.println("Stopped");
  }
}

int readSensors() {
  int sensor1 = digitalRead(HALL_PIN1) == HIGH ? 1 : 0;
  int sensor2 = digitalRead(HALL_PIN2) == HIGH ? 2 : 0;
  int sensor3 = digitalRead(HALL_PIN3) == HIGH ? 3 : 0;

  return sensor1 + sensor2 + sensor3;
}

int8_t calculateInputMagnitude() {
  uint16_t timeSinceLastTrigger = millis() - lastTriggerTime;

  lastTriggerTime = millis();

  // Calculate speed in Centimeters/Second
  float speed = distancePerMagnet / (timeSinceLastTrigger / 1000.0f);  // millimeters per millisecond

  // Clamp speed to MaxSpeed
  speed = speed > maxSpeed ? maxSpeed : speed;

  // Normalize speed to fit between 0-127
  int8_t normalizedSpeed = (speed * (float)axisMax) / maxSpeed;

  return normalizedSpeed;
}

int8_t calculateInputDirection(int sensorState) {

  int direction = 1;

  // Direction is determined based on the order that the sensorStates come in
  // going from 3 -> 2 would indicate a positive direction while
  // going from 3 -> 1 would indicate a positive direction.
  //
  // positive <- | -> negative
  // -----3-----3-----3-----3-
  // ---2-----2-----2-----2---
  // -1-----1-----1-----1-----

  switch (lastSensorState) {
    case 1:
      direction = sensorState == 3 ? 1 : -1;
      break;

    case 2:
      direction = sensorState == 1 ? 1 : -1;
      break;

    case 3:
      direction = sensorState == 2 ? 1 : -1;
      break;

    default:
      direction = 1;
      break;
  }

  return direction;
}

//========MOCK SENSORS========
#if MOCK_SENSORS
int mockFrequency = 200;
unsigned long lastMockTriggerTime;
int lastMockState = 1;

int readMockSensors() {
  int output = 0;

  mockFrequency = ((sin(millis()/1200.0f)/2)+0.5f)*150+50;

  int direction = sign(sin(millis()/1000.0f)*100);
  //int direction = -1;

  if (lastMockTriggerTime + mockFrequency < millis()) {
    lastMockTriggerTime = millis();

    //Serial.print(direction);

    output = lastMockState + direction;

    if (output == 4) {
      output = 1;
    }

    if(output == 0){
      output = 3;
    }

    lastMockState = output;
    Serial.print(output * direction);
  }

  return output;
}

int sign(int number){
  return (number > 0) - (number < 0);
}

#endif
