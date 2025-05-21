#include <math.h>

// Motor pins
#define M1_IN1 2
#define M1_IN2 3
#define M1_EN 9

#define M2_IN1 4
#define M2_IN2 5
#define M2_EN 10

#define M3_IN1 6
#define M3_IN2 7
#define M3_EN 11

String input = "";
int angle = 0;
int pitch = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT); pinMode(M1_EN, OUTPUT);
  pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT); pinMode(M2_EN, OUTPUT);
  pinMode(M3_IN1, OUTPUT); pinMode(M3_IN2, OUTPUT); pinMode(M3_EN, OUTPUT);
}

// Helper to run motor with signed speed
void setMotor(int in1, int in2, int en, float speed) {
  bool direction = speed >= 0;
  digitalWrite(in1, direction);
  digitalWrite(in2, !direction);
  analogWrite(en, constrain(abs(speed), 0, 255));
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (isDigit(c)) {
      input += c;
    }

    // When full 7 digits received
    if (input.length() == 7) {
      angle = input.substring(0, 3).toInt();   // e.g. "045" => 45°
      pitch = input.substring(3).toInt();      // e.g. "0750" => 750

      input = ""; // Reset for next input

      // Convert angle to radians
      float theta = radians(angle);
      float power = map(pitch, 0, 1000, 0, 255);

      // Wheel orientations in degrees
      float phi1 = radians(0);
      float phi2 = radians(120);
      float phi3 = radians(240);

      // Compute wheel speeds
      float v1 = sin(theta - phi1) * power;
      float v2 = sin(theta - phi2) * power;
      float v3 = sin(theta - phi3) * power;

      // Apply to motors
      setMotor(M1_IN1, M1_IN2, M1_EN, v1);
      setMotor(M2_IN1, M2_IN2, M2_EN, v2);
      setMotor(M3_IN1, M3_IN2, M3_EN, v3);
    }

    // If more than 7 digits received (invalid input), reset
    if (input.length() > 7) {
      input = "";
    }
  }
}