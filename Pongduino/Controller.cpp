#include "Arduino.h"
#include "Controller.h"


Controller::Controller(float fieldXDim, float fieldYDim, float paddleWidth, float paddleLength, float ballSize, int speakerPin, int leftDSTriggerPin, int leftDSEchoPin, int rightDSTriggerPin, int rightDSEchoPin) : 
    _topBorder(-1, -3, fieldXDim - 1 + 3, 3),
    _botBorder(-1, fieldYDim, fieldXDim - 1 + 3, 3),
    _leftDeadzone(-3 , 0, 3, fieldYDim - 1),
    _rightDeadzone(fieldXDim, 0, 3, fieldYDim - 1),
    _leftPaddle(1, (float)(fieldYDim / 2 - 1) - (paddleLength - 1) / 2, paddleWidth, paddleLength, fieldYDim - 2 - paddleLength, 1),
    _rightPaddle((float)fieldXDim - 2 - paddleWidth, (float)(fieldYDim / 2 - 1) - (paddleLength - 1) / 2, paddleWidth, paddleLength, fieldYDim - 2 - paddleLength, 1),
    _ball((float)(fieldXDim / 2 - 1) - (ballSize - 1) / 2, (float)(fieldYDim / 2 - 1) - (ballSize - 1) / 2, ballSize, ballSize, 40),
    _speaker(speakerPin),
    _leftDistanceSensor(leftDSTriggerPin, leftDSEchoPin),
    _rightDistanceSensor(rightDSTriggerPin, rightDSEchoPin) {

    // Game State
    _gameState = {0, 0, true};

}


void Controller::startup() {

    _display.begin();

    // Greetings
    delay(1000);
    _speaker.playStartSound();
    delay(1000);

}


void Controller::tick() {

    //_display->writeHelloWorld();
    //Serial.println(_ball->getX());
    Serial.println(_rightPaddle.getX());
    _display.drawGame(_ball, _leftPaddle, _rightPaddle);
    //Serial.println("davor");
    delay(2000);

}