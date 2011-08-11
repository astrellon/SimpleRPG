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

	virtual void saveProperties(FormattedFile &file);
	virtual void saveProperty(const EntityProperty &propertyId, FormattedFile &file);
	virtual void loadProperties(FormattedFileIterator &iter);

protected:
};
