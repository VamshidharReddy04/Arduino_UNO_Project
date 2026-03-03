#include <Servo.h>
#include <TM1637Display.h>

// ---------------- Pins ----------------
#define TOUCH_PIN 2

#define CLK 3
#define DIO 4

#define SERVO1_PIN 5
#define SERVO2_PIN 6

#define BUZZER 8
#define RED_LED 9
#define GREEN_LED 10

// ------------ Objects ----------------
Servo gate1;
Servo gate2;

TM1637Display display(CLK, DIO);

// ------------ Settings --------------
int openPos = 90;
int closePos = 0;

// ------------ State -----------------
bool trainActive = false;

void setup() {
  pinMode(TOUCH_PIN, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  gate1.attach(SERVO1_PIN);
  gate2.attach(SERVO2_PIN);

  gate1.write(openPos);
  gate2.write(openPos);

  display.setBrightness(7);
  display.clear();

  digitalWrite(GREEN_LED, HIGH);

  Serial.begin(9600);
  Serial.println("Smart Train Tollgate Ready...");
}

void loop() {

  if (digitalRead(TOUCH_PIN) == HIGH && !trainActive) {
    trainActive = true;
    startTrainSequence();
  }

}

// ------------ Functions -------------

void startTrainSequence() {

  // ---- Countdown 10 -> 0 ----
  for (int i = 10; i >= 0; i--) {
    display.showNumberDec(i, true);
    delay(1000);
  }

  // ---- Close Gates ----
  closeGate();

  // ---- Warning ON ----
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  // ---- Train Passing Timer 0 -> 20 ----
  for (int i = 0; i <= 20; i++) {
    display.showNumberDec(i, true);
    tone(BUZZER, 1500);
    delay(500);
    noTone(BUZZER);
    delay(500);
  }

  // ---- Open Gates ----
  openGate();

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  noTone(BUZZER);

  display.clear();

  trainActive = false;
}

void closeGate() {
  for (int pos = openPos; pos >= closePos; pos--) {
    gate1.write(pos);
    gate2.write(180 - pos);
    delay(15);
  }
}

void openGate() {
  for (int pos = closePos; pos <= openPos; pos++) {
    gate1.write(pos);
    gate2.write(180 - pos);
    delay(15);
  }
}
