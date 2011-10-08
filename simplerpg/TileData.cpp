#include "TileData.h"
#include "Game.h"

extern const char *TileDataPropertyNames[] = { "fv", "rr", "mfv" };

TileData::TileData()
{
	mFoodValue = -1.0f;
	mRegrowthRate = -1.0f;
	mMaxFoodValue = -1.0f;

	mFoodValueChanged = false;
	mRegrowthRateChanged = false;
	mActive = false;

	mTile = NULL;
}

TileData::TileData(Tile *tile)
{
	setFromTile(tile);
}

void TileData::setFoodValue(float value)
{
	if (value > getMaxFoodValue())
	{
		value = getMaxFoodValue();
	}

	if (mFoodValue != value)
	{
		mFoodValue = value;
		if(mActive == true && value >= getMaxFoodValue())
		{
			mActive = false;
			Game::CURRENT_GAME->removeActiveTile(this);
		}
		else if(mActive == false && value < getMaxFoodValue())
		{
			mActive = true;
			Game::CURRENT_GAME->addActiveTile(this);
		}
		mFoodValueChanged = true;
	}
}

void TileData::setFromLoaded(TileData &data)
{
	mActive = true;
	if(data.getFoodValueChanged())
	{
		setFoodValue(data.getFoodValue());
	}
	if(data.getRegrowthRateChanged())
	{
		setRegrowthRate(data.getRegrowthRate());
	}
	if(data.getRegrowthRateChanged())
	{
		setRegrowthRate(data.getRegrowthRate());
	}
}

void TileData::setFromTile(Tile *tile)
{
	if (tile == NULL)
	{
		return;
	}
	mTile = tile;
	mFoodValue = tile->getFoodValue();
	mRegrowthRate = tile->getRegrowthRate();
	mMaxFoodValue = tile->getMaxFoodValue();

	mFoodValueChanged = false;
	mRegrowthRateChanged = false;
}

void TileData::displayData(UIText &text)
{
	format fmt("<12>%.1f</>/<12>%.1f</>\n");
	fmt % getFoodValue() % getMaxFoodValue();
	text << "<15>Food</>:\t\t" << fmt.str();
	text << "<15>Regrowth</>:\t" << getRegrowthRate() << '\n';
}

void TileData::saveToFile(FormattedFile &file)
{
	if (mTile == NULL || getFoodValue() != mTile->getFoodValue())
	{
		file << TileDataPropertyNames[FOOD_VALUE] << ' ' << getFoodValue() << ' ';
	}
	if (mTile == NULL || getRegrowthRate() != mTile->getRegrowthRate())
	{
		file << TileDataPropertyNames[REGROWTH_VALUE] << ' ' << getRegrowthRate() << ' ';
	}
	if (mTile == NULL || getMaxFoodValue() != mTile->getMaxFoodValue())
	{
		file << TileDataPropertyNames[MAX_FOOD_VALUE] << ' ' << getMaxFoodValue() << ' ';
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
		else if(iequals(TileDataPropertyNames[MAX_FOOD_VALUE], line))
		{
			setMaxFoodValue(lexical_cast<float>(*iter)); ++iter;
		}
		
		line = *iter;
		++iter;
	}
}

float TileData::eatTile(float amountWanted)
{
	if(getFoodValue() < amountWanted)
	{
		amountWanted = getFoodValue();
	}

	changeFoodValue(-amountWanted);
	return amountWanted;
}
