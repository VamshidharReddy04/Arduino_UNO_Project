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
  SEG_E | SEG_G,                           // r
  SEG_C | SEG_E | SEG_G                    // n
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

    if(i >= 8)
    {
      setRed();

      if(checkHorn()) return;   // restart traffic if horn detected
    }
    else if(i >= 1)
    {
      setYellow();

      if(checkHorn()) return;   // restart traffic
    }
    else
    {
      setGreen();   // horn detection disabled
    }

    delay(1000);
  }

  // Green phase countdown
  for(int i = 15; i >= 0; i--)
  {
    setGreen();
    display.showNumberDec(i);
    delay(1000);
  }
}


// -------- HORN CHECK --------

bool checkHorn()
{
  int soundValue = analogRead(soundPin);
  Serial.println(soundValue);

  if(soundValue > soundThreshold)
  {
    hornMode();
    return true;   // signal restart
  }

  return false;
}


// -------- HORN MODE --------

void hornMode()
{
  Serial.println("HORN DETECTED");

  unsigned long startTime = millis();

  while(true)
  {
    int soundValue = analogRead(soundPin);

    setRed();                 // stop traffic
    display.setSegments(HORN);

    // stop if horn stops
    if(soundValue < soundThreshold)
      break;

    // stop after 3 seconds
    if(millis() - startTime > 3000)
      break;

    delay(2000);
  }

  // reset display
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
