const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
const int buttonPin = 2;

int buttonState;
int lastButtonState = HIGH;
int count = 0;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Detect button press (falling edge)
  if (lastButtonState == HIGH && buttonState == LOW) {
    count++;
    if (count > 8) count = 1;
    delay(200); // debounce
  }

  lastButtonState = buttonState;

  // Reset all
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  // Color selection
  switch (count) {
    case 1: digitalWrite(redPin, HIGH); break;
    case 2: digitalWrite(greenPin, HIGH); break;
    case 3: digitalWrite(bluePin, HIGH); break;
    case 4: // White
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      break;
    case 5: // Red + Green
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, HIGH);
      break;
    case 6: // Green + Blue
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      break;
    case 7: // Blue + Red
      digitalWrite(bluePin, HIGH);
      digitalWrite(redPin, HIGH);
      break;
    case 8: // All
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, HIGH);
      break;
  }
}
