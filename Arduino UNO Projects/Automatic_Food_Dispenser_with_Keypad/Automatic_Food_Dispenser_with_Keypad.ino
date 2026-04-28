#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {6,7,8,10};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Time variables
int hh = 0, mm = 0, ss = 0;
int field = 0; // 0 = SS, 1 = MM, 2 = HH

bool started = false;
unsigned long previousMillis = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  servo.attach(9);

  lcd.setCursor(0,0);
  lcd.print("Automatic Food");
  lcd.setCursor(0,1);
  lcd.print("Dispenser");
  delay(2000);

  displayTime();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    handleKey(key);
  }

  if (started) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;

      if (ss > 0) ss--;
      else {
        ss = 59;
        if (mm > 0) mm--;
        else {
          mm = 59;
          if (hh > 0) hh--;
        }
      }

      displayTime();

      if (hh==0 && mm==0 && ss==0) {
        dispenseFood();
        started = false;
      }
    }
  }
}

// Handle keypad input
void handleKey(char key) {

  // NUMBER INPUT
  if (key >= '0' && key <= '9') {
    int val = key - '0';

    if (field == 0) {        // SS
      ss = (ss * 10 + val) % 60;
    }
    else if (field == 1) {   // MM
      mm = (mm * 10 + val) % 60;
    }
    else if (field == 2) {   // HH
      hh = (hh * 10 + val) % 24;
    }

    displayTime();
  }

  // 👉 MOVE FORWARD (*)
  else if (key == '*') {
    field++;
    if (field > 2) field = 0;
  }

  // 👉 MOVE BACKWARD (#)
  else if (key == '#') {
    field--;
    if (field < 0) field = 2;
  }

  // START
  else if (key == 'A') {
    started = true;
  }

  // REMOVE LAST DIGIT
  else if (key == 'B') {
    if (field == 0) ss /= 10;
    else if (field == 1) mm /= 10;
    else if (field == 2) hh /= 10;

    displayTime();
  }

  // CLEAR ALL
  else if (key == 'C') {
    hh = mm = ss = 0;
    displayTime();
  }

  // RESET
  else if (key == 'D') {
    hh = mm = ss = 0;
    started = false;
    field = 0;
    displayTime();
  }
}

// Display function
void displayTime() {
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("SetTime HH:MM:SS");

  lcd.setCursor(0,1);

  if (hh < 10) lcd.print("0");
  lcd.print(hh); lcd.print(":");

  if (mm < 10) lcd.print("0");
  lcd.print(mm); lcd.print(":");

  if (ss < 10) lcd.print("0");
  lcd.print(ss);

  // 👉 Cursor indicator
  if (field == 0) lcd.setCursor(12,1); // SS
  else if (field == 1) lcd.setCursor(9,1); // MM
  else lcd.setCursor(6,1); // HH

  lcd.blink();
}

// Servo function
void dispenseFood() {
  lcd.clear();
  lcd.print("Dispensing...");

  servo.write(90);
  delay(1000);

  servo.write(0);
  delay(1000);
}
