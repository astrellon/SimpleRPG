#include "Animal.h"
#include "Game.h"

const Pixel Animal::GRAPHIC_DEAD('X', COLOR_MAGENTA, false);

Animal::Animal(Game *game) : GameEntity(game)
{
	mWalking = true;
	mWalkingSpeed = 1.1f;
	mRunningSpeed = 10.0f;
	mTurningSpeed = 2.0f;
	
	mName = "Animal";

	mHealth = 1.0f;
	mMaxHealth = 1.0f;
	mHunger = 0.0f;
	mHungerLowerLimit = 0.4f;
	mHungerUpperLimit = 0.8f;
	mHungerBreedLimit = 1.2f;
	mHungerDamageCooldown = 0.0f;
	mHungerHealCooldown = 0.0f;
	mAggression = 0.0f;
	mDesiredNumChildren = 2.0f;
	mFindFoodCooldown = 0.0f;

	mEnergyNeededPerDay = 1.0f;
	mEnergy = 1.0f;
	mAccumulatedEnergy = 0.0f;
	mOldEnergyMultiplier = 1.0f;

	mDiet = 0.5f;
	mDamageBase = 1.0f;
	mAttackRate = 1.0f;
	mAttackCooldown = 0.0f;

	mStrength = 1;
	mDexterity = 1;
	mIntelligence = 1;

	mMaxMenuLevel = 3;

	mAttackedByCooldown = 0.0f;

	mMutationRate = 0.1f;
	mMutationAmount = 0.1f;

	mAge = 0.0f;
	mLifeExpectancy = 100.0f;
	mBreedingAge = 30.0f;
	mBreedingRate = 20.0f;
	mBreedingCount = 0.0f;
	mFertility = 1.0f;

	mBirthdate = "0 0";
	mDeathdate = "0 0";
	mBirthtime = 0.0f;
	mDeathtime = -1.0f;
	mLocalPopulationMax = 20.0f;
	mDeathBy = "Not Dead";

	mMateFindCooldown = 0.0f;
}

Animal::~Animal(void)
{
}

Destination &Animal::getDestination()
{
	TargetAction *action = dynamic_cast<TargetAction *>(getCurrentAction());
	if(action != NULL)
	{
		return *action;
	}
	return mDestination;
}

string Animal::getSpeciesName()
{
	float age = getAgeInDays();
	if(age < getBreedingAge())
	{
		return getSpecies() + " (Young)";
	}
	else if(age > getLifeExpectancy() * 1.1f)
	{
		return getSpecies() + " (Ancient)";
	}
	else if(age > getLifeExpectancy() * 0.8f)
	{
		return getSpecies() + " (Elderly)";
	}
	else if(age > getLifeExpectancy() * 0.6f)
	{
		return getSpecies() + " (Senior)";
	}
	
	return getSpecies();
}

float Animal::getAgeInDays()
{
	if(mGame != NULL)
	{
		return getAge() / mGame->getDayLength();
	}
	return getAge();
}

void Animal::setAgeInDays(float days)
{
	if(mGame != NULL)
	{
		setAge(days / mGame->getDayLength());
	}
	setAge(days);
}

void Animal::displayActions(UIContainer &hud)
{
	if(!mRedisplay && mGame->getGamePaused())
		return;

	GameEntity::displayActions(hud);

	UIText &text = *mHudText;

	if(mMenuLevel == 0)
	{
		Vector2f dest = getDestination().getLocation();

		format fmtDestination("%.1f, %.1f\n");
		fmtDestination % dest.x % dest.y;
		text << "<15>Dest</>:\t" << fmtDestination.str();

		GameEntity *target = getDestination().getEntity();
		text << "<15>Target</>:\t";
		if(target != NULL)
		{
			float f = getEntityThreat(target);
			text << target->getEntityName() << " (" << target->getId() << ")\n";
			text << "<15>Threat</>:\t" << f << '\n';
		}
		else
		{
			text << "No target\n";
		}
	
		format fmtHealth = format("<12>%.1f</>/<12>%.1f</> ");
		fmtHealth % getHealth() % getMaxHealth();
		text << "<15>Health</>:\t" << fmtHealth.str();
		if(isDead())
		{
			int ma = text.getMaxWidth();
			text << "<7>(Dead)</>";
			text << "\n<15>Death By</>:\t" << getDeathBy();
		}

		text << "\n\n";

		text << "<15>Damage</>:\t" << getDamageBase() << '\n';
		text << "<15>ARate</>:\t" << getAttackRate() << '\n';
		text << "<15>Str</>:\t" << getStrength() << '\n';
		text << "<15>Dex</>:\t" << getDexterity() << '\n';
		text << "<15>Int</>:\t" << getIntelligence() << '\n';
		text << "<15>Diet</>:\t" << getDiet() << '\n';

		text << "<15>Amount Eaten</>:\t" << getAmountEaten() << ", " << getAmountFoodLeft() << '\n';
		
		format fmtEnergy("%.5f (%.0f) [%.1f]\n");
		fmtEnergy % getEnergy() % getEnergyNeededPerDay() % mOldEnergyMultiplier;
		text << "<15>Energy</>:\t" << fmtEnergy.str();

		format fmtAgeBreed("%.1f (%.0f)\n");
		fmtAgeBreed % getAgeInDays() % getLifeExpectancy();
		text << "<15>Age</>:\t" << fmtAgeBreed.str();
		text << "<15>Birth</>:\t" << getBirthdate() << '\n';
		text << "<15>BreedAge</>:\t" << getBreedingAge() << '\n';
		
		fmtAgeBreed.clear();
		fmtAgeBreed % getBreedingRate() % (getBreedingCount() / mGame->getDayLength());
		text << "<15>BreedRate</>:\t" << fmtAgeBreed.str();
		text << "<15>Fitness</>:\t" << getFitness() << '\n';

		text << "<15>WantsBreed</>:\t" << (wantsToBreed() ? "Yes\n" : "No\n");
		text << "<15>Death</>:\t" << getChanceOfDeath() << '\n';
	}
	else if(mMenuLevel == 2)
	{
		text << "<15>Alignments</>:\n\n<15>";
		for(SpeciesAlignment::iterator iter = mSpeciesAlignment.begin(); iter != mSpeciesAlignment.end(); ++iter)
		{
			text << iter->first << " <12>" << iter->second << "</>\n";
		}
	}
}

