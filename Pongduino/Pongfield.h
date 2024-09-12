#ifndef Pongfield_h
#define Pongfield_h

#include "Arduino.h"


// forward declarations
class Ball;
class Paddle;
class Border;
class Deadzone;


struct GameState {

    unsigned short leftScore;
    unsigned short rightScore;
    bool paused;

};


struct HitState {

    bool paddle;
    bool border;
    bool leftDeadzone;
    bool rightDeadzone;

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


class MoveableObject : public PongObject {

    public:
        MoveableObject(int x, int y, int x_dim, int y_dim);
        float getMovementDirectionX();
        float getMovementDirectionY();
        void stayStill();

    protected:
        void setMovementDirection(float x, float y);
        void setCoordinates(float newX, float newY);
        float calcNewX(float movement);
        float calcNewY(float movement);
        float calcMovement(float end, float start, float direction);
        unsigned long getLastMovementTime();

    private:
        float _movementDirectionX; // gets normalized by setMovementDirection
        float _movementDirectionY;
        unsigned long _lastMovementTime;

};


class Ball : public MoveableObject {

    public:
        Ball(int x, int y, int x_dim, int y_dim, float velocity); //velocity should be in px/s,
        void move(HitState& hit, Paddle& leftPaddle, Paddle& rightPaddle, Border& topBorder, Border& botBorder, Deadzone& leftDeadzone, Deadzone& rightDeadzone); // mutates HitState
        void reset();

    protected:
        float handleCollisions(HitState& hit, Paddle& leftPaddle, Paddle& rightPaddle, Border& topBorder, Border& botBorder, Deadzone& leftDeadzone, Deadzone& rightDeadzone);

    private:
        int generateRandomSign();
        float handleCollision(Paddle& paddle); // returns corrected distance
        float handleCollision(Border& border);
        bool handleCollision(Deadzone& deadzone); // returns true when hit
        float _velocity;
        int _xStart;
        int _yStart;

};


class Paddle : public MoveableObject {

    public:
        Paddle(int x, int y, int x_dim, int y_dim, int maxY, int minY, int measurementRange, int measurmentOffset);
        void setPositionFromMeasurement(float measurement);

    private:
        void setPosition(float newY); //sets y Position of Paddle
        int _maxY;
        int _minY;
        float _distanceConversionFactor;
        int _measurementOffset;
        
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