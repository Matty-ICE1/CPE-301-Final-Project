#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include <DS3231.h>
#include "readAnalog.h"
#include "readDigital.h"
#include "globalTimer.h"
#include "motorControl.h"

#include "config.h"


//Globals-----------------------------------------------
enum State {idle, running, disabled, error};
State swampCooler, lastState;
float temperature = 0.0;
float humidity = 0.0;
unsigned long lastStatusCheck = 0;
unsigned int waterLevel, controlPot;
bool isDisabled;

// Hardware---------------------------------------------
Servo vent;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); // rs, enable, d4, d5, d6, d7
DHT_nonblocking dht_sensor( DHT_PIN, DHT_SENSOR_TYPE );
DS3231 rtcModule;
RTCDateTime rtcTime;

// Function Protoypes----------------------------------
unsigned char getWaterLevel();
void updateState();
bool measure_environment( float *temperature, float *humidity );


//MAIN=================================================
void setup() {
  Serial.begin(9600);
  //Output pins
  DDRB |= 0b00001111; // PB 4,5,6,7 as output

  startPortL();

  //ADC
  startADC();  // start the ADC

  //Servo
  vent.attach(SERVO_PIN);

  //LCD Setup
  lcd.begin(16, 2); // cols rows
  lcd.noAutoscroll();
  lcd.clear();

  // Motor
  setMotorSpeed(0);

  // Clock
  rtcModule.begin();
  rtcModule.setDateTime(__DATE__, __TIME__); // compile time
  rtcTime = rtcModule.getDateTime();
  
  //Timer
  startGlobalTimer();

  waterLevel = readAnalog(WATER_PIN);
  dht_sensor.measure(&temperature, &humidity);

  swampCooler = lastState = idle;
  updateState();
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
    waterLevel = readAnalog (WATER_PIN);
    rtcTime = rtcModule.getDateTime();
    updateState();
    sensorDisplayTimer = 0;
  }

  // Based on testing, this sensor only works if polled continuously. 
  if (!isDisabled) dht_sensor.measure(&temperature, &humidity);
  vent.write(map(readAnalog(SERVO_KNOB), 0, 1023, 0, 255));
  globalTimerRun();  // update the global timer
}

// Additional Functions--------------------------------

void updateState() {

  if (isDisabled)  swampCooler = disabled;
  else if (waterLevel < WATER_CUTOFF) swampCooler = error;
  else if (temperature < TEMP_THRESHOLD) swampCooler = idle;
  else swampCooler = running;

  if (swampCooler != lastState) {
    if ( (swampCooler == running && lastState != running) || ((swampCooler != running && lastState == running))) { // if we were/are running
      Serial.print("Motor Transition: ");
      Serial.print(swampCooler == running ? "ON " : "OFF ");
      Serial.print("@ ");
      Serial.print(rtcTime.year);   Serial.print("-");
      Serial.print(rtcTime.month);  Serial.print("-");
      Serial.print(rtcTime.day);    Serial.print(" ");
      Serial.print(rtcTime.hour);   Serial.print(":");
      Serial.print(rtcTime.minute); Serial.print(":");
      Serial.print(rtcTime.second); Serial.println("");
    }

    lastState = swampCooler;
  }


  unsigned char l;
  lcd.flush();
  lcd.clear();
  lcd.setCursor(9, 0);
  lcd.print(temperature); lcd.print("'C");
  lcd.setCursor(9, 1);
  lcd.print(humidity); lcd.print("%H");

  if (swampCooler == idle || swampCooler == running) {
    lcd.setCursor(0, 1);
    lcd.print(rtcTime.hour); lcd.print(':');
    lcd.print(rtcTime.minute); lcd.print(':');
    lcd.print(rtcTime.second);
  }
  lcd.setCursor(0,0);  // prime for display

  switch (swampCooler)
    {
    case disabled:
      l = 0b00000001;
      setMotorSpeed(0);
      lcd.clear();
      lcd.print("Disabled");
      break;

    case error:
      l = 0b00000010;
      setMotorSpeed(0);
      lcd.print("Error");
      lcd.setCursor(0, 1);
      lcd.print("Refill");
      break;

    case idle:
      l = 0b00001000;
      setMotorSpeed(0);
      lcd.print("Idle");
      break;

    case running:
      l = 0b00000100;
      setMotorSpeed(255);
      lcd.print("Running");
      break;
    
    default:
      break;
    }

    PORTB = l;
}
