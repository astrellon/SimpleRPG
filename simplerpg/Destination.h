#pragma once

#include <vector>
#include "Vector2.h"
#include "Game.h"
#include "GameEntity.h"

using std::vector;

class Destination
{
public:
	Destination();
	~Destination();

	void clear();

	void setGame(Game *game) { mGame = game; }
	Game *getGame() { return mGame; }

	void setDestination(const Vector2f &location);
	void setDestination(const float &x, const float &y);
	Vector2f getDestination();

	void setEntity(GameEntity *entity);
	GameEntity *getEntity();

	void setEntityId(const unsigned int &id);
	unsigned int getEntityId();

	vector<Vector2f> getPath(Vector2f &startPosition);

protected:
	bool mPathDirty;
	Vector2f mDestination;
	GameEntity *mEntity;
	unsigned int mEntityId;
	vector<Vector2f> mPath;
	Game *mGame;

	Vector2i mOldDestination;
	Vector2i mOldStart;

	void updatePath(Vector2f &startPosition);
};