#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo object
Servo gateServo;

// Pin definitions
#define trigPin 9
#define echoPin 10
#define servoPin 6
#define ledPin 7
#define buzzerPin 8

// 🔧 CHANGE DISTANCE HERE (cm)
int detectDistance = 10;

// Vehicle data
String vehicleNames[] = {"Car", "Bike", "Bus", "Truck"};
String vehicleNumbers[] = {
  "TS01AB1234",
  "TS09CD5678",
  "TS08EF9012",
  "TS10GH3456"
};

int vehicleCount = 4;
int currentVehicle = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  gateServo.attach(servoPin);
  gateServo.write(0);   // Gate closed

  lcd.begin();
  lcd.backlight();

  // Welcome screen
  lcd.setCursor(0, 0);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Toll Plaza");

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void loop() {

  long duration;
  int distance;

  // Ultrasonic trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Vehicle detected
  if (distance > 0 && distance <= detectDistance) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vehicle: ");
    lcd.print(vehicleNames[currentVehicle]);

    lcd.setCursor(0, 1);
    lcd.print("No: ");
    lcd.print(vehicleNumbers[currentVehicle]);

    // Open gate
    gateServo.write(90);

    // LED ON
    digitalWrite(ledPin, HIGH);

    // Buzzer beep
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);

    delay(3000);  // Vehicle passing time

    // Close gate
    gateServo.write(0);
    digitalWrite(ledPin, LOW);

    // Next vehicle
    currentVehicle++;
    if (currentVehicle >= vehicleCount) {
      currentVehicle = 0;
    }

    // Welcome screen again
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome to");
    lcd.setCursor(0, 1);
    lcd.print("Toll Plaza");
  }

  delay(300);
}
