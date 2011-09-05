#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/lexical_cast.hpp>

#include "Map.h"
#include "Rect.h"
#include "IKeyActions.h"
#include "FormattedFile.h"
#include "FormattedFileIterator.h"

#include "ui/ui.hpp"

using std::vector;
using std::string;
using std::map;

using boost::math::round;
using boost::algorithm::iequals;
using boost::lexical_cast;

class GameEntity;
class Animal;

enum MenuLevel { MENU_MAIN, MENU_LOOK, MENU_FIND, MENU_QUIT, MENU_OPTIONS };
enum GameOption { HUD_WIDTH, CURRENT_TIME, CURRENT_DAY, DAY_LENGTH };

const char *GamePropertyNames[];

typedef struct _FindEntityResult
{
	GameEntity *entity;
	vector<Vector2f> *path;

	_FindEntityResult();
	_FindEntityResult(GameEntity *e, vector<Vector2f> *p);
	void clear();
} FindEntityResult;

typedef struct _RayResult
{
	GameEntity *entity;
	Vector2f point;

	_RayResult();
	_RayResult(GameEntity *e);
	_RayResult(const Vector2f &p);
} RayResult;

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
	Vector2i getCursorPosition() { return mCursor; }
	void setCursorPosition(int xPos, int yPos);
	void displayUnderCursor(UIContainer &hud);

	virtual void keyActions(const int key);
	virtual void displayActions();

	virtual EntityList getUnderCursor();

	void setDebugAnimal(Animal *animal);

	FindEntityResult findClosestEntity(Vector2f startPosition, const string &entityType);
	FindEntityResult findClosestEntity(Vector2f startPosition, const string &entityType, const GameEntity *ignore);
	vector<Vector2f> *findPath(Vector2i startPosition, Vector2i endPosition);

	virtual bool getGameRunning() { return mGameRunning; }
	virtual bool getGamePaused() { return mGamePaused; }
	virtual void setGamePaused(bool paused) { mGamePaused = paused; }

	virtual int  getCurrentDay() { return mCurrentDay; }
	virtual void setCurrentDay(int day) { mCurrentDay = day;}
	virtual void advanceDay(int days) { mCurrentDay += days; }

	virtual float getCurrentTime() { return mCurrentTime; }
	virtual void  setCurrentTime(float time); 
	virtual void  advanceTime(float dt);

	virtual float getDayLength() { return mDayLength; }
	virtual void  setDayLength(float length) { mDayLength = length; }

	virtual string getCurrentTimeString();

	static Game *CURRENT_GAME;

	virtual RayResult fireRay(const Vector2f &point, const Vector2f &direction);

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

	UIList mWholeHud;
	UIText mWholeHudText;

	UIList mHud;
	UIText mHudText;

	MenuLevel mMenuLevel;
	Vector2i mCursor;
	bool mCursorMode;
	int mLookFor;

	float mCurrentTime;
	int mCurrentDay;
	float mDayLength;

	// Used to display the "(Saved)" text in the menu when the game has been saved.
	// Counts down to zero when the graphic will disappear.
	int mSaveCounter;

	void saveOptions(FormattedFile &file);
	void saveOption(const GameOption &option, FormattedFile &file);
	void loadOptions(string option, FormattedFileIterator &iter);

	void switchKeyItem(IKeyActions *item, UIContainer &hud);

	static const char *LOOK_FOR_TABLE[];
};
