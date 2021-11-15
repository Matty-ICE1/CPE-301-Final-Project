#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "readAnalog.h"

#define WATER_PIN 0  // A0
#define WATER_CUTOFF 100

#define SERVO_PIN 6
#define SERVO_KNOB 1  // A1

// Globals---------------------------------------------
bool isDisabled = false;
unsigned char status = 0;
Servo vent;
LiquidCrystal lcd(8,9, 4,5,6,7); // rs, enable, d4, d5, d6, d7

// Function Protoypes----------------------------------
unsigned char getWaterLevel();
void runLCD(unsigned char);

void setup() {
  startADC();  // start the ADC
  Serial.begin(9600);
  vent.attach(SERVO_PIN);

  //LCD Setup
  lcd.begin(16, 2); // cols rows
  lcd.clear();
}

void loop() {
  vent.write(map(readAnalog(SERVO_KNOB), 2, 894, 0, 100));
  runLCD(status);
}

// Additional Functions--------------------------------
unsigned char getWaterLevel() {
  unsigned int reading = readAnalog(WATER_PIN);
  if (reading < WATER_CUTOFF) return 0;
  return (unsigned char) map(reading, WATER_CUTOFF, 896, 1, 100);
}

void runLCD(unsigned char s) {
  if (s== 0) {
    lcd.print("Status:");
  }
}
