#include "train.hpp"


Train::Train(float x, float y, float angle, float speed)
    : _x(x), _y(y), _angle(angle), _vitesse(speed)
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
Train::move(auto coordonne)
{
    auto x = getX();
    auto y = getY();
    auto x1 = coordonne[0]; 
    auto y1= coordonne[1];
    
    auto xhyp = x1;
    auto yhyp = y;
    auto coteadja = sqrt((xhyp - x) ^ 2 + (yhyp - y) ^ 2);
    auto cotehypo = sqrt((x1-x)^2+(y1-y)^2);
    auto angle = cos(coteadja / coteadja);  // calculer l'angle. 
    _x += static_cast<float>(cos(angle * 3.14 / 180.0) * _vitesse);
    _y += static_cast<float>(sin(angle * 3.14 / 180.0) * _vitesse);
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
    if (_vitesse < 3.f)
        _vitesse += 0.05f;
}

void
Train::speedDown()
{
    if (_vitesse > 0.f)
        _vitesse -= 0.05f;
    else
        _vitesse = 0;
}