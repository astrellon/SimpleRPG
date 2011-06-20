#pragma once

#include "Game.h"
#include "GameEntity.h"
#include "Pixel.h"

#include <time.h>

class Animal : public GameEntity
{
public:
	Animal(Game *game);
	~Animal(void);

	Pixel virtual getGraphic();
	void virtual update(float dt);

	virtual void setDestination(const Vector2 &dest);
	virtual void setDestination(const MathType &xPos, const MathType &yPos);
	virtual Vector2 getDestination() { return mDestination; }

	vector<Vector2> *getPath() { return mPath; }

	virtual void updateMovePath();

	virtual void displayActions(HUD &hud)
	{
		if(!mRedisplay)
			return;

		GameEntity::displayActions(hud);

		hud << "Destination: " << mDestination.x << ", " << mDestination.y << '\n';
	}

	virtual string getEntityName() { return "Animal"; }

	virtual MathType getRunningSpeed() { return mRunningSpeed; }
	virtual void setRunningSpeed(MathType speed) { mRunningSpeed = speed; }

	virtual MathType getWalkingSpeed() { return mWalkingSpeed; }
	virtual void setWalkingSpeed(MathType speed) { mWalkingSpeed = speed; }

	virtual bool getWalking() { return mWalking; }
	virtual void setWalking(bool walking) { mWalking = walking; }

	virtual MathType getCurrentSpeed() { return getWalking() ? getWalkingSpeed() : getRunningSpeed(); }

	virtual MathType getAggression() { return mAggression; }
	virtual void setAggression(MathType aggression) { mAggression = aggression; }

	virtual MathType getDiet() { return mDiet; }
	virtual void setDiet(MathType diet) { mDiet = diet; }

	virtual MathType getSize() { return mSize; }
	virtual void setSize(MathType size) { mSize = size; }

	virtual int getStrength() { return mStrength; }
	virtual void setStrength(int strength) { mStrength = strength; }

	virtual int getIntelligence() { return mIntelligence; }
	virtual void setIntelligence(int intelligence) { mIntelligence = intelligence; }

protected:

	vector<Vector2> *mPath;
	Vector2 mDestination;
	int mState;

	MathType mRunningSpeed;
	MathType mTurningSpeed;
	MathType mWalkingSpeed;
	MathType mAggression;
	MathType mDiet;
	MathType mSize;

	int mStrength;
	int mIntelligence;

	bool mWalking;

	virtual void doStateIdle(float dt) {}
	virtual void doStateMoving(float dt);
	float nextFloat() { return (float)rand() / (float)RAND_MAX; }

	virtual void saveProperties(ofstream &file);
	virtual void saveProperty(const int &propertyId, ofstream &file);
	virtual void loadProperties(boost::sregex_token_iterator &iter);

	float getTurnAmount(float facing, float dest);

	virtual void onAddedToGame()
	{
		updateMovePath();
	}
};

