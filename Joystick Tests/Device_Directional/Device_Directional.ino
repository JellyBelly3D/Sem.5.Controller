#include <Joystick.h>

#define HALL_PIN1 0
#define HALL_PIN2 1
#define HALL_PIN3 2


Joystick_ controller;

const int8_t magnetCount = 9;
const float wheelcircumference = 200;                              // in centimeters
const float distancePerMagnet = wheelcircumference / magnetCount;  // in centimeters
const float maxSpeed = 800;                                        // in centimeters per second
const int8_t stopDelay = 100;                                      // in milliseconds
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
  int sensorState = readSensors();

  if (sensorState != 0 && lastSensorState != sensorState) {

    lastSensorState = sensorState;

    int8_t inputMagnitude = calculateInputMagnitude();

    int8_t direction = calculateInputDirection(sensorState);

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
  int sensor1 = digitalRead(HALL_PIN1) == LOW ? 1 : 0;
  int sensor2 = digitalRead(HALL_PIN2) == LOW ? 2 : 0;
  int sensor3 = digitalRead(HALL_PIN3) == LOW ? 3 : 0;

  return sensor1 + sensor2 + sensor3;
}

int8_t calculateInputMagnitude() {
  uint16_t timeSinceLastMagnet = millis() - lastTriggerTime;

  lastTriggerTime = millis();

  // Calculate speed in Centimeters/Second
  float speed = distancePerMagnet / (timeSinceLastMagnet / 1000.0f);  // millimeters per millisecond

  // Clamp speed to MaxSpeed
  speed = speed > maxSpeed ? maxSpeed : speed;

  // Normalize speed to fit between 0-127
  int8_t normalizedSpeed = (speed * (float)axisMax) / maxSpeed;

  return normalizedSpeed;
}

int8_t calculateInputDirection(int sensorState) {

  int direction = 1;

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