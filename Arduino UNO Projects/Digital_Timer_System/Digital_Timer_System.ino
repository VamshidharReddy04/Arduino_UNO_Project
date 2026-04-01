#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buttons
const int hourBtn = 4;
const int minBtn = 3;
const int secBtn = 2;

// Output
const int ledPin = 8;
const int buzzer = 9;

// Time
int hours = 0;
int minutes = 0;
int seconds = 0;

bool timerStarted = false;

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  pinMode(hourBtn, INPUT_PULLUP);
  pinMode(minBtn, INPUT_PULLUP);
  pinMode(secBtn, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin();
  lcd.backlight();

  // -------- START SCREEN --------
  lcd.setCursor(0, 0);
  lcd.print(" Digital Timer ");
  lcd.setCursor(0, 1);
  lcd.print("    System");
  delay(2000);

  lcd.clear();
}

void loop() {

  // -------- TIME SET MODE --------
  if (!timerStarted) {

    lcd.setCursor(0, 0);
    lcd.print("Set Time:");

    // Hour button
    if (digitalRead(hourBtn) == LOW) {
      delay(200);
      hours++;
      if (hours > 23) hours = 0;
    }

    // Minute button
    if (digitalRead(minBtn) == LOW) {
      delay(200);
      minutes++;
      if (minutes > 59) minutes = 0;
    }

    // Second button
    if (digitalRead(secBtn) == LOW) {
      delay(200);
      seconds++;
      if (seconds > 59) seconds = 0;
    }

    // Display time
    lcd.setCursor(0, 1);
    printTime(hours, minutes, seconds);

    // -------- START CONDITION --------
    // Long press sec button to start
    if (digitalRead(secBtn) == LOW) {
      delay(1000); // long press
      if (digitalRead(secBtn) == LOW && (hours > 0 || minutes > 0 || seconds > 0)) {
        timerStarted = true;
        lcd.clear();
      }
    }
  }

  // -------- COUNTDOWN --------
  if (timerStarted) {

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Decrease time
      if (seconds == 0) {
        if (minutes == 0) {
          if (hours > 0) {
            hours--;
            minutes = 59;
            seconds = 59;
          }
        } else {
          minutes--;
          seconds = 59;
        }
      } else {
        seconds--;
      }

      // Display
      lcd.setCursor(0, 0);
      lcd.print("Counting Down");

      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      printTime(hours, minutes, seconds);

      // LED Blink
      digitalWrite(ledPin, !digitalRead(ledPin));
    }

    // -------- TIME END --------
    if (hours == 0 && minutes == 0 && seconds == 0) {
      digitalWrite(ledPin, LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("    Time Up!");

      schoolBell();

      timerStarted = false;
    }
  }
}

// -------- PRINT TIME --------
void printTime(int h, int m, int s) {
  if (h < 10) lcd.print("0");
  lcd.print(h);
  lcd.print(":");

  if (m < 10) lcd.print("0");
  lcd.print(m);
  lcd.print(":");

  if (s < 10) lcd.print("0");
  lcd.print(s);
}

// -------- BUZZER SOUND --------
void schoolBell() {
  for (int i = 0; i < 6; i++) {
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
    delay(200);
  }
}
