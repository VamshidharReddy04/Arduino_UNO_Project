#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
int mq135Pin = A0;     // MQ135 analog output pin
int greenLED = 8;      // Green LED pin
int redLED = 9;        // Red LED pin
int buzzer = 10;       // Buzzer pin

void setup() {
  lcd.begin();          // Initialize LCD
  lcd.backlight();     // Turn ON backlight

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Gas Detection");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValue = analogRead(mq135Pin);  // Read MQ135 sensor value

  // Convert analog value to approximate PPM
  int ppm = map(sensorValue, 0, 1023, 0, 500);

  lcd.clear();

  // -------- Smoke Safe Range --------
  if (ppm >= 10 && ppm <= 80) {
    lcd.setCursor(0, 0);
    lcd.print("Smoke: ");
    lcd.print(ppm);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Not Harmful");
    digitalWrite(greenLED, HIGH); // Green Led ON
    digitalWrite(redLED, LOW);    // Red Led OFF
    digitalWrite(buzzer, LOW);   // No buzzer sound
  }

  // -------- Smoke Detected (Danger) --------
  else if (ppm >= 80 && ppm <= 120) {
    lcd.setCursor(0, 0);
    lcd.print("Smoke: ");
    lcd.print(ppm);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Harmful Smoke!");
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);  // Buzzer ON
    delay(3000);                 // Buzzer sounds for 3 seconds
    digitalWrite(buzzer, LOW);   // Buzzer OFF after 3 seconds
  }

  // -------- Alcohol Range --------
  else if (ppm >= 121 && ppm <= 200) {
    lcd.setCursor(0, 0);
    lcd.print("Alcohol: ");
    lcd.print(ppm);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Person not Drink");
    digitalWrite(greenLED, HIGH); // Green Led ON
    digitalWrite(redLED, LOW);    // Red Led OFF
    digitalWrite(buzzer, LOW);    // Buzzer OFF
  }
  // --------  Alcohol High Range --------
  else if (ppm > 201) {
    lcd.setCursor(0, 0);
    lcd.print("Alcohol: ");
    lcd.print(ppm);
    lcd.print("ppm");

    lcd.setCursor(0, 1);
    lcd.print("Person Drank");
    digitalWrite(greenLED, LOW);  // Green Led OFF
    digitalWrite(redLED, HIGH);   //Red Led ON 
    digitalWrite(buzzer, HIGH);  // Continuous buzzer for Alcohol range
  }
  delay(1500);  // Delay for stable sensor readings
}