Pixel Animal::getGraphic()
{
	Pixel graphic = GameEntity::getGraphic();
	if(mGraphicFlashCooldown < GRAPHIC_FLASH_COOLDOWN * 0.5f)
	{
		if(isDead())
		{
			return GRAPHIC_DEAD;
		}
		else
		{
			double angle = mTransform.getAngle() * 180 / M_PI;
			Pixel facingGraphic;
			facingGraphic.setColour(graphic.getColour());
			facingGraphic.bold = graphic.bold;

			if (angle >= -45 && angle < 45)
				facingGraphic.graphic = 'V';
			else if(angle >= 45 && angle < 135)
				facingGraphic.graphic = '<';
			else if(angle >= -135 && angle < -45)
				facingGraphic.graphic = '>';
			else
				facingGraphic.graphic = '^';

			return facingGraphic;
		}
	}
	return graphic;
}

void Animal::loadProperties(FormattedFileIterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, EntityPropertyNames[DESTINATION]))
	{
		++iter;
		mDestination.loadDestination(iter);
	}
	else if(iequals(propertyName, EntityPropertyNames[HEALTH]))
	{
		++iter;
		mHealth = lexical_cast<float>(*iter); ++iter;
		if(mHealth < 0.0f)
		{
			mHealth = 0.0f;
		}
		mMaxHealth = lexical_cast<float>(*iter); ++iter;
	}

	else loadProp(STRENGTH, setStrength, float)
	else loadProp(DEXTERITY, setDexterity, float)
	else loadProp(INTELLIGENCE, setIntelligence, float)
	else loadProp(RUNNING_SPEED, setRunningSpeedBase, float)
	else loadProp(WALKING_SPEED, setWalkingSpeedBase, float)
	else loadProp(TURNING_SPEED, setTurningSpeedBase, float)
	else loadProp(DIET, setDiet, float)
	else loadProp(DAMAGE_BASE, setDamageBase, float)
	else loadProp(ATTACK_RATE, setAttackRate, float)
	else loadProp(ATTACK_COOLDOWN, setAttackCooldown, float)
	else loadProp(ATTACKED_BY_COOLDOWN, setAttackedByCooldown, float)
	else loadProp(ENERGY, setEnergy, float)
	else loadProp(REST_ENERGY_PER_DAY, setEnergyNeededPerDay, float)
	else loadProp(HUNGER_DAMAGE_COOLDOWN, setHungerDamageCooldown, float)
	else loadProp(HUNGER_HEAL_COOLDOWN, setHungerHealCooldown, float)
	else loadProp(MUTATION_RATE, setMutationRate, float)
	else loadProp(MUTATION_AMOUNT, setMutationAmount, float)
	else loadProp(ACCUMULATED_ENERGY, setAccumulatedEnergy, float)
	else loadProp(LIFE_EXPECTANCY, setLifeExpectancy, float)
	else loadProp(BREEDING_AGE, setBreedingAge, float)
	else loadProp(DEATHTIME, setDeathtime, float)
	else loadProp(BIRTHTIME, setBirthtime, float)
	else loadProp(MATE_FIND_COOLDOWN, setMateFindCooldown, float)
	else loadProp(FERTILITY, setFertility, float)
	else loadProp(AGGRESSION, setAggression, float)
	else loadProp(LOCAL_POPULATION_MAX, setLocalPopulationMax, float)
	else loadProp(DESIRED_NUM_CHILDREN, setDesiredNumChildren, float)

	else if(iequals(propertyName, EntityPropertyNames[CHILDREN]))
	{
		++iter;
		string token = *iter;
		while (!iequals(token, "end"))
		{
			mChildren.push_back(AnimalRef(lexical_cast<int>(token)));
			++iter;
			token = *iter;
		}
		++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[DEATHBY]))
	{
		string deathby;
		++iter;
		deathby = *iter;
		if(deathby[0] == '"')
		{
			deathby = deathby.substr(1, deathby.size() - 2);
		}
		setDeathBy(deathby);	++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[AGE]))
	{
		++iter;
		float age = lexical_cast<float>(*iter);		++iter;
		if(mGame != NULL)
		{
			age *= mGame->getDayLength();
		}
		setAge(age);
	}

	else if(iequals(propertyName, EntityPropertyNames[BREEDING_RATE]))
	{
		++iter;
		setBreedingRate(lexical_cast<float>(*iter));	++iter;
		setBreedingCount(lexical_cast<float>(*iter));	++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[BIRTHDATE]))
	{
		string birthdate;
		++iter;
		birthdate = *iter;
		if(birthdate[0] == '"')
		{
			birthdate = birthdate.substr(1, birthdate.size() - 2);
		}
		setBirthdate(birthdate);	++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[DEATHDATE]))
	{
		string deathdate;
		++iter;
		deathdate = *iter;
		if(deathdate[0] == '"')
		{
			deathdate = deathdate.substr(1, deathdate.size() - 2);
		}
		setDeathdate(deathdate);	++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[HUNGER_LIMITS]))
	{
		++iter;
		setHungerLowerLimit(lexical_cast<float>(*iter));	++iter;
		setHungerUpperLimit(lexical_cast<float>(*iter));	++iter;
		setHungerBreedLimit(lexical_cast<float>(*iter));	++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[ATTACKED_BY]))
	{
		++iter;
		mAttackedBy.setEntityId(lexical_cast<int>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[PARENTS]))
	{
		++iter;
		getParent1().setEntityId(lexical_cast<int>(*iter));	++iter;
		getParent2().setEntityId(lexical_cast<int>(*iter));	++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[SPECIES_ALIGNMENT]))
	{
		++iter;
		string token = *iter;
		string speciesName;
		while (!iequals(token, "end"))
		{
			if(speciesName.empty())
			{
				speciesName = token;
				if(speciesName[0] == '"')
				{
					speciesName = speciesName.substr(1, speciesName.size() - 2);
				}
			}
			else
			{
				setSpeciesAlignment(speciesName, lexical_cast<float>(token));
				speciesName.clear();
			}
			++iter;
			token = *iter;
		}
		++iter;
	}
	else
	{
		GameEntity::loadProperties(iter);
	}
}

