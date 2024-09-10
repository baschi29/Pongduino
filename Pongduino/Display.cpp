#include "Arduino.h"
#include "Display.h"
#include "Pongfield.h"
#include <U8g2lib.h>


Display::Display() : _u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE) {}


// function is neccessary as it does not seem possible to call begin before the setup function executed
void Display::begin() {

    _u8g2.begin();

    // make sure draw color is set to 1 (https://github.com/olikraus/u8g2/wiki/u8g2reference#setdrawcolor)
    _u8g2.setDrawColor(1);

    _u8g2.setFont(u8g2_font_ncenB14_tr);

}


void Display::drawLogo() {

    char *text = "Pongduino";
    u8g2_uint_t width = _u8g2.getStrWidth(text);
    u8g2_uint_t x = (u8g2_uint_t)((127 - width) / 2);

    _u8g2.clearBuffer();
    _u8g2.drawStr(x, 20, "Pongduino");
    _u8g2.sendBuffer();

}


void Display::drawGame(Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle) {

    if (millis() - _lastDraw > 20) {

        _u8g2.clearBuffer();
        this->drawPongObject(ball);
        this->drawPongObject(leftPaddle);
        this->drawPongObject(rightPaddle);
        _u8g2.sendBuffer();

        _lastDraw = millis();

    }

}


void Display::drawPongObject(PongObject& pongObject) {

    _u8g2.drawBox(round(pongObject.getX()), round(pongObject.getY()), pongObject.getXDim(), pongObject.getYDim());

}