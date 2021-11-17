#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "readAnalog.h"

//PORT B LED Configuration
#define RED 7
#define BLUE 6
#define GREEN 5
#define YELLOW 4

//Water Sensor Configuration
#define WATER_PIN 0  // A0
#define WATER_CUTOFF 100

//Servo Motor Configuration
#define SERVO_PIN 6
#define SERVO_KNOB 1  // A1

// Temperature Configuration
#define TEMP_THRESHOLD 25 // *C

#define RS 22
#define EN 23
#define D4 24
#define D5 25
#define D6 26
#define D7 27

// Globals---------------------------------------------
bool isDisabled = false;
char status;
Servo vent;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); // rs, enable, d4, d5, d6, d7

// Function Protoypes----------------------------------
unsigned char getWaterLevel();
void runLCD(unsigned char);
void digitalWriteLED(unsigned char, bool);
float getTemperature();


//MAIN=================================================
void setup() {
  //Output pins
  DDRB |= 0b00001111; // PB 4,5,6,7

  //ADC
  startADC();  // start the ADC

  //Servo
  vent.attach(SERVO_PIN);

  //LCD Setup
  lcd.begin(16, 2); // cols rows
  lcd.clear();
}

void loop() {
  if(!isDisabled) {
    if (getWaterLevel() < 0) status = 'E';  // Disabled
    else status = 'I'; // Idle

    if (getTemperature() > TEMP_THRESHOLD) status = 'R';
    else status = 'I';
  }
  else {
    //yellow LED
  }
}

// Additional Functions--------------------------------
unsigned char getWaterLevel() {
  unsigned int reading = readAnalog(WATER_PIN);
  if (reading < WATER_CUTOFF) return 0;
  return (unsigned char) map(reading, WATER_CUTOFF, 896, 1, 100);
}

void runLCD(unsigned char s) {
}

void digitalWriteLED(unsigned char l, bool state) {
    switch(l) {
        case 'r':  // MEGA pin 50
            l = 0b00001000;
            break;

        case 'y':  // MEGA pin 51
            l = 0b00000100;
            break;

        case 'g':  // MEGA pin 52
            l = 0b00000010;
            break;

        case 'b':  // MEGA pin 53
            l = 0b00000001;
            break;

        default:
            break;
    }

    if (state) PORTB |= l;
    else PORTB &= ~l;
}

float getTemperature(){
  return 0.0;
}
