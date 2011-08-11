#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "curses.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/lexical_cast.hpp>

#include "Map.h"
#include "Rect.h"
#include "IKeyActions.h"
#include "FormattedFile.h"
#include "FormattedFileIterator.h"

#include "UIList.h"
#include "UIText.h"

using std::vector;
using std::string;
using std::map;

using boost::math::round;
using boost::algorithm::iequals;
using boost::lexical_cast;

class GameEntity;
class Animal;

enum MenuLevel { MENU_MAIN, MENU_LOOK, MENU_FIND, MENU_QUIT, MENU_OPTIONS };
enum GameOption { HUD_WIDTH };

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

typedef vector<GameEntity *> EntityList;

class Game
{
public:
	Game(int width, int height);
	~Game(void);

	void resize(int width, int height);
	void setHudWidth(int width);
	int getHudWidth() { return mHudWidth; }

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

protected:
	bool mRedisplay;
	bool mGameRunning;
	bool mGamePaused;
	void clearCanvas();

	int mHudWidth;
	int mGameWidth;
	int mGameHeight;

	int mLastKey;

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

	MenuLevel mMenuLevel;
	int mCursorX;
	int mCursorY;
	bool mCursorMode;
	int mLookFor;

	// Used to display the "(Saved)" text in the menu when the game has been saved.
	// Counts down to zero when the graphic will disappear.
	int mSaveCounter;

	void saveOptions(FormattedFile &file);
	void saveOption(const GameOption &option, FormattedFile &file);
	void loadOptions(string option, FormattedFileIterator &iter);

	void switchKeyItem(IKeyActions *item, UIContainer &hud);

	static const char *LOOK_FOR_TABLE[];
};
