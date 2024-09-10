#ifndef Display_h
#define Display_h

#include "Arduino.h"
#include "Pongfield.h"
#include <U8g2lib.h>

class Display {

    public:
        Display();
        void drawLogo();
        void begin();
        void drawGame(Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle);
        void drawScore(int leftScore, int rightScore);
        
    private:
        void drawPongObject(PongObject& pongObject);
        void drawCenteredString(const char *str, int y);
        U8G2_SH1106_128X64_NONAME_2_HW_I2C _u8g2;
        unsigned long _lastDraw;       
        
};

#endif