void Animal::saveProperties(FormattedFile &file)
{
	GameEntity::saveProperties(file);
	saveProperty(DESTINATION, file);
	saveProperty(HEALTH, file);
	saveProperty(STRENGTH, file);
	saveProperty(DEXTERITY, file);
	saveProperty(INTELLIGENCE, file);
	saveProperty(RUNNING_SPEED, file);
	saveProperty(WALKING_SPEED, file);
	saveProperty(TURNING_SPEED, file);
	saveProperty(DIET, file);
	saveProperty(DAMAGE_BASE, file);
	saveProperty(ATTACK_RATE, file);
	saveProperty(ATTACK_COOLDOWN, file);
	saveProperty(ATTACKED_BY, file);
	saveProperty(ATTACKED_BY_COOLDOWN, file);
	saveProperty(ENERGY, file);
	saveProperty(REST_ENERGY_PER_DAY, file);
	saveProperty(ACCUMULATED_ENERGY, file);
	saveProperty(AGE, file);
	saveProperty(BIRTHDATE, file);
	saveProperty(BIRTHTIME, file);
	saveProperty(DEATHDATE, file);
	saveProperty(DEATHTIME, file);
	saveProperty(DEATHBY, file);
	saveProperty(LIFE_EXPECTANCY, file);
	saveProperty(BREEDING_AGE, file);
	saveProperty(BREEDING_RATE, file);
	saveProperty(MATE_FIND_COOLDOWN, file);
	saveProperty(FERTILITY, file);
	saveProperty(PARENTS, file);
	saveProperty(DESIRED_NUM_CHILDREN, file);
	saveProperty(CHILDREN, file);
	saveProperty(HUNGER_LIMITS, file);
	saveProperty(HUNGER_DAMAGE_COOLDOWN, file);
	saveProperty(HUNGER_HEAL_COOLDOWN, file);
	saveProperty(AGGRESSION, file);
	saveProperty(MUTATION_RATE, file);
	saveProperty(MUTATION_AMOUNT, file);
	saveProperty(LOCAL_POPULATION_MAX, file);
	saveProperty(SPECIES_ALIGNMENT, file);
}

void Animal::saveProperty(const EntityProperty &propertyId, FormattedFile &file)
{
	switch(propertyId)
	{
	case DESTINATION:
		 getDestination().saveDestination(EntityPropertyNames[DESTINATION], file);
		break;
	case HEALTH:
		file << EntityPropertyNames[HEALTH] << ' ' << getHealth() << ' ' << getMaxHealth() << '\n';
		break;
	
	saveProp(STRENGTH, getStrength)
	saveProp(DEXTERITY, getDexterity)
	saveProp(INTELLIGENCE, getIntelligence)
	saveProp(RUNNING_SPEED, getRunningSpeedBase)
	saveProp(WALKING_SPEED, getWalkingSpeedBase)
	saveProp(TURNING_SPEED, getTurningSpeedBase)
	saveProp(DAMAGE_BASE, getDamageBase)
	saveProp(DIET, getDiet)
	saveProp(ATTACK_RATE, getAttackRate)
	saveProp(ATTACK_COOLDOWN, getAttackCooldown)
	saveProp(ATTACKED_BY_COOLDOWN, getAttackedByCooldown)
	saveProp(ENERGY, getEnergy)
	saveProp(REST_ENERGY_PER_DAY, getEnergyNeededPerDay)
	saveProp(HUNGER_DAMAGE_COOLDOWN, getHungerDamageCooldown)
	saveProp(HUNGER_HEAL_COOLDOWN, getHungerHealCooldown)
	saveProp(MUTATION_RATE, getMutationRate)
	saveProp(MUTATION_AMOUNT, getMutationAmount)
	saveProp(ACCUMULATED_ENERGY, getAccumulatedEnergy)
	saveProp(LIFE_EXPECTANCY, getLifeExpectancy)
	saveProp(BREEDING_AGE, getBreedingAge)
	saveProp(MATE_FIND_COOLDOWN, getBreedingRate)
	saveProp(FERTILITY, getFertility)
	saveProp(DEATHTIME, getDeathtime)
	saveProp(BIRTHTIME, getBirthtime)
	saveProp(AGGRESSION, getAggression)
	saveProp(LOCAL_POPULATION_MAX, getLocalPopulationMax)
	saveProp(DESIRED_NUM_CHILDREN, getDesiredNumChildren)

	case CHILDREN:
		file << EntityPropertyNames[CHILDREN] << '\n';
		
		file.changeTabLevel(1);
		for(vector<AnimalRef>::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			file << iter->getEntityId() << ' ';
		}
		file.changeTabLevel(-1);
		
		file << "\nend\n";
		break;
	case DEATHBY:
		file << EntityPropertyNames[DEATHBY] << " \"" << getDeathBy() << "\"\n";
		break;
	case AGE:
		file << EntityPropertyNames[AGE] << ' ' << (getAge() / mGame->getDayLength()) << '\n';
		break;
	case BREEDING_RATE:
		file << EntityPropertyNames[BREEDING_RATE] << ' ' << getBreedingRate() << ' ' << getBreedingCount() << '\n';
		break;
	case BIRTHDATE:
		file << EntityPropertyNames[BIRTHDATE] << " \"" << getBirthdate() << "\"\n";
		break;
	case DEATHDATE:
		file << EntityPropertyNames[DEATHDATE] << " \"" << getDeathdate() << "\"\n";
		break;
	case PARENTS:
		file << EntityPropertyNames[PARENTS] << ' ' << getParent1().getEntityId() << ' ' << getParent2().getEntityId() << '\n';
		break;
	case HUNGER_LIMITS:
		file << EntityPropertyNames[HUNGER_LIMITS] << ' ' << getHungerLowerLimit() << ' ' << getHungerUpperLimit() << ' ' << getHungerBreedLimit() << '\n';
		break;
	case ATTACKED_BY:
		file << EntityPropertyNames[ATTACKED_BY] << ' ' << mAttackedBy.getEntityId() << '\n';
		break;
	
	case SPECIES_ALIGNMENT:
		file << EntityPropertyNames[SPECIES_ALIGNMENT] << '\n';
		file.changeTabLevel(1);
		for(SpeciesAlignment::iterator iter = mSpeciesAlignment.begin(); iter != mSpeciesAlignment.end(); ++iter)
		{
			file << '"' << iter->first << "\" " << iter->second << '\n';
		}
		file.changeTabLevel(-1);
		file << "end\n";

		break;
	default:
		GameEntity::saveProperty(propertyId, file);
		break;
	}
}

