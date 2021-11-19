#ifndef CONFIG_H
#define CONFIG_H

    //Water Sensor Configuration
    #define WATER_PIN 0  // A0
    #define WATER_CUTOFF 100

    //Servo Motor Configuration
    #define SERVO_PIN 6
    #define SERVO_KNOB 1  // A1

    // Temperature Configuration
    #define DHT_PIN 2
    #define TEMP_THRESHOLD 25 // *C
    #define DHT_SENSOR_TYPE DHT_TYPE_11

    //LCD Pins on MEGA
    #define RS 22
    #define EN 23
    #define D4 24
    #define D5 25
    #define D6 26
    #define D7 27

#endif