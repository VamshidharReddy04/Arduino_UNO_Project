#include <IRremote.h>

#define IR_RECEIVE_PIN 11

#define ENA 5
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9
#define ENB 10

IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

unsigned long lastCommand = 0;

int motorSpeedA = 150;
int motorSpeedB = 150;

void setup() {
  Serial.begin(115200);
  Serial.println("Car Ready");

  irrecv.enableIRIn();

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Stop();
}

void loop() {
  if (irrecv.decode(&results)) {

    unsigned long command = results.value;

    // Handle repeat signal
    if (command == 0xFFFFFFFF) {
      command = lastCommand;
    }

    Serial.print("Code: ");
    Serial.println(command, HEX);

    switch (command) {

      case 0x1FED827: // 2 → Forward
        Forward();
        break;

      case 0x1FEF00F: // 8 → Backward
        Backward();
        break;

      case 0x1FE30CF: // 4 → Left
        Left();
        break;

      case 0x1FE708F: // 6 → Right
        Right();
        break;

      case 0x1FEB04F: // 5 → Stop
        Stop();
        break;
    }

    lastCommand = command;
    irrecv.resume();
  }
}

// ===== MOTOR CONTROL =====

void Forward() {
  analogWrite(ENA, motorSpeedA);
  analogWrite(ENB, motorSpeedB);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Backward() {
  analogWrite(ENA, motorSpeedA);
  analogWrite(ENB, motorSpeedB);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Left() {
  analogWrite(ENA, motorSpeedA);
  analogWrite(ENB, motorSpeedB);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Right() {
  analogWrite(ENA, motorSpeedA);
  analogWrite(ENB, motorSpeedB);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