void Animal::update(float dt)
{
	mGraphicFlashCooldown -= dt;
	while(mGraphicFlashCooldown < 0.0f)
	{
		mGraphicFlashCooldown += GRAPHIC_FLASH_COOLDOWN;
	}

	if (isDead())
	{
		if(mGame->getCurrentDay() - getDeathtime() / mGame->getDayLength() > 4.0f)
		{
			mGame->removeEntity(this);
		}
		return;
	}

	if (mAttackCooldown > 0.0f)
	{
		// When the animals' attack is cooling down, we want to do so at
		// normal speed.
		mAttackCooldown -= dt;
	}
	else
	{
		// When the animal is attack very quickly we don't want the cooldown to
		// have as much affect. As we want to support multiple attacks per
		// frame but we don't want a really high dt to cause the animal to have a
		// big advantage should the dt return to normal again. This is a minor tweek.
		mAttackCooldown += dt * 0.25f;
	}

	mEnergyUsage.clear();

	setAge(getAge() + dt);
	if (getAgeInDays() > getBreedingAge())
	{
		setBreedingCount(getBreedingCount() + dt);
		if (getMateFindCooldown() > 0.0f)
		{
			setMateFindCooldown(getMateFindCooldown() - dt);
		}
	}

	float chanceOfDeath = getChanceOfDeath();
	if (chanceOfDeath > 0.0f)
	{
		if(math::nextFloat() < chanceOfDeath)
		{
			killAnimal();
			setDeathBy("Old Age");
		}
	}

	setAttackedByCooldown(getAttackCooldown() - dt);
	if (getAttackCooldown() < 0.0f)
	{
		setAttackedByCooldown(0.0f);
		
	}

	if (getHungerDamageCooldown() > 0.0f)
	{
		setHungerDamageCooldown(getHungerDamageCooldown() - dt);
	}

	if (mFindFoodCooldown > 0.0f)
	{
		mFindFoodCooldown -= dt;
	}

	if (getEnergy() < 0.1f)
	{
		if (getHungerDamageCooldown() <= 0.0f)
		{
			receiveDamage(getMaxHealth() / 20.0f, NULL);
			if(isDead())
			{
				setDeathBy("Starvation");
			}
			setHungerDamageCooldown(20.0f);
		}
	}

	if(getHungerHealCooldown() > 0.0f)
	{
		setHungerHealCooldown(getHungerHealCooldown() - dt);
	}

	float midEnergy = (getHungerLowerLimit() +  getHungerUpperLimit()) * 0.5f;
	if(getEnergy() > midEnergy && getHealth() < getMaxHealth())
	{
		if(getHungerHealCooldown() <= 0.0f)
		{
			float newHealth = getMaxHealth() / 20.0f + getHealth();
			if (newHealth > getMaxHealth())
			{
				newHealth = getMaxHealth();
			}
			setHealth(newHealth);
			setHungerHealCooldown(15.0f);
		}
	}

	Action *action = getCurrentAction();
	// If not in danger. Continue doing the same action.

	mSurroundingEntities.clear();
	getNearbyEntities(getLineOfSightRadius(), mSurroundingEntities);

	dealWithThreats(dt);

	switch(action->getAction())
	{
	default:
	case MOVE:
		// Simple move action sets that the animal only wants to move
		// to a given spot.
		doActionMove(dt);
		break;
	case IDLE:
		// Checks if the animal is hungry or wants to breed when idle.
		if (mFindFoodCooldown <= 0.0f && (isHungry() || (wantsToBreed() && isHungry(2))))
		{
			setCurrentAction(new TargetAction(EAT));
		}
		else if(getMateFindCooldown() <= 0.0f && wantsToBreed() && !isHungry())
		{
			setCurrentAction(new TargetAction(BREED));
		}
		break;
	case EAT:
		// Goes through the steps required to find food, move to it, 
		// kill it if it's an animal, and eat it.
		doActionEat(dt);
		break;
	case FLEE:
		// Goes through the steps of choosing a position to run away from
		// an enemy and to continue running from an enemy if it's still within
		// sight.
		doActionFlee(dt);
		break;
	case ATTACK:
		doActionAttack(dt);
		break;
	case BREED:
		doActionBreed(dt);
		break;
	}

	moveAnimal(dt);

	float usageMultiplier = 1.0f;
	for(vector<float>::iterator iter = mEnergyUsage.begin(); iter != mEnergyUsage.end(); ++iter)
	{
		usageMultiplier *= *iter;
	}

	mOldEnergyMultiplier = usageMultiplier;
	changeEnergy(-getEnergyNeededPerDay() * usageMultiplier * dt / mGame->getDayLength());
}

void Animal::dealWithThreats(float dt)
{
	Animal *threat = findGreatestThreat();
	if(threat == NULL)
	{
		return;
	}
	float threatLevel = getEntityThreat(threat);
	float agg = getAggression();
	if(threatLevel < agg)
	{
		return;
	}

	Action *action = getCurrentAction();
	TargetAction *target = dynamic_cast<TargetAction *>(action);
	if(target != NULL)
	{
		
		if(target->getEntity() == threat && target->getAction() == FLEE)
		{
			return;
		}
	}
	target = new TargetAction(FLEE);
	
	target->setEntity(threat);
	setCurrentAction(target);
}

Animal *Animal::findGreatestThreat()
{
	Animal *greatest = NULL;
	float greatestThreat = -100.0f;
	for(EntityList::iterator iter = mSurroundingEntities.begin(); iter != mSurroundingEntities.end(); ++iter)
	{
		Animal *animal = dynamic_cast<Animal *>(*iter);
		//if(animal == NULL || iequals(animal->getSpecies(), getSpecies()))
		if(animal == NULL || animal->getSpeciesId() == getSpeciesId())
		{
			continue;
		}
		float threat = getEntityThreat(animal);
		
		if (threat > greatestThreat)
		{
			greatestThreat = threat;
			greatest = animal;
		}
	}
	return greatest;
}

void Animal::moveAnimal(float dt)
{
	Vector2f pos = getPosition();
	vector<Vector2f> path = getDestination().getPath(pos);
	if(path.empty())
	{
		return;
	}
	if((Vector2i)(path[0]) == (Vector2i)(pos))
	{
		path.erase(path.begin());
	}
	if(path.empty())
	{
		return;
	}
	float facing = (float)getFacing();
	float timeTaken = 0.0f;

	if (getWalking())
	{
		mEnergyUsage.push_back(1.1f * getWalkingDexMultiplier());
	}
	else
	{
		mEnergyUsage.push_back(4.0f * getRunningDexMultiplier());
	}
	
	while(timeTaken < dt)
	{
		Vector2f nextPos = path[0];
		Vector2f toNextPos = nextPos.sub(pos);
		double l = toNextPos.length();
		if(l < 0.4f)
		{
			path.erase(path.begin());
			if(path.empty())
			{
				break;
			}
			continue;
		}
		
		float toFacing = atan2(toNextPos.y, toNextPos.x) - M_PIF * 0.5f;

		float facingDiff = getTurnAmount(facing, toFacing);
		float maxFacingChange = dt * mTurningSpeed;
		if (facingDiff > maxFacingChange)
			facingDiff = maxFacingChange;
		if (facingDiff < -maxFacingChange)
			facingDiff = -maxFacingChange;

		turn(facingDiff);

		facing = (float)getFacing();
		float turnLeft = getTurnAmount(facing, toFacing);

		if (turnLeft > 0.01f || turnLeft < -0.01f)
		{
			break;
		}

		float distTime = (float)toNextPos.length() / getCurrentSpeed();
		if (distTime > dt - timeTaken)
		{
			distTime = dt - timeTaken;
		}

		float travelDist = distTime * getCurrentSpeed();
		timeTaken += distTime;

		move(0, travelDist);

		pos = getPosition();
	}
}

