#pragma once

#include <ostream>
#include "GameEntity.h"
#include "Plant.h"
#include "ui/ui.hpp"
#include "Destination.h"
#include "GameEntityRef.h"

#include <boost/format.hpp>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

using boost::format;
using namespace std;

typedef map<string, float> SpeciesAlignment;

#define loadProp(name, func, type)	\
	if(iequals(propertyName, EntityPropertyNames[name])) { \
	++iter;							\
	func(lexical_cast<type>(*iter));\
	++iter; }

#define saveProp(name, func)	\
	case name:					\
		file << EntityPropertyNames[name] << ' ' << func() << '\n';	\
		break;

class Game;
class Animal;

typedef GameEntityRef<Animal> AnimalRef;
typedef struct _AnimalChildren
{
	Animal *child1;
	Animal *child2;

	_AnimalChildren()
	{
		child1 = NULL;
		child2 = NULL;
	}

	_AnimalChildren(Animal *c1, Animal *c2)
	{
		child1 = c1;
		child2 = c2;
	}
} AnimalChildren;

class Animal : public GameEntity
{
public:
	Animal(Game *game);
	~Animal(void);

	Pixel virtual getGraphic();
	void virtual update(float dt);

	Destination *getDestination();

	virtual void displayActions(UIContainer &hud);

	virtual float getRunningSpeed();
	virtual float getRunningSpeedBase() { return mRunningSpeed; }
	virtual void  setRunningSpeedBase(float speed) { mRunningSpeed = speed; }

	virtual float getWalkingSpeed();
	virtual float getWalkingSpeedBase() { return mWalkingSpeed; }
	virtual void  setWalkingSpeedBase(float speed) { mWalkingSpeed = speed; }

	virtual bool  getWalking() { return mWalking; }
	virtual void  setWalking(bool walking) { mWalking = walking; }

	virtual float getCurrentSpeed() { return getWalking() ? getWalkingSpeed() : getRunningSpeed(); }

	virtual float getTurningSpeed();
	virtual float getTurningSpeedBase() { return mTurningSpeed; }
	virtual void  setTurningSpeedBase(float speed) { mTurningSpeed = speed; }

	virtual float getAggression() { return mAggression; }
	virtual void  setAggression(float aggression) { mAggression = aggression; }

	virtual float getDiet() { return mDiet; }
	virtual void  setDiet(float diet) { mDiet = diet; }

	virtual float getStrength() { return mStrength; }
	virtual void  setStrength(float strength) { mStrength = strength; }

	virtual float getDexterity() { return mDexterity; }
	virtual void  setDexterity(float dexterity) { mDexterity = dexterity; }

	virtual float getIntelligence() { return mIntelligence; }
	virtual void  setIntelligence(float intelligence) { mIntelligence = intelligence; }

	virtual string getEntityType() { return "Animal"; }

	virtual void  eatPlant(Plant *plant);
	virtual void  eatAnimal(Animal *animal);

	virtual float getEnergyNeededPerDay() { return mEnergyNeededPerDay; }
	virtual void  setEnergyNeededPerDay(float energy) { mEnergyNeededPerDay = energy; }
	
	virtual float getEnergy() { return mEnergy; }
	virtual void  setEnergy(float energy) 
	{
		if (energy < 0.0f)
		{
			energy = 0.0f;
		}
		mEnergy = energy; 
	}
	virtual void  changeEnergy(float energy) { setEnergy(getEnergy() + energy); }

	virtual float getHungerLowerLimit() { return mHungerLowerLimit; }
	virtual void  setHungerLowerLimit(float limit) { mHungerLowerLimit = limit; }

	virtual float getHungerUpperLimit() { return mHungerUpperLimit; }
	virtual void  setHungerUpperLimit(float limit) { mHungerUpperLimit = limit; }

	virtual float getHealth() { return mHealth; }
	virtual void  setHealth(float health);
	virtual void  changeHealth(float health);

	virtual float getMaxHealth() { return mMaxHealth; }
	virtual void  setMaxHealth(float health) { mMaxHealth = health; }

