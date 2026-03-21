#include <Servo.h>

Servo myServo;

int leftSensor = 2;
int rightSensor = 3;

int angle = 90;
int sweepAngle = 0;
bool sweepForward = true;
bool sweepMode = false;

void setup() {
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  myServo.attach(9);
  myServo.write(angle);

  Serial.begin(9600);
}

void loop() {
  int leftState = digitalRead(leftSensor);
  int rightState = digitalRead(rightSensor);

  // 👉 BOTH sensors touched → start sweep mode
  if (leftState == HIGH && rightState == HIGH) {
    sweepMode = true;
  } 
  else {
    sweepMode = false;
  }

  // ✅ SWEEP MODE (non-blocking)
  if (sweepMode) {
    Serial.println("Sweeping...");

    myServo.write(sweepAngle);

    if (sweepForward) {
      sweepAngle++;
      if (sweepAngle >= 180) sweepForward = false;
    } else {
      sweepAngle--;
      if (sweepAngle <= 0) sweepForward = true;
    }

    delay(15);
    return;  // important: skip other controls
  }

  // 👉 STOP exactly where released
  angle = myServo.read();

  // LEFT control
  if (leftState == HIGH) {
    angle -= 5;
    if (angle < 0) angle = 0;

    myServo.write(angle);
    Serial.println("Moving Left");
    delay(100);
  }

  // RIGHT control
  else if (rightState == HIGH) {
    angle += 5;
    if (angle > 180) angle = 180;

    myServo.write(angle);
    Serial.println("Moving Right");
    delay(100);
  }
}
