#include <Wire.h>

#define ADXL345_ADDR 0x53

// LED Pins
int leftLEDs[] = {2, 3, 4};
int rightLEDs[] = {5, 6, 7};

int16_t X_out;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Initialize ADXL345
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x2D); // Power register
  Wire.write(8);    // Enable measurement
  Wire.endTransmission();

  // Set LED pins as output
  for (int i = 0; i < 3; i++) {
    pinMode(leftLEDs[i], OUTPUT);
    pinMode(rightLEDs[i], OUTPUT);
  }
}

void loop() {
  // Read X-axis data
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDR, 6, true);

  X_out = Wire.read() | Wire.read() << 8;

  Serial.println(X_out);

  int threshold = 150; // Adjust sensitivity

  if (X_out < -threshold) {
    // LEFT tilt → Left LEDs ON
    for (int i = 0; i < 3; i++) {
      digitalWrite(leftLEDs[i], HIGH);
      digitalWrite(rightLEDs[i], LOW);
    }
  }
  else if (X_out > threshold) {
    // RIGHT tilt → Right LEDs ON
    for (int i = 0; i < 3; i++) {
      digitalWrite(rightLEDs[i], HIGH);
      digitalWrite(leftLEDs[i], LOW);
    }
  }
  else {
    // Neutral → All OFF
    for (int i = 0; i < 3; i++) {
      digitalWrite(leftLEDs[i], LOW);
      digitalWrite(rightLEDs[i], LOW);
    }
  }

  delay(200);
}
