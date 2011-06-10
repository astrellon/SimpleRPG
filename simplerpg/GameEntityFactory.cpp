#include "GameEntityFactory.h"
#include "Game.h"

GameEntity *GameEntityFactory::create(Game *game, string name)
{
	if(boost::algorithm::iequals(name, "Animal"))
	{
		return new Animal(game);
	}
	return NULL;
}