#pragma once

#include "Vector2.h"
#include "Tile.h"

class AStarNode
{
public:
	AStarNode(Tile *tile = NULL)
	{
		reset();
		this->tile = tile;
		useCounter = 0;
		parent = NULL;
		group = -1;
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

	int group;

	long useCounter;

	// Should be float instead of int because as it's more
	// flexible.
	Vector2f position;

	AStarNode *parent;

	Tile *tile;
};

