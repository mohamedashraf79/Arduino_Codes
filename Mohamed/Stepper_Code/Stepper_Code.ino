#include <AccelStepper.h>

#define STEP_PIN 9
#define DIR_PIN 8
#define ENABLE_PIN 7

#define MOTOR_INTERFACE_TYPE 1

AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);  // Enable driver

  stepper.setMaxSpeed(3000);       // Start low
  stepper.setSpeed(-2667);          // Continuous speed
}

void loop() {
  stepper.runSpeed();             // Continuous rotation
}
