#ifndef READ_DIGITAL_PORTL
#define READ_DIGITAL_PORTL

    bool risingReadPortL(unsigned char);
    bool startPortL();
    bool lastStates[8] = {false};

    bool startPortL() {
        DDRL &= 0b11111110; // PL0 is input
        PORTL |= 0b00000001; // enable pullup resistor
    }

    bool risingReadPortL(unsigned char port) {
        bool currentState = (PINL << (7-port)) >> 7 == 0;
        if (currentState != lastStates[port]) {
            lastStates[port] = currentState;
            if (currentState) return true;
        }
        return false;
    }
#endif