#include "Plant.h"

Plant::Plant(Game *game) : GameEntity(game)
{
	mName = "Plant";
}

Plant::~Plant()
{

}

Pixel Plant::getGraphic()
{
	mGraphic.bold = true;
	mGraphic.setColour(COLOR_GREEN);
	mGraphic.graphic = '*';
	
	return GameEntity::getGraphic();
}