#ifndef READ_DIGITAL_PORTL
#define READ_DIGITAL_PORTL

    bool risingReadPortL(unsigned char);
    bool lastStates[8] = {false};

    bool risingReadPortL(unsigned char port) {
        bool currentState = (PINL << (7-port)) >> 7 == 0;
        if (currentState != lastStates[port]) {
            lastStates[port] = currentState;
            if (currentState) return true;
        }
        return false;
    }
#endif