#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

    void setMotorSpeed(unsigned char);

    void setMotorSpeed(unsigned char speed) {
        //Mode 7
        DDRH |= 0b01000000;
        TCCR2A |= 0b00100011;
        TCCR2B |= 1;
        OCR2B = speed;
    }

#endif