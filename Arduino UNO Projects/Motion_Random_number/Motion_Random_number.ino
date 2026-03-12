// Tilt Motion Random Number Generator

int tiltPin = 2;

// 7-segment pins
int a = 3;
int b = 4;
int c = 5;
int d = 6;
int e = 7;
int f = 8;
int g = 9;

int lastState = LOW;

void setup()
{
  pinMode(tiltPin, INPUT);

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);

  randomSeed(analogRead(0));
}

void loop()
{
  int tiltState = digitalRead(tiltPin);

  if (tiltState == HIGH && lastState == LOW)
  {
    int number = random(0,10);
    displayNumber(number);
  }

  lastState = tiltState;
}

// Function to display number
void displayNumber(int num)
{
  bool digits[10][7] = {
    {1,1,1,1,1,1,0}, //0
    {0,1,1,0,0,0,0}, //1
    {1,1,0,1,1,0,1}, //2
    {1,1,1,1,0,0,1}, //3
    {0,1,1,0,0,1,1}, //4
    {1,0,1,1,0,1,1}, //5
    {1,0,1,1,1,1,1}, //6
    {1,1,1,0,0,0,0}, //7
    {1,1,1,1,1,1,1}, //8
    {1,1,1,1,0,1,1}  //9
  };

  digitalWrite(a, digits[num][0]);
  digitalWrite(b, digits[num][1]);
  digitalWrite(c, digits[num][2]);
  digitalWrite(d, digits[num][3]);
  digitalWrite(e, digits[num][4]);
  digitalWrite(f, digits[num][5]);
  digitalWrite(g, digits[num][6]);
}
