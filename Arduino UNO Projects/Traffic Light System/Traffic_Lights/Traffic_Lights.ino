// Traffic Lights System 
#include <TM1637Display.h>

// 4-digit display pins
#define CLK 2
#define DIO 3

// Traffic light pins
#define GREEN 8
#define YELLOW 9
#define RED 10

TM1637Display display(CLK, DIO);

// Real-life timing values (seconds)
int greenTime = 10;
int yellowTime = 3;
int redTime = 10;

void setup() {
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  display.setBrightness(7); // Display brightness
}

void loop() {

  // ===== GREEN LIGHT PHASE =====
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);

  for (int i = greenTime; i > 0; i--) {
    display.showNumberDec(i, true);
    delay(1000);
  }

  // ===== YELLOW LIGHT PHASE =====
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, LOW);

  for (int i = yellowTime; i > 0; i--) {
    display.showNumberDec(i, true);
    delay(1000);
  }

  // ===== RED LIGHT PHASE =====
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);

  for (int i = redTime; i > 0; i--) {
    display.showNumberDec(i, true);
    delay(1000);
  }
}