float Animal::getTurnAmount(float facing, float dest)
{
	float diff = dest - facing;
	if(diff < 0.0f)
	{
		if(diff < -M_PIF)
		{
			diff += M_PIF2;
		}
	}
	else
	{
		if(diff > M_PIF)
		{
			diff -= M_PIF2;
		}
	}
	return diff;
}

float Animal::getRunningSpeed()
{
	return getRunningSpeedBase() * getRunningDexMultiplier();
}

float Animal::getWalkingSpeed()
{
	return getWalkingSpeedBase() * getWalkingDexMultiplier();
}

float Animal::getTurningSpeed()
{
	return getTurningSpeedBase() * getTurningDexMultiplier();
}

void Animal::attackAnimal(Animal *target, float dt)
{
	if(target == NULL || dt <= 0.0f)
	{
		return;
	}

	mEnergyUsage.push_back(2.0f);

	while(mAttackCooldown <= 0.0f)
	{
		mAttackCooldown += 1.0f / getAttackRate();
		target->receiveDamage(getAttackDamage(), this);
	}
}

void Animal::receiveDamage(float damage, Animal *from)
{
	if(from != NULL)
	{
		for(EntityList::iterator iter = mSurroundingEntities.begin(); iter != mSurroundingEntities.end(); ++iter)
		{
			Animal *other = dynamic_cast<Animal *>(*iter);
			//if(iequals(other->getSpecies(), getSpecies()))
			if(other->getSpeciesId() == getSpeciesId())
			{
				other->dealWithAttackFrom(from);
			}
		}
		dealWithAttackFrom(from);
	}
	changeHealth(-damage);
	if(from != NULL && isDead())
	{
		setDeathBy("Killed by " + from->getLongName());
	}
}

void Animal::dealWithAttackFrom(Animal *from)
{
	mAttackedBy.setEntity(from);
	mAttackedByCooldown = 100.0f;
	changeSpeciesAlignment(from, -0.05f);
	float threat = getEntityThreat(from);
	if(threat > getAggression() && getCurrentAction()->getAction() != FLEE)
	{
		TargetAction *action = new TargetAction(FLEE);
		
		action->setEntity(from);
		setCurrentAction(action);
	}
	else if(threat <= getAggression() && getCurrentAction()->getAction() != ATTACK)
	{
		TargetAction *action = new TargetAction(ATTACK);
		
		action->setEntity(from);
		setCurrentAction(action);
	}
}

TargetAction *Animal::castTargetAction(Action *action, const string &actionName, bool checkForSelfTarget)
{
	TargetAction *target = dynamic_cast<TargetAction *>(getCurrentAction());

	if(target == NULL)
	{
		clog << "Error! Current action is not a target action for '" << actionName << "' action!" << endl;
		setCurrentAction(new TargetAction(IDLE));
		return NULL;
	}

	
	if(checkForSelfTarget && target->getEntity() == this)
	{
		clog << "Error! Current action (" << actionName << ") is targetting self (" << getEntityName() << ')' << endl;
		setCurrentAction(new TargetAction(IDLE));
		return NULL;
	}
	return target;
}

void Animal::doActionMove(float dt)
{
	TargetAction *action = castTargetAction(getCurrentAction(), "Move");
	if(action == NULL)
	{
		return;
	}

	if(action->getStep() == 0)
	{
		// Direct distance between this animal and the target.
		setWalking(false);
		
		//double simpleDist = getPosition().sub(action->getLocation()).length();
		//if(simpleDist <= getAttackRange())
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getLocation());
		if(onSpot)
		{
			action->nextStep();
		}
	}
	else
	{
		setCurrentAction(new TargetAction(IDLE));
	}
}

void Animal::doActionFlee(float dt)
{
	TargetAction *action = castTargetAction(getCurrentAction(), "Flee");
	if(action == NULL)
	{
		return;
	}
	
	Animal *fleeFrom = dynamic_cast<Animal *>(action->getEntity());
	if(fleeFrom == NULL)
	{
		setCurrentAction(new TargetAction(IDLE));
		return;
	}

	Vector2f toAttacker = fleeFrom->getPosition().sub(getPosition());
	if(toAttacker.length() > getLineOfSightRadius())
	{
		setCurrentAction(new TargetAction(IDLE));
		return;
	}
	toAttacker.normalise();
	toAttacker = toAttacker.scale(-8);
	
	action->setLocation(getPosition().add(toAttacker));
	
	if(action->getPath(getPosition()).size() == 0)
	{
		float angle = math::nextFloat() * M_PI * 2.0f;
		toAttacker.x = cos(angle);
		toAttacker.y = sin(angle);

		toAttacker = toAttacker.scale(8);
		
		action->setLocation(getPosition().add(toAttacker));
	}
	
	if(action->getStep() == 0)
	{
		// Direct distance between this animal and the target.
		setWalking(false);

		double simpleDist = action->getLocation().sub(getPosition()).length();
		if(simpleDist <= getAttackRange())
		{
			setCurrentAction(new TargetAction(IDLE));
		}
	}
	else
	{
		setCurrentAction(new TargetAction(IDLE));
	}
}

void Animal::doActionAttack(float dt)
{
	TargetAction *action = castTargetAction(getCurrentAction(), "Attack");
	if(action == NULL)
	{
		return;
	}

	if(action->getStep() == 0)
	{
		// Direct distance between this animal and the target.
		setWalking(false);
		//double simpleDist = distanceToEntity(action->getEntity());
		//if(simpleDist <= getAttackRange())
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getEntity()->getPosition());
		if(onSpot)
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 1)
	{
		//double simpleDist = distanceToEntity(action->getEntity());
		//if(simpleDist > getAttackRange())
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getEntity()->getPosition());
		if(!onSpot)
		{
			action->prevStep();
		}
		setWalking(true);
		Animal *animal = dynamic_cast<Animal *>(action->getEntity());
		Plant *plant = dynamic_cast<Plant *>(action->getEntity());
		if(animal != NULL)
		{
			if(!animal->isDead())
			{
				// Attack animal.
				attackAnimal(animal, dt);
			}
			else
			{
				// Plants cannot be attacked.
				action->setCompleted(true, mGame->getCurrentTimeString(true));
				setCurrentAction(new TargetAction(IDLE));
			}
		}
		else if(plant != NULL)
		{
			clog << "Cannot attack plant (" << plant->getEntityName() << ")" << endl;
			action->setCompleted(true, mGame->getCurrentTimeString(true));
			setCurrentAction(new TargetAction(IDLE));
		}
		else
		{
			clog << getEntityName() << " cannot attack " << action->getEntity()->getEntityType() << endl;
			action->setCompleted(true, mGame->getCurrentTimeString(true));
			setCurrentAction(new TargetAction(IDLE));
			return;
		}
	}
}

