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

class Animal : public GameEntity
{
public:
	Animal(Game *game);
	~Animal(void);

	Pixel virtual getGraphic();
	void virtual update(float dt);

	Destination &getDestination();

	void displayActions(UIContainer &hud);

	virtual string getSpeciesName();

	float getRunningSpeed();
	inline float getRunningSpeedBase() { return mRunningSpeed; }
	inline void  setRunningSpeedBase(float speed) { mRunningSpeed = speed; }

	float getWalkingSpeed();
	inline float getWalkingSpeedBase() { return mWalkingSpeed; }
	inline void  setWalkingSpeedBase(float speed) { mWalkingSpeed = speed; }

	inline bool  getWalking() { return mWalking; }
	inline void  setWalking(bool walking) { mWalking = walking; }

	inline float getCurrentSpeed() { return getWalking() ? getWalkingSpeed() : getRunningSpeed(); }

	inline float getTurningSpeed();
	inline float getTurningSpeedBase() { return mTurningSpeed; }
	inline void  setTurningSpeedBase(float speed) { mTurningSpeed = speed; }

	inline float getWalkingDexMultiplier() { return (1 + getDexterity() / 20); }
	inline float getRunningDexMultiplier() { return (1 + getDexterity() / 10); }
	inline float getTurningDexMultiplier() { return (1 + getDexterity() / 10); }

	float getAggression() { return mAggression; }
	void  setAggression(float aggression) { mAggression = aggression; }

	float getDiet() { return mDiet; }
	void  setDiet(float diet) { mDiet = diet; }

	float getStrength() { return mStrength; }
	void  setStrength(float strength) { mStrength = strength; }

	float getDexterity() { return mDexterity; }
	void  setDexterity(float dexterity) { mDexterity = dexterity; }

	float getIntelligence() { return mIntelligence; }
	void  setIntelligence(float intelligence) { mIntelligence = intelligence; }

	string getEntityType() { return "Animal"; }

	float getEnergyNeededPerDay() { return mEnergyNeededPerDay; }
	void  setEnergyNeededPerDay(float energy) { mEnergyNeededPerDay = energy; }
	
	float getEnergy() { return mEnergy; }
	void  setEnergy(float energy) 
	{
		if (energy < 0.0f)
		{
			energy = 0.0f;
		}
		if(energy > mEnergy)
		{
			setAccumulatedEnergy(getAccumulatedEnergy() + (energy - mEnergy));
		}
		mEnergy = energy;
	}
	void  changeEnergy(float energy) { setEnergy(getEnergy() + energy); }

	float getHungerLowerLimit() { return mHungerLowerLimit; }
	void  setHungerLowerLimit(float limit) { mHungerLowerLimit = limit; }

	float getHungerUpperLimit() { return mHungerUpperLimit; }
	void  setHungerUpperLimit(float limit) { mHungerUpperLimit = limit; }

	float getHungerBreedLimit() { return mHungerBreedLimit; }
	void  setHungerBreedLimit(float limit) { mHungerBreedLimit = limit; }

	float getHealth() { return mHealth; }
	void  setHealth(float health);
	void  changeHealth(float health);

	float getMaxHealth() { return mMaxHealth; }
	void  setMaxHealth(float health) { mMaxHealth = health; }

	float getDamageBase() { return mDamageBase; }
	void  setDamageBase(float damage) { mDamageBase = damage; }

	float getAttackDamage() { return getDamageBase(); }

	float getAttackRate() { return mAttackRate; }
	void  setAttackRate(float rate) { mAttackRate = rate; }

	float getAttackCooldown() { return mAttackCooldown; }
	void  setAttackCooldown(float cooldown) { mAttackCooldown = cooldown; }

	float getAttackedByCooldown() { return mAttackedByCooldown; }
	void  setAttackedByCooldown(float cooldown) { mAttackedByCooldown = cooldown; }

	float getHungerDamageCooldown() { return mHungerDamageCooldown; }
	void  setHungerDamageCooldown(float cooldown) { mHungerDamageCooldown = cooldown; }

	float getHungerHealCooldown() { return mHungerHealCooldown; }
	void  setHungerHealCooldown(float cooldown) { mHungerHealCooldown = cooldown; }

	void  receiveDamage(float damage, Animal *from = NULL);

	void  killAnimal();
	bool  isDead() { return mHealth <= 0.0f; }
	bool  diedToday(int day);
	bool  bornToday(int day);

	float getSpeciesAlignment(GameEntity *entity);
	float getSpeciesAlignment(const string &species);

	void  setSpeciesAlignment(GameEntity *entity, const float &alignment);
	void  setSpeciesAlignment(const string &species, const float &alignment);

	void  changeSpeciesAlignment(GameEntity *entity, const float &alignment);
	void  changeSpeciesAlignment(const string &species, const float &alignment);

	float getEntityThreat(Animal *entity);
	float getEntityThreat(GameEntity *entity);

	float getMutationRate() { return mMutationRate; }
	void  setMutationRate(float rate) { mMutationRate = rate; }

	float getMutationAmount() { return mMutationAmount; }
	void  setMutationAmount(float amount) { mMutationAmount = amount; }

