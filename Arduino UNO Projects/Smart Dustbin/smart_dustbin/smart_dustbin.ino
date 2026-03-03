// Smart Dustbin using Ultrasonic Sensor and Servo Motor
// Arduino UNO Project
#include <Servo.h>   // Servo library
// Pin Definitions
#define trigPin 9
#define echoPin 10
#define servoPin 6
Servo dustbinServo;
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  dustbinServo.attach(servoPin);  // Attach servo
  dustbinServo.write(0);          // Lid closed position
  Serial.begin(9600);
}

void loop() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read echo time
  duration = pulseIn(echoPin, HIGH);
  // Calculate distance in cm
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  // If hand detected near dustbin
  if (distance > 0 && distance <= 15) {
    dustbinServo.write(90);   // Open lid
    delay(5000);              // Wait 5 seconds
    dustbinServo.write(0);    // Close lid
  }
  delay(200);
}
