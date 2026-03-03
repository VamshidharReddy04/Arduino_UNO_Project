// RFID Based Secure Locker System
// Innovation Project

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

// RFID Pins
#define SS_PIN 10
#define RST_PIN 9

// LED & Buzzer Pins
#define GREEN_LED 6
#define RED_LED 5
#define BUZZER 4

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 👉 Replace this with YOUR real card UID
String authorizedUID = "713fec08";   // Example only

void setup() {

  Serial.begin(9600);

  // LCD Init
  lcd.begin();
  lcd.backlight();

  // RFID Init
  SPI.begin();
  rfid.PCD_Init();

  // Outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Secure Locker");
  lcd.setCursor(0, 1);
  lcd.print("Scan RFID Card");

  Serial.println("RFID Secure Locker Ready...");
}

void loop() {

  // Wait for card
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  // Build UID properly
  String cardUID = "";

  for (byte i = 0; i < rfid.uid.size; i++) {

    if (rfid.uid.uidByte[i] < 0x10)
      cardUID += "0";

    cardUID += String(rfid.uid.uidByte[i], HEX);
  }

  // Print UID
  Serial.print("Card UID: ");
  Serial.println(cardUID);

  lcd.clear();

  // Compare UID
  if (cardUID == authorizedUID) {

    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print("Welcome User");

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);

  } else {

    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    lcd.setCursor(0, 1);
    lcd.print("Invalid Card");

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
  }

  delay(2000);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Card");

  // Stop communication with card
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
