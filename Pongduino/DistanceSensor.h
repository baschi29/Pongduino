#ifndef DistanceSensor_h
#define DistanceSensor_h

#include "Arduino.h"

class DistanceSensor {
    public:
        DistanceSensor(int triggerPin, int echoPin);
        float measure();
    private:
        int _triggerPin;
        int _echoPin;
        float _lastMeasurementValue;
        unsigned long _lastMeasurementTime; // using millis function

};

#endif