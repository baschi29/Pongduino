#include "Arduino.h"
#include "Controller.h"


Controller::Controller() {

    // Game State
    _gameState = {0, 0, true};

}


Controller::~Controller() {

    delete _speaker;
    delete _leftDistanceSensor;
    delete _rightDistanceSensor;
    delete _display;
    delete _ball;
    delete _leftPaddle;
    delete _rightPaddle;
    delete _topBorder;
    delete _botBorder;
    delete _leftDeadzone;
    delete _rightDeadzone;

}


void Controller::startup(int speakerPin, int leftDSTriggerPin, int leftDSEchoPin, int rightDSTriggerPin, int rightDSEchoPin) {

    // allocated memory is freed in Destructor

    // Hardware
    _speaker = new Speaker(speakerPin);
    _leftDistanceSensor = new DistanceSensor(leftDSTriggerPin, leftDSEchoPin);
    _rightDistanceSensor = new DistanceSensor(rightDSTriggerPin, rightDSEchoPin);
    _display = new Display();

    // Ball
    int ballSize = 3; //should be uneven
    Tuple coordinatePlaceholder = {(float)(128 / 2 - 1) - (ballSize - 1) / 2, (float)(64 / 2 - 1) - (ballSize - 1) / 2};
    Tuple dimensionPlaceholder = {ballSize, ballSize};
    _ball = new Ball(coordinatePlaceholder, dimensionPlaceholder, 40);

    // Paddles
    int paddleLength = 9; // should be uneven
    int paddleWidth = 3;
    int minY = 1;
    int maxY = 62 - paddleLength;
    coordinatePlaceholder = {1, (float)(64 / 2 - 1) - (paddleLength - 1) / 2};
    dimensionPlaceholder = {(float)paddleWidth, (float)paddleLength};
    _leftPaddle = new Paddle(coordinatePlaceholder,  dimensionPlaceholder, maxY, minY);
    coordinatePlaceholder = {(float)126 - paddleWidth, (float)(64 / 2 - 1) - (paddleLength - 1) / 2};
    _rightPaddle = new Paddle(coordinatePlaceholder,  dimensionPlaceholder, maxY, minY);

    // Borders
    coordinatePlaceholder = {-1, -3};
    dimensionPlaceholder = {130, 3};
    _topBorder = new Border(coordinatePlaceholder, dimensionPlaceholder);
    coordinatePlaceholder = {-1, 64};
    _botBorder = new Border(coordinatePlaceholder, dimensionPlaceholder);

    // Deadzones
    coordinatePlaceholder = {-3, 0};
    dimensionPlaceholder = {3, 63};
    _leftDeadzone = new Deadzone(coordinatePlaceholder, dimensionPlaceholder);
    coordinatePlaceholder = {128, 0};
    _rightDeadzone = new Deadzone(coordinatePlaceholder, dimensionPlaceholder);

    // Greetings
    _display->begin();
    _speaker->playStartSound();

}


void Controller::tick() {

    // todo

}