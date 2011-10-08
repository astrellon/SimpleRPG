#include "Tile.h"

map<int, Tile *> Tile::sTiles;

Tile Tile::UNKNOWN_TILE(Pixel('?', COLOR_MAGENTA, true), -1, "Bad tile code");

Tile::Tile()
{
	setPassable(true);
	setTransparent(true);
	pixel.setColour(15);
	pixel.graphic = ' ';
	setName("Unnamed");

	setFoodValue(0.0f);
	setRegrowthRate(0.0f);
	setMaxFoodValue(0.0f);
}

Tile::Tile(Pixel pix, int code, string name, bool passable, bool transparent)
{
	setPassable(passable);
	setTransparent(transparent);
	pixel = pix;
	setCode(code);
	setName(name);

	setFoodValue(0.0f);
	setRegrowthRate(0.0f);
	setMaxFoodValue(0.0f);
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
	tile->setMaxFoodValue(20);
	tile->setRegrowthRate(0.03f);
	Tile::registerTile(tile);

	// Tree or bush
	tile = new Tile(Pixel('^', COLOR_GREEN, true), 1, "Trees", false);
	//tile->setMaxFoodValue(500);
	//tile->setRegrowthRate(0.025f);
	Tile::registerTile(tile);

	// Grass
	tile = new Tile(Pixel(',', COLOR_GREEN, false), 2, "Grass");
	tile->setMaxFoodValue(60);
	tile->setRegrowthRate(0.04f);
	Tile::registerTile(tile);

	// Tall grass
	tile = new Tile(Pixel(';', COLOR_GREEN, false), 3, "Tall grass");
	tile->setMaxFoodValue(120);
	tile->setRegrowthRate(0.05f);
	Tile::registerTile(tile);

	// Fence
	tile = new Tile(Pixel('#', COLOR_YELLOW, false), 4, "Fence", false);
	Tile::registerTile(tile);

	// Sand
	tile = new Tile(Pixel(',', COLOR_YELLOW, false), 5, "Sand");
	tile->setMaxFoodValue(10);
	tile->setRegrowthRate(0.005f);
	Tile::registerTile(tile);

	// Water
	tile = new Tile(Pixel('~', COLOR_BLUE, true), 6, "Water", false);
	Tile::registerTile(tile);

	// Deep water
	tile = new Tile(Pixel('~', COLOR_BLUE, false), 7, "Deep water", false);
	Tile::registerTile(tile);

	// Stone
	tile = new Tile(Pixel('#', COLOR_WHITE, false), 8, "Stone", false, false);
	Tile::registerTile(tile);

	// Lime Stone
	tile = new Tile(Pixel('#', COLOR_WHITE, true), 9, "Lime Stone", false, false);
	Tile::registerTile(tile);

	// Thick trees
	tile = new Tile(Pixel('^', COLOR_GREEN, false), 10, "Thick Trees", false, false);
	Tile::registerTile(tile);

	// Dirt
	tile = new Tile(Pixel(',', COLOR_RED, false), 11, "Dirt");
	Tile::registerTile(tile);

	// Shallow water
	tile = new Tile(Pixel('~', COLOR_CYAN, true), 12, "Shallow water");
	Tile::registerTile(tile);
}