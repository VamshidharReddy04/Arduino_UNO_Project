#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int ldrDigital = 2;
int count = 0;

bool objectDetected = false;

void setup() {
  pinMode(ldrDigital, INPUT);

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Object Counter");
  lcd.setCursor(0,1);
  lcd.print("System");
  delay(2000);
  lcd.clear();
}

void loop() {
  int state = digitalRead(ldrDigital);

  // When beam is blocked
  if (state == LOW && objectDetected == false) {
    count++;
    objectDetected = true;
  }

  // When beam returns
  if (state == HIGH) {
    objectDetected = false;
  }

  lcd.setCursor(0, 0);
  lcd.print("Object Count: ");
  lcd.print(count);

  delay(100);
}
