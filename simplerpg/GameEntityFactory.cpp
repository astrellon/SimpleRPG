#include "GameEntityFactory.h"
#include "Game.h"

GameEntity *GameEntityFactory::create(Game *game, string name)
{
	if(iequals(name, "Animal"))
	{
		return new Animal(game);
	}
	if(iequals(name, "Plant"))
	{
		return new Plant(game);
	}
	return NULL;
}