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

	inline void setTile(const int &x, const int &y, Tile *tile)
	{ 
		if(x < 0 || x >= mWidth || y < 0 || y >= mHeight)
			return;

		mMapData[x][y].tile = tile; 
	}

	inline void setTile(const Vector2i &pos, Tile *tile)
	{ 
		if(pos.x < 0 || pos.x >= mWidth || pos.y < 0 || pos.y >= mHeight)
			return;

		mMapData[pos.x][pos.y].tile = tile; 
	}

	inline Tile *getTile(const int &x, const int &y) 
	{ 
		if(x < 0 || x >= mWidth || y < 0 || y >= mHeight)
			return NULL;

		return mMapData[x][y].tile; 
	}

	inline Tile *getTile(const Vector2i &pos) 
	{ 
		if(pos.x < 0 || pos.x >= mWidth || pos.y < 0 || pos.y >= mHeight)
			return NULL;

		return mMapData[pos.x][pos.y].tile; 
	}

	inline int getGroup(const int &x, const int &y) 
	{ 
		if(x < 0 || x >= mWidth || y < 0 || y >= mHeight)
			return -1;

		return mMapData[x][y].group;
	}

	inline int getGroup(const Vector2i &pos)
	{
		if(pos.x < 0 || pos.x >= mWidth || pos.y < 0 || pos.y >= mHeight)
			return -1;

		return mMapData[pos.x][pos.y].group;
	}

	void analyseMapForGroups();

	vector<Vector2f> *search(const Vector2i &start, const Vector2i &end);

	void renderMap(Rect &rect, WINDOW *wnd);

	void setMappedTiles(map<char, Tile *> tiles) { mMappedTiles = tiles; }
	map<char, Tile *> *getMappedTiles() { return &mMappedTiles; }
	void logGroups();

protected:
	AStarNode **mMapData;

	long mNodeUseCounter;

	int mWidth;
	int mHeight;

	void getNeighbors(Vector2f position);
	bool checkNeighbor(const int &x, const int &y);

	bool floodCheck(const int &x, const int &y, const int &groupNum);

	vector<AStarNode *> mOpenList;
	vector<AStarNode *> mClosedList;
	vector<AStarNode *> mNeighbors;

	map<char, Tile *> mMappedTiles;

	vector<Vector2f> *getPath(AStarNode *node);
};
