#include "Arduino.h"
#include "Display.h"
#include "Pongfield.h"
#include <U8g2lib.h>
#include <stdio.h>


Display::Display() : _u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE) {}


// function is neccessary as it does not seem possible to call begin before the setup function executed
void Display::begin() {

    _u8g2.begin();

    // make sure draw color is set to 1 (https://github.com/olikraus/u8g2/wiki/u8g2reference#setdrawcolor)
    _u8g2.setDrawColor(1);

    _u8g2.setFont(u8g2_font_ncenB14_tr);

}


void Display::drawLogo() {

    this->prepareDraw();
    const char* str = "Pongduino";
    this->drawCenteredString(str, 20);
    this->executeDraw();

}


void Display::drawScore(int leftScore, int rightScore) {

    this->prepareDraw();

    const char* str1 = "Pongduino";
    this->drawCenteredString(str1, 20);
    // this will stop to work when scores go beyond 99
    char score[8];
    snprintf(score, 8, "%d : %d", leftScore, rightScore);
    this->drawCenteredString(score, 50);

    this->executeDraw();

}


void Display::drawGame(Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle) {

    if (millis() - _lastDraw > 20) {

        this->prepareDraw();

        this->drawPongObject(ball);
        this->drawPongObject(leftPaddle);
        this->drawPongObject(rightPaddle);

        this->executeDraw();

    }

}


void Display::drawPongObject(PongObject& pongObject) {

    _u8g2.drawBox(round(pongObject.getX()), round(pongObject.getY()), pongObject.getXDim(), pongObject.getYDim());

}


void Display::drawCenteredString(const char* str, int y) {

    u8g2_uint_t width = _u8g2.getStrWidth(str);
    u8g2_uint_t x = (u8g2_uint_t)((127 - width) / 2);

    _u8g2.drawStr(x, y, str);

}


void Display::prepareDraw() {

    _u8g2.clearBuffer();

}


void Display::executeDraw() {

    _u8g2.sendBuffer();
    _lastDraw = millis();

}