#include "Tile.h"

map<int, Tile *> Tile::sTiles;

Tile Tile::UNKNOWN_TILE(Pixel('?', COLOR_MAGENTA, true), -1, "Bad tile code");

Tile::Tile()
{
	setPassable(true);
	pixel.setColour(15);
	pixel.graphic = ' ';
	setName("Unnamed");
}

Tile::Tile(Pixel pix, int code, string name, bool passable)
{
	setPassable(passable);
	pixel = pix;
	setCode(code);
	setName(name);
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
	Tile *tile = new Tile(Pixel('.', COLOR_GREEN, true), 0, "Short grass");
	Tile::registerTile(tile);

	// Tree or bush
	tile = new Tile(Pixel('^', COLOR_GREEN, false), 1, "Trees", false);
	Tile::registerTile(tile);

	// Grass
	tile = new Tile(Pixel(',', COLOR_GREEN, true), 2, "Grass");
	Tile::registerTile(tile);

	// Tall grass
	tile = new Tile(Pixel(';', COLOR_GREEN, false), 3, "Tall grass");
	Tile::registerTile(tile);

	// Fence
	tile = new Tile(Pixel('#', COLOR_YELLOW, false), 4, "Fence", false);
	Tile::registerTile(tile);

	// Sand
	tile = new Tile(Pixel(',', COLOR_YELLOW, false), 5, "Sand");
	Tile::registerTile(tile);

	// Water
	tile = new Tile(Pixel('~', COLOR_BLUE, true), 6, "Water", false);
	Tile::registerTile(tile);

	// Deep water
	tile = new Tile(Pixel('~', COLOR_BLUE, false), 7, "Deep water", false);
	Tile::registerTile(tile);
}