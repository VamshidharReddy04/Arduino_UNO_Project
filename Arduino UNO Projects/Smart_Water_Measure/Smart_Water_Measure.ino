#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
int waterSensor = A0;
int buzzer = 8;

// Custom bar character
byte bar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// Calibration values (adjust if needed)
int dryValue = 300;
int fullValue = 700;

void setup()
{
  pinMode(buzzer, OUTPUT);

  lcd.begin();
  lcd.backlight();

  lcd.createChar(0, bar);

  lcd.setCursor(0,0);
  lcd.print("Water Monitor");
  lcd.setCursor(0,1);
  lcd.print("System Start");
  delay(2000);
  lcd.clear();
}

void loop()
{
  long total = 0;

  // Take multiple readings for stable value
  for(int i = 0; i < 10; i++)
  {
    total += analogRead(waterSensor);
    delay(10);
  }

  int sensorValue = total / 10; // average value

  // Convert to percentage
  int percent = map(sensorValue, dryValue, fullValue, 0, 100);

  percent = constrain(percent, 0, 100);

  lcd.clear();

  // First line
  lcd.setCursor(0,0);
  lcd.print("Water Level:");
  lcd.print(percent);
  lcd.print("%");

  // Second line bar
  lcd.setCursor(0,1);

  int bars = percent / 10;

  for(int i = 0; i < bars; i++)
  lcd.write(byte(0));

  for(int i = bars; i < 10; i++)
  lcd.print(".");

  // Alert
  if(percent >= 90)
  digitalWrite(buzzer, HIGH);
  else
  digitalWrite(buzzer, LOW);

  delay(1200);  // slower update
}
