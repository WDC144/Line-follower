// Include libraries
#include <AFMotor.h>
#include <QTRSensors.h>
#include <SoftwareSerial.h>

// Set up motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

// Set up IR sensors
#define NUM_SENSORS 3
#define TIMEOUT 2500
#define EMITTER_PIN 2
QTRSensorsRC qtrrc((unsigned char[]) {A0, A1, A2}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);

// Set up Bluetooth
SoftwareSerial bluetooth(10, 11); // RX, TX

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Set up Bluetooth
  bluetooth.begin(9600);

  // Calibrate IR sensors
  int i;
  for (i = 0; i < 250; i++) {
    qtrrc.calibrate();
  }

  // Turn off IR emitter
  pinMode(EMITTER_PIN, OUTPUT);
  digitalWrite(EMITTER_PIN, LOW);
}

void loop() {
  // Read IR sensors
  unsigned int sensorValues[NUM_SENSORS];
  qtrrc.read(sensorValues);

  // Determine motor speeds based on sensor readings
  int16_t speedDifference = (sensorValues[1] - sensorValues[0]) * 5;
  int16_t leftSpeed = 50 + speedDifference / 2;
  int16_t rightSpeed = 50 - speedDifference / 2;

  // Move the motors
  motor1.setSpeed(abs(leftSpeed));
  motor2.setSpeed(abs(rightSpeed));
  if (leftSpeed > 0) {
    motor1.run(FORWARD);
  } else {
    motor1.run(BACKWARD);
  }
  if (rightSpeed > 0) {
    motor2.run(FORWARD);
  } else {
    motor2.run(BACKWARD);
  }

  // Check for remote control input
  if (bluetooth.available()) {
    char command = bluetooth.read();
    switch (command) {
      case 'w':
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        break;
      case 'a':
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
        break;
      case 'd':
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
        break;
      case 's':
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);
        break;
    }
  }
}
