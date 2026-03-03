/**************************************************
 * Hand Motion Controlled Servo System
 * Using ADXL345 Accelerometer & Arduino UNO
 **************************************************/

#include <Wire.h>                      // I2C communication
#include <Adafruit_Sensor.h>           // Adafruit unified sensor
#include <Adafruit_ADXL345_U.h>         // ADXL345 library
#include <Servo.h>                     // Servo library

// Create sensor and servo objects
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
Servo handServo;

// Variables
long servoAngle = 90;

void setup() {
  Serial.begin(9600);
  Serial.println("Hand Motion Controlled Servo System");

  // Initialize ADXL345 sensor
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected. Check wiring!");
    while (1);   // Stop program
  }

  // Set accelerometer range
  accel.setRange(ADXL345_RANGE_2_G);

  // Attach servo motor
  handServo.attach(9);
  handServo.write(servoAngle);   // Neutral position

  delay(1000);
}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);

  // Read X-axis acceleration value
  float xAxis = event.acceleration.x;

  // Map X-axis value (-10 to +10) to servo angle (0 to 180)
  servoAngle = map(xAxis, -10, 10, 0, 180);

  // Limit servo angle
  servoAngle = constrain(servoAngle, 0, 180);

  // Rotate servo
  handServo.write(servoAngle);

  // Print values for debugging
  Serial.print("X-axis: ");
  Serial.print(xAxis);
  Serial.print("  Servo Angle: ");
  Serial.println(servoAngle);

  delay(50);
}
