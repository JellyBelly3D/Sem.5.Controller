void setup() {
  // put your setup code here, to run once:
}

void loop() {

  uint32_t time = micros();

  uint32_t loopCount = 1000000;
  uint32_t result;
  for (int32_t i = 0; i < loopCount; i++) {
    result += normalizeSpeed(i);
  }

  time = micros() - time;

  Serial.print("Total_Time:");
  Serial.print(time);
  Serial.print(" result:");
  Serial.print(result);
  Serial.print(" Average_Time:");
  Serial.println(time / (float)loopCount);

  delay(2000);
}

uint16_t maxSpeed = 40;

uint16_t normalizeSpeed(uint16_t speed) {
  return (speed * 128) / maxSpeed;
}
