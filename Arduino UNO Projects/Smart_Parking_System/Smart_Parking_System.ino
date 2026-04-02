#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo entryServo;
Servo exitServo;

int irEntry = 2;
int irExit = 3;

int count = 0;
int maxSlots = 5;

void setup() {
  pinMode(irEntry, INPUT);
  pinMode(irExit, INPUT);

  entryServo.attach(9);
  exitServo.attach(10);

  entryServo.write(0);  // Gate closed
  exitServo.write(0);

  lcd.beghyynin();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Smart Parking");
  delay(2000);
  lcd.clear();
}

void loop() {

  // ENTRY SENSOR
  if (digitalRead(irEntry) == LOW) {
    if (count < maxSlots) {
      count++;

      entryServo.write(90); // Open gate
      delay(2000);
      entryServo.write(0);  // Close gate

      while(digitalRead(irEntry) == LOW); // wait
    }
  }

  // EXIT SENSOR
  if (digitalRead(irExit) == LOW) {
    if (count > 0) {
      count--;

      exitServo.write(90);
      delay(2000);
      exitServo.write(0);

      while(digitalRead(irExit) == LOW);
    }
  }

  // LCD DISPLAY
  lcd.setCursor(0,0);
  lcd.print("Slots: ");
  lcd.print(maxSlots - count);
  lcd.print("   ");

  lcd.setCursor(0,1);

  if (count == maxSlots) {
    lcd.print("Parking Full   ");
  }
  else if (count == 0) {
    lcd.print("Slots Free     ");
  }
  else {
    lcd.print("Vehicles: ");
    lcd.print(count);
    lcd.print("   ");
  }

  delay(300);
}
