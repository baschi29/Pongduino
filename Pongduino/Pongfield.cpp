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


void MovableObject::setMovementDirection(float x, float y) {

    // normalize and set
    float length = sqrt(x * x + y * y);
    _movementDirection = {x / length, y / length};

}


Tuple MovableObject::getMovementDirection() {

    return _movementDirection;

}


Ball::Ball(Tuple coordinates, Tuple dimension, float velocity) : MovableObject(coordinates, dimension) {

    _velocity = velocity;

}


void Ball::move() {

    float newX = this->getCoordinates().x + _velocity * this->getMovementDirection().x;
    float newY = this->getCoordinates().y + _velocity * this->getMovementDirection().y;

    this->setCoordinates(newX, newY);

}


bool Ball::handleCollision(Paddle paddle) {

    if (this->isColliding(paddle)) {

        if (sgn(this->getMovementDirection().x > 0)) { //moves to the right -> position left from paddle

            this->setCoordinates(paddle.getMinOccupied().x - this->getDimension().x, this->getCoordinates().y);

        }
        else { // moves to the left -> position right from paddle

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