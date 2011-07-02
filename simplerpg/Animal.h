#pragma once

#include "Game.h"
#include "GameEntity.h"
#include "Pixel.h"

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

	virtual void displayActions(UIContainer &hud)
	{
		if(!mRedisplay)
			return;

		GameEntity::displayActions(hud);

		*mHudText << "Destination: " << mDestination.x << ", " << mDestination.y << '\n';
	}

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

	virtual string getEntityType() { return "Animal"; }

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

	float mLineOfSightDistance;
	float mLineOfSightAngle;

	virtual void doStateIdle(float dt) {}
	virtual void doStateMoving(float dt);
	
	virtual void doStateHungry(float dt) {}
	virtual void doStateFindMeat(float dt) {}
	virtual void doStateFindPlant(float dt) {}

	virtual void saveProperties(ofstream &file);
	virtual void saveProperty(const int &propertyId, ofstream &file);
	virtual void loadProperties(boost::sregex_token_iterator &iter);

	float getTurnAmount(float facing, float dest);

	virtual void onAddedToGame()
	{
		updateMovePath();
	}
};