void Animal::doActionEat(float dt)
{
	TargetAction *action = castTargetAction(getCurrentAction(), "Eat");
	if(action == NULL)
	{
		return;
	}

	if(action->getStep() == 0)
	{
		if(getDiet() < 0.5f)
		{
			Vector2i foodLocation = mGame->findClosestTileWithFood(this);
			if(foodLocation.x >= 0 && foodLocation.y >= 0)
			{
				action->setLocation(foodLocation);
				action->setStep(4);
			}
			else
			{
				mFindFoodCooldown = 60.0f;
			}
		}
		else
		{
			Animal *result = mGame->findClosestEdibleAnimal(this);

			if(result != NULL)
			{
				action->setEntity(result);
				action->nextStep();

				// Desperate, will eat own kind when 
				// really hungry and no other food is around.
				if(getEnergy() < getHungerLowerLimit() * 0.5f)
				{
					result = mGame->findClosestEdibleAnimal(this, true);

					if(result != NULL)
					{
						action->setEntity(result);
						action->nextStep();
					}
				}
			}
			else
			{
				mFindFoodCooldown = 60.0f;
			}
		}
	}
	else if(action->getStep() == 1)
	{
		// Direct distance between this animal and the target.
		//double simpleDist = distanceToEntity(action->getEntity());
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getEntity()->getPosition());
		setWalking(action->getEntity()->isDead());
		//if(simpleDist <= getAttackRange())
		if(onSpot)
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 2)
	{
		//double simpleDist = distanceToEntity(action->getEntity());
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getEntity()->getPosition());
		//if(simpleDist >	getAttackRange())
		if(!onSpot)
		{
			action->prevStep();
		}
		setWalking(true);
		Animal *animal = dynamic_cast<Animal *>(action->getEntity());
		
		if(animal != NULL)
		{
			if(!animal->isDead())
			{
				// Attack animal.
				attackAnimal(animal, dt);
			}
			else
			{
				action->nextStep();
			}
		}
		else
		{
			clog << getEntityName() << " cannot eat " << action->getEntity()->getEntityType() << endl;
			action->setCompleted(true, mGame->getCurrentTimeString(true));
			setCurrentAction(new Action(IDLE));
			return;
		}
	}
	else if(action->getStep() == 3)
	{
		//double simpleDist = distanceToEntity(action->getEntity());
		//if(simpleDist > 1.0f)
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getEntity()->getPosition());
		if(!onSpot)
		{
			action->setStep(1);
			return;
		}
		Animal *ani = dynamic_cast<Animal *>(action->getEntity());
		bool animalPlentiful = eatAnimal(ani, dt);

		int limit = 1;
		if(wantsToBreed())
		{
			limit = 2;
		}
		if(!isHungry(limit))
		{
			action->setCompleted(true, mGame->getCurrentTimeString(true));
			setCurrentAction(new Action(IDLE));
		}
		else if(!animalPlentiful)
		{
			action->setStep(0);
		}
	}

	// Eating off tiles
	else if(action->getStep() == 4)
	{
		// Direct distance between this animal and the target.
		//double simpleDist = action->getLocation().sub(getPosition()).length();
		setWalking(true);
		//if(simpleDist <= 1.0f)
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getLocation());
		if(onSpot)
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 5)
	{
		Vector2i pos = action->getLocation();
		//double simpleDist = pos.sub(getPosition()).length();
		//if(simpleDist >	getAttackRange())
		bool onSpot = (pos).equals((Vector2i)action->getLocation());
		if(!onSpot)
		{
			action->setStep(0);
		}
		TileData *data = mGame->getTileData(pos);
		if(data == NULL)
		{
			clog << "Error attempting to eat at tile " << pos.toString() << endl;
			setCurrentAction(new Action(IDLE));
			return;
		}
		bool tilePlentiful = eatTile(data, dt);
		int limit = 1;
		if(wantsToBreed())
		{
			limit = 2;
		}
		if(!isHungry(limit))
		{
			action->setCompleted(true, mGame->getCurrentTimeString(true));
			setCurrentAction(new Action(IDLE));
		}
		else if(!tilePlentiful)
		{
			action->setStep(0);
		}
	}
}

void Animal::doActionBreed(float dt)
{
	TargetAction *action = castTargetAction(getCurrentAction(), "Attack");
	if(action == NULL)
	{
		return;
	}

	if(action->getStep() == 0)
	{
		vector<Animal *> mates;
		getNearbyAnimals(getLineOfSightRadius() * 1.5f, mates, getSpecies());
		
		bool mateFound = false;

		int numAliveChildren = 0;
		for(vector<AnimalRef>::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			Animal *child = iter->getEntity();

			if(!child->isDead())
			{
				numAliveChildren++;
			}
		}

		if(mates.empty())
		{
			FindEntityResult closest = mGame->findBreedingPartner(this);
			if(closest.entity != NULL)
			{
				action->setEntity(closest.entity);
				mateFound = true;
			}
		}
		else if(mates.size() < (int)getLocalPopulationMax() && numAliveChildren < round(getDesiredNumChildren()))
		{
			vector<float> fitness;
			fitness.reserve(mates.size());
			for(int i = 0; i < mates.size(); i++)
			{
				if(!mates[i]->wantsToBreed())
				{
					mates.erase(mates.begin() + i);
					continue;
				}
				fitness.push_back(mates[i]->getFitness());
			}

			int mateIndex = math::nextRoulette(fitness);
			if (mateIndex >= 0)
			{
				action->setEntity(mates[mateIndex]);
				mateFound = true;
			}
		}
		
		if(!mateFound)
		{
			setMateFindCooldown(mGame->getDayLength() * 0.5f);
			setCurrentAction(new TargetAction(IDLE));
		}
		else
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 1)
	{
		// Direct distance between this animal and the target.
		setWalking(true);
		//double simpleDist = distanceToEntity(action->getEntity());
		//if(simpleDist <= 1.0f)
		bool onSpot = ((Vector2i)getPosition()).equals((Vector2i)action->getEntity()->getPosition());
		if(onSpot)
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 2)
	{
		vector<Animal *> children;
		Animal *other = dynamic_cast<Animal *>(action->getEntity());
		breed(children, this, other);
		other->breedWith(this);
		breedWith(other);
		setCurrentAction(new TargetAction(IDLE));

		for(vector<Animal *>::iterator iter = children.begin(); iter != children.end(); ++iter)
		{
			mGame->addEntity(*iter);
		}
	}
}

