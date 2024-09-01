#include "Arduino.h"
#include "Display.h"
#include <U8g2lib.h>

// aktuelles Problem: beim include hängt der code an der Stelle der Initialisierung - es geht nicht weiter?
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);


// Placeholder
Display::Display() {

}


// function is neccessary as it does not seem possible to call begin before the setup function executed
void Display::begin() {

    u8g2.begin();

}


void Display::writeHelloWorld() {

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"Hello World!");
    u8g2.sendBuffer();

}