#include "Destination.h"
#include "GameEntity.h"

Destination::Destination()
{
	mGame = NULL;
	clear();
}

Destination::~Destination()
{
	clear();
}

void Destination::clear()
{
	mPath.clear();
	mEntity = NULL;
	mEntityId = -1;
	mLocation = -1.0f;
	mOldLocation = -1;
	mOldStart = -1;
	mPathDirty = false;
}

void Destination::setLocation(const Vector2f &location)
{
	setEntity(NULL);
	mLocation = location;
	mPathDirty = true;
}

void Destination::setLocation(const float &x, const float &y)
{
	setLocation(Vector2f(x, y));
}

void Destination::saveDestination(string propertyName, FormattedFile &file)
{
	if(getEntity() != NULL)
	{
		file << propertyName << " @ " << getEntity()->getId() << '\n';
	}
	else
	{
		Vector2f v = getLocation();
		file << propertyName << ' ' << v.x << ' ' << v.y << '\n';
	}
}

void Destination::loadDestination(FormattedFileIterator &iter)
{
	string typeCheck = *iter; ++iter;
	if(typeCheck[0] == '@')
	{
		unsigned int following = lexical_cast<unsigned int>(*iter); ++iter;
		setEntityId(following);
	}
	else
	{
		float x = lexical_cast<float>(typeCheck);
		float y = lexical_cast<float>(*iter); ++iter;
		setLocation(x, y);
	}
}

Vector2f Destination::getLocation()
{
	if(getEntity() != NULL)
	{
		return getEntity()->getPosition();
	}
	return mLocation;
}

void Destination::setEntity(GameEntity *entity)
{
	if(entity != mEntity)
	{
		mEntity = entity;
		if(entity != NULL)
			mEntityId = entity->getId();

		mPathDirty = true;
	}
}

GameEntity *Destination::getEntity()
{
	if((mEntity == NULL && mEntityId != -1) || (mEntity != NULL && mEntity->getId() != mEntityId))
	{
		setEntity(GameEntity::getEntityById(mEntityId));
	}

	return mEntity;
}

void Destination::setEntityId(const unsigned int &id)
{
	if(mEntity != NULL && mEntity->getId() == id)
		return;
	else if(mEntityId == id)
		return;

	mEntity = NULL;
	mEntityId = id;
	mPathDirty = true;
}
unsigned int Destination::getEntityId()
{
	return mEntityId;
}

vector<Vector2f> Destination::getPath(Vector2f &startPosition)
{
	if(mPathDirty || 
		mOldLocation.sub(getLocation()).lengthSqrd() >= 1 ||
		mOldStart.sub(startPosition).lengthSqrd() >= 1)
	{
		updatePath(startPosition);
	}
	mOldStart = startPosition;
	return mPath;
}

void Destination::updatePath(Vector2f &startPosition)
{
	Vector2f dest = getLocation();
	mOldLocation = dest;
	mPathDirty = false;
	if(dest.x < 0 || dest.y < 0)
	{
		mPath.clear();
		return;
	}
	if(mGame != NULL && mGame->getMap() != NULL)
	{
		mPath.clear();
		vector<Vector2f> *p = mGame->getMap()->search(startPosition, dest);
		if(p != NULL)
		{
			mPath = *p;
			delete p;
		}
	}
}

void Destination::clearPath()
{
	if(!mPath.empty())
	{
		mPath.clear();
		mPathDirty = true;
	}
}