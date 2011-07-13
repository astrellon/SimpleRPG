#pragma once

#include <ostream>
#include "GameEntity.h"
#include "Plant.h"
#include "UIText.h"

using namespace std;

class Game;

class Animal : public GameEntity
{
public:
	Animal(Game *game);
	~Animal(void);

	Pixel virtual getGraphic();
	void virtual update(float dt);

	virtual void setDestination(const Vector2f &dest);
	virtual void setDestination(const float &xPos, const float &yPos);
	virtual Vector2f getDestination() { return mDestination; }

	vector<Vector2f> *getPath() { return mPath; }

	virtual void updateMovePath();

	virtual void displayActions(UIContainer &hud);

	virtual float getRunningSpeed() { return mRunningSpeed; }
	virtual void setRunningSpeed(float speed) { mRunningSpeed = speed; }

	virtual float getWalkingSpeed() { return mWalkingSpeed; }
	virtual void setWalkingSpeed(float speed) { mWalkingSpeed = speed; }

	virtual bool getWalking() { return mWalking; }
	virtual void setWalking(bool walking) { mWalking = walking; }

	virtual float getCurrentSpeed() { return getWalking() ? getWalkingSpeed() : getRunningSpeed(); }

	virtual float getAggression() { return mAggression; }
	virtual void setAggression(float aggression) { mAggression = aggression; }

	virtual float getDiet() { return mDiet; }
	virtual void setDiet(float diet) { mDiet = diet; }

	virtual float getSize() { return mSize; }
	virtual void setSize(float size) { mSize = size; }

	virtual int getStrength() { return mStrength; }
	virtual void setStrength(int strength) { mStrength = strength; }

	virtual int getIntelligence() { return mIntelligence; }
	virtual void setIntelligence(int intelligence) { mIntelligence = intelligence; }

	virtual string getEntityType() { return "Animal"; }

	virtual void eatPlant(Plant *plant);
	virtual void eatAnimal(Animal *animal);

protected:

	vector<Vector2f> *mPath;
	Vector2f mDestination;
	int mState;

	float mRunningSpeed;
	float mTurningSpeed;
	float mWalkingSpeed;
	float mAggression;
	float mDiet;
	float mSize;

	float mHunger;
	float mHealth;

	int mStrength;
	int mIntelligence;

	bool mWalking;

	float mMetabolicRate;

	//virtual float calculateHunger(float dt);
	//virtual bool isHungry();
	
	virtual void eatEntity(GameEntity *entity);

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

