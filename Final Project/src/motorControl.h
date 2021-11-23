#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

    void setMotorSpeed(unsigned char);

    void setMotorSpeed(unsigned char speed) {
        //Mode 7
        DDRH |= 0b01000000;
        TCCR2A |= 0b00100011;

        if (speed == 0) { 
            TCCR2B &= 0;
        }
        else {
            OCR2B = speed;
            TCCR2B |= 1;
        }
    }

#endif