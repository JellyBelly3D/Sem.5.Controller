void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT);
}

bool lastState = HIGH;
int magnetCount = 0;
unsigned long startTime = 0;
unsigned long lastMagnetTime = 0;

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, LOW);


  bool currentState = digitalRead(0) == LOW ? true: false;

  if(lastState != currentState){
    lastState = currentState;
    if(currentState == true){

      if(startTime == 0){
        startTime = millis();
      }
      digitalWrite(LED_BUILTIN, HIGH);
      lastMagnetTime = millis();

      magnetCount++;
    }
  }

  if(startTime != 0 && millis() > lastMagnetTime + 100){
    bool success = magnetCount == 5;

    int duration = lastMagnetTime-startTime;

    int perMagnetDuration = duration/magnetCount;


    Serial.print("Duration:");
    Serial.print(duration);
    Serial.print(" PerMagnetDuration:");
    Serial.print(perMagnetDuration);
    Serial.print(" MagnetCount:");
    Serial.print(magnetCount);
    Serial.print(" Success:");
    Serial.println(success);

    startTime = 0;
    magnetCount = 0;
  }
}
