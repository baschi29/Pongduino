#include "Arduino.h"
#include "DistanceSensor.h"


DistanceSensor::DistanceSensor(int triggerPin, int echoPin) {

    _triggerPin = triggerPin;
    _echoPin = echoPin;

    // initialize Pins
    pinMode(_triggerPin, OUTPUT);
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    pinMode(_echoPin, INPUT);

    // fill past values with random measurements
    // setup double exponential smoothing like https://en.wikipedia.org/wiki/Exponential_smoothing
    _lastMeasurementTime = 0;
    this->doSingleMeasurement();
    _smoothedValue = _lastMeasurementValue;
    delay(60);
    this->doSingleMeasurement();

    _bestTrendEstimate = _lastMeasurementValue - _smoothedValue;

    _dataSmoothingFactor = 0.9;
    _trendSmoothingFactor = 0.9;

}


float DistanceSensor::measure() {

    unsigned long now = millis();

    if (this->isReadyForNewMeasurement()) {

        float lastSmoothedValue = _smoothedValue;
        float lastBestTrendEstimate = _bestTrendEstimate;

        this->doSingleMeasurement();

        _smoothedValue = _dataSmoothingFactor * _lastMeasurementValue + (1 - _dataSmoothingFactor) * (lastSmoothedValue + lastBestTrendEstimate);
        _bestTrendEstimate = _trendSmoothingFactor * (_smoothedValue - lastSmoothedValue) + (1 - _trendSmoothingFactor) * lastBestTrendEstimate;
        
        return _smoothedValue;

    }
    else {

        return _smoothedValue;

    }

}


void DistanceSensor::doSingleMeasurement() {

    
    if (this->isReadyForNewMeasurement()) { 

        digitalWrite(_triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_triggerPin, LOW);
        int duration = pulseIn(_echoPin, HIGH);
        _lastMeasurementTime = millis();
        _lastMeasurementValue = duration / 58; // #TODO wo kommt die 58 her

    }

}


// according to the datasheet measurement cycles should be over 60ms
bool DistanceSensor::isReadyForNewMeasurement() {

    // this should work even when millis overflows according to https://stackoverflow.com/a/15030332
    return ((unsigned long)(millis() - _lastMeasurementTime) >= 60);

}
