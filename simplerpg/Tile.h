#pragma once

#include <map>
#include "Pixel.h"

using namespace std;

class Tile
{
public:
	Tile();
	~Tile(void);

	bool getPassable() { return mPassable; }
	void setPassable(bool passable) { mPassable = passable; }

	void setCode(int code) { mCode = code; }
	int getCode() { return mCode; }

	static void registerTile(Tile* tile);
	static Tile* getTile(int code);
	static void registerDefaults();

	Pixel pixel;

protected:
	
	bool mPassable;
	int mCode;

	static map<int, Tile *> sTiles;
};