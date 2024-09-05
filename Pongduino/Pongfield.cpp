#include "Arduino.h"
#include "Pongfield.h"


// signum template copied from https://stackoverflow.com/a/4609795
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


PongObject::PongObject(Tuple coordinates, Tuple dimension) {

    _coordinates = coordinates;
    _dimension = dimension;
    _maxOccupied = { coordinates.x + dimension.x - 1, coordinates.y + dimension.y - 1};
    _minOccupied = coordinates;

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


MovableObject::MovableObject(Tuple coordinates, Tuple dimension) : PongObject(coordinates, dimension) {

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
    _movementAngle = atan2((double)_movementDirection.y, (double)_movementDirection.x);

}


Tuple MovableObject::getMovementDirection() {

    return _movementDirection;

}


unsigned long MovableObject::getLastMovementTime() {

    return _lastMovementTime;

}


double MovableObject::getMovementAngle() {

    return _movementAngle;

}


Ball::Ball(Tuple coordinates, Tuple dimension, float velocity) : MovableObject(coordinates, dimension) {

    _velocity = velocity;
    _startCoordinates = coordinates;

}


void Ball::move() { // todo: check use of sin, cos and atan2

    unsigned long now = millis();

    float overallMovement = _velocity * (float)(now - this->getLastMovementTime()) / 1000; //velocity in px/s, time in ms

    float newX = this->getCoordinates().x + overallMovement * cos(this->getMovementAngle());
    float newY = this->getCoordinates().y + overallMovement * sin(this->getMovementAngle());

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


Paddle::Paddle(Tuple coordinates, Tuple dimension, float maxY, float minY) : MovableObject(coordinates, dimension) {

    _maxY = maxY;
    _minY = minY;

}


void Paddle::setPosition(float newY) {

    //respect max&min
    newY = min(_maxY, max(_minY, newY));

    this->setMovementDirection(0, newY - this->getCoordinates().y);
    this->setCoordinates(this->getCoordinates().x, newY);

}


Border::Border(Tuple coordinates, Tuple dimension) : PongObject(coordinates, dimension) {}


Deadzone::Deadzone(Tuple coordinates, Tuple dimension) : PongObject(coordinates, dimension) {}