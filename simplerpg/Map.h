#pragma once

#include <iostream>
#include "ui/ui.hpp"
#include <vector>
#include <algorithm>

#include "Tile.h"
#include "Rect.h"
#include "Pixel.h"
#include "Vector2.h"
#include "AStarNode.h"
#include "GameMath.h"

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

	vector<Vector2f> *search(const Vector2i &start, const Vector2i &end);

	void renderMap(Rect rect, WINDOW *wnd);

	void setMappedTiles(map<char, Tile *> tiles) { mMappedTiles = tiles; }
	map<char, Tile *> *getMappedTiles() { return &mMappedTiles; }

protected:
	AStarNode **mMapData;

	int mWidth;
	int mHeight;

	vector<AStarNode *> *getNeighbors(Vector2f position);
	bool checkNeighbor(vector<AStarNode *> *nodes, int x, int y);

	map<char, Tile *> mMappedTiles;

	vector<Vector2f> *getPath(AStarNode *node);
};

