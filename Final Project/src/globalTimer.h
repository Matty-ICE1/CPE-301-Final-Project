#ifndef GLOBAL_TIMER_H
#define GLOBAL_TIMER_H

    unsigned int sensorDisplayTimer = 0;
    unsigned int buttonRefreshTimer = 0;

    void startGlobalTimer();
    void globalTimerRun();
    
    void startGlobalTimer() {
        //Mode 4, input denoiser, rising trigger
        TCCR1A |= 0b00000011;
        TCCR1B |= 0b11111000;

        //Compare out to reset on 16,000
        OCR1A = 0x3E;
        OCR1AL = 0x80;

        // Zero timer
        TCNT1 = 0;
        TIFR1 |= 0b10;  // clear outputs

        // Start
        TCCR1B |=1;

    }

    void globalTimerRun() {
        // if match on OCR1A
        if ((TIFR1 & 0b10) == 0b10) {
            sensorDisplayTimer++;
            buttonRefreshTimer++;
        }
        TIFR1 |= 0b10;  // reset flag
    }

#endif