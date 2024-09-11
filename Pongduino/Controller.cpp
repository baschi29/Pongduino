#include "Arduino.h"
#include "Controller.h"


Controller::Controller(int fieldXDim, int fieldYDim, int paddleWidth, int paddleLength, int ballSize, int speakerPin, int leftDSTriggerPin, int leftDSEchoPin, int rightDSTriggerPin, int rightDSEchoPin) : 
    _topBorder(-1, -3, fieldXDim - 1 + 3, 3),
    _botBorder(-1, fieldYDim, fieldXDim - 1 + 3, 3),
    _leftDeadzone(-6 , 0, 3, fieldYDim - 1),
    _rightDeadzone(fieldXDim, 0, 3, fieldYDim - 1),
    _leftPaddle(1, (float)(fieldYDim / 2 - 1) - (paddleLength - 1) / 2, paddleWidth, paddleLength, fieldYDim - 2 - paddleLength, 1, fieldYDim, 25, 5),
    _rightPaddle((float)fieldXDim - 2 - paddleWidth, (float)(fieldYDim / 2 - 1) - (paddleLength - 1) / 2, paddleWidth, paddleLength, fieldYDim - 2 - paddleLength, 1, fieldYDim, 25, 5),
    _ball((float)(fieldXDim / 2 - 1) - (ballSize - 1) / 2, (float)(fieldYDim / 2 - 1) - (ballSize - 1) / 2, ballSize, ballSize, 40),
    _speaker(speakerPin),
    _leftDistanceSensor(leftDSTriggerPin, leftDSEchoPin),
    _rightDistanceSensor(rightDSTriggerPin, rightDSEchoPin) {

    // Game State
    _gameState = {0, 0, true};

    randomSeed(analogRead(0));

}


void Controller::startup() {

    _display.begin();
    _leftDistanceSensor.begin();
    _rightDistanceSensor.begin();

    // Greetings
    delay(100);
    _display.drawLogo();
    _speaker.playStartSound();
    delay(500);

}


void Controller::pause() {

    _gameState.paused = true;

}


void Controller::unpause() {

    _gameState.paused = false;
    _speaker.playReadySound();
    // reset movement timer
    _ball.stayStill();

}


void Controller::tick() {

    if (_gameState.paused) {

        _display.drawScore(_gameState.leftScore, _gameState.rightScore);
        delay(2000);
        _display.drawGame(_ball, _leftPaddle, _rightPaddle);
        delay(500);
        this->unpause();

    }
    else {

        HitState hit {false, false, false, false};
        
        _leftPaddle.setPositionFromMeasurement(_leftDistanceSensor.measure());
        _rightPaddle.setPositionFromMeasurement(_rightDistanceSensor.measure());
        _ball.move(hit, _leftPaddle, _rightPaddle, _topBorder, _botBorder, _leftDeadzone, _rightDeadzone);

        if (hit.paddle or hit.border) {

            _speaker.playHitSound();

        }
        else if (hit.leftDeadzone) {

            _speaker.playStopSound();
            _gameState.rightScore += 1;
            this->pause();

        }
        else if (hit.rightDeadzone) {

            _speaker.playStopSound();
            _gameState.leftScore += 1;
            this->pause();

        }

        _display.drawGame(_ball, _leftPaddle, _rightPaddle);

    }

}