#pragma once

#include "Vector2.h"
#include "Tile.h"

class AStarNode
{
public:
	AStarNode(Tile *tile = NULL)
	{
		reset();
		position.x = 0;
		position.y = 0;
		this->tile = tile;
	}
	~AStarNode(void)
	{
	}

	inline void reset()
	{
		h = 0;
		f = 0;
		g = 1;
		parent = NULL;
	}

	double f;
	double g;
	double h;

	Vector2 position;

	AStarNode *parent;

	Tile *tile;
};

