#include "Animal.h"
#include "Game.h"

Animal::Animal(Game *game) : GameEntity(game)
{
	srand( (unsigned int)time(NULL) );

	mWalking = true;
	mWalkingSpeed = 1.1f;
	mRunningSpeed = 10.0f;
	mTurningSpeed = 2.0f;
	
	mState = STATE_IDLE;
	mPath = NULL;
	mDestination = Vector2f(-1, -1);

	mName = "Animal";
}

Animal::~Animal(void)
{
}

void Animal::displayActions(UIContainer &hud)
{
	if(!mRedisplay)
		return;

	GameEntity::displayActions(hud);

	*mHudText << "Destination: " << mDestination.x << ", " << mDestination.y << '\n';
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
	if(iequals(propertyName, "facing"))
	{
		iter++;
		float f = lexical_cast<float>(*iter++);
		setFacing(f);
	}
	else if(iequals(propertyName, "destination"))
	{
		iter++;
		float x = lexical_cast<float>(*iter++);
		float y = lexical_cast<float>(*iter++);
		setDestination(x, y);
	}
	else
	{
		GameEntity::loadProperties(iter);
	}
}

void Animal::saveProperties(ofstream &file)
{
	GameEntity::saveProperties(file);
	saveProperty(PROPERTY_FACING, file);
	saveProperty(PROPERTY_DESTINATION, file);
}

void Animal::saveProperty(const int &propertyId, ofstream &file)
{
	Vector2f v;
	switch(propertyId)
	{
	case PROPERTY_FACING:
		file << "facing " << getFacing() << endl;
		break;
	case PROPERTY_DESTINATION:
		v = getDestination();
		file << "destination " << v.x << ' ' << v.y << endl;
		break;
	default:
		GameEntity::saveProperty(propertyId, file);
		break;
	}
}

void Animal::update(float dt)
{
	switch(mState)
	{
	default:
	case STATE_IDLE:
		doStateIdle(dt);
		break;
	case STATE_MOVING:
		doStateMoving(dt);
		break;
	case STATE_HUNGRY:
		doStateHungry(dt);
		break;
	}
}

void Animal::doStateMoving(float dt)
{
	if(mPath == NULL || mPath->empty())
	{
		mState = STATE_IDLE;
		return;
	}

	Vector2f pos = getPosition();
	float facing = (float)getFacing();
	float timeTaken = 0.0f;
	while(timeTaken < dt)
	{
		Vector2f nextPos = (*mPath)[0];
		Vector2f toNextPos = nextPos.sub(pos);
		if(toNextPos.length() < 0.4f)
		{
			mPath->erase(mPath->begin());
			if(mPath->empty())
			{
				mState = STATE_IDLE;
				break;
			}
			continue;
		}
		
		float toFacing = atan2(toNextPos.y, toNextPos.x) - M_PIF / 2.0f;

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

void Animal::setDestination(const float &xPos, const float &yPos)
{
	setDestination(Vector2f(xPos, yPos));
}

void Animal::setDestination(const Vector2f &dest)
{
	mDestination = dest;
	mDestination.x = (float)math::round(mDestination.x);
	mDestination.y = (float)math::round(mDestination.y);
	updateMovePath();
}

void Animal::updateMovePath()
{
	if(mDestination.x < 0 || mDestination.y < 0)
	{
		if(mPath != NULL)
		{
			delete mPath;
			mPath = NULL;
		}
		return;
	}
	if(mGame != NULL && mGame->getMap() != NULL)
	{
		if(mPath != NULL)
		{
			delete mPath;
			mPath = NULL;
		}
		Vector2f pos = getPosition();
		pos.x = (float)math::round(pos.x);
		pos.y = (float)math::round(pos.y);
		mPath = mGame->getMap()->search(pos, mDestination);
		mState = STATE_MOVING;
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