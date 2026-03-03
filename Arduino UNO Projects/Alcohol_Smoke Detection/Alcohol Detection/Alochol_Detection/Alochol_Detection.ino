#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C address 0x27, 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
int mq135Pin = A0;      // MQ135 analog output
int greenLED = 8;       // Green LED pin
int redLED = 9;         // Red LED pin
int buzzer = 10;        // Buzzer pin

void setup() {
  lcd.begin();           // Initialize LCD
  lcd.backlight();      // Turn ON LCD backlight

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Alcohol Detect");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValue = analogRead(mq135Pin);   // Read MQ135 sensor value
  // Convert analog value to approximate PPM
  int ppm = map(sensorValue, 0, 1023, 0, 500);
  lcd.clear();
  // -------- Alcohol Safe Range --------
  if (ppm >= 10 && ppm <= 100) {
    lcd.setCursor(0, 0);
    lcd.print("Alcohol: ");
    lcd.print(ppm);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Person Not Drink");
    digitalWrite(greenLED, HIGH);  // Green LED ON
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);     // Buzzer OFF
  }

  // -------- Alcohol Detected --------
  else if (ppm >= 101 && ppm <= 300) {
    lcd.setCursor(0, 0);
    lcd.print("Alcohol: ");
    lcd.print(ppm);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Person Drinked");
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);    // Red LED ON
    digitalWrite(buzzer, HIGH);    // Buzzer ON
  }

  delay(1500);  // Delay for stable readings
}
