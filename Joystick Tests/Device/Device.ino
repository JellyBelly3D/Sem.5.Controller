#include <Joystick.h>

Joystick_ stick;

const float wheelcircumference = 1;
const int magnetCount = 2;
const float distancePerMagnet = wheelcircumference / magnetCount;

const unsigned long magnetRate = 250;
unsigned long previousMagnetTime;
unsigned long lastTriggerTime;

void setup() {
  // put your setup code here, to run once:
  stick.useManualSend(true);
  stick.begin();
  lastTriggerTime = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT);
}


void loop() {

  bool triggered = digitalRead(0) == LOW ? true: false;

  digitalWrite(LED_BUILTIN, LOW);
  /*
  if (millis() >= previousMagnetTime + magnetRate) {
    triggered = true;
    previousMagnetTime = millis();
    digitalWrite(LED_BUILTIN, HIGH);
  }
  */


  if (triggered) {
    unsigned long timeSinceLastMagnet = millis() - lastTriggerTime;

    lastTriggerTime = millis();

    float speed = distancePerMagnet / ((float)timeSinceLastMagnet / 1000.0);

    String time = "Time Since: ";
    String speed2 = ", Speed: ";

    Serial.println(time + timeSinceLastMagnet + speed2 + speed);
  }


  stick.X(9);
  stick.Y(5);

  stick.send_now();
}
