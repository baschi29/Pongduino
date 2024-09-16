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

}


void DistanceSensor::begin() {

    // fill past values with random measurements
    // setup double exponential smoothing like https://en.wikipedia.org/wiki/Exponential_smoothing
    _lastMeasurementTime = 0;
    delay(60);
    _smoothedValue = this->doSingleMeasurement();;
    delay(60);
    _bestTrendEstimate = this->doSingleMeasurement() - _smoothedValue;

}


float DistanceSensor::measure() {

    unsigned long now = millis();

    if (this->isReadyForNewMeasurement()) {

        float dataSmoothingFactor = 0.9;
        float trendSmoothingFactor = 0.95;

        float lastSmoothedValue = _smoothedValue;

        _smoothedValue = dataSmoothingFactor * this->doSingleMeasurement() + (1 - dataSmoothingFactor) * (lastSmoothedValue + _bestTrendEstimate);
        _bestTrendEstimate = trendSmoothingFactor * (_smoothedValue - lastSmoothedValue) + (1 - trendSmoothingFactor) * _bestTrendEstimate;
        
        return _smoothedValue;

    }
    else {

        return _smoothedValue;

    }

}


float DistanceSensor::doSingleMeasurement() {

    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);
    unsigned long duration = pulseIn(_echoPin, HIGH);
    _lastMeasurementTime = millis();

    return (float)duration / 58; // #TODO wo kommt die 58 her

}


// according to the datasheet measurement cycles should be over 60ms
bool DistanceSensor::isReadyForNewMeasurement() {

    // this should work even when millis overflows according to https://stackoverflow.com/a/15030332
    return ((unsigned long)(millis() - _lastMeasurementTime) >= 60);

}
