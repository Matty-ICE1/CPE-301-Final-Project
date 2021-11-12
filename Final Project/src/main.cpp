#include <Arduino.h>
#include "readAnalog.h"

#define WATER_PIN 0
#define WATER_CUTOFF 100

// Globals---------------------------------------------
bool isDisabled = false;

// Function Protoypes----------------------------------
unsigned char getWaterLevel();

void setup() {
  startADC();  // start the ADC
  Serial.begin(9600);
}

void loop() {
}

// Additional Functions--------------------------------
unsigned char getWaterLevel() {
  unsigned int reading = readAnalog(WATER_PIN);
  if (reading < WATER_CUTOFF) return 0;
  return map(reading, WATER_CUTOFF, 500, 1, 100);
}