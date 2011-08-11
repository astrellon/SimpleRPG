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

void Plant::saveProperties(FormattedFile &file)
{
	GameEntity::saveProperties(file);
	saveProperty(GRAPHIC, file);
}

void Plant::saveProperty(const EntityProperty &propertyId, FormattedFile &file)
{
	Pixel graphic = getGraphic();
	switch(propertyId)
	{
	case GRAPHIC:
		file << "graphic " << graphic.getColour() << ' ' << (graphic.bold ? '1' : '0') << ' ' << graphic.graphic << '\n';
		break;
	default:
		GameEntity::saveProperty(propertyId, file);
	}
}

void Plant::loadProperties(FormattedFileIterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, "graphic"))
	{
		++iter;
		mGraphic.setColour(lexical_cast<int>(*iter));	++iter;
		mGraphic.bold = (lexical_cast<bool>(*iter));	++iter;
		mGraphic.graphic = string(*iter)[0];			++iter;
	}
	else
	{
		GameEntity::loadProperties(iter);
	}
}