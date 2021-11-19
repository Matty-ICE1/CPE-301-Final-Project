#ifndef READ_DIGITAL_PORTL
#define READ_DIGITAL_PORTL

    bool lastStates[8] = {false};
    void initDigitalRead();
    bool digitalReadPortL(unsigned char);

    void initDigitalRead() {
    }

    bool digitalReadPortL(unsigned char port) { return (PINL << (7-port)) >>7 == 1 ? true : false; }

#endif