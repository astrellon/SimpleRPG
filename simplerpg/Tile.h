#pragma once

#include <map>
#include <string>
#include "Pixel.h"

using namespace std;

class Tile
{
public:
	Tile();
	~Tile(void);

	bool getPassable() { return mPassable; }
	void setPassable(const bool &passable) { mPassable = passable; }

	void setCode(const int &code) { mCode = code; }
	int getCode() { return mCode; }

	void setName(const string &name) { mName = name; }
	string getName() { return mName; }

	static void registerTile(Tile *tile);
	static Tile *getTile(const int &code);
	static void registerDefaults();

	Pixel pixel;

protected:
	
	bool mPassable;
	int mCode;
	string mName;

	static map<int, Tile *> sTiles;
};