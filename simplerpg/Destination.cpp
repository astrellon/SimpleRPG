#include "Destination.h"

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
	mDestination = -1.0f;
	mOldDestination = -1;
	mOldStart = -1;
	mPathDirty = false;
}

void Destination::setDestination(const Vector2f &location)
{
	setEntity(NULL);
	mDestination = location;
	mPathDirty = true;
}

void Destination::setDestination(const float &x, const float &y)
{
	setDestination(Vector2f(x, y));
}

Vector2f Destination::getDestination()
{
	if(getEntity() != NULL)
	{
		return getEntity()->getPosition();
	}
	return mDestination;
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
		mOldDestination.sub(getDestination()).lengthSqrd() >= 1 ||
		mOldStart.sub(startPosition).lengthSqrd() >= 1)
	{
		updatePath(startPosition);
	}
	mOldStart = startPosition;
	return mPath;
}

void Destination::updatePath(Vector2f &startPosition)
{
	Vector2f dest = getDestination();
	mOldDestination = dest;
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