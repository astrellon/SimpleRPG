#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "curses.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/math/special_functions/round.hpp>
using boost::math::round;

#include "Map.h"
#include "Rect.h"
#include "IKeyActions.h"

#include "UIList.h"
#include "UIText.h"

class GameEntity;
class Animal;

typedef struct _FindEntityResult
{
	GameEntity *entity;
	vector<Vector2f> *path;

	_FindEntityResult()
	{
		entity = NULL;
		path = NULL;
	}
	_FindEntityResult(GameEntity *e, vector<Vector2f> *p)
	{
		entity = e;
		path = p;
	}
	void clear()
	{
		entity = NULL;
		if(path != NULL)
		{
			delete path;
			path = NULL;
		}
	}
} FindEntityResult;

using std::vector;
using std::string;

typedef vector<GameEntity *> EntityList;

class Game
{
public:
	Game(void);
	~Game(void);

	Map *getMap() { return mMap; }
	void setMap(Map *map) { mMap = map; }

	void addEntity(GameEntity* entity);
	void removeEntity(GameEntity* entity);

	void update(float dt);
	void render(WINDOW *wnd);

	void moveCamera(int dx, int dy);

	void loadMap(string filename);
	void saveMap(string filename);

	bool getCursorMode() { return mCursorMode; }
	void setCursorMode(bool mode)
	{
		if(mode)
			mUnderCursorDirty = true;
		mCursorMode = mode;
		mSelectedItem = NULL;
	}
	Vector2i getCursorPosition() { return Vector2i(mCursorX, mCursorY); }
	void setCursorPosition(int xPos, int yPos);
	void displayUnderCursor(UIContainer &hud);

	virtual void keyActions(const int key);
	virtual void displayActions();

	virtual EntityList getUnderCursor();

	void setDebugAnimal(Animal *animal);

	FindEntityResult findClosestEntity(Vector2f startPosition, string entityType);
	vector<Vector2f> *findPath(Vector2i startPosition, Vector2i endPosition);

	virtual bool getGameRunning() { return mGameRunning; }
	virtual bool getGamePaused() { return mGamePaused; }
	virtual void setGamePaused(bool paused) { mGamePaused = paused; }

	static void changeOutputFileLevel(int diff)
	{
		setOutputFileLevel(getOutputFileLevel() + diff);
		
	}
	static void setOutputFileLevel(int level)
	{
		sOutputFileLevel = level;
		if (sOutputFileLevel < 0)
			sOutputFileLevel = 0;
	}
	static int getOutputFileLevel() { return sOutputFileLevel; }

	static string getOutputTabs()
	{
		if(sOldOutputFileLevel != sOutputFileLevel)
		{
			sOutputTabString = "";
			sOutputTabString.reserve(sOutputFileLevel);
			for(int i = 0; i < sOutputFileLevel; i++)
			{
				sOutputTabString += '\t';
			}
			sOldOutputFileLevel = sOutputFileLevel;
		}
		return sOutputTabString;
	}

protected:
	bool mRedisplay;
	bool mGameRunning;
	bool mGamePaused;
	void clearCanvas();

	Map *mMap;
	Rect mScreenSize;
	EntityList mEntities;
	EntityList mUnderCursor;
	bool mUnderCursorDirty;

	Animal *mDebugAnimal;
	FindEntityResult mFoundEntity;
	IKeyActions *mSelectedItem;

	UIList mHud;
	UIText mHudText;

	int mMenuLevel;
	int mCursorX;
	int mCursorY;
	bool mCursorMode;
	int mLookFor;

	int mSaveCounter;

	void switchKeyItem(IKeyActions *item, UIContainer &hud);

	static const char *LOOK_FOR_TABLE[];
	static int sOutputFileLevel;
	static int sOldOutputFileLevel;
	static string sOutputTabString;
	
};
