#pragma once 
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include<vector>
using namespace std;
using namespace sf;


class Train
{
private:
	float _x, _y;
	float _angle;
	float _vitesse;
public:
	Train(float x, float y, float angle, float speed);

	float getX() const;
	float getY() const;
	float getAngle() const;

	void move(auto coordonne);

	void turnLeft();
	void turnRight();
	void speedUp();
	void speedDown();


};