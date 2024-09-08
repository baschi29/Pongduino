#ifndef Pongfield_h
#define Pongfield_h

#include "Arduino.h"


// forward declarations
class Ball;
class Paddle;
class Border;
class Deadzone;


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
        PongObject(float x, float y, float x_dim, float y_dim); // everything is a rectangle, coordinates mark top left point
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
        void setOccupied(); // x,y top left
        void setDimension(float x, float y);
        Tuple _coordinates;
        Tuple _dimension;
        Tuple _maxOccupied;
        Tuple _minOccupied;

};


class MovableObject : public PongObject {

    public:
        MovableObject(float x, float y, float x_dim, float y_dim);
        Tuple getMovementDirection();

    protected:
        void setMovementDirection(float x, float y);
        void setCoordinates(float newX, float newY);
        unsigned long getLastMovementTime();

    private:
        Tuple _movementDirection; // gets normalized by setMovementDirection (vector)
        unsigned long _lastMovementTime;

};


class Ball : public MovableObject {

    public:
        Ball(float x, float y, float x_dim, float y_dim, float velocity); //velocity should be in px/s,
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
        Paddle(float x, float y, float x_dim, float y_dim, float maxY, float minY);
        void setPosition(float newY); //sets y Position of Peddle

    private:
        float _maxY;
        float _minY;
        
};


class Border : public PongObject {

    public:
        Border(float x, float y, float x_dim, float y_dim);

};


class Deadzone : public PongObject {

    public:
        Deadzone(float x, float y, float x_dim, float y_dim);

};


#endif