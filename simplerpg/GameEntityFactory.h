#pragma once

#include <string>
#include <boost/algorithm/string/predicate.hpp>

#include "GameEntity.h"
#include "Animal.h"
#include "Plant.h"

using std::string;
using boost::algorithm::iequals;

class Game;

class GameEntityFactory
{
public:
	static GameEntity *create(Game *game, string name);
};
