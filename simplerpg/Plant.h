#pragma once

#include "Game.h"
#include "GameEntity.h"
#include "Pixel.h"

class Plant : public GameEntity
{
public:
	Plant(Game *game);
	~Plant();

	virtual string getEntityType() { return "Plant"; }

	virtual void saveProperties(ofstream &file);
	virtual void saveProperty(const EntityProperty &propertyId, ofstream &file);
	virtual void loadProperties(boost::sregex_token_iterator &iter);

protected:
};