	float getAccumulatedEnergy() { return mAccumulatedEnergy; }
	void  setAccumulatedEnergy(float energy) { mAccumulatedEnergy = energy; }

	float getAttackRange();

	float getAge() { return mAge; }
	void  setAge(float age) { mAge = age; }

	float getAgeInDays();
	void  setAgeInDays(float days);

	float getLifeExpectancy() { return mLifeExpectancy; }
	void  setLifeExpectancy(float life) { mLifeExpectancy = life; }

	float getBreedingAge() { return mBreedingAge; }
	void  setBreedingAge(float age) { mBreedingAge = age; }

	float getBreedingRate() { return mBreedingRate; }
	void  setBreedingRate(float rate) { mBreedingRate = rate; }

	float getBreedingCount() { return mBreedingCount; }
	void  setBreedingCount(float count) { mBreedingCount = count; }

	float getFertility() { return mFertility; }
	void  setFertility(float fertility) { mFertility = fertility; }

	float getMateFindCooldown() { return mMateFindCooldown; }
	void  setMateFindCooldown(float cooldown) { mMateFindCooldown = cooldown; }

	string getBirthdate() { return mBirthdate; }
	void setBirthdate(string date) { mBirthdate = date; }

	string getDeathdate() { return mDeathdate; }
	void setDeathdate(string date) { mDeathdate = date; }

	float getDeathtime() { return mDeathtime ; }
	void  setDeathtime(float time) { mDeathtime = time; }

	float getBirthtime() { return mBirthtime ; }
	void  setBirthtime(float time) { mBirthtime = time; }

	float getLocalPopulationMax() { return mLocalPopulationMax; }
	void  setLocalPopulationMax(float population) { mLocalPopulationMax = population; }

	static void breed(vector<Animal *> &children, Animal *parent1, Animal *parent2);

	bool wantsToBreed();

	float getChanceOfDeath() { return ((2.5f / getLifeExpectancy() * getAgeInDays()) - 2.0f) / 10000.0f; }

	float getFitness();

	string getDeathBy() { return mDeathBy; }
	void setDeathBy(string death) { mDeathBy = death; }

	void breedWith(Animal *other);

	EntityList &getSurroundingEntities() { return mSurroundingEntities; }

	void getNearbyAnimalsForBreeding(const float &radius, vector<Animal *> &entities);

	AnimalRef &getParent1() { return mParent1; }
	AnimalRef &getParent2() { return mParent2; }

	float getAmountFoodLeft() { return getEnergyNeededPerDay() + getEnergy() - getAmountEaten(); }

	virtual float beEaten(float amountWanted, GameEntity *eater);

	float getDesiredNumChildren() { return mDesiredNumChildren; }
	void  setDesiredNumChildren(float num) { mDesiredNumChildren = num; }

	vector<AnimalRef> &getChildren() { return mChildren; }

	void dealWithAttackFrom(Animal *from);

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
	float mHungerBreedLimit;
	float mHungerDamageCooldown;
	float mHungerHealCooldown;
	float mFindFoodCooldown;

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

	EntityList mSurroundingEntities;
	Destination mAttackedBy;
	float mAttackedByCooldown;

	string mDeathBy;

	float mMutationRate;
	float mMutationAmount;

	float mLocalPopulationMax;

	float mAccumulatedEnergy;

	float mAge;
	float mLifeExpectancy;

	float mBreedingAge;
	float mBreedingRate;
	float mBreedingCount;
	float mFertility;
	vector<AnimalRef> mChildren;
	float mDesiredNumChildren;

	float mMateFindCooldown;

	string mBirthdate;
	string mDeathdate;
	float mBirthtime;
	float mDeathtime;
	
	virtual float calculateKcalPerDay();
	virtual bool isHungry(int limitLevel = 0);
	
	virtual bool eatAnimal(Animal *animal, float dt);
	virtual bool eatTile(TileData *data, float dt);

	virtual void doStateIdle(float dt) {}
	virtual void moveAnimal(float dt);

	virtual void dealWithThreats(float dt);

	virtual Animal *findGreatestThreat();
	
	virtual void attackAnimal(Animal *target, float dt);
	virtual void doActionEat(float dt);
	virtual void doActionMove(float dt);
	virtual void doActionFlee(float dt);
	virtual void doActionAttack(float dt);
	virtual void doActionBreed(float dt);

	virtual void saveProperties(FormattedFile &file);
	virtual void saveProperty(const EntityProperty &propertyId, FormattedFile &file);
	virtual void loadProperties(FormattedFileIterator &iter);

	virtual void getNearbyAnimals(const float &radius, vector<Animal *> &result);
	virtual void getNearbyAnimals(const float &radius, vector<Animal *> &result, string *restrictToSpecies);
	virtual void getNearbyAnimals(const float &radius, vector<Animal *> &result, string &restrictToSpecies);

	virtual TargetAction *castTargetAction(Action *action, const string &actionName, bool checkForSelfTarget = true);

	float getTurnAmount(float facing, float dest);

	static float breedProperty(const float &parent1Value, const float &parent2Value, const float &mutationAmount = -1.0f, const float &mutationRate = -1.0f, float diff = 2.0f, float minClamp = 0.0f, float maxClamp = -1.0f);

	static const Pixel GRAPHIC_DEAD;
};

