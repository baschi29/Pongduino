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


void PongObject::setOccupied() {

    _maxOccupied.x = _coordinates.x + _dimension.x - 1;
    _maxOccupied.y = _coordinates.y + _dimension.y - 1;
    _minOccupied.x = _coordinates.x;
    _minOccupied.y = _coordinates.y;

}


void PongObject::setDimension(float x, float y) {

    _dimension.x = x;
    _dimension.y = y;

}


bool PongObject::isXColliding(PongObject otherObject) {

    if (this->getMinOccupied().x >= otherObject.getMinOccupied().x) {

        if (this->getMinOccupied().x > otherObject.getMaxOccupied().x) {

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


bool PongObject::isYColliding(PongObject otherObject) {

    if (this->getMinOccupied().y >= otherObject.getMinOccupied().y) {

        if (this->getMinOccupied().y > otherObject.getMaxOccupied().y) {

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


bool PongObject::isColliding(PongObject otherObject) {

    if (this->isXColliding(otherObject) and this->isYColliding(otherObject)) {

        return true;

    }
    else {

        return false;

    }

}


void PongObject::setCoordinates(float newX, float newY) {

    _coordinates.x = newX;
    _coordinates.y = newY;

    this->setOccupied();

}


Tuple PongObject::getMaxOccupied() {

    return _maxOccupied;

}


Tuple PongObject::getMinOccupied() {

    return _minOccupied;

}


Tuple PongObject::getCoordinates() {

    return _coordinates;

}


Tuple PongObject::getDimension() {

    return _dimension;
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
    _movementDirection = {x / length, y / length};

}


Tuple MovableObject::getMovementDirection() {

    return _movementDirection;

}


unsigned long MovableObject::getLastMovementTime() {

    return _lastMovementTime;

}


Ball::Ball(float x, float y, float x_dim, float y_dim, float velocity) : MovableObject(x, y, x_dim, y_dim) {

    _velocity = velocity;
    _startCoordinates.x = x;
    _startCoordinates.y = y;

}


void Ball::move() {

    unsigned long now = millis();

    float overallMovement = _velocity * (float)(now - this->getLastMovementTime()) / 1000; //velocity in px/s, time in ms

    float newX = this->getCoordinates().x + overallMovement * this->getMovementDirection().x;
    float newY = this->getCoordinates().y + overallMovement * this->getMovementDirection().y;

    this->setCoordinates(newX, newY);

}


void Ball::reset() {

    this->setCoordinates(_startCoordinates.x, _startCoordinates.y);

}


bool Ball::handleCollision(Paddle paddle) {

    if (this->isColliding(paddle)) {

        if (sgn(this->getMovementDirection().x > 0)) { //moved to the right -> position left from paddle

            this->setCoordinates(paddle.getMinOccupied().x - this->getDimension().x, this->getCoordinates().y);

        }
        else { // moved to the left -> position right from paddle

            this->setCoordinates(paddle.getMaxOccupied().x + 1, this->getCoordinates().y);

        }

        // set new movement direction: reflection on paddle
        this->setMovementDirection(-(this->getMovementDirection().x), this->getMovementDirection().y);

        return true;

    }
    else {

        return false;

    }

}


bool Ball::handleCollision(Border border) { // for now borders are always horizontal

    if (this->isColliding(border)) {

        if (sgn(this->getMovementDirection().y > 0)) { //moved to the bottom -> position top of border

            this->setCoordinates(this->getCoordinates().x, border.getMinOccupied().y - this->getDimension().y);

        }
        else { // moved to the top -> position bottom of border

            this->setCoordinates( this->getCoordinates().x, border.getMaxOccupied().y + 1);

        }

        // set new movement direction: reflection on border
        this->setMovementDirection(this->getMovementDirection().x, -(this->getMovementDirection().y));

        return true;

    }
    else {

        return false;

    }

}


bool Ball::handleCollision(Deadzone deadzone) {

    return this->isColliding(deadzone);

}


Paddle::Paddle(float x, float y, float x_dim, float y_dim, float maxY, float minY) : MovableObject(x, y, x_dim, y_dim) {

    _maxY = maxY;
    _minY = minY;

}


void Paddle::setPosition(float newY) {

    //respect max&min
    newY = min(_maxY, max(_minY, newY));

    this->setMovementDirection(0, newY - this->getCoordinates().y);
    this->setCoordinates(this->getCoordinates().x, newY);

}


Border::Border(float x, float y, float x_dim, float y_dim) : PongObject(x, y, x_dim, y_dim) {}


Deadzone::Deadzone(float x, float y, float x_dim, float y_dim) : PongObject(x, y, x_dim, y_dim) {}