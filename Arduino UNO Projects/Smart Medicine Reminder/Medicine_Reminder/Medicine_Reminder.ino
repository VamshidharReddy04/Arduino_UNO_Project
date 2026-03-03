#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

// ===== PINS =====
#define SERVO_PIN 5
#define BUZZER_PIN 8
#define LED_PIN 7

// ===== SERVO CONTROL =====
int currentSlot = 0;
int slotAngles[4] = {0, 45, 90, 135};  
// 🔴 Adjust these after calibration

// ===== DEMO TIMES =====
unsigned long morningTime = 10000;
unsigned long afternoonTime = 20000;
unsigned long nightTime = 30000;

bool morningDone = false;
bool afternoonDone = false;
bool nightDone = false;

void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(slotAngles[0]); // start at home

  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Medicine Box");
  lcd.setCursor(0,1);
  lcd.print("4 Slot Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentTime = millis();

  lcd.setCursor(0,0);
  lcd.print("Waiting...     ");

  if (currentTime >= morningTime && !morningDone) {
    moveNextSlot("Morning");
    morningDone = true;
  }

  if (currentTime >= afternoonTime && !afternoonDone) {
    moveNextSlot("Afternoon");
    afternoonDone = true;
  }

  if (currentTime >= nightTime && !nightDone) {
    moveNextSlot("Night");
    nightDone = true;

    // ⭐ AFTER NIGHT → RETURN HOME
    delay(4000);
    returnToHome();
  }
}

// ===== MOVE TO NEXT SLOT =====
void moveNextSlot(String period) {

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(period);
  lcd.setCursor(0,1);
  lcd.print("Take Medicine");

  digitalWrite(LED_PIN, HIGH);

  // buzzer alert
  for(int i=0;i<5;i++){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    delay(300);
  }

  // move to next slot
  currentSlot++;
  if(currentSlot > 3) currentSlot = 0;

  myServo.write(slotAngles[currentSlot]);
  delay(1500);

  digitalWrite(LED_PIN, LOW);
  lcd.clear();
}

// ===== RETURN TO START =====
void returnToHome() {
  lcd.setCursor(0,0);
  lcd.print("Reset Position");

  currentSlot = 0;
  myServo.write(slotAngles[0]); // back to slot 0
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cycle Complete");
}
