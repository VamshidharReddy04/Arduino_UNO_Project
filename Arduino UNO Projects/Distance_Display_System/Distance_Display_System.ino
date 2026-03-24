#include <LedControl.h>

// MAX7219 pins
#define DIN 11
#define CS 12
#define CLK 13

LedControl lc = LedControl(DIN, CLK, CS, 1);

// Ultrasonic pins
#define TRIG 9
#define ECHO 10

long duration;
int distance;

// Display function
void displayLevel(int rows) {
  lc.clearDisplay(0);

  for (int i = 0; i < rows; i++) {
    for (int col = 0; col < 8; col++) {
      lc.setLed(0, 7 - i, col, true);
    }
  }
}

// Get stable distance (average of 5 readings)
int getDistance() {
  long sum = 0;
  int validReadings = 0;

  for (int i = 0; i < 5; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH, 30000);

    if (duration > 0) {
      int d = duration * 0.034 / 2;
      sum += d;
      validReadings++;
    }

    delay(10);
  }

  if (validReadings == 0) return -1;

  return sum / validReadings;
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  Serial.begin(9600);
}

void loop() {

  int rawDistance = getDistance();

  if (rawDistance == -1) {
    Serial.println("No Echo");
    return;
  }

  // Scale 0–55 → 0–100
  distance = map(rawDistance, 0, 55, 0, 100);
  distance = constrain(distance, 0, 100);

  Serial.print("Stable Distance: ");
  Serial.println(distance);

  int rows;

  if (distance > 80) rows = 1;
  else if (distance > 70) rows = 2;
  else if (distance > 60) rows = 3;
  else if (distance > 50) rows = 4;
  else if (distance > 40) rows = 5;
  else if (distance > 30) rows = 6;
  else if (distance > 20) rows = 7;
  else rows = 8;

  displayLevel(rows);

  delay(100);
}
