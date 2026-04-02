int flamePin = 2;
int ledPin = 8;
int buzzerPin = 9;

void setup() {
  pinMode(flamePin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int flameState = digitalRead(flamePin);

  if (flameState == LOW) { // Fire detected
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("🔥 Fire Detected!");
  } 
  else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    Serial.println("No Fire");
  }

  delay(200);
}
