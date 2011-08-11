#pragma once

#include <vector>
#include <string>

#include "Vector2.h"
#include "Game.h"

#include "FormattedFileIterator.h"
#include "FormattedFile.h"

#include <boost/lexical_cast.hpp>

using std::vector;
using std::string;

using boost::lexical_cast;

class GameEntity;

class Destination
{
public:
	Destination();
	~Destination();

	void clear();

	void setGame(Game *game) { mGame = game; }
	Game *getGame() { return mGame; }

	void setLocation(const Vector2f &location);
	void setLocation(const float &x, const float &y);
	Vector2f getLocation();

	void setEntity(GameEntity *entity);
	GameEntity *getEntity();

	void setEntityId(const unsigned int &id);
	unsigned int getEntityId();

	vector<Vector2f> getPath(Vector2f &startPosition);

	void saveDestination(string propertyName, FormattedFile &file);
	void loadDestination(FormattedFileIterator &iter);

	void clearPath();

protected:
	bool mPathDirty;
	Vector2f mLocation;
	GameEntity *mEntity;
	unsigned int mEntityId;
	vector<Vector2f> mPath;
	Game *mGame;

	Vector2i mOldLocation;
	Vector2i mOldStart;

	void updatePath(Vector2f &startPosition);
};