void Animal::breedWith(Animal *other)
{
	setBreedingCount(0.0f);
	setMateFindCooldown(mGame->getDayLength() * 0.5f);
}

bool Animal::eatAnimal(Animal *animal, float dt)
{
	if(animal == NULL)
	{
		return false;
	}
	float amountWanted = getSize() * getEnergyNeededPerDay() / 10.0f * dt;
	float amountEaten = animal->beEaten(amountWanted, this);

	changeEnergy(amountEaten);

	return amountEaten > amountWanted * 0.5f;
}

float Animal::beEaten(float amountWanted, GameEntity *eater)
{
	float totalEnergy = getEnergyNeededPerDay() + getEnergy();
	if(amountWanted + getAmountEaten() > totalEnergy)
	{
		float remaining = getAmountEaten() - totalEnergy;
		setAmountEaten(totalEnergy);
		return remaining;
	}
	setAmountEaten(amountWanted + getAmountEaten());
	return amountWanted;
}

bool Animal::eatTile(TileData *tile, float dt)
{
	if(tile == NULL)
	{
		return false;
	}

	float amountWanted = getSize() * getEnergyNeededPerDay() / 10.0f * dt;
	float amountEaten = tile->eatTile(amountWanted);
	changeEnergy(amountEaten);

	return amountEaten > amountWanted * 0.5f;
}

float Animal::calculateKcalPerDay()
{
	// mSize should be in cm.
	// mMass should be in kg.
	//return 2.889 * mSize + 17.237 * mMass + 69.818 * mStrength;

	//if (mEnergyNeededPerDay > 0)
	return mEnergyNeededPerDay;

	/*float energy = pow(getSize() * getMass(), 0.277f) * (getStrength() * getDexterity()) * 10.36 - 459.622;
	// Only works within a small range of animals, does not work for large or very small ones. 
	if (energy < 0.0f)
		return 0.0f;

	return energy;*/
}

bool Animal::isHungry(int limitLevel)
{
	float limit = getHungerLowerLimit();
	if (limitLevel == 1)
	{
		limit = getHungerUpperLimit();
	}
	else if(limitLevel == 2)
	{
		limit = getHungerBreedLimit() * getFertility();
	}
	return getEnergy() < calculateKcalPerDay() * limit;
}

void Animal::changeHealth(float health)
{
	setHealth(getHealth() + health);
}

void Animal::setHealth(float health)
{
	if(health <= 0 && mHealth > 0)
	{
		mHealth = 0;
		killAnimal();
		return;
	}
	mHealth = health;
}

void Animal::killAnimal()
{
	setDeathdate(mGame->getCurrentTimeString(true));
	setDeathtime(mGame->getCurrentDay() * mGame->getDayLength() + mGame->getCurrentTime());
	if(getHealth() > 0)
	{
		setHealth(0);
		return;
	}
}

float Animal::getSpeciesAlignment(GameEntity *entity)
{
	return getSpeciesAlignment(entity->getSpecies());
}

float Animal::getSpeciesAlignment(const string &species)
{
	if(iequals(species, getSpecies()))
	{
		return 1.0f;
	}
	SpeciesAlignment::iterator iter = mSpeciesAlignment.find(species);
	if (iter == mSpeciesAlignment.end())
	{
		setSpeciesAlignment(species, 0.5f);
		return 0.5f;
	}
	return iter->second;
}

void Animal::setSpeciesAlignment(GameEntity *entity, const float &alignment)
{
	setSpeciesAlignment(entity->getSpecies(), alignment);
}

void Animal::setSpeciesAlignment(const string &species, const float &alignment)
{
	mSpeciesAlignment[species] = alignment;
}

void Animal::changeSpeciesAlignment(GameEntity *entity, const float &alignment)
{
	changeSpeciesAlignment(entity->getSpecies(), alignment);
}

void Animal::changeSpeciesAlignment(const string &species, const float &alignment)
{
	mSpeciesAlignment[species] = getSpeciesAlignment(species) + alignment;
}

float Animal::getEntityThreat(Animal *entity)
{
	//if (iequals(entity->getSpecies(), getSpecies()))
	if(entity->getSpeciesId() == getSpeciesId())
	{
		return 0.3f;
	}

	float sizePer = entity->getSize() / getSize();
	float strPer = entity->getStrength() / getStrength();
	float dietDiff = entity->getDiet() - getDiet();
	float total = sizePer * strPer + 1;
	
	int floatSign = (dietDiff < 0) ? -1 : 1;
	total *= exp(floatSign * 4.0f * dietDiff * dietDiff);

	total = log10(total);

	if(mAttackedBy.getEntity() == entity)
	{
		total *= 1.2f;
	}
	return total;
}

float Animal::getEntityThreat(GameEntity *entity) 
{ 
	Animal *ani = dynamic_cast<Animal *>(entity);
	if(ani != NULL)
	{
		return getEntityThreat(ani);
	}
	return 0.0f; 
}

float Animal::getAttackRange()
{
	return getSize() * 0.6f;
}

