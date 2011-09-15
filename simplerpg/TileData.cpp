#include "TileData.h"

extern const char *TileDataPropertyNames[] = { "fv", "rr" };

TileData::TileData()
{
	mFoodValue = -1.0f;
	mRegrowthRate = -1.0f;

	mFoodValueChanged = false;
	mRegrowthRateChanged = false;

	mTile = NULL;
}

TileData::TileData(Tile *tile)
{
	setFromTile(tile);
}

void TileData::setFromLoaded(TileData &data)
{
	if(data.getFoodValueChanged())
	{
		setFoodValue(data.getFoodValue());
	}
	if(data.getRegrowthRateChanged())
	{
		setRegrowthRate(data.getRegrowthRate());
	}
}

void TileData::setFromTile(Tile *tile)
{
	if(tile == NULL)
	{
		return;
	}
	mTile = tile;
	mFoodValue = tile->getFoodValue();
	mRegrowthRate = tile->getRegrowthRate();

	mFoodValueChanged = false;
	mRegrowthRateChanged = false;
}

void TileData::saveToFile(FormattedFile &file)
{
	if(mTile == NULL || getFoodValue() != mTile->getFoodValue())
	{
		file << TileDataPropertyNames[FOOD_VALUE] << ' ' << getFoodValue() << ' ';
	}
	if(mTile == NULL || getRegrowthRate() != mTile->getRegrowthRate())
	{
		file << TileDataPropertyNames[REGROWTH_VALUE] << ' ' << getRegrowthRate() << ' ';
	}
	file << "| ";
}

void TileData::loadFromFile(string line, FormattedFileIterator &iter)
{
	while(!equals(line, "|"))
	{
		if(iequals(TileDataPropertyNames[FOOD_VALUE], line))
		{
			setFoodValue(lexical_cast<float>(*iter)); ++iter;
		}
		else if(iequals(TileDataPropertyNames[REGROWTH_VALUE], line))
		{
			setRegrowthRate(lexical_cast<float>(*iter)); ++iter;
		}
		
		line = *iter;
		++iter;
	}
}
