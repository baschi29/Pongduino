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
        void setCoordinates(float newX, float newY);

    private:
        Tuple _movementDirection; // gets normalized by setMovementDirection
        unsigned long _lastMovementTime;

};


class Ball : public MovableObject {

    public:
        Ball(Tuple coordinates, Tuple dimension, float velocity); //velocity should be in px/s,
        void move();
        void reset();
        bool handleCollision(Paddle paddle); // returns true if collision was detected
        bool handleCollision(Border border);
        bool handleCollision(Deadzone deadzone);

    private:
        float _velocity;
        Tuple _startCoordinates;

};


class Paddle : public MovableObject {

    public:
        Paddle(Tuple coordinates, Tuple dimension, float maxY, float minY);
        void setPosition(float newY); //sets y Position of Peddle

    private:
        float _maxY;
        float _minY;
        
};


class Border : public PongObject {

    public:
        Border(Tuple coordinates, Tuple dimension);

};


class Deadzone : public PongObject {

    public:
        Deadzone(Tuple coordinates, Tuple dimension);

};


#endif