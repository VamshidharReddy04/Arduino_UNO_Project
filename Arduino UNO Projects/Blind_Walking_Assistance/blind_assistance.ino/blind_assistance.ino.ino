// Smart Blind Walking Assistance System
// Components: Ultrasonic Sensor, Buzzer, LED, Arduino UNO

// Pin Definitions
#define trigPin 9
#define echoPin 10
#define buzzerPin 6
#define ledPin 5

long duration;     // Stores echo time
int distance;      // Stores calculated distance

void setup() {
  // Set pin modes
  pinMode(trigPin, OUTPUT);   // Ultrasonic TRIG
  pinMode(echoPin, INPUT);    // Ultrasonic ECHO
  pinMode(buzzerPin, OUTPUT); // Buzzer
  pinMode(ledPin, OUTPUT);    // LED

  // Start serial monitor
  Serial.begin(9600);
}

void loop() {
  // Clear TRIG pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send ultrasonic pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  distance = duration * 0.034 / 2;

  // Display distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Condition for obstacle detection
  if (distance > 0 && distance <= 25) {
    digitalWrite(buzzerPin, HIGH); // Turn buzzer ON
    digitalWrite(ledPin, HIGH);    // Turn LED ON
  } else {
    digitalWrite(buzzerPin, LOW);  // Turn buzzer OFF
    digitalWrite(ledPin, LOW);     // Turn LED OFF
  }

  delay(200); // Small delay
}
