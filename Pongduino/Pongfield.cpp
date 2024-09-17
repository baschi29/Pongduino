#include "Arduino.h"
#include "Pongfield.h"


// signum template copied from https://stackoverflow.com/a/4609795
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


PongObject::PongObject(int x, int y, int x_dim, int y_dim) {

    this->setDimension(x_dim, y_dim);
    this->setCoordinates(x, y);

}


void PongObject::setDimension(int x, int y) {

    _xDim = x;
    _yDim = y;

}


bool PongObject::isXColliding(PongObject& otherObject) {

    if (this->getMinOccupiedX() >= otherObject.getMinOccupiedX()) {

        if (this->getMinOccupiedX() > otherObject.getMaxOccupiedX()) {

            return false;

        }
        else {

            return true;

        }

    }
    else {

        return otherObject.isXColliding(*this);

    }

}


bool PongObject::isYColliding(PongObject& otherObject) {

    if (this->getMinOccupiedY() >= otherObject.getMinOccupiedY()) {

        if (this->getMinOccupiedY() > otherObject.getMaxOccupiedY()) {

            return false;

        }
        else {

            return true;

        }

    }
    else {

        return otherObject.isYColliding(*this);

    }

}


bool PongObject::isColliding(PongObject& otherObject) {

    if (this->isXColliding(otherObject) and this->isYColliding(otherObject)) {

        return true;

    }
    else {

        return false;

    }

}


void PongObject::setCoordinates(float newX, float newY) {

    _x = newX;
    _y = newY;

}


float PongObject::getMaxOccupiedX() {

    return this->getX() + this->getXDim() - 1;

}


float PongObject::getMaxOccupiedY() {

    return this->getY() + this->getYDim() - 1;

}


float PongObject::getMinOccupiedX() {

    return this->getX();

}


float PongObject::getMinOccupiedY() {

    return this->getY();

}


float PongObject::getX() {

    return _x;

}


float PongObject::getY() {

    return _y;

}


int PongObject::getXDim() {

    return _xDim;
}


int PongObject::getYDim() {

    return _yDim;
}


MoveableObject::MoveableObject(int x, int y, int x_dim, int y_dim) : PongObject(x, y, x_dim, y_dim) {

    this->setMovementDirection(0, 0);
    this->setVelocity(0);

}


void MoveableObject::setCoordinates(float newX, float newY) {

    PongObject::setCoordinates(newX, newY);
    _lastMovementTime = millis();

}


void MoveableObject::setMovementDirection(float x, float y) {

    // normalize and set
    float length = sqrt(x * x + y * y);
    _movementDirectionX = x / length;
    _movementDirectionY = y / length;

}


void MoveableObject::setVelocity(float newVelocity) {

    _velocity = newVelocity;

}


float MoveableObject::getVelocity() {

    return _velocity;

}


float MoveableObject::getMovementDirectionX() {

    return _movementDirectionX;

}


float MoveableObject::getMovementDirectionY() {

    return _movementDirectionY;

}


unsigned long MoveableObject::getLastMovementTime() {

    return _lastMovementTime;

}


void MoveableObject::stayStill() {

    this->setCoordinates(this->getX(), this->getY());

}


float MoveableObject::calcNewX(float movement) {

    return this->getX() + movement * this->getMovementDirectionX();

}


float MoveableObject::calcNewY(float movement) {

    return this->getY() + movement * this->getMovementDirectionY();

}


float MoveableObject::calcMovement(float end, float start, float direction) {

    return (end - start) / direction;

}


Ball::Ball(int x, int y, int x_dim, int y_dim, float velocity) : MoveableObject(x, y, x_dim, y_dim) {

    this->setVelocity(velocity);

    _startVelocity = velocity;
    _baseVelocity = velocity;
    _xStart = x;
    _yStart = y;

    this->reset();

}


// mutates HitState
void Ball::move(HitState& hit, Paddle& leftPaddle, Paddle& rightPaddle, Border& topBorder, Border& botBorder, Deadzone& leftDeadzone, Deadzone& rightDeadzone) {

    unsigned long now = millis();

    float overallMovement = this->getVelocity() * (float)(now - this->getLastMovementTime()) / 1000; //velocity in px/s, time in 
    
    // iterate over line to move to detect (most) collisions
    while (overallMovement > 0 
        and not (hit.leftDeadzone or hit.rightDeadzone)) {

        float currentMovement = min(overallMovement, 1);

        this->setCoordinates(this->calcNewX(currentMovement), this->calcNewY(currentMovement));

        // account for collision correction
        float correctionDistance = this->handleCollisions(hit, leftPaddle, rightPaddle, topBorder, botBorder, leftDeadzone, rightDeadzone);

        currentMovement -= correctionDistance;
        overallMovement -= currentMovement;

    }

}


void Ball::reset() {

    this->setMovementDirection(this->generateRandomSign() * random(1, 5), this->generateRandomSign() * random(1, 5));
    this->setCoordinates(_xStart, _yStart);
    this->setVelocity(_startVelocity);
    _baseVelocity = _startVelocity;

}


