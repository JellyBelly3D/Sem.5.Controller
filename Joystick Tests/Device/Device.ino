#include <Joystick.h>

Joystick_ stick;

const float wheelcircumference = 2;
const int magnetCount = 9; //xD
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

bool lastState = false;

void loop() {
  PinStatus pin = digitalRead(0);

  bool state = pin == LOW ? true: false;

  bool triggered = false;

  if(lastState != state ){
    lastState = state;

    if(lastState == true){
      triggered = true;
    }else{
      triggered = false;
    }
  }

  digitalWrite(LED_BUILTIN, pin == LOW ? HIGH : LOW);
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


    Serial.print("Time_Since:");
    Serial.print(timeSinceLastMagnet);
    Serial.print(",");
    Serial.print("Speed:");
    Serial.println(speed);
  }


  stick.X(9);
  stick.Y(5);

  stick.send_now();
}
