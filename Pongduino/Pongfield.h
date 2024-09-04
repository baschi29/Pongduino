#ifndef Pongfield_h
#define Pongfield_h

#include "Arduino.h"


// forward declarations
class Ball;
class Paddle;


struct Tuple {

    float x;
    float y;

};


struct GameState {

    int leftScore;
    int rightScore;
    bool paused;

};


class PongObject {

    public:
        PongObject(Tuple coordinates, Tuple dimension); // everything is a rectangle, coordinates mark top left point
        bool isColliding(PongObject otherObject);
        Tuple getMaxOccupied();
        Tuple getMinOccupied();
        Tuple getCoordinates();
        Tuple getDimension();

    protected:
        void setCoordinates(float newX, float newY);

    private:
        bool isXColliding(PongObject otherObject);
        bool isYColliding(PongObject otherObject);
        Tuple _coordinates;
        Tuple _dimension;
        Tuple _maxOccupied;
        Tuple _minOccupied;

};


class MovableObject : public PongObject {

    public:
        MovableObject(Tuple coordinates, Tuple dimension);
        Tuple getMovementDirection();

    protected:
        void setMovementDirection(float x, float y);

    private:
        Tuple _movementDirection; // gets normalized by setMovementDirection

};


class Ball : public MovableObject {

    public:
        Ball(Tuple coordinates, Tuple dimension, float velocity); //velocity should be in px/s,
        void move();
        bool handleCollision(Paddle paddle); // returns true if collision was detected, for more: overload

    private:
        float _velocity;

};


class Paddle : public MovableObject {

    public:
        Paddle(Tuple coordinate, Tuple dimension);
        void setPosition(float y); //sets y Position of Peddle

    private:
        
};

#endif