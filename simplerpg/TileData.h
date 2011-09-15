#pragma once

#include <string>

#include "FormattedFile.h"
#include "FormattedFileIterator.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "Tile.h"

enum TileProperty { FOOD_VALUE, REGROWTH_VALUE };

const char *TileDataPropertyNames[];

using std::string;
using boost::lexical_cast;
using boost::algorithm::equals;
using boost::algorithm::iequals;

class TileData
{
public:
	TileData();
	TileData(Tile *tile);
	~TileData() {}

	void saveToFile(FormattedFile &file);
	void loadFromFile(string line, FormattedFileIterator &iter);

	float getFoodValue() { return mFoodValue; }
	void  setFoodValue(float value)
	{
		if (mFoodValue != value)
		{
			mFoodValue = value;
			mFoodValueChanged = true;
		}
	}
	void  changeFoodValue(float value) { setFoodValue(getFoodValue() + value); }

	float getRegrowthRate() { return mRegrowthRate; }
	void  setRegrowthRate(float rate)
	{
		if (mRegrowthRate != rate)
		{
			mRegrowthRate = rate; 
			mRegrowthRateChanged = true;
		}
	}

	bool getFoodValueChanged() { return mFoodValueChanged; }
	bool getRegrowthRateChanged() { return mRegrowthRateChanged; }

	Tile *getTile() { return mTile; }
	void setTile(Tile *tile) { mTile = tile; }

	void setFromTile(Tile *tile);
	void setFromLoaded(TileData &data);

protected:
	float mFoodValue;
	float mRegrowthRate;

	bool mFoodValueChanged;
	bool mRegrowthRateChanged;
	Tile *mTile;

};
