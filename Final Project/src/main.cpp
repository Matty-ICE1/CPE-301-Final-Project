#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include "readAnalog.h"
#include "readDigital.h"
#include "globalTimer.h"
#include "motorControl.h"

#include "config.h"

//Globals-----------------------------------------------
enum State {idle, running, disabled, error};
State swampCooler;
float temperature = 0.0;
float humidity = 0.0;
unsigned long lastStatusCheck = 0;
unsigned char waterLevel, controlPot;
bool isDisabled;

// Hardware---------------------------------------------
Servo vent;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); // rs, enable, d4, d5, d6, d7
DHT_nonblocking dht_sensor( DHT_PIN, DHT_SENSOR_TYPE );

// Function Protoypes----------------------------------
unsigned char getWaterLevel();
void digitalWriteLED(State, bool);
void updateState();


//MAIN=================================================
void setup() {
  //Output pins
  DDRB |= 0b00001111; // PB 4,5,6,7 as output
  DDRL &= 0b11111110; // PL0 is input
  DDRE |= 0b00001000; // PE3 as output
  PORTL |= 0b00000001; // enable pullup resistor

  //ADC
  startADC();  // start the ADC

  //Servo
  vent.attach(SERVO_PIN);

  //LCD Setup
  lcd.begin(16, 2); // cols rows
  lcd.clear();

  // Motor
  setMotorSpeed(0);

  //Timer
  startGlobalTimer();

  //Debug
  Serial.begin(9600);
}

void loop() {

  // Secondary Polling Block---------------------------
  // This function runs every BUTTON_UPD... interval
  // configured in 'config.h'
  // nothing else really should go here
  if (buttonRefreshTimer > BUTTON_UPDATE_INT) {
    if (risingReadPortL(0)) {
      if (isDisabled) isDisabled = false;
      else isDisabled = true;
      updateState();
    }
    buttonRefreshTimer = 0;
  }

  // Primary Polling Block----------------------------
  // Runs every five seconds. NO SOONER OR GLITCHIES
  // configured in 'config.h'
  if (sensorDisplayTimer > SENSOR_UPDATE_INT) {
    waterLevel = analogRead(WATER_PIN);
    //Temperature and humidity
    sensorDisplayTimer = 0;
  }

  globalTimerRun();  // update the global timer
}

// Additional Functions--------------------------------

void digitalWriteLED(State c, bool state) {
    unsigned char l;
    switch(c) {
        case error:  // MEGA pin 50
            l = 0b00001000;
            break;

        case disabled:  // MEGA pin 51
            l = 0b00000100;
            break;

        case running:  // MEGA pin 52
            l = 0b00000010;
            break;

        case idle:  // MEGA pin 53
            l = 0b00000001;
            break;

        default:
            break;
    }

    if (state) PORTB = l;
    else PORTB = ~l;
}

void updateState() {

  if (isDisabled) swampCooler = disabled;
  else if (waterLevel < WATER_CUTOFF) swampCooler = error;
  else if (temperature < TEMP_THRESHOLD) swampCooler = idle;
  else swampCooler = running;

  digitalWriteLED(swampCooler, 1);

  switch (swampCooler)
    {
    case disabled:
      setMotorSpeed(0);
      break;

    case error:
      setMotorSpeed(0);
      break;

    case idle:
      setMotorSpeed(0);
      break;

    case running:
      setMotorSpeed(100);
      break;
    
    default:
      break;
    }
}
