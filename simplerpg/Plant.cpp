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

void Plant::saveProperties(ofstream &file)
{
	GameEntity::saveProperties(file);
	saveProperty(GRAPHIC, file);
}

void Plant::saveProperty(const EntityProperty &propertyId, ofstream &file)
{
	Pixel graphic = getGraphic();
	switch(propertyId)
	{
	case GRAPHIC:
		file << Game::getOutputTabs() << "graphic " << graphic.getColour() << ' ' << (graphic.bold ? '1' : '0') << ' ' << graphic.graphic << endl;
		break;
	default:
		GameEntity::saveProperty(propertyId, file);
	}
}

void Plant::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, "graphic"))
	{
		iter++;
		mGraphic.setColour(lexical_cast<int>(*iter++));
		mGraphic.bold = (lexical_cast<bool>(*iter++));
		mGraphic.graphic = string(*iter++)[0];
	}
	else
	{
		GameEntity::loadProperties(iter);
	}
}