	virtual float getDamageBase() { return mDamageBase; }
	virtual void  setDamageBase(float damage) { mDamageBase = damage; }

	virtual float getAttackDamage() { return getDamageBase(); }

	virtual float getAttackRate() { return mAttackRate; }
	virtual void  setAttackRate(float rate) { mAttackRate = rate; }

	virtual float getAttackCooldown() { return mAttackCooldown; }
	virtual void  setAttackCooldown(float cooldown) { mAttackCooldown = cooldown; }

	virtual float getAttackedByCooldown() { return mAttackedByCooldown; }
	virtual void  setAttackedByCooldown(float cooldown) { mAttackedByCooldown = cooldown; }

	virtual float getHungerDamageCooldown() { return mHungerDamageCooldown; }
	virtual void  setHungerDamageCooldown(float cooldown) { mHungerDamageCooldown = cooldown; }

	virtual void  receiveDamage(float damage, GameEntity *from = NULL);

	virtual void  killAnimal();
	virtual bool  isDead() { return mHealth <= 0.0f; }

	virtual float getSpeciesAlignment(GameEntity *entity);
	virtual float getSpeciesAlignment(const string &species);

	virtual void  setSpeciesAlignment(GameEntity *entity, const float &alignment);
	virtual void  setSpeciesAlignment(const string &species, const float &alignment);

	virtual void  changeSpeciesAlignment(GameEntity *entity, const float &alignment);
	virtual void  changeSpeciesAlignment(const string &species, const float &alignment);

	virtual float getEntityThreat(Animal *entity);
	virtual float getEntityThreat(GameEntity *entity);

	virtual float getAttackRange();

	static AnimalChildren breed(Animal *parent1, Animal *parent2);

	AnimalRef &getParent1() { return mParent1; }
	AnimalRef &getParent2() { return mParent2; }

protected:

	Destination mDestination;
	
	float mRunningSpeed;
	float mTurningSpeed;
	float mWalkingSpeed;
	float mAggression;
	float mDiet;
	
	float mDamageBase;
	float mAttackRate;
	float mAttackCooldown;
	
	float mHunger;
	float mHungerLowerLimit;
	float mHungerUpperLimit;
	float mHungerDamageCooldown;

	float mStrength;
	float mDexterity;
	float mIntelligence;

	bool mWalking;

	float mEnergy;
	float mEnergyNeededPerDay;

	SpeciesAlignment mSpeciesAlignment;

	vector<float> mEnergyUsage;
	float mOldEnergyMultiplier;

	AnimalRef mParent1;
	AnimalRef mParent2;

	vector<GameEntity *> mSurroundingEntities;
	Destination mAttackedBy;
	float mAttackedByCooldown;
	
	virtual float calculateKcalPerDay();
	virtual bool isHungry(bool useUpperLimit = false);
	
	virtual void eatEntity(GameEntity *entity);
	virtual bool eatTile(TileData *data, float dt);

	virtual void doStateIdle(float dt) {}
	virtual void moveAnimal(float dt);

	virtual void dealWithThreats(float dt);

	virtual GameEntity *findGreatestThreat();
	
	virtual void attackAnimal(Animal *target, float dt);
	virtual void doActionEat(float dt);
	virtual void doActionFlee(float dt) {}
	virtual void doActionAttack(float dt);

	virtual void saveProperties(FormattedFile &file);
	virtual void saveProperty(const EntityProperty &propertyId, FormattedFile &file);
	virtual void loadProperties(FormattedFileIterator &iter);

	virtual TargetAction *castTargetAction(Action *action, const string &actionName, bool checkForSelfTarget = true);

	float getTurnAmount(float facing, float dest);

	static float breedProperty(const float &parent1Value, const float &parent2Value, const float &mutationAmount, const float &mutationRate, float diff = 4.0f, float minClamp = -1.0f, float maxClamp = -1.0f);

	static const Pixel GRAPHIC_DEAD;
};

