#include <Servo.h>

Servo myServo;

int joystickX = A0;   // Joystick X-axis
int servoPin = 9;

int joyValue;
int angle;

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  joyValue = analogRead(joystickX);   // Read joystick

  // Map joystick value to servo angle
  angle = map(joyValue, 0, 1023, 0, 180);

  myServo.write(angle);   // Move servo

  Serial.print("Joystick: ");
  Serial.print(joyValue);
  Serial.print("  Angle: ");
  Serial.println(angle);

  delay(20);
}
