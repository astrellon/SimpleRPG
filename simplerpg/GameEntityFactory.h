#pragma once

#include <string>
#include <boost/algorithm/string/predicate.hpp>

#include "GameEntity.h"
#include "Animal.h"

using namespace std;

class Game;

class GameEntityFactory
{
public:
	static GameEntity *create(Game *game, string name);

};
