#include "GameEntity.h"
#include "Game.h"

GameEntity::GameEntity(Game *game)
{
	mGame = game;
	mGraphic = Pixel('o', 7, false);
	mSpeed = 1.1f;
	mTurningSpeed = 2.0f;
	mState = STATE_IDLE;
	mPath = NULL;
	mDestination = Vector2(-1, -1);
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::move(MathType dx, MathType dy, bool inObjectSpace)
{
	Vector2 v(dx, dy);
	if(inObjectSpace)
		mTransform.transformVectorConst(&v);

	int newPosX = math::round(mTransform.zx + v.x);
	int newPosY = math::round(mTransform.zy + v.y);

	Tile *tile = mGame->getMap()->getTile(newPosX, newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(v);
		return;
	}

	tile = mGame->getMap()->getTile(math::round(mTransform.zx), newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(0, v.y);
		return;
	}

	tile = mGame->getMap()->getTile(newPosX, math::round(mTransform.zy));
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(v.x, 0);
		return;
	}
}

void GameEntity::move(Vector2 vec, bool inObjectSpace)
{
	move(vec.x, vec.y, inObjectSpace);
}

void GameEntity::turn(MathType angle)
{
	mTransform.rotate(angle);
}

void GameEntity::update(float dt)
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

void GameEntity::doStateMoving(float dt)
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
		MathType distTime = dist / mSpeed;

		if (distTime > dt - timeTaken)
		{
			distTime = dt - timeTaken;
		}

		MathType travelDist = distTime * mSpeed;
		timeTaken += distTime;

		move(0, travelDist);

		pos = getPosition();
	}
}

void GameEntity::setDestination(const MathType &xPos, const MathType &yPos)
{
	setDestination(Vector2(xPos, yPos));
}

void GameEntity::setDestination(const Vector2 &dest)
{
	mDestination = dest;
	mDestination.x = math::round(mDestination.x);
	mDestination.y = math::round(mDestination.y);
	updateMovePath();
}

void GameEntity::updateMovePath()
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

void GameEntity::render(Rect screenSize, WINDOW *wnd)
{
	int xPos = math::round(getX()) - screenSize.getX();
	int yPos = math::round(getY()) - screenSize.getY();

	if (xPos < 0 || xPos >= screenSize.getWidth() ||
		yPos < 0 || yPos >= screenSize.getHeight())
	{
		return;
	}

	getGraphic().render(wnd, xPos, yPos);
}

void GameEntity::loadFromFile(boost::sregex_token_iterator &iter)
{
	boost::sregex_token_iterator end;

	while(iter != end)
	{
		string line = *iter;
		if(iequals(line, "end"))
		{
			break;
		}
		loadProperties(iter);
	}
}

void GameEntity::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter++;
	if(iequals(propertyName, "facing"))
	{
		float f = atof(string(*iter++).c_str());
		setFacing(f);
	}
	else if(iequals(propertyName, "position"))
	{
		float x = atof(string(*iter++).c_str());
		float y = atof(string(*iter++).c_str());
		move(x, y, false);
	}
	else if(iequals(propertyName, "destination"))
	{
		float x = atof(string(*iter++).c_str());
		float y = atof(string(*iter++).c_str());
		setDestination(x, y);
	}
	else
	{
		cout << "Unable to load unknown property '" << propertyName << "'" << endl;
	}
}

void GameEntity::saveToFile(ofstream &file)
{
	file << getEntityName() << endl;

	saveProperties(file);

	file << "end" << endl << endl;
}

void GameEntity::saveProperties(ofstream &file)
{
	saveProperty(PROPERTY_FACING, file);
	saveProperty(PROPERTY_POSITION, file);
	saveProperty(PROPERTY_DESTINATION, file);
}

void GameEntity::saveProperty(const int &propertyId, ofstream &file)
{
	Vector2 v;
	switch(propertyId)
	{
	case PROPERTY_FACING:
		file << "facing " << getFacing() << endl;
		break;
	case PROPERTY_POSITION:
		v = getPosition();
		file << "position " << v.x << ' ' << v.y << endl;
		break;
	case PROPERTY_DESTINATION:
		v = getDestination();
		file << "destination " << v.x << ' ' << v.y << endl;
		break;
	default:
		cout << "Unable to save unknown property " << propertyId << endl;
		break;
	}
}