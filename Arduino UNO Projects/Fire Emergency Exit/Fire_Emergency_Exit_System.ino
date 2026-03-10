#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);
Servo gateServo;

int flameSensor = 2;
int redLED = 8;
int greenLED = 9;
int buzzer = 7;

void setup()
{
  pinMode(flameSensor, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  gateServo.attach(3);

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Fire Exit System");
  lcd.setCursor(0,1);
  lcd.print("System Ready");

  delay(2000);
}

void loop()
{
  int fireStatus = digitalRead(flameSensor);

  lcd.clear();

  if(fireStatus == LOW) // Fire detected
  {
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(buzzer, HIGH);

    gateServo.write(90); // Open gate

    lcd.setCursor(0,0);
    lcd.print("FIRE ALERT!");
    lcd.setCursor(0,1);
    lcd.print("Exit Gate Open");
  }
  else
  {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    digitalWrite(buzzer, LOW);

    gateServo.write(0); // Gate closed

    lcd.setCursor(0,0);
    lcd.print("System Safe");
    lcd.setCursor(0,1);
    lcd.print("Gate Closed");
  }

  delay(5000);
}
