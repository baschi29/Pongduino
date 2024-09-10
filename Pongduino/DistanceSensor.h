#ifndef DistanceSensor_h
#define DistanceSensor_h

#include "Arduino.h"

class DistanceSensor {

    public:
        DistanceSensor(int triggerPin, int echoPin);
        float measure();
        void begin();

    private:
        float doSingleMeasurement();
        bool isReadyForNewMeasurement();
        int _triggerPin;
        int _echoPin;
        float _bestTrendEstimate;
        float _smoothedValue;
        unsigned long _lastMeasurementTime; // using millis function
        
};

#endif