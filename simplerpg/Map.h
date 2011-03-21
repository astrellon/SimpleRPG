#pragma once

#include <iostream>
#include <curses.h>
#include <vector>
#include <algorithm>

#include "Tile.h"
#include "Rect.h"
#include "Pixel.h"
#include "Vector2.h"
#include "AStarNode.h"

using namespace std;

class Map
{
public:
	Map(int width, int height);
	~Map(void);

	inline int getWidth() { return mWidth; }
	inline int getHeight() { return mHeight; }

	inline void setTile(int x, int y, Tile *tile)
	{ 
		if(x < 0 || x >= mWidth || y < 0 || y >= mHeight)
			return;

		mMapData[x][y].tile = tile; 
	}

	inline Tile *getTile(int x, int y) 
	{ 
		if(x < 0 || x >= mWidth || y < 0 || y >= mHeight)
			return NULL;

		return mMapData[x][y].tile; 
	}

	vector<Vector2> *search(Vector2 start, Vector2 end);

	void renderMap(Rect rect, WINDOW *wnd);

protected:
	AStarNode **mMapData;

	int mWidth;
	int mHeight;

	vector<AStarNode *> *getNeighbors(Vector2 position);

	vector<Vector2> *getPath(AStarNode *node);
};

