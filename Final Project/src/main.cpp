#include <Arduino.h>
#include <Servo.h>
#include "readAnalog.h"

#define WATER_PIN 0  // A0
#define WATER_CUTOFF 100

#define SERVO_PIN 6
#define SERVO_KNOB 1  // A1

// Globals---------------------------------------------
bool isDisabled = false;
Servo vent;

// Function Protoypes----------------------------------
unsigned char getWaterLevel();

void setup() {
  startADC();  // start the ADC
  Serial.begin(9600);
  vent.attach(SERVO_PIN);
}

void loop() {
  vent.write(map(readAnalog(SERVO_KNOB), 2, 894, 0, 100));
}

// Additional Functions--------------------------------
unsigned char getWaterLevel() {
  unsigned int reading = readAnalog(WATER_PIN);
  if (reading < WATER_CUTOFF) return 0;
  return (unsigned char) map(reading, WATER_CUTOFF, 896, 1, 100);
}
