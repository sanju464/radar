#include <Servo.h>

const int trigPin = 6;
const int echoPin = 7;
const int servoPin = 9;

Servo radarServo;

void setup() {
  Serial.begin(9600);

  radarServo.attach(servoPin);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Radar starting...");
}

// Function to measure distance in cm
float getDistanceCM() {
  // Clear trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // 10us trigger pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse in microseconds
  long duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms (~5m range)

  if (duration == 0) {
    // No echo received (out of range or error)
    return -1;
  }

  // Speed of sound ≈ 0.034 cm/us
  float distance = (duration * 0.034) / 2.0;
  return distance;
}

void loop() {
  // Sweep forward: 15° to 165°
  for (int angle = 15; angle <= 165; angle++) {
    radarServo.write(angle);    // move servo
    delay(20);                  // allow servo to reach position

    float distance = getDistanceCM();

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" deg  |  Distance: ");

    if (distance < 0) {
      Serial.println("Out of range");
    } else {
      Serial.print(distance);
      Serial.println(" cm");
    }
  }

  // Sweep backward: 165° to 15°
  for (int angle = 165; angle >= 15; angle--) {
    radarServo.write(angle);
    delay(20);

    float distance = getDistanceCM();

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" deg  |  Distance: ");

    if (distance < 0) {
      Serial.println("Out of range");
    } else {
      Serial.print(distance);
      Serial.println(" cm");
    }
  }
}


