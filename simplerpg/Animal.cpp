#include "Animal.h"


Animal::Animal(Game *game) : GameEntity(game)
{
	srand( (unsigned int)time(NULL) );
}

Animal::~Animal(void)
{
}

Pixel Animal::getGraphic()
{
	double angle = mTransform.getAngle() * 180 / M_PI;
	if(angle >= -45 && angle < 45)
		mGraphic.graphic = 'V';
	else if(angle >= 45 && angle < 135)
		mGraphic.graphic = '<';
	else if(angle >= -135 && angle < -45)
		mGraphic.graphic = '>';
	else
		mGraphic.graphic = '^';

	return GameEntity::getGraphic();
}

void Animal::update(float dt)
{
	GameEntity::update(dt);
}
