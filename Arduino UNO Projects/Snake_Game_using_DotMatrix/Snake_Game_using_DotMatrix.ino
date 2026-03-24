#include <LedControl.h>

#define DIN 11
#define CS 10
#define CLK 13

LedControl lc = LedControl(DIN, CLK, CS, 1);

// Joystick Pins
#define VRX A0
#define VRY A1

int snakeX[64];
int snakeY[64];
int length = 2;

int dirX = 1;
int dirY = 0;

int foodX = 4;
int foodY = 4;

unsigned long lastMove = 0;
int speedDelay = 300;

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);

  snakeX[0] = 3;
  snakeY[0] = 3;
  snakeX[1] = 2;
  snakeY[1] = 3;

  randomSeed(analogRead(0));
}

void loop() {
  readJoystick();

  if (millis() - lastMove > speedDelay) {
    moveSnake();
    checkFood();
    drawGame();
    lastMove = millis();
  }
}

void readJoystick() {
  int x = analogRead(VRX);
  int y = analogRead(VRY);

  if (x < 400) { dirX = -1; dirY = 0; }   // LEFT
  else if (x > 600) { dirX = 1; dirY = 0; } // RIGHT
  else if (y < 400) { dirX = 0; dirY = 1; } // DOWN
  else if (y > 600) { dirX = 0; dirY = -1; } // UP
}

void moveSnake() {
  for (int i = length; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] += dirX;
  snakeY[0] += dirY;

  // Wrap around edges
  if (snakeX[0] < 0) snakeX[0] = 7;
  if (snakeX[0] > 7) snakeX[0] = 0;
  if (snakeY[0] < 0) snakeY[0] = 7;
  if (snakeY[0] > 7) snakeY[0] = 0;
}

void checkFood() {
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    length++;
    foodX = random(0, 8);
    foodY = random(0, 8);
  }
}

void drawGame() {
  lc.clearDisplay(0);

  // Draw snake
  for (int i = 0; i < length; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }

  // Draw food
  lc.setLed(0, foodY, foodX, true);
}
