#include "GameEntityFactory.h"
#include "Game.h"

GameEntity *GameEntityFactory::create(Game *game, string name)
{
	if(boost::algorithm::iequals(name, "Animal"))
	{
		return new Animal(game);
	}
	if(boost::algorithm::iequals(name, "Plant"))
	{
		return new Plant(game);
	}
	return NULL;
}