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

	virtual void saveProperty(const int &propertyId, ofstream &file);
	virtual void loadProperties(boost::sregex_token_iterator &iter);

	virtual string getEntityName() { return "Animal"; }
};

