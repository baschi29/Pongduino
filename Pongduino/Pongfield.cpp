#include "Arduino.h"
#include "Pongfield.h"


// signum template copied from https://stackoverflow.com/a/4609795
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


PongObject::PongObject(float x, float y, float x_dim, float y_dim) {

    this->setDimension(x_dim, y_dim);
    this->setCoordinates(x, y);

}


void PongObject::setDimension(float x, float y) {

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


float PongObject::getXDim() {

    return _xDim;
}


float PongObject::getYDim() {

    return _yDim;
}


MovableObject::MovableObject(float x, float y, float x_dim, float y_dim) : PongObject(x, y, x_dim, y_dim) {

    this->setMovementDirection(0, 0);

}


void MovableObject::setCoordinates(float newX, float newY) {

    PongObject::setCoordinates(newX, newY);
    _lastMovementTime = millis();

}


void MovableObject::setMovementDirection(float x, float y) {

    // normalize and set
    float length = sqrt(x * x + y * y);
    _movementDirectionX = x / length;
    _movementDirectionY = y / length;

}


float MovableObject::getMovementDirectionX() {

    return _movementDirectionX;

}


float MovableObject::getMovementDirectionY() {

    return _movementDirectionY;

}


unsigned long MovableObject::getLastMovementTime() {

    return _lastMovementTime;

}


Ball::Ball(float x, float y, float x_dim, float y_dim, float velocity) : MovableObject(x, y, x_dim, y_dim) {

    _velocity = velocity;
    _xStart = x;
    _yStart = y;

    this->setMovementDirection(1, 0);

}


void Ball::move() {

    unsigned long now = millis();

    float overallMovement = _velocity * (float)(now - this->getLastMovementTime()) / 1000; //velocity in px/s, time in ms

    float newX = this->getX() + overallMovement * this->getMovementDirectionX();
    float newY = this->getY() + overallMovement * this->getMovementDirectionY();

    this->setCoordinates(newX, newY);

}


void Ball::reset() {

    this->setCoordinates(_xStart, _yStart);

}


bool Ball::handleCollision(Paddle& paddle) {

    if (this->isColliding(paddle)) {

        if (sgn(this->getMovementDirectionX() > 0)) { //moved to the right -> position left from paddle

            this->setCoordinates(paddle.getMinOccupiedX() - this->getXDim(), this->getY());

        }
        else { // moved to the left -> position right from paddle

            this->setCoordinates(paddle.getMaxOccupiedX() + 1, this->getY());

        }

        // set new movement direction: reflection on paddle
        this->setMovementDirection(-(this->getMovementDirectionX()), this->getMovementDirectionY());

        return true;

    }
    else {

        return false;

    }

}


bool Ball::handleCollision(Border& border) { // for now borders are always horizontal

    if (this->isColliding(border)) {

        if (sgn(this->getMovementDirectionY() > 0)) { //moved to the bottom -> position top of border

            this->setCoordinates(this->getX(), border.getMinOccupiedY() - this->getYDim());

        }
        else { // moved to the top -> position bottom of border

            this->setCoordinates( this->getX(), border.getMaxOccupiedY() + 1);

        }

        // set new movement direction: reflection on border
        this->setMovementDirection(this->getMovementDirectionX(), -(this->getMovementDirectionY()));

        return true;

    }
    else {

        return false;

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


Paddle::Paddle(float x, float y, float x_dim, float y_dim, float maxY, float minY) : MovableObject(x, y, x_dim, y_dim) {

    _maxY = maxY;
    _minY = minY;

}


void Paddle::setPosition(float newY) {

    //respect max&min
    newY = min(_maxY, max(_minY, newY));

    this->setMovementDirection(0, newY - this->getY());
    this->setCoordinates(this->getX(), newY);

}


Border::Border(float x, float y, float x_dim, float y_dim) : PongObject(x, y, x_dim, y_dim) {}


Deadzone::Deadzone(float x, float y, float x_dim, float y_dim) : PongObject(x, y, x_dim, y_dim) {}