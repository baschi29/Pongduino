#ifndef Display_h
#define Speaker_h

#include "Arduino.h"
#include <U8g2lib.h>

class Display {
    public:
        Display();
        void writeHelloWorld();
        void begin();
    private:
        
};

#endif