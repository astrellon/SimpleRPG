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

class Game;

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
	void  setFoodValue(float value);
	
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

	inline bool getActive() { return mActive; }

	inline bool getFoodValueChanged() { return mFoodValueChanged; }
	inline bool getMaxFoodValueChanged() { return mMaxFoodValueChanged; }
	inline bool getRegrowthRateChanged() { return mRegrowthRateChanged; }
	
	inline Tile *getTile() { return mTile; }
	inline void setTile(Tile *tile) { mTile = tile; }

	inline void update(float dt)
	{
		if (getFoodValue() < getMaxFoodValue() && getRegrowthRate() > 0.0f)
		{
			changeFoodValue(getRegrowthRate() * dt);
		}

		if (getFoodValue() > getMaxFoodValue())
		{
			setFoodValue(getMaxFoodValue());
		}
	}

	void displayData(UIText &text);

	float eatTile(float amountWanted);

	void setFromTile(Tile *tile);
	void setFromLoaded(TileData &data);

protected:
	float mFoodValue;
	float mRegrowthRate;
	float mMaxFoodValue;

	bool mActive;
	bool mFoodValueChanged;
	bool mMaxFoodValueChanged;
	bool mRegrowthRateChanged;
	Tile *mTile;

};
