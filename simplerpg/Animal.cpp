#include "Animal.h"


Animal::Animal(Game *game) : GameEntity(game)
{
	srand( (unsigned int)time(NULL) );

	mWalking = true;
	mWalkingSpeed = 1.1f;
	mRunningSpeed = 10.0f;
	mTurningSpeed = 2.0f;
	
	mState = STATE_IDLE;
	mPath = NULL;
	mDestination = Vector2(-1, -1);

	mName = "Animal";
}

Animal::~Animal(void)
{
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

void Animal::update(float dt)
{
	switch(mState)
	{
	case STATE_IDLE:
		doStateIdle(dt);
		break;
	case STATE_MOVING:
		doStateMoving(dt);
		break;
	default:
		doStateIdle(dt);
	}
}

void Animal::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, "facing"))
	{
		iter++;
		float f = atof(string(*iter++).c_str());
		setFacing(f);
	}
	else if(iequals(propertyName, "destination"))
	{
		iter++;
		float x = atof(string(*iter++).c_str());
		float y = atof(string(*iter++).c_str());
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
	Vector2 v;
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

void Animal::doStateMoving(float dt)
{
	if(mPath == NULL || mPath->empty())
	{
		mState = STATE_IDLE;
		return;
	}

	Vector2 pos = getPosition();
	MathType facing = getFacing();
	float timeTaken = 0.0f;
	while(timeTaken < dt)
	{
		Vector2 nextPos = (*mPath)[0];
		Vector2 toNextPos = nextPos.sub(pos);
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
		
		MathType toFacing = atan2(toNextPos.y, toNextPos.x) - M_PI / 2.0f;

		MathType facingDiff = getTurnAmount(facing, toFacing);
		MathType maxFacingChange = dt * mTurningSpeed;
		if (facingDiff > maxFacingChange)
			facingDiff = maxFacingChange;
		if (facingDiff < -maxFacingChange)
			facingDiff = -maxFacingChange;

		turn(facingDiff);

		facing = getFacing();

		MathType turnLeft = getTurnAmount(facing, toFacing);

		if (turnLeft > 0.01f || turnLeft < -0.01f)
		{
			break;
		}

		MathType dist = toNextPos.length();
		MathType distTime = dist / getCurrentSpeed();

		if (distTime > dt - timeTaken)
		{
			distTime = dt - timeTaken;
		}

		MathType travelDist = distTime * getCurrentSpeed();
		timeTaken += distTime;

		move(0, travelDist);

		pos = getPosition();
	}
}

void Animal::setDestination(const MathType &xPos, const MathType &yPos)
{
	setDestination(Vector2(xPos, yPos));
}

void Animal::setDestination(const Vector2 &dest)
{
	mDestination = dest;
	mDestination.x = math::round(mDestination.x);
	mDestination.y = math::round(mDestination.y);
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
		Vector2 pos = getPosition();
		pos.x = (MathType)math::round(pos.x);
		pos.y = (MathType)math::round(pos.y);
		mPath = mGame->getMap()->search(pos, mDestination);
		mState = STATE_MOVING;
	}
}

float Animal::getTurnAmount(float facing, float dest)
{
	float diff = dest - facing;
	if(diff < 0.0f)
	{
		if(diff < -M_PI)
		{
			diff += M_PI * 2.0f;
		}
	}
	else
	{
		if(diff > M_PI)
		{
			diff -= M_PI * 2.0f;
		}
	}
	return diff;
}