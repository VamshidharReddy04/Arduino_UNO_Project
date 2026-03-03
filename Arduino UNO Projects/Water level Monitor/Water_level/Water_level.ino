// Water level Monitoring System
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Initialize LCD (Address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
#define trigPin 9
#define echoPin 10
#define greenLED 6
#define redLED 7
#define buzzer 8
// Tank height in cm (adjust according to your tank)
#define tankHeight 20  

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Water Monitor");
  delay(2000);
  lcd.clear();
}

void loop() {
  long duration;
  int distance;
  int waterLevel;
  int percentage;
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Receive echo
  duration = pulseIn(echoPin, HIGH);
  // Calculate distance in cm
  distance = duration * 0.034 / 2;
  // Calculate water level
  waterLevel = tankHeight - distance;
  // Limit values
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > tankHeight) waterLevel = tankHeight;
  // Convert to percentage
  percentage = (waterLevel * 100) / tankHeight;
  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Water Level:");
  lcd.print(percentage);
  lcd.print("%   ");
  // Draw bar graph
  lcd.setCursor(0, 1);
  int bars = percentage / 10;

  for (int i = 0; i < bars; i++) {
    lcd.print((char)255); // Full block
  }
  for (int i = bars; i < 10; i++) {
    lcd.print(" ");
  }
  // LED & Buzzer Logic
  if (percentage <= 85) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
  } 
  else {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
  }
  delay(1000);
}
