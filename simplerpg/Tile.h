#pragma once

#include <map>
#include "Pixel.h"

using namespace std;

class Tile
{
public:
	Tile();
	~Tile(void);
	/*
	char getGraphic() { return mGraphic; }
	void setGraphic(char graphic) { mGraphic = graphic; }

	int getColour() { return mColour; }
	void setColour(int colour) { mColour = colour; }
	*/
	bool getPassable() { return mPassable; }
	void setPassable(bool passable) { mPassable = passable; }

	static void registerTile(Tile* tile, int code);
	static Tile* getTile(int code);
	static void registerDefaults();

	Pixel pixel;

protected:
	
	//char mGraphic;
	bool mPassable;
	//int mColour;

	static map<int, Tile *> sTiles;
};