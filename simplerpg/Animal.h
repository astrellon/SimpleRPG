#pragma once

#include "Game.h"
#include "GameEntity.h"
#include "Pixel.h"

#include <time.h>

class Animal : public GameEntity
{
public:
	Animal(Game *game);
	~Animal(void);

	Pixel virtual getGraphic();
	void virtual update(float dt);

protected:
	float nextFloat() { return (float)rand() / (float)RAND_MAX; }
};

