#include "Tile.h"

map<int, Tile *> Tile::sTiles;

Tile::Tile()
{
	setPassable(true);
	pixel.setColour(15);
	pixel.graphic = ' ';
	setName("Unnamed");
}


Tile::~Tile(void)
{
}

void Tile::registerTile(Tile *tile)
{
	Tile::sTiles[tile->getCode()] = tile;
}

Tile *Tile::getTile(const int &code)
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
	tile->setCode(0);
	tile->setName("Short grass");
	Tile::registerTile(tile);

	// Tree or bush
	tile = new Tile();
	tile->pixel.graphic = '^';
	tile->pixel.setColour(COLOR_GREEN);
	tile->pixel.bold = false;
	tile->setPassable(false);
	tile->setCode(1);
	tile->setName("Trees");
	Tile::registerTile(tile);

	// Grass
	tile = new Tile();
	tile->pixel.graphic = ',';
	tile->pixel.setColour(COLOR_GREEN);
	tile->pixel.bold = true;
	tile->setCode(2);
	tile->setName("Grass");
	Tile::registerTile(tile);

	// Tall grass
	tile = new Tile();
	tile->pixel.graphic = ';';
	tile->pixel.setColour(COLOR_GREEN);
	tile->pixel.bold = false;
	tile->setCode(3);
	tile->setName("Tall grass");
	Tile::registerTile(tile);

	// Fence
	tile = new Tile();
	tile->pixel.graphic = '#';
	tile->setPassable(false);
	tile->pixel.setColour(COLOR_YELLOW);
	tile->pixel.bold = false;
	tile->setCode(4);
	tile->setName("Fence");
	Tile::registerTile(tile);

	// Sand
	tile = new Tile();
	tile->pixel.graphic = ',';
	tile->pixel.setColour(COLOR_YELLOW);
	tile->pixel.bold = false;
	tile->setCode(5);
	tile->setName("Sand");
	Tile::registerTile(tile);

	// Water
	tile = new Tile();
	tile->pixel.graphic = '~';
	tile->pixel.setColour(COLOR_BLUE);
	tile->pixel.bold = true;
	tile->setPassable(false);
	tile->setCode(6);
	tile->setName("Water");
	Tile::registerTile(tile);
}