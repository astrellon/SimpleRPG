#include "Plant.h"

Plant::Plant(Game *game) : GameEntity(game)
{
	mName = "Plant";

	mGraphic.bold = true;
	mGraphic.setColour(COLOR_GREEN);
	mGraphic.graphic = '*';
}

Plant::~Plant()
{

}
