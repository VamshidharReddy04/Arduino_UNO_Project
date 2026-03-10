#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
int flameAnalog = A0;   // Analog pin for fire level
int flameDigital = 2;   // Digital pin for fire detection
int redLED = 8;
int greenLED = 9;
int buzzer = 7;

void setup()
{
  pinMode(flameDigital, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Fire Detection");
  lcd.setCursor(0,1);
  lcd.print("System Ready");

  delay(2000);
  lcd.clear();
}

void loop()
{
  int flameStatus = digitalRead(flameDigital); // Fire detected or not
  int fireLevel = analogRead(flameAnalog);     // Fire intensity value

  lcd.clear();

  if(flameStatus == LOW)   // Fire detected
  {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(buzzer, HIGH);

    lcd.setCursor(0,0);
    lcd.print("FIRE ALERT!");

    lcd.setCursor(0,1);
    lcd.print("Level:");
    lcd.print(fireLevel);
  }
  else
  {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(buzzer, LOW);

    lcd.setCursor(0,0);
    lcd.print("System Safe");

    lcd.setCursor(0,1);
    lcd.print("Level:");
    lcd.print(fireLevel);
  }

  delay(1000);
}
