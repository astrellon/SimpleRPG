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
	mHungerDamageCooldown = 0.0f;

	mEnergyNeededPerDay = 1.0f;
	mEnergy = 1.0f;
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
	else if(mMenuLevel == 2)
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
			facingGraphic.setColour(7);
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
	else loadProp(MUTATION_RATE, setMutationRate, float)
	else loadProp(MUTATION_AMOUNT, setMutationAmount, float)

	else if(iequals(propertyName, EntityPropertyNames[HUNGER_LIMITS]))
	{
		++iter;
		setHungerLowerLimit(lexical_cast<float>(*iter));	++iter;
		setHungerUpperLimit(lexical_cast<float>(*iter));	++iter;
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
	saveProperty(PARENTS, file);
	saveProperty(HUNGER_LIMITS, file);
	saveProperty(HUNGER_DAMAGE_COOLDOWN, file);
	saveProperty(MUTATION_RATE, file);
	saveProperty(MUTATION_AMOUNT, file);
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
	saveProp(MUTATION_RATE, getMutationRate)
	saveProp(MUTATION_AMOUNT, getMutationAmount)
	
	case PARENTS:
		file << EntityPropertyNames[PARENTS] << ' ' << getParent1().getEntityId() << ' ' << getParent2().getEntityId() << '\n';
		break;
	case HUNGER_LIMITS:
		file << EntityPropertyNames[HUNGER_LIMITS] << ' ' << getHungerLowerLimit() << ' ' << getHungerUpperLimit() << '\n';
		break;
	case ATTACKED_BY:
		file << EntityPropertyNames[ATTACKED_BY] << ' ' << mAttackedBy.getEntityId() << '\n';
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
	mGraphicFlashCooldown -= dt;
	if(mGraphicFlashCooldown < 0.0f)
	{
		mGraphicFlashCooldown += GRAPHIC_FLASH_COOLDOWN;
	}

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

	mAttackedByCooldown -= dt;
	if(mAttackedByCooldown < 0.0f)
	{
		mAttackedByCooldown = 0.0f;
		mAttackedBy.setEntity(NULL);
	}

	if(getHungerDamageCooldown() > 0.0f)
	{
		setHungerDamageCooldown(getHungerDamageCooldown() - dt);
	}

	if(getEnergy() < 0.1f)
	{
		if(getHungerDamageCooldown() <= 0.0f)
		{
			receiveDamage(getMaxHealth() / 20.0f, NULL);
			setHungerDamageCooldown(20.0f);
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

void Animal::dealWithThreats(float dt)
{
	GameEntity *threat = findGreatestThreat();
	if(threat == NULL || getEntityThreat(threat) < 1.0f)
	{
		return;
	}

	Action *action = getCurrentAction();
	TargetAction *target = dynamic_cast<TargetAction *>(action);
	if(target != NULL)
	{
		if(target->getTarget()->getEntity() == threat && target->getAction() == FLEE)
		{
			return;
		}
	}
	target = new TargetAction(FLEE);
	target->getTarget()->setEntity(threat);
	setCurrentAction(target);
}

GameEntity *Animal::findGreatestThreat()
{
	GameEntity *greatest = NULL;
	float greatestThreat = -100.0f;
	for(vector<GameEntity *>::iterator iter = mSurroundingEntities.begin();
		iter != mSurroundingEntities.end(); iter++)
	{
		float threat = getEntityThreat(*iter);
		if (threat > greatestThreat)
		{
			greatestThreat = threat;
			greatest = *iter;
		}
	}
	return greatest;
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
		target->receiveDamage(getAttackDamage(), this);
	}
}

void Animal::receiveDamage(float damage, GameEntity *from)
{
	if(from != NULL)
	{
		mAttackedBy.setEntity(from);
		mAttackedByCooldown = 100.0f;
		changeSpeciesAlignment(from, -0.05f);
	}
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
			clog << getEntityName() << " cannot attack " << action->getTarget()->getEntity()->getEntityType() << endl;
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
			Vector2i foodLocation = mGame->findClosestTileWithFood(getPosition());
			if(foodLocation.x >= 0 && foodLocation.y >= 0)
			{
				action->getTarget()->setLocation(foodLocation);
				action->setStep(4);
			}
		}
		else
		{
			FindEntityResult result;
			result = mGame->findClosestEntity(getPosition(), "Animal", this);

			if(result.entity != NULL && result.path != NULL)
			{
				action->getTarget()->setEntity(result.entity);
				result.clear();
				action->nextStep();
			}
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
			action->setCompleted(true, mGame->getCurrentTimeString(true));
			setCurrentAction(new Action(IDLE));
			return;
		}
	}
	else if(action->getStep() == 3)
	{
		double simpleDist = distanceToEntity(action->getTarget()->getEntity());
		if(simpleDist <= 1.0f)
		{
			action->setStep(1);
			return;
		}
		eatEntity(action->getTarget()->getEntity());
		if(!isHungry(true))
		{
			action->setCompleted(true, mGame->getCurrentTimeString(true));
			setCurrentAction(new Action(IDLE));
		}
	}

	else if(action->getStep() == 4)
	{
		// Direct distance between this animal and the target.
		double simpleDist = action->getTarget()->getLocation().sub(getPosition()).length();
		setWalking(true);
		if(simpleDist <= 1.0f)
		{
			action->nextStep();
		}
	}
	else if(action->getStep() == 5)
	{
		Vector2i pos = action->getTarget()->getLocation();
		double simpleDist = pos.sub(getPosition()).length();
		if(simpleDist >	getAttackRange())
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
		if(!isHungry(true))
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

void Animal::eatEntity(GameEntity *entity)
{
	
}

void Animal::eatAnimal(Animal *animal)
{
	eatEntity(animal);
}

void Animal::eatPlant(Plant *plant)
{
	eatEntity(plant);
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

bool Animal::isHungry(bool useUpperLimit)
{
	float limit = useUpperLimit ? getHungerUpperLimit() : getHungerLowerLimit();
	return getEnergy() < calculateKcalPerDay() * limit;
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

AnimalChildren Animal::breed(Animal *p1, Animal *p2)
{
	Animal *child1 = new Animal(p1->mGame);
	Animal *child2 = new Animal(p2->mGame);

	float r = (p1->getMutationRate() + p2->getMutationRate()) * 0.5f;
	float a = (p1->getMutationAmount() + p2->getMutationAmount()) * 0.5f;

	for(int i = 0; i <= 1; i++)
	{
		Animal *c = child1;
		if(i == 1)
		{
			c = child2;
		}

		if(iequals(p1->getSpecies(), p2->getSpecies()))
		{
			c->setSpecies(p1->getSpecies());
		}
		else
		{
			c->setSpecies(p1->getSpecies() + '_' + p2->getSpecies());
		}

		c->getParent1().setEntity(p1);
		c->getParent2().setEntity(p2);



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
		c->setMaxHealth(breedProperty(p1->getMaxHealth(), p2->getMaxHealth(), a, r));
		c->setHealth(c->getMaxHealth());
		c->setMass(breedProperty(p1->getMass(), p2->getMass(), a, r));
		c->setSize(breedProperty(p1->getSize(), p2->getSize(), a, r));
		c->setDamageBase(breedProperty(p1->getDamageBase(), p2->getDamageBase(), a, r));
		c->setAttackRate(breedProperty(p1->getAttackRate(), p2->getAttackRate(), a, r));

		c->setMutationAmount(breedProperty(p1->getMutationAmount(), p2->getMutationAmount(), a, r));
		c->setMutationRate(breedProperty(p1->getMutationRate(), p2->getMutationRate(), a, r));
	}

	return AnimalChildren(child1, child2);
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
	return value;
}