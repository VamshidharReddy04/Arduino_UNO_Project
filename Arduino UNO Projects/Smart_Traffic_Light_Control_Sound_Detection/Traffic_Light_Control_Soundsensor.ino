#include <TM1637Display.h>

// Display pins
#define CLK 4
#define DIO 5
TM1637Display display(CLK, DIO);

// Traffic lights
int redLight = 8;
int yellowLight = 9;
int greenLight = 10;

// Sound sensor
int soundPin = A0;
int soundThreshold = 135;

// H O R N display segments
const uint8_t HORN[] = {
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // H
  SEG_C | SEG_D | SEG_E | SEG_G,           // O
  SEG_E | SEG_G,                           // R
  SEG_C | SEG_E | SEG_G                    // N
};

void setup()
{
  pinMode(redLight, OUTPUT);
  pinMode(yellowLight, OUTPUT);
  pinMode(greenLight, OUTPUT);

  display.setBrightness(7);

  Serial.begin(9600);
}

void loop()
{
  normalTraffic();
}

// -------- NORMAL TRAFFIC --------
void normalTraffic()
{
  // Countdown 15 → 0
  for(int i = 15; i >= 0; i--)
  {
    display.showNumberDec(i);

    // Traffic light sequence
    if(i >= 5)
    {
      setRed();
    }
    else if(i >= 1)
    {
      setYellow();
    }
    else
    {
      setGreen();
    }

    // Disable sound sensor during green light
    if(i >= 1)
    {
      int soundValue = analogRead(soundPin);
      Serial.println(soundValue);

      if(soundValue > soundThreshold)
      {
        hornMode();
        return;
      }
    }

    delay(1000);
  }

  // Forward countdown 0 → 15
  for(int i = 15; i >= 0; i--)
  {
    display.showNumberDec(i);

    if(i == 0)
    {
      setGreen();
    }

    delay(1000);
  }
}

// -------- HORN MODE --------
void hornMode()
{
  Serial.println("HORN DETECTED");

  while(true)
  {
    int soundValue = analogRead(soundPin);
    Serial.println(soundValue);

    // Stop traffic
    setRed();

    // Display HORN text
    display.setSegments(HORN);

    // Exit horn mode when sound stops
    if(soundValue < soundThreshold)
    {
      break;
    }

    delay(200);
  }

  // Reset traffic timer
  display.showNumberDec(15);
  delay(1000);
}

// -------- LIGHT FUNCTIONS --------

void setRed()
{
  digitalWrite(redLight, HIGH);
  digitalWrite(yellowLight, LOW);
  digitalWrite(greenLight, LOW);
}

void setYellow()
{
  digitalWrite(redLight, LOW);
  digitalWrite(yellowLight, HIGH);
  digitalWrite(greenLight, LOW);
}

void setGreen()
{
  digitalWrite(redLight, LOW);
  digitalWrite(yellowLight, LOW);
  digitalWrite(greenLight, HIGH);
}
