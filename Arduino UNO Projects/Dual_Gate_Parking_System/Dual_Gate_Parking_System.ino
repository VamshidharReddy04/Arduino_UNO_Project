#include <Servo.h>

Servo entryGate;
Servo exitGate;

int irEntry = 2;
int irExit = 3;

int greenLED = 4; // Entry indicator
int redLED = 5;   // Exit indicator

void setup() {
  pinMode(irEntry, INPUT);
  pinMode(irExit, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  entryGate.attach(9);
  exitGate.attach(10);

  entryGate.write(0);   // Closed
  exitGate.write(0);    // Closed

  Serial.begin(9600);
}

void loop() {
  int entryStatus = digitalRead(irEntry);
  int exitStatus = digitalRead(irExit);

  // 🔹 BOTH sensors detect → ONLY OPEN both gates
  if (entryStatus == LOW && exitStatus == LOW) {
    Serial.println("Both Sensors Detected");

    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, HIGH);

    entryGate.write(90);   // Open
    exitGate.write(90);    // Open

    // ❌ No closing here (as per your requirement)
  }

  // 🔹 Entry Sensor
  else if (entryStatus == LOW) {
    Serial.println("Vehicle at Entry");

    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);

    entryGate.write(90);
    delay(1500);
    entryGate.write(0);

    digitalWrite(greenLED, LOW);
  }

  // 🔹 Exit Sensor
  else if (exitStatus == LOW) {
    Serial.println("Vehicle at Exit");

    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);

    exitGate.write(90);
    delay(1500);
    exitGate.write(0);

    digitalWrite(redLED, LOW);
  }

  // 🔹 No Detection → LEDs OFF
  else {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);

    // Optional: keep gates closed when idle
    entryGate.write(0);
    exitGate.write(0);
  }
}
