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

	mEnergyNeededPerDay = 0.0f;
	mEnergy = 0.0f;

	mMass = 1.0f;
	mSize = 1.0f;
	mDiet = 0.5f;
	mDamageBase = 1.0f;

	mStrength = 1;
	mDexterity = 1;
	mIntelligence = 1;
	
	mDestination.setGame(game);
}

Animal::~Animal(void)
{
}

void Animal::displayActions(UIContainer &hud)
{
	if(!mRedisplay)
		return;

	GameEntity::displayActions(hud);

	static char buff[128];
	sprintf(buff, "%.1f, %.1f\n", mDestination.getDestination().x, mDestination.getDestination().y);

	*mHudText << "<15>Dest</>: " << buff;
	GameEntity *target = mDestination.getEntity();
	if(mDestination.getEntityId() != -1)
	{
		*mHudText << "<15>Target</>: " << (target == NULL ? "null" : target->getEntityName()) << '\n';
		*mHudText << "<15>TargetId</>: " << mDestination.getEntityId() << '\n';
	}
	
	sprintf(buff, "<12>%.1f/%.1f</>\n", getHealth(), getMaxHealth());
	*mHudText << "<15>Health</>: <12>" << buff;
	if(isDead())
	{
		*mHudText << "<7>(Dead)</>\n";
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

void Animal::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, EntityPropertyNames[DESTINATION]))
	{
		iter++;
		string typeCheck = *iter++;
		if(typeCheck[0] == '@')
		{
			unsigned int following = lexical_cast<unsigned int>(*iter++);
			mDestination.setEntityId(following);
		}
		else
		{
			float x = lexical_cast<float>(typeCheck);
			float y = lexical_cast<float>(*iter++);
			mDestination.setDestination(x, y);
		}
	}
	else if(iequals(propertyName, EntityPropertyNames[HEALTH]))
	{
		iter++;
		mHealth = lexical_cast<float>(*iter++);
		if(mHealth < 0.0f)
		{
			mHealth = 0.0f;
		}
		mMaxHealth = lexical_cast<float>(*iter++);
	}
	else if(iequals(propertyName, EntityPropertyNames[STRENGTH]))
	{
		iter++;
		setStrength(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[DEXTERITY]))
	{
		iter++;
		setDexterity(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[INTELLIGENCE]))
	{
		iter++;
		setIntelligence(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[RUNNING_SPEED]))
	{
		iter++;
		setRunningSpeedBase(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[WALKING_SPEED]))
	{
		iter++;
		setWalkingSpeedBase(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[TURNING_SPEED]))
	{
		iter++;
		setTurningSpeedBase(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[ENTITY_SIZE]))
	{
		iter++;
		setSize(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[ENTITY_MASS]))
	{
		iter++;
		setMass(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[DIET]))
	{
		iter++;
		setDiet(lexical_cast<float>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[DAMAGE_BASE]))
	{
		iter++;
		setDamageBase(lexical_cast<float>(*iter++));
	}
	else
	{
		GameEntity::loadProperties(iter);
	}
}

void Animal::saveProperties(ofstream &file)
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
}

void Animal::saveProperty(const EntityProperty &propertyId, ofstream &file)
{
	Vector2f v;
	Destination *dest = NULL;
	switch(propertyId)
	{
	case DESTINATION:
		dest = getDestination();
		if(dest->getEntity() != NULL)
			file << Game::getOutputTabs() << EntityPropertyNames[DESTINATION] << " @ " << dest->getEntity()->getId() << endl;
		else
		{
			v = dest->getDestination();
			file << Game::getOutputTabs() << EntityPropertyNames[DESTINATION] << ' ' << v.x << ' ' << v.y << endl;
		}
		break;
	case HEALTH:
		file << Game::getOutputTabs() << EntityPropertyNames[HEALTH] << ' ' << getHealth() << ' ' << getMaxHealth() << endl;
		break;
	case STRENGTH:
		file << Game::getOutputTabs() << EntityPropertyNames[STRENGTH] << ' ' << getStrength() << endl;
		break;
	case DEXTERITY:
		file << Game::getOutputTabs() << EntityPropertyNames[DEXTERITY] << ' ' << getDexterity() << endl;
		break;
	case INTELLIGENCE:
		file << Game::getOutputTabs() << EntityPropertyNames[INTELLIGENCE] << ' ' << getIntelligence() << endl;
		break;
	case RUNNING_SPEED:
		file << Game::getOutputTabs() << EntityPropertyNames[RUNNING_SPEED] << ' ' << getRunningSpeedBase() << endl;
		break;
	case WALKING_SPEED:
		file << Game::getOutputTabs() << EntityPropertyNames[WALKING_SPEED] << ' ' << getWalkingSpeedBase() << endl;
		break;
	case TURNING_SPEED:
		file << Game::getOutputTabs() << EntityPropertyNames[TURNING_SPEED] << ' ' << getTurningSpeedBase() << endl;
		break;
	case ENTITY_SIZE:
		file << Game::getOutputTabs() << EntityPropertyNames[ENTITY_SIZE] << ' ' << getSize() << endl;
		break;
	case ENTITY_MASS:
		file << Game::getOutputTabs() << EntityPropertyNames[ENTITY_MASS] << ' ' << getMass() << endl;
		break;
	case DAMAGE_BASE:
		file << Game::getOutputTabs() << EntityPropertyNames[DAMAGE_BASE] << ' ' << getDamageBase() << endl;
		break;
	case DIET:
		file << Game::getOutputTabs() << EntityPropertyNames[DIET] << ' ' << getDiet() << endl;
		break;
	default:
		GameEntity::saveProperty(propertyId, file);
		break;
	}
}

void Animal::update(float dt)
{
	Action *action = getCurrentAction();
	
	switch(action->getAction())
	{
	default:
	case IDLE:
		// Do nothing!
	case EAT:
	case FLEE:
	case ATTACK:
		// Do some more nothing.
		action->getCompleted();
	}
	
	doStateMoving(dt);
}

void Animal::doStateMoving(float dt)
{
	Vector2f pos = getPosition();
	vector<Vector2f> path = mDestination.getPath(pos);
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