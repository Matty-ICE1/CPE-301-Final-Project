#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include "readAnalog.h"
#include "readDigital.h"
#include "globalTimer.h"

#include "config.h"

//Globals-----------------------------------------------
enum State {idle, running, disabled, error};
State swampCooler;

float temperature, humidity;
unsigned char waterLevel, controlPot;
bool isDisabled;

// Hardware---------------------------------------------
Servo vent;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); // rs, enable, d4, d5, d6, d7
DHT_nonblocking dht_sensor( DHT_PIN, DHT_SENSOR_TYPE );

// Function Protoypes----------------------------------
unsigned char getWaterLevel();
void runLCD(unsigned char);
void digitalWriteLED(unsigned char, bool);
float getTemperature();


//MAIN=================================================
void setup() {
  //Output pins
  DDRB |= 0b00001111; // PB 4,5,6,7
  DDRL &= 0b11111110; // PL0 is input
  PORTL |= 0b00000001; // enable pullup resistor

  //ADC
  startADC();  // start the ADC

  //Servo
  vent.attach(SERVO_PIN);

  //LCD Setup
  lcd.begin(16, 2); // cols rows
  lcd.clear();

  //Debug
  Serial.begin(9600);
}

void loop() {
  waterLevel = analogRead(WATER_PIN);

  // state setting
  if (isDisabled) swampCooler = disabled;
  else if (waterLevel < WATER_CUTOFF) swampCooler = error;
  else if (temperature < TEMP_THRESHOLD) swampCooler = idle;
  else swampCooler = running;

  switch (swampCooler)
  {
  case disabled:
    digitalWriteLED('y', 1);
    break;

  case error:
    digitalWriteLED('r', 1);
    break;

  case idle:
    digitalWriteLED('b', 1);
    break;

  case running:
    digitalWriteLED('g', 1);
    break;
  
  default:
    break;
  }
}

// Additional Functions--------------------------------

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
