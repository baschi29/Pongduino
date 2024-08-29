#include "Arduino.h"
#include "Speaker.h"
#include "Pitches.h"

Speaker::Speaker(int pin) {
    _pin = pin;
}

void Speaker::playStartSound() {

    tone(_pin, NOTE_C4);
    delay(100);
    noTone(_pin);
    delay(100);
    tone(_pin, NOTE_C4);
    delay(100);
    tone(_pin, NOTE_E4);
    delay(200);
    tone(_pin, NOTE_G4);
    delay(200);
    noTone(_pin);

}


void Speaker::playStopSound() {

    tone(_pin, NOTE_G4);
    delay(100);
    noTone(_pin);
    delay(100);
    tone(_pin, NOTE_G4);
    delay(100);
    tone(_pin, NOTE_E4);
    delay(200);
    tone(_pin, NOTE_C4);
    delay(200);
    noTone(_pin);

}