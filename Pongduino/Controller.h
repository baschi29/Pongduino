#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "Pongfield.h"
#include "Speaker.h"
#include "Display.h"
#include "DistanceSensor.h"

class Controller {

    public:
        Controller(float fieldXDim, float fieldYDim, float paddleLength, float paddleWidth, float ballSize, int speakerPin, int leftDSTriggerPin, int leftDSEchoPin, int rightDSTriggerPin, int rightDSEchoPin);
        void tick();
        void startup();
        void pause();
        void unpause();

    private:
        GameState _gameState;
        Ball _ball;
        Paddle _leftPaddle;  
        Paddle _rightPaddle;
        Border _topBorder;
        Border _botBorder;
        Deadzone _leftDeadzone;
        Deadzone _rightDeadzone;
        Speaker _speaker;
        DistanceSensor _leftDistanceSensor;
        DistanceSensor _rightDistanceSensor;
        Display _display;


};

#endif