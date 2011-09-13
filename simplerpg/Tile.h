#pragma once

#include <map>
#include <string>
#include "Pixel.h"

using namespace std;

class Tile
{
public:
	Tile();
	Tile(Pixel pix, int code, string name, bool passable = true, bool transparent = true);
	~Tile(void);

	bool getPassable() { return mPassable; }
	void setPassable(const bool &passable) { mPassable = passable; }

	bool getTransparent() { return mTransparent; }
	void setTransparent(bool transparent) { mTransparent = transparent; }

	void setCode(const int &code) { mCode = code; }
	int getCode() { return mCode; }

	void setName(const string &name) { mName = name; }
	string getName() { return mName; }

	static void registerTile(Tile *tile);
	static Tile *getTile(const int &code);
	static void registerDefaults();

	Pixel pixel;

	static Tile UNKNOWN_TILE;

protected:
	
	bool mTransparent;
	bool mPassable;
	int mCode;
	string mName;

	static map<int, Tile *> sTiles;

	
};