#include "Tile.h"

map<int, Tile *> Tile::sTiles;

Tile::Tile()
{
	//mGraphic = graphic;
	mPassable = true;
	//mColour = 15;
	pixel.setColour(15);
	pixel.graphic = ' ';
}


Tile::~Tile(void)
{
}

void Tile::registerTile(Tile *tile, int code)
{
	Tile::sTiles[code] = tile;
}

Tile* Tile::getTile(int code)
{
	return Tile::sTiles[code];
}

void Tile::registerDefaults()
{
	// Short grass
	Tile* tile = new Tile();
	tile->pixel.graphic = '.';
	tile->pixel.setColour(COLOR_GREEN);
	tile->pixel.bold = true;
	Tile::registerTile(tile, 0);

	// Tree or bush
	tile = new Tile();
	tile->pixel.graphic = '^';
	tile->pixel.setColour(COLOR_GREEN);
	tile->pixel.bold = false;
	tile->setPassable(false);
	Tile::registerTile(tile, 1);

	// Grass
	tile = new Tile();
	tile->pixel.graphic = ',';
	tile->pixel.setColour(COLOR_GREEN);
	tile->pixel.bold = true;
	Tile::registerTile(tile, 2);

	// Tall grass
	tile = new Tile();
	tile->pixel.graphic = ';';
	tile->pixel.setColour(COLOR_GREEN);
	tile->pixel.bold = false;
	Tile::registerTile(tile, 3);

	// Fence
	tile = new Tile();
	tile->pixel.graphic = '#';
	tile->setPassable(false);
	tile->pixel.setColour(COLOR_YELLOW);
	tile->pixel.bold = false;
	Tile::registerTile(tile, 4);

	// Sand
	tile = new Tile();
	tile->pixel.graphic = ',';
	tile->pixel.setColour(COLOR_YELLOW);
	tile->pixel.bold = false;
	Tile::registerTile(tile, 5);
}