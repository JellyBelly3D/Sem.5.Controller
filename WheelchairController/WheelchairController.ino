const int hallPin = 14;  // the number of the Hall sensor pin
const int ledPin = LED_BUILTIN; 
int count = 0;
bool magnet = false;

int hallState = 0; 

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(hallPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  hallState = digitalRead(hallPin);

  if (hallState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    if(!magnet){
      count++;
      Serial.println(count);
    }
    magnet = true;
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    magnet = false;
  }
}
