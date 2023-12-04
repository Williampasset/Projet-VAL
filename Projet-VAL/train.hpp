
#include <SFML/Graphics.hpp>

class Train
{
private:
	float x, y;
	float angle;
	float vitesse;
public:
	Train(float x, float y, float angle, float speed);

	float getX() const;
	float getY() const;
	float getAngle() const;

	void move();

	void turnLeft();
	void turnRight();
	void speedUp();
	void speedDown();


};