#ifndef DistanceSensor_h
#define DistanceSensor_h

#include "Arduino.h"

class DistanceSensor {

    public:
        DistanceSensor(int triggerPin, int echoPin);
        float measure();

    private:
        void doSingleMeasurement();
        bool isReadyForNewMeasurement();
        int _triggerPin;
        int _echoPin;
        float _lastMeasurementValue;
        float _bestTrendEstimate;
        float _smoothedValue;
        float _dataSmoothingFactor;
        float _trendSmoothingFactor;
        unsigned long _lastMeasurementTime; // using millis function
        
};

#endif