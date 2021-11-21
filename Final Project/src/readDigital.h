#ifndef READ_DIGITAL_PORTL
#define READ_DIGITAL_PORTL

    bool lastStates[8] = {false};
    bool T3isRunning = false;
    bool currentReading = false;
    unsigned char currentPort = 0;

    bool digitalReadPortL(unsigned char);

    bool digitalReadPortL(unsigned char port) {

        // Check if we are already conducting a debounce
        // on another port
        if (T3isRunning && port != currentPort) {
            //throw "Debouncer Busy";
        }

        currentPort = port; // otherwise, we operate on this port
        currentReading = (bool) ((PINL << (7-port)) >> 7);

        // if timer is not running, and state change detected
        if (!T3isRunning && currentReading != lastStates[port]) {
            TCNT3 = 25536;  // 5 ms delay
            TCCR3B |= 0x1;  // start
            T3isRunning = true;
        }

        //if ovf happened, and still reading a diff state
        else if (((TIFR3 & 0x1) != 0) && currentReading != lastStates[port]) {
            lastStates[port] = currentReading;  // new state is valid. Store it
            TCCR3B &= 0xF8; // stop timer
            TIFR3 |= 0x1;  // reset overflow flag
            T3isRunning = false;
        }

        return lastStates[port];  // return the current state

    }
#endif