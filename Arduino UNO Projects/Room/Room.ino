#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ---------------- RFID ----------------
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ---------------- LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- Servo ----------------
Servo doorServo;
#define SERVO_PIN 6

// ---------------- AUTHORIZED CARD UID ----------------
// 🔴 Replace with your card UID
byte authorizedUID[4] = {0xF3, 0xB6, 0x2A, 0x0E};

void setup() {

  Serial.begin(9600);

  // RFID init
  SPI.begin();
  mfrc522.PCD_Init();

  // LCD init
  lcd.begin();
  lcd.backlight();

  // Servo init
  doorServo.attach(SERVO_PIN);
  doorServo.write(0);   // Door closed

  // Startup message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Home");
  lcd.setCursor(0, 1);
  lcd.print("Place Card...");
}

void loop() {

  // Wait for card
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  bool accessGranted = true;

  // Compare UID
  for (byte i = 0; i < 4; i++) {
    if (mfrc522.uid.uidByte[i] != authorizedUID[i]) {
      accessGranted = false;
      break;
    }
  }

  lcd.clear();

  if (accessGranted) {

    lcd.setCursor(0, 0);
    lcd.print("Family Member");
    lcd.setCursor(0, 1);
    lcd.print("Door Open");

    // Open door
    doorServo.write(90);
    delay(3000);

    // Close door
    doorServo.write(0);

  } else {

    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    lcd.setCursor(0, 1);
    lcd.print("Try Again");
    delay(2000);
  }

  // Reset display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Home");
  lcd.setCursor(0, 1);
  lcd.print("Place Card...");

  // Stop RFID
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1500);
}
