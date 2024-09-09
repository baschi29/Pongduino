#ifndef Pongfield_h
#define Pongfield_h

#include "Arduino.h"


// forward declarations
class Ball;
class Paddle;
class Border;
class Deadzone;


struct GameState {

    int leftScore;
    int rightScore;
    bool paused;

};


class PongObject {

    public:
        PongObject(float x, float y, float x_dim, float y_dim); // everything is a rectangle, coordinates mark top left point
        bool isColliding(PongObject otherObject);
        float getMaxOccupiedX();
        float getMaxOccupiedY();
        float getMinOccupiedX();
        float getMinOccupiedY();
        float getX();
        float getY();
        float getXDim();
        float getYDim();

    protected:
        void setCoordinates(float newX, float newY);

    private:
        bool isXColliding(PongObject otherObject);
        bool isYColliding(PongObject otherObject);
        void setDimension(float x, float y);
        float _x;
        float _y;
        float _xDim;
        float _yDim;

};


class MovableObject : public PongObject {

    public:
        MovableObject(float x, float y, float x_dim, float y_dim);
        float getMovementDirectionX();
        float getMovementDirectionY();

    protected:
        void setMovementDirection(float x, float y);
        void setCoordinates(float newX, float newY);
        unsigned long getLastMovementTime();

    private:
        float _movementDirectionX; // gets normalized by setMovementDirection
        float _movementDirectionY;
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
        float _xStart;
        float _yStart;

};


class Paddle : public MovableObject {

    public:
        Paddle(float x, float y, float x_dim, float y_dim, float maxY, float minY);
        void setPosition(float newY); //sets y Position of Paddle

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