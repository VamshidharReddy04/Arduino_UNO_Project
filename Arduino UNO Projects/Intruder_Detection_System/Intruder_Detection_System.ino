#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

int pirSensor = 2;
int led = 8;
int buzzer = 7;

int motionState = LOW;

void setup()
{
  pinMode(pirSensor, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  // Startup Message
  lcd.setCursor(0,0);
  lcd.print("Home Security");
  lcd.setCursor(0,1);
  lcd.print("Initializing");
  
  Serial.println("PIR Sensor Warming Up...");
  
  delay(3000);   // PIR warm-up time (30 seconds)

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop()
{
  motionState = digitalRead(pirSensor);

  if(motionState == HIGH)
  {
    Serial.println("Motion Detected!");

    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);

    lcd.setCursor(0,0);
    lcd.print("WARNING!");
    lcd.setCursor(0,1);
    lcd.print("Intruder Detect");
  }
  else
  {
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);

    lcd.setCursor(0,0);
    lcd.print("Status:  ");
    lcd.setCursor(0,1);
    lcd.print("Area Secure     ");
  }

  delay(200);
}
