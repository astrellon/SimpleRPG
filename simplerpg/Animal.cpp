#include "Animal.h"
#include "Game.h"

Animal::Animal(Game *game) : GameEntity(game)
{
	srand( (unsigned int)time(NULL) );

	mWalking = true;
	mWalkingSpeed = 1.1f;
	mRunningSpeed = 10.0f;
	mTurningSpeed = 2.0f;
	
	mName = "Animal";

	mHealth = 1.0f;
	mMaxHealth = 1.0f;
	mHunger = 0.0f;

	mEnergyNeededPerDay = 1.0f;
	mEnergy = 1.0f;
	mOldEnergyMultiplier = 1.0f;

	mMass = 1.0f;
	mSize = 1.0f;
	mDiet = 0.5f;
	mDamageBase = 1.0f;
	mAttackRate = 1.0f;
	mAttackCooldown = 0.0f;

	mStrength = 1;
	mDexterity = 1;
	mIntelligence = 1;

	mMaxMenuLevel = 2;
}

Animal::~Animal(void)
{
}

Destination *Animal::getDestination()
{
	TargetAction *action = dynamic_cast<TargetAction *>(getCurrentAction());
	if(action != NULL)
	{
		return action->getTarget();
	}
	return &mDestination;
}

void Animal::displayActions(UIContainer &hud)
{
	if(!mRedisplay && mGame->getGamePaused())
		return;

	GameEntity::displayActions(hud);

	UIText &text = *mHudText;

	if(mMenuLevel == 0)
	{
		Vector2f dest = getDestination()->getLocation();

		format fmt("%.1f, %.1f\n");
		fmt % dest.x % dest.y;

		text << "<15>Dest</>:\t" << fmt.str();
		GameEntity *target = getDestination()->getEntity();
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
	
		fmt = format("<12>%.1f</>/<12>%.1f</> ");
		fmt % getHealth() % getMaxHealth();
		text << "<15>Health</>:\t" << fmt.str();
		if(isDead())
		{
			int ma = text.getMaxWidth();
			text << "<7>(Dead)</>";
		}

		text << "\n\n";

		text << "<15>Damage</>:\t" << getDamageBase() << '\n';
		text << "<15>ARate</>:\t" << getAttackRate() << '\n';
		text << "<15>Str</>:\t" << getStrength() << '\n';
		text << "<15>Dex</>:\t" << getDexterity() << '\n';
		text << "<15>Int</>:\t" << getIntelligence() << '\n';
		text << "<15>Diet</>:\t" << getDiet() << '\n';
		format fmt2("%.5f (%.0f) [%.1f]\n");
		fmt2 % getEnergy() % getEnergyNeededPerDay() % mOldEnergyMultiplier;

		text << "<15>Energy</>:\t" << fmt2.str();
		text << "\n<15>Cooldown</>: " << getAttackCooldown() << '\n';
		
	}
	else if(mMenuLevel == 1)
	{
		text << "<15>Alignments</>:\n\n<15>";
		for(SpeciesAlignment::iterator iter = mSpeciesAlignment.begin(); iter != mSpeciesAlignment.end(); iter++)
		{
			text << iter->first << " <12>" << iter->second << "</>\n";
		}
	}
}

Pixel Animal::getGraphic()
{
	double angle = mTransform.getAngle() * 180 / M_PI;
	if(angle >= -45 && angle < 45)
		mGraphic.graphic = 'V';
	else if(angle >= 45 && angle < 135)
		mGraphic.graphic = '<';
	else if(angle >= -135 && angle < -45)
		mGraphic.graphic = '>';
	else
		mGraphic.graphic = '^';
	
	return GameEntity::getGraphic();
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
	else if(iequals(propertyName, EntityPropertyNames[STRENGTH]))
	{
		++iter;
		setStrength(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[DEXTERITY]))
	{
		++iter;
		setDexterity(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[INTELLIGENCE]))
	{
		++iter;
		setIntelligence(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[RUNNING_SPEED]))
	{
		++iter;
		setRunningSpeedBase(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[WALKING_SPEED]))
	{
		++iter;
		setWalkingSpeedBase(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[TURNING_SPEED]))
	{
		++iter;
		setTurningSpeedBase(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[ENTITY_SIZE]))
	{
		++iter;
		setSize(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[ENTITY_MASS]))
	{
		++iter;
		setMass(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[DIET]))
	{
		++iter;
		setDiet(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[DAMAGE_BASE]))
	{
		++iter;
		setDamageBase(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[ATTACK_RATE]))
	{
		++iter;
		setAttackRate(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[ATTACK_COOLDOWN]))
	{
		++iter;
		mAttackCooldown = lexical_cast<float>(*iter); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[ENERGY]))
	{
		++iter;
		setEnergy(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[REST_ENERGY_PER_DAY]))
	{
		++iter;
		setEnergyNeededPerDay(lexical_cast<float>(*iter)); ++iter;
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
	saveProperty(ENTITY_SIZE, file);
	saveProperty(ENTITY_MASS, file);
	saveProperty(DIET, file);
	saveProperty(DAMAGE_BASE, file);
	saveProperty(ATTACK_RATE, file);
	saveProperty(ATTACK_COOLDOWN, file);
	saveProperty(ENERGY, file);
	saveProperty(REST_ENERGY_PER_DAY, file);
	saveProperty(SPECIES_ALIGNMENT, file);
}