// generates a random sign
int Ball::generateRandomSign() {

    int sign = random(0, 2);

    if (sign == 0) {

        sign = -1;

    }
    else {

        sign = 1;

    }

    return sign;

}


// returns overall distance the ball was moved back, mutates HitState
float Ball::handleCollisions(HitState& hit, Paddle& leftPaddle, Paddle& rightPaddle, Border& topBorder, Border& botBorder, Deadzone& leftDeadzone, Deadzone& rightDeadzone) {

    // handle collisions, calculate and set "return" values
    float paddleCorrection = this->handleCollision(leftPaddle) + this->handleCollision(rightPaddle);
    if (paddleCorrection > 0) {

        hit.paddle = true;

    }

    float borderCorrection = this->handleCollision(topBorder) + this->handleCollision(botBorder);
    if (borderCorrection > 0) {

        hit.border = true;

    }

    if (this->handleCollision(leftDeadzone)) {

        hit.leftDeadzone = true;

    }
    else if (this->handleCollision(rightDeadzone)) {

        hit.rightDeadzone = true;

    }

    return paddleCorrection + borderCorrection;

}


float Ball::handleCollision(Paddle& paddle) {

    if (this->isColliding(paddle)) {

        float toMove;
        // maximum angle the ball bounces when it hits the paddle at the edge in radian
        // see https://gamedev.stackexchange.com/a/4255
        float maxBounceAngle = 65 * PI / 180;
        float maxVelocityAddition = _baseVelocity;

        if (sgn(this->getMovementDirectionX() > 0)) { //moved to the right -> position left from paddle

            toMove = this->calcMovement(paddle.getMinOccupiedX() - this->getXDim(), this->getX(), this->getMovementDirectionX());

        }
        else { // moved to the left -> position right from paddle

            toMove = this->calcMovement(paddle.getMaxOccupiedX() + 1, this->getX(), this->getMovementDirectionX());

        }

        // calculates relative intersection of paddle and ball and normalizes it
        float relativeIntersectY = this->getY() + this->getYDim() / 2 - paddle.getY() - paddle.getYDim() / 2;
        float normalizedRelativeIntersectY = relativeIntersectY / ((paddle.getYDim() + this->getYDim()) / 2);

        // set new coordinates accordingly
        this->setCoordinates(this->calcNewX(toMove), this->calcNewY(toMove));
        
        // set new movement direction in relation to where paddle got hit
        float bounceAngle = normalizedRelativeIntersectY * maxBounceAngle;
        this->setMovementDirection(- sgn(this->getMovementDirectionX()) * cos(bounceAngle), sin(bounceAngle));
        // adds to velocity accordingly
        this->setVelocity(_baseVelocity + maxVelocityAddition * abs(normalizedRelativeIntersectY));
        
        // get faster the more paddles get hit in a round
        _baseVelocity += 2;        

        return abs(toMove);

    }
    else {

        return 0;

    }

}


float Ball::handleCollision(Border& border) { // for now borders are always horizontal

    if (this->isColliding(border)) {

        float toMove;

        if (sgn(this->getMovementDirectionY() > 0)) { //moved to the bottom -> position top of border

            toMove = this->calcMovement(border.getMinOccupiedY() - this->getYDim(), this->getY(), this->getMovementDirectionY());

        }
        else { // moved to the top -> position bottom of border

            toMove = this->calcMovement(border.getMaxOccupiedY() + 1, this->getY(), this->getMovementDirectionY());

        }

        // set new coordinates accordingly
        this->setCoordinates(this->calcNewX(toMove), this->calcNewY(toMove));

        // set new movement direction: reflection on border
        this->setMovementDirection(this->getMovementDirectionX(), -(this->getMovementDirectionY()));

        return abs(toMove);

    }
    else {

        return 0;

    }

}


bool Ball::handleCollision(Deadzone& deadzone) {

    if (this->isColliding(deadzone)) {

        this->reset();
        return true;

    }
    else {

        return false;

    }

}


Paddle::Paddle(int x, int y, int x_dim, int y_dim, int maxY, int minY, int measurementRange, int measurementOffset) : MoveableObject(x, y, x_dim, y_dim) {

    _maxY = maxY - y_dim; // rectangles start at top left
    _minY = minY;

    _measurementOffset = measurementOffset;

    _distanceConversionFactor = - maxY / (measurementRange);

}


void Paddle::setPositionFromMeasurement(float measurement) {

    this->setPosition((measurement - _measurementOffset) * _distanceConversionFactor + _maxY);

}


void Paddle::setPosition(float newY) {

    //respect max&min
    newY = min(_maxY, max(_minY, newY));

    this->setMovementDirection(0, newY - this->getY());
    this->setCoordinates(this->getX(), newY);

}


Border::Border(int x, int y, int x_dim, int y_dim) : PongObject(x, y, x_dim, y_dim) {}


Deadzone::Deadzone(int x, int y, int x_dim, int y_dim) : PongObject(x, y, x_dim, y_dim) {}