#include <Servo.h>

Servo gateServo;

int trigPin = 9;
int echoPin = 10;

int redLED = 2;
int yellowLED = 3;
int greenLED = 4;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  gateServo.attach(6);

  Serial.begin(9600);

  // Initial state
  gateServo.write(0); // gate closed
  digitalWrite(redLED, HIGH);
}

void loop() {

  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // Vehicle detected
  if (distance > 0 && distance < 15) {

    // Yellow ON (warning)
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
    delay(1000);

    // Open gate
    gateServo.write(90);

    // Green ON
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);

    delay(4000); // vehicle pass time

    // Close gate
    gateServo.write(0);

    // Red ON again
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);

    delay(2000);
  }
}