void Animal::saveProperty(const EntityProperty &propertyId, FormattedFile &file)
{
	switch(propertyId)
	{
	case DESTINATION:
		 getDestination()->saveDestination(EntityPropertyNames[DESTINATION], file);
		break;
	case HEALTH:
		file << EntityPropertyNames[HEALTH] << ' ' << getHealth() << ' ' << getMaxHealth() << '\n';
		break;
	case STRENGTH:
		file << EntityPropertyNames[STRENGTH] << ' ' << getStrength() << '\n';
		break;
	case DEXTERITY:
		file << EntityPropertyNames[DEXTERITY] << ' ' << getDexterity() << '\n';
		break;
	case INTELLIGENCE:
		file << EntityPropertyNames[INTELLIGENCE] << ' ' << getIntelligence() << '\n';
		break;
	case RUNNING_SPEED:
		file << EntityPropertyNames[RUNNING_SPEED] << ' ' << getRunningSpeedBase() << '\n';
		break;
	case WALKING_SPEED:
		file << EntityPropertyNames[WALKING_SPEED] << ' ' << getWalkingSpeedBase() << '\n';
		break;
	case TURNING_SPEED:
		file << EntityPropertyNames[TURNING_SPEED] << ' ' << getTurningSpeedBase() << '\n';
		break;
	case ENTITY_SIZE:
		file << EntityPropertyNames[ENTITY_SIZE] << ' ' << getSize() << '\n';
		break;
	case ENTITY_MASS:
		file << EntityPropertyNames[ENTITY_MASS] << ' ' << getMass() << '\n';
		break;
	case DAMAGE_BASE:
		file << EntityPropertyNames[DAMAGE_BASE] << ' ' << getDamageBase() << '\n';
		break;
	case DIET:
		file << EntityPropertyNames[DIET] << ' ' << getDiet() << '\n';
		break;
	case ATTACK_RATE:
		file << EntityPropertyNames[ATTACK_RATE] << ' ' << getAttackRate() << '\n';
		break;
	case ATTACK_COOLDOWN:
		file << EntityPropertyNames[ATTACK_COOLDOWN] << ' ' << getAttackCooldown() << '\n';
		break;
	case ENERGY:
		file << EntityPropertyNames[ENERGY] << ' ' << getEnergy() << '\n';
		break;
	case REST_ENERGY_PER_DAY:
		file << EntityPropertyNames[REST_ENERGY_PER_DAY] << ' ' << getEnergyNeededPerDay() << '\n';
		break;
	case SPECIES_ALIGNMENT:
		file << EntityPropertyNames[SPECIES_ALIGNMENT] << '\n';
		file.changeTabLevel(1);
		for(SpeciesAlignment::iterator iter = mSpeciesAlignment.begin(); iter != mSpeciesAlignment.end(); iter++)
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
	if(isDead())
	{
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

	Action *action = getCurrentAction();
	// If not in danger. Continue doing the same action.

	switch(action->getAction())
	{
	default:
	case IDLE:
		// Do nothing!
		if(isHungry())
		{
			setCurrentAction(new TargetAction(EAT));
		}
		break;
	case EAT:
		doActionEat(dt);
		break;
	case FLEE:
		doActionFlee(dt);
	case ATTACK:
		// Do some more nothing.
		doActionAttack(dt);
	}

	moveAnimal(dt);

	float usageMultiplier = 1.0f;
	for(vector<float>::iterator iter = mEnergyUsage.begin(); iter != mEnergyUsage.end(); iter++)
	{
		usageMultiplier *= *iter;
	}

	mOldEnergyMultiplier = usageMultiplier;
	changeEnergy(-getEnergyNeededPerDay() * usageMultiplier * dt / mGame->getDayLength());
}

void Animal::moveAnimal(float dt)
{
	Vector2f pos = getPosition();
	vector<Vector2f> path = getDestination()->getPath(pos);
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
		mEnergyUsage.push_back(1.1f);
	}
	else
	{
		mEnergyUsage.push_back(4.0f);
	}
	
	while(timeTaken < dt)
	{
		Vector2f nextPos = path[0];
		Vector2f toNextPos = nextPos.sub(pos);
		double l = toNextPos.length();
		if(l < 0.4)
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
	return getRunningSpeedBase();
}

float Animal::getWalkingSpeed()
{
	return getWalkingSpeedBase();
}

float Animal::getTurningSpeed()
{
	return getTurningSpeedBase();
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
		target->receiveDamage(getAttackDamage());
	}
}

void Animal::receiveDamage(float damage)
{
	changeHealth(-damage);
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

	if(checkForSelfTarget && target->getTarget()->getEntity() == this)
	{
		clog << "Error! Current action (" << actionName << ") is targetting self (" << getEntityName() << ')' << endl;
		setCurrentAction(new TargetAction(IDLE));
		return NULL;
	}
	return target;
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
		double simpleDist = distanceToEntity(action->getTarget()->getEntity());
		if(simpleDist <= getAttackRange())
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 1)
	{
		double simpleDist = distanceToEntity(action->getTarget()->getEntity());
		if(simpleDist > getAttackRange())
		{
			action->prevStep();
		}
		setWalking(true);
		Animal *animal = dynamic_cast<Animal *>(action->getTarget()->getEntity());
		Plant *plant = dynamic_cast<Plant *>(action->getTarget()->getEntity());
		if(animal != NULL)
		{
			if(!animal->isDead())
			{
				// Attack animal.
				attackAnimal(animal, dt);
			}
			else
			{
				action->setCompleted(true);
				setCurrentAction(new TargetAction(IDLE));
			}
		}
		else if(plant != NULL)
		{
			clog << "Cannot attack plant (" << plant->getEntityName() << ")" << endl;
			action->setCompleted(true);
			setCurrentAction(new TargetAction(IDLE));
		}
		else
		{
			clog << getEntityName() << " cannot attack " << action->getTarget()->getEntity()->getEntityType() << endl;
			action->setCompleted(true);
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
		FindEntityResult result;
		if(getDiet() < 0.5f)
		{
			result = mGame->findClosestEntity(getPosition(), "Plant", this);
		}
		else if(getDiet() >= 0.5f)
		{
			result = mGame->findClosestEntity(getPosition(), "Animal", this);
		}

		if(result.entity != NULL && result.path != NULL)
		{
			action->getTarget()->setEntity(result.entity);
			result.clear();
			action->nextStep();
		}
	}
	else if(action->getStep() == 1)
	{
		// Direct distance between this animal and the target.
		double simpleDist = distanceToEntity(action->getTarget()->getEntity());
		setWalking(false);
		if(simpleDist <= getAttackRange())
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 2)
	{
		double simpleDist = distanceToEntity(action->getTarget()->getEntity());
		if(simpleDist >	getAttackRange())
		{
			action->prevStep();
		}
		setWalking(true);
		Animal *animal = dynamic_cast<Animal *>(action->getTarget()->getEntity());
		Plant *plant = dynamic_cast<Plant *>(action->getTarget()->getEntity());
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
		else if(plant != NULL)
		{
			action->nextStep();
		}
		else
		{
			clog << getEntityName() << " cannot eat " << action->getTarget()->getEntity()->getEntityType() << endl;
			action->setCompleted(true);
			setCurrentAction(new Action(IDLE));
			return;
		}
	}
	else if(action->getStep() >= 3)
	{
		double simpleDist = distanceToEntity(action->getTarget()->getEntity());
		if(simpleDist <= 1.0f)
		{
			action->setStep(1);
			return;
		}
		eatEntity(action->getTarget()->getEntity());
		if(!isHungry())
		{
			action->setCompleted(true);
			setCurrentAction(new Action(IDLE));
		}
	}
}

void Animal::eatEntity(GameEntity *entity)
{
	
}

void Animal::eatAnimal(Animal *animal)
{
	eatEntity(animal);
}

void Animal::eatPlant(Plant *plant)
{
	eatPlant(plant);
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

bool Animal::isHungry()
{
	float kcalday = calculateKcalPerDay();
	float diff = kcalday - getEnergy();
	return diff < kcalday * 0.5f;
}

void Animal::changeHealth(float health)
{
	setHealth(getHealth() + health);
}

void Animal::setHealth(float health)
{
	if(health < 0 && mHealth > 0)
	{
		mHealth = 0;
		killAnimal();
		return;
	}
	mHealth = health;
}

void Animal::killAnimal()
{
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

float Animal::getEntityThreat(Animal *entity)
{
	if (iequals(entity->getSpecies(), getSpecies()))
	{
		return 0.3f;
	}

	float sizePer = entity->getSize() / getSize();
	float strPer = entity->getStrength() / getStrength();
	float dietDiff = entity->getDiet() - getDiet();
	float total = sizePer * strPer + 1;
	
	int floatSign = (dietDiff < 0) ? -1 : 1;
	total *= exp(floatSign * 4.0f * dietDiff * dietDiff);

	return log10(total);
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
