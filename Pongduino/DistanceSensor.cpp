#include "Arduino.h"
#include "DistanceSensor.h"


DistanceSensor::DistanceSensor(int triggerPin, int echoPin) {

    _triggerPin = triggerPin;
    _echoPin = echoPin;
    _lastMeasurementValue = -1; //to indicate that there was no last measurement

    // initialize Pins
    pinMode(_triggerPin, OUTPUT);
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    pinMode(_echoPin, INPUT);

}


float DistanceSensor::measure() {

    unsigned long now = millis();

    // according to the datasheet measurement cycles should be over 60ms
    if ((unsigned long)(now - _lastMeasurementTime) < 60) { // this should work even when millis overflows according to https://stackoverflow.com/a/15030332

        return _lastMeasurementValue;

    }
    else {

        digitalWrite(_triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_triggerPin, LOW);
        int duration = pulseIn(_echoPin, HIGH);
        _lastMeasurementTime = millis();
        float measurementValue = duration / 58; // #TODO wo kommt die 58 her

        // smooth measurement with linear recursive exponential filter
        _secondLastMeasurementValue = _lastMeasurementValue;
        _lastMeasurementValue = measurementValue * 0.8 + _lastMeasurementValue * 0.16 + _secondLastMeasurementValue * 0.04;

        return _lastMeasurementValue;

    }

}
