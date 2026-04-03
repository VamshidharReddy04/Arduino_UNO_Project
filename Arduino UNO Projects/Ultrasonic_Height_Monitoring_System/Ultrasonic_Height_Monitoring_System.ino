#include <TM1637Display.h>
#include <SimpleKalmanFilter.h>

// Pin definitions
const int trigPin = 9;
const int echoPin = 10;
const int CLK_PIN = 2;
const int DIO_PIN = 3;

// Constants
const int SENSOR_HEIGHT = 190;
const unsigned long MEASURE_INTERVAL = 100;
const unsigned long PULSE_TIMEOUT = 30000;
const int MAX_STABLE_VALUE = 20;
const int STABLE_COUNT = 1;  // You can keep 1 as per your logic

// Variables
int currentHeight = 0;
int lastStableHeight = -1;
unsigned long previousMeasureTime = 0;
int stabilityCounter = 0;

// Objects
TM1637Display display(CLK_PIN, DIO_PIN);
SimpleKalmanFilter heightFilter(2, 2, 0.1);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);

  display.setBrightness(7);
  display.clear();

  Serial.begin(9600);
  Serial.println("System started...");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousMeasureTime >= MEASURE_INTERVAL) {
    previousMeasureTime = currentTime;

    int rawHeight = measureHeight();

    float filteredHeight = heightFilter.updateEstimate(rawHeight);

    currentHeight = processStableValue((int)filteredHeight);

    updateDisplay(currentHeight);

    Serial.print("Raw: ");
    Serial.print(rawHeight);
    Serial.print("mm, Filtered: ");
    Serial.print(filteredHeight);
    Serial.print("mm, Display: ");
    Serial.println(currentHeight);
  }
}

int measureHeight() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, PULSE_TIMEOUT);

  if (duration == 0) {
    return (lastStableHeight == -1) ? 0 : lastStableHeight;
  }

  float distance = duration * 0.343 / 2.0;
  int height = SENSOR_HEIGHT - (int)distance;

  return constrain(height, 0, SENSOR_HEIGHT);
}

int processStableValue(int newValue) {
  if (lastStableHeight == -1) {
    lastStableHeight = newValue;
    return newValue;
  }

  if (abs(newValue - lastStableHeight) <= MAX_STABLE_VALUE) {
    stabilityCounter++;
    if (stabilityCounter >= STABLE_COUNT) {
      lastStableHeight = newValue;
      stabilityCounter = 0;
    }
    return lastStableHeight;
  } else {
    stabilityCounter = 0;
    return lastStableHeight + (newValue > lastStableHeight ? 1 : -1);
  }
}

void updateDisplay(int value) {
  if (value > 9999) value = 9999;

  static int lastDisplayed = -1;
  if (value != lastDisplayed) {
    display.showNumberDec(value, true);
    lastDisplayed = value;
  }
}
