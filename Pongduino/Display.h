#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "Pongfield.h"
#include <U8g2lib.h>

class Display {

    public:
        Display();
        void writeHelloWorld();
        void begin();
        void drawGame(Ball ball, Paddle leftPaddle, Paddle rightPaddle);
        
    private:
        
        
};

#endif