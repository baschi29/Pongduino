#ifndef Speaker_h
#define Speaker_h

#include "Arduino.h"

class Speaker {

    public:
        Speaker(int pin);
        void playStartSound();
        void playStopSound();
        void playHitSound();

    private:
        int _pin;
        
};

#endif