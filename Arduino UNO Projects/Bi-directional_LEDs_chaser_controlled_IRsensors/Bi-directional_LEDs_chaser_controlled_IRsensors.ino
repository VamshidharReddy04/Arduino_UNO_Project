// IR Sensor Pins
const int irRight = 2;
const int irLeft = 3;

// LED Pins
int leds[] = {4, 5, 6, 7, 8};

void setup() {
  pinMode(irRight, INPUT);
  pinMode(irLeft, INPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {

  // Right IR Sensor Triggered
  if (digitalRead(irRight) == LOW) {
    for (int i = 4; i >= 0; i--) {
      digitalWrite(leds[i], HIGH);
      delay(1000);
      digitalWrite(leds[i], LOW);
    }
  }

  // Left IR Sensor Triggered
  else if (digitalRead(irLeft) == LOW) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(leds[i], HIGH);
      delay(1000);
      digitalWrite(leds[i], LOW);
    }
  }
}