void Animal::breed(vector<Animal *> &children, Animal *p1, Animal *p2)
{
	float r = (p1->getMutationRate() + p2->getMutationRate()) * 0.5f;
	float a = (p1->getMutationAmount() + p2->getMutationAmount()) * 0.5f;

	int numChildren = round(breedProperty(p1->getFertility(), p2->getFertility(), a, r));

	p1->changeEnergy(p1->getEnergyNeededPerDay() * numChildren * -0.2f);
	p2->changeEnergy(p2->getEnergyNeededPerDay() * numChildren * -0.2f);

	p1->setAccumulatedEnergy(0.0f);
	p2->setAccumulatedEnergy(0.0f);

	string birthdate = "0 0";
	if(p1->mGame != NULL)
	{
		birthdate = p1->mGame->getCurrentTimeString(true);		
	}

	string species;
	//if(iequals(p1->getSpecies(), p2->getSpecies()))
	if(p1->getSpeciesId() == p2->getSpeciesId())
	{
		species = p1->getSpecies();
	}
	else
	{
		species = p1->getSpecies() + '_' + p2->getSpecies();
	}

	for(int i = 0; i < numChildren; i++)
	{
		Animal *c = new Animal(p1->mGame);
		children.push_back(c);

		p1->getChildren().push_back(AnimalRef(c));
		p2->getChildren().push_back(AnimalRef(c));

		c->setSpecies(species);
		if (p1->mGame != NULL)
		{
			c->move(p1->getPosition(), false);
			c->setEntityName(p1->mGame->getRandomName());
		}
		c->setFacing(p1->getFacing());

		c->getParent1().setEntity(p1);
		c->getParent2().setEntity(p2);

		c->setBirthdate(birthdate);
		c->setBirthtime(p1->mGame->getCurrentTime() + p1->mGame->getCurrentDay() * p1->mGame->getDayLength());

		c->mGraphic = p1->mGraphic;

		c->setDiet(breedProperty(p1->getDiet(), p2->getDiet(), r, a, 2.0f, 0.0f, 1.0f));
		
		c->setStrength(breedProperty(p1->getStrength(), p2->getStrength(), a, r));
		c->setDexterity(breedProperty(p1->getDexterity(), p2->getDexterity(), a, r));
		c->setIntelligence(breedProperty(p1->getIntelligence(), p2->getIntelligence(), a, r));

		c->setRunningSpeedBase(breedProperty(p1->getRunningSpeedBase(), p2->getRunningSpeedBase(), a, r));
		c->setWalkingSpeedBase(breedProperty(p1->getWalkingSpeedBase(), p2->getWalkingSpeedBase(), a, r));
		c->setTurningSpeedBase(breedProperty(p1->getTurningSpeedBase(), p2->getTurningSpeedBase(), a, r));
		c->setAggression(breedProperty(p1->getAggression(), p2->getAggression(), a, r));
		c->setEnergyNeededPerDay(breedProperty(p1->getEnergyNeededPerDay(), p2->getEnergyNeededPerDay(), a, r));
		c->setHungerLowerLimit(breedProperty(p1->getHungerLowerLimit(), p2->getHungerLowerLimit(), a, r));
		c->setHungerUpperLimit(breedProperty(p1->getHungerUpperLimit(), p2->getHungerUpperLimit(), a, r));
		c->setHungerBreedLimit(breedProperty(p1->getHungerBreedLimit(), p2->getHungerBreedLimit(), a, r));
		c->setMaxHealth(breedProperty(p1->getMaxHealth(), p2->getMaxHealth(), a, r));
		c->setHealth(c->getMaxHealth());
		c->setMass(breedProperty(p1->getMass(), p2->getMass(), a, r));
		c->setSize(breedProperty(p1->getSize(), p2->getSize(), a, r));
		c->setDamageBase(breedProperty(p1->getDamageBase(), p2->getDamageBase(), a, r));
		c->setAttackRate(breedProperty(p1->getAttackRate(), p2->getAttackRate(), a, r));
		c->setLocalPopulationMax(breedProperty(p1->getLocalPopulationMax(), p2->getLocalPopulationMax(), a, r));
		c->setDesiredNumChildren(breedProperty(p1->getDesiredNumChildren(), p2->getDesiredNumChildren(), a, r));

		c->setMutationAmount(breedProperty(p1->getMutationAmount(), p2->getMutationAmount(), a, r));
		c->setMutationRate(breedProperty(p1->getMutationRate(), p2->getMutationRate(), a, r));

		c->setBreedingAge(breedProperty(p1->getBreedingAge(), p2->getBreedingAge(), a, r));
		c->setBreedingRate(breedProperty(p1->getBreedingRate(), p2->getBreedingRate(), a, r));
		c->setLifeExpectancy(breedProperty(p1->getLifeExpectancy(), p2->getLifeExpectancy(), a, r));
		c->setFertility(breedProperty(p1->getFertility(), p2->getFertility(), a, r));
	}
}

float Animal::breedProperty(const float &parent1Value, const float &parent2Value, const float &mutationAmount, const float &mutationRate, float diff, float minClamp, float maxClamp)
{
	float average = (parent1Value + parent2Value) * 0.5f;
	float t = diff * (max(parent1Value, parent2Value) - average);
		
	float maxv = average + t;
	if (maxClamp >= 0.0f && maxv > maxClamp)
	{
		maxv = maxClamp;
	}

	float minv = average - t;
	if (minClamp >= 0.0f && minv < minClamp)
	{
		minv = minClamp;
	}

	float value = math::nextDist(minv, maxv, average);

	if(mutationRate > 0.0f && mutationAmount > 0.0f)
	{
		float mutate = math::nextFloat();
		if (mutate < mutationRate)
		{
			float amount = mutationAmount * value;
			value = value + (amount * (math::nextFloat() - 0.5f) * 2.0f);
		}
	}
	if(value < 0)
	{
		// Break!
		clog << "Ahh negative breed! " << value << endl;
	}
	return value;
}

float Animal::getFitness()
{
	float foodAccumulated = mAccumulatedEnergy / mEnergyNeededPerDay;
	return foodAccumulated;
}

bool Animal::wantsToBreed()
{
	return (mBreedingCount / mGame->getDayLength()) > getBreedingRate();
}

void Animal::getNearbyAnimals(const float &radius, vector<Animal *> &result)
{
	getNearbyAnimals(radius, result, NULL);
}

void Animal::getNearbyAnimals(const float &radius, vector<Animal *> &result, string &restrictToSpecies)
{
	getNearbyAnimals(radius, result, &restrictToSpecies);
}

void Animal::getNearbyAnimals(const float &radius, vector<Animal *> &result, string *restrictToSpecies)
{
	vector<GameEntity *> temporaryResults;
	getNearbyEntities(radius, temporaryResults, restrictToSpecies);

	for(vector<GameEntity *>::iterator iter = temporaryResults.begin(); iter != temporaryResults.end(); ++iter)
	{
		Animal *ani = dynamic_cast<Animal *>(*iter);
		if(ani != NULL)
		{
			result.push_back(ani);
		}
	}
}

bool Animal::diedToday(int day)
{
	if(!isDead())
	{
		return false;
	}
	float deathtime = getDeathtime();
	int deathday = (int)(deathtime / mGame->getDayLength());
	return day == deathday;
}

bool Animal::bornToday(int day)
{
	if(isDead())
	{
		return false;
	}
	float birthtime = getBirthtime();
	int birthday = (int)(birthtime / mGame->getDayLength());
	return day == birthday;
}