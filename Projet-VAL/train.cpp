#include "train.hpp"

Train::Train(float x, float y, float angle, float speed)
    : _x(x), _y(y), _angle(angle), _speed(speed)
{}

float
Train::getX() const
{
    return _x;
}

float
Train::getY() const
{
    return _y;
}

float
Train::getAngle() const
{
    return _angle;
}

void
Train::move()
{
    _x += static_cast<float>(cos(_angle * M_PI / 180.0) * _speed);
    _y += static_cast<float>(sin(_angle * M_PI / 180.0) * _speed);
}

void
Train::turnLeft()
{
    _angle += 1.f;
}

void
Train::turnRight()
{
    _angle -= 1.f;
}

void
Train::speedUp()
{
    if (_speed < 3.f)
        _speed += 0.05f;
}

void
Train::speedDown()
{
    if (_speed > 0.f)
        _speed -= 0.05f;
    else
        _speed = 0;
}