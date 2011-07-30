#pragma once

#include <ostream>
#include "GameEntity.h"
#include "Plant.h"
#include "UIText.h"
#include "Destination.h"

using namespace std;

class Game;

class Animal : public GameEntity
{
public:
	Animal(Game *game);
	~Animal(void);

	Pixel virtual getGraphic();
	void virtual update(float dt);

	Destination *getDestination() { return &mDestination; }

	virtual void displayActions(UIContainer &hud);

	virtual float getRunningSpeed() { return mRunningSpeed; }
	virtual void  setRunningSpeed(float speed) { mRunningSpeed = speed; }

	virtual float getWalkingSpeed() { return mWalkingSpeed; }
	virtual void  setWalkingSpeed(float speed) { mWalkingSpeed = speed; }

	virtual bool getWalking() { return mWalking; }
	virtual void setWalking(bool walking) { mWalking = walking; }

	virtual float getCurrentSpeed() { return getWalking() ? getWalkingSpeed() : getRunningSpeed(); }

	virtual float getTurningSpeed() { return mTurningSpeed; }
	virtual void  setTurningSpeed(float speed) { mTurningSpeed = speed; }

	virtual float getAggression() { return mAggression; }
	virtual void  setAggression(float aggression) { mAggression = aggression; }

	virtual float getDiet() { return mDiet; }
	virtual void  setDiet(float diet) { mDiet = diet; }

	virtual float getSize() { return mSize; }
	virtual void  setSize(float size) { mSize = size; }

	virtual float getMass() { return mMass; }
	virtual void  setMass(float mass) { mMass = mass; }

	virtual int  getStrength() { return mStrength; }
	virtual void setStrength(int strength) { mStrength = strength; }

	virtual int  getDexderity() { return mDexderity; }
	virtual void setDexderity(int dexderity) { mDexderity = dexderity; }

	virtual int  getIntelligence() { return mIntelligence; }
	virtual void setIntelligence(int intelligence) { mIntelligence = intelligence; }

	virtual string getEntityType() { return "Animal"; }

	virtual void eatPlant(Plant *plant);
	virtual void eatAnimal(Animal *animal);

	virtual float getEnergyNeededPerDay() { return mEnergyNeededPerDay; }
	virtual void  setEnergyNeededPerDay(float energy) { mEnergyNeededPerDay = energy; }
	
	virtual void  setEnergy(float energy) { mEnergy = energy; }
	virtual float getEnergy() { return mEnergy; }

	virtual float getHealth() { return mHealth; }
	virtual void  setHealth(float health);
	virtual void  changeHealth(float health);

	virtual float getMaxHealth() { return mMaxHealth; }
	virtual void  setMaxHealth(float health) { mMaxHealth = health; }

	virtual float getDamageBase() { return mDamageBase; }
	virtual void  setDamageBase(float damage) { mDamageBase = damage; }

	virtual void killAnimal();
	virtual bool isDead() { return mHealth <= 0.0f; }
	
protected:

	Destination mDestination;
	
	float mRunningSpeed;
	float mTurningSpeed;
	float mWalkingSpeed;
	float mAggression;
	float mDiet;
	float mSize;
	float mMass;

	float mDamageBase;
	float mHealth;
	float mMaxHealth;

	float mHunger;

	int mStrength;
	int mDexderity;
	int mIntelligence;

	bool mWalking;

	float mEnergy;
	float mEnergyNeededPerDay;

	virtual float calculateKcalPerDay();
	virtual bool isHungry();
	
	virtual void eatEntity(GameEntity *entity);

	virtual void doStateIdle(float dt) {}
	virtual void doStateMoving(float dt);
	
	virtual void doStateHungry(float dt) {}
	virtual void doStateFindMeat(float dt) {}
	virtual void doStateFindPlant(float dt) {}

	virtual void saveProperties(ofstream &file);
	virtual void saveProperty(const EntityProperty &propertyId, ofstream &file);
	virtual void loadProperties(boost::sregex_token_iterator &iter);

	float getTurnAmount(float facing, float dest);
};

