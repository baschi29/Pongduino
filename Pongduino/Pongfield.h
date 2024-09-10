#ifndef Pongfield_h
#define Pongfield_h

#include "Arduino.h"


// forward declarations
class Ball;
class Paddle;
class Border;
class Deadzone;


struct GameState {

    unsigned short int leftScore;
    unsigned short int rightScore;
    bool paused;

};


class PongObject {

    public:
        PongObject(int x, int y, int x_dim, int y_dim); // everything is a rectangle, coordinates mark top left point
        bool isColliding(PongObject& otherObject);
        float getMaxOccupiedX();
        float getMaxOccupiedY();
        float getMinOccupiedX();
        float getMinOccupiedY();
        float getX();
        float getY();
        int getXDim();
        int getYDim();

    protected:
        void setCoordinates(float newX, float newY);

    private:
        bool isXColliding(PongObject& otherObject);
        bool isYColliding(PongObject& otherObject);
        void setDimension(int x, int y);
        float _x;
        float _y;
        int _xDim;
        int _yDim;

};


class MovableObject : public PongObject {

    public:
        MovableObject(int x, int y, int x_dim, int y_dim);
        float getMovementDirectionX();
        float getMovementDirectionY();
        void stayStill();

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
        Ball(int x, int y, int x_dim, int y_dim, float velocity); //velocity should be in px/s,
        void move();
        void reset();
        bool handleCollision(Paddle& paddle); // returns true if collision was detected
        bool handleCollision(Border& border);
        bool handleCollision(Deadzone& deadzone);

    private:
        float _velocity;
        int _xStart;
        int _yStart;

};


class Paddle : public MovableObject {

    public:
        Paddle(int x, int y, int x_dim, int y_dim, int maxY, int minY);
        void setPosition(float newY); //sets y Position of Paddle

    private:
        int _maxY;
        int _minY;
        
};


class Border : public PongObject {

    public:
        Border(int x, int y, int x_dim, int y_dim);

};


class Deadzone : public PongObject {

    public:
        Deadzone(int x, int y, int x_dim, int y_dim);

};


#endif