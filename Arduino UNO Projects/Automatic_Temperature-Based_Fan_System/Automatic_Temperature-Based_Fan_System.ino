#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

// Motor driver pins
int IN1 = 9;   // PWM pin
int IN2 = 8;

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);

  dht.begin();

  lcd.begin();
  lcd.backlight();

  // Startup Message
  lcd.setCursor(0,0);
  lcd.print("Temperature Based");
  lcd.setCursor(0,1);
  lcd.print("Fan System");
  delay(3000);
  lcd.clear();
}

void loop()
{
  float temp = dht.readTemperature();

  // Check if sensor failed
  if (isnan(temp))
  {
    Serial.println("Sensor Error!");
    lcd.setCursor(0,0);
    lcd.print("Sensor Error");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C");

  // COOL
  if(temp < 31.10)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    lcd.setCursor(0,1);
    lcd.print("Cool - Fan OFF");

    Serial.println("Fan OFF (Cool)");
  }

  // WARM (Medium Speed)
  else if(temp >= 31.30 && temp <= 31.70)
  {
    analogWrite(IN1, 150);
    digitalWrite(IN2, LOW);

    lcd.setCursor(0,1);
    lcd.print("Warm - Fan MID");

    Serial.println("Fan Medium Speed");
  }

  // HOT (High Speed)
  else
  {
    analogWrite(IN1, 255);
    digitalWrite(IN2, LOW);

    lcd.setCursor(0,1);
    lcd.print("Hot - Fan HIGH");

    Serial.println("Fan High Speed");
  }

  delay(2000);
}
