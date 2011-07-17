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
	mHunger = 0.0f;

	mEnergyNeededPerDay = 0.0f;
	mEnergy = 0.0f;

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

	*mHudText << "Destination: " << mDestination.getDestination().x << ", " << mDestination.getDestination().y << '\n';
	GameEntity *target = mDestination.getEntity();
	*mHudText << "Target: " << (target == NULL ? "null" : target->getEntityName()) << '\n';
	*mHudText << "TargetId: " << mDestination.getEntityId() << '\n';
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
	if(iequals(propertyName, "destination"))
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
	else
	{
		GameEntity::loadProperties(iter);
	}
}

void Animal::saveProperties(ofstream &file)
{
	GameEntity::saveProperties(file);
	saveProperty(DESTINATION, file);
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
			file << "destination @ " << dest->getEntity()->getId() << endl;
		else
		{
			v = dest->getDestination();
			file << "destination " << v.x << ' ' << v.y << endl;
		}
		break;
	default:
		GameEntity::saveProperty(propertyId, file);
		break;
	}
}

void Animal::update(float dt)
{
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

	/*float energy = pow(getSize() * getMass(), 0.277f) * (getStrength() * getDexderity()) * 10.36 - 459.622;
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