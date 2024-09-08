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
    //Tuple coordinatePlaceholder = {(float)(128 / 2 - 1) - (ballSize - 1) / 2, (float)(64 / 2 - 1) - (ballSize - 1) / 2};
    //Tuple dimensionPlaceholder = {ballSize, ballSize};
    float x = (float)(128 / 2 - 1) - (ballSize - 1) / 2;
    float y = (float)(64 / 2 - 1) - (ballSize - 1) / 2;
    _ball = new Ball(x, y, ballSize, ballSize, 40);
    delay(1000);
    Serial.println(_ball->getCoordinates().y);
    delay(1000);
    Serial.println(_ball->getCoordinates().x);
    delay(1000);    
    
    // Paddles
    int paddleLength = 9; // should be uneven
    int paddleWidth = 3;
    int minY = 1;
    int maxY = 62 - paddleLength;
    x = 1;
    y = (float)(64 / 2 - 1) - (paddleLength - 1) / 2;
    //coordinatePlaceholder = {1, (float)(64 / 2 - 1) - (paddleLength - 1) / 2};
    //dimensionPlaceholder = {(float)paddleWidth, (float)paddleLength};
    _leftPaddle = new Paddle(x, y, paddleWidth, paddleLength, maxY, minY);
    //coordinatePlaceholder = {(float)126 - paddleWidth, (float)(64 / 2 - 1) - (paddleLength - 1) / 2};
    x = (float)126 - paddleWidth;
    _rightPaddle = new Paddle(x, y, paddleWidth, paddleLength, maxY, minY);
    
    // Borders
    //coordinatePlaceholder = {-1, -3};
    x = -1;
    y = -3;
    int x_dim = 130;
    int y_dim = 3;
    //dimensionPlaceholder = {130, 3};
    _topBorder = new Border(x, y, x_dim, y_dim);
    //coordinatePlaceholder = {-1, 64};
    y = 64;
    _botBorder = new Border(x, y, x_dim, y_dim);
    
    // Deadzones
    //coordinatePlaceholder = {-3, 0};
    x = -3;
    y = 0;
    //dimensionPlaceholder = {3, 63};
    x_dim = 3;
    y_dim = 63;
    _leftDeadzone = new Deadzone(x, y, x_dim, y_dim);
    //coordinatePlaceholder = {128, 0};
    x = 128;
    y = 0;
    _rightDeadzone = new Deadzone(x, y, x_dim, y_dim);

    delay(1000);
    Serial.println("_ball->getCoordinates().y");
    delay(1000);
    Serial.println("_ball->getCoordinates().x");
    delay(1000); 

    // Greetings
    _display->begin();
    delay(1000);
    _speaker->playStartSound();

    delay(1000);

}


void Controller::tick() {

    //_display->writeHelloWorld();
    //Serial.println((*_ball).getCoordinates().y);
    //_display->drawGame(*_ball, *_leftPaddle, *_rightPaddle);
    Serial.println("davor");
    delay(1000);
    Serial.println("danach");

}