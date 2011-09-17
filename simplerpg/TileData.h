#pragma once

#include <string>

#include "FormattedFile.h"
#include "FormattedFileIterator.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include "ui/ui.hpp"

#include "Tile.h"

using boost::format;

enum TileProperty { FOOD_VALUE, REGROWTH_VALUE, MAX_FOOD_VALUE };

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

	inline float getFoodValue() { return mFoodValue; }
	inline void  setFoodValue(float value)
	{
		if (mFoodValue != value)
		{
			mFoodValue = value;
			mFoodValueChanged = true;
		}
	}
	inline void  changeFoodValue(float value) { setFoodValue(getFoodValue() + value); }

	inline float getMaxFoodValue() { return mMaxFoodValue; }
	inline void  setMaxFoodValue(float value)
	{
		if (mMaxFoodValue != value)
		{
			mMaxFoodValue = value;
			mMaxFoodValueChanged = true;
		}
	}

	inline float getRegrowthRate() { return mRegrowthRate; }
	inline void  setRegrowthRate(float rate)
	{
		if (mRegrowthRate != rate)
		{
			mRegrowthRate = rate; 
			mRegrowthRateChanged = true;
		}
	}

	inline bool getFoodValueChanged() { return mFoodValueChanged; }
	inline bool getMaxFoodValueChanged() { return mMaxFoodValueChanged; }
	inline bool getRegrowthRateChanged() { return mRegrowthRateChanged; }
	
	inline Tile *getTile() { return mTile; }
	inline void setTile(Tile *tile) { mTile = tile; }

	void update(float dt);

	void displayData(UIText &text);

	void setFromTile(Tile *tile);
	void setFromLoaded(TileData &data);

protected:
	float mFoodValue;
	float mRegrowthRate;
	float mMaxFoodValue;

	bool mFoodValueChanged;
	bool mMaxFoodValueChanged;
	bool mRegrowthRateChanged;
	Tile *mTile;

};
