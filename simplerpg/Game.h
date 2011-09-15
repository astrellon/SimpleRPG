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

enum MenuLevel { MENU_MAIN, MENU_LOOK, MENU_FIND, MENU_QUIT, MENU_OPTIONS, MENU_RAY, MENU_NEAR };
enum GameOption { HUD_WIDTH, CURRENT_TIME, CURRENT_DAY, DAY_LENGTH, CAMERA_LOCATION };

typedef struct _MapData
{
	float foodValue;
} MapData;

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
	Tile *tile;

	_RayResult();
	_RayResult(const Vector2f &p, GameEntity *e);
	_RayResult(const Vector2f &p, Tile *t);
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

	MapData **getMapData() { return mMapData; }
	MapData *getMapData(int x, int y);

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

	virtual RayResult fireRay(const Vector2f &point, const Vector2f &direction, const float &length);
	virtual RayResult fireRay(const Vector2f &point, const float &direction, const float &length);
	virtual void findNearby(Vector2f origin, const float &radius, vector<GameEntity *> &results);
	virtual void drawLine(WINDOW *wnd, const char &c, const Vector2f &point, const Vector2f &direction, const float &length);
	virtual void drawLine(WINDOW *wnd, const char &c, const Vector2f &point, const float &direction, const float &length);
	virtual void drawLine(WINDOW *wnd, float x1, float y1, float x2, float y2, const char &c);

protected:
	bool mRedisplay;
	bool mGameRunning;
	bool mGamePaused;
	void clearCanvas();

	int mHudWidth;
	int mGameWidth;
	int mGameHeight;

	int mLastKey;

	string mMapDataFilename;

	Map *mMap;
	MapData **mMapData;

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
	int mCursorAngle;
	int mCursorLength;
	bool mCursorMode;
	int mLookFor;

	float mCurrentTime;
	int mCurrentDay;
	float mDayLength;

	// Used to display the "(Saved)" text in the menu when the game has been saved.
	// Counts down to zero when the graphic will disappear.
	int mSaveCounter;

	bool compareString(const char *data, const string &str);

	

	void saveOptions(FormattedFile &file);
	void saveOption(const GameOption &option, FormattedFile &file);
	void loadOptions(string option, FormattedFileIterator &iter);

	void renderChar(WINDOW *wnd, int x, int y, char c);

	void switchKeyItem(IKeyActions *item, UIContainer &hud);

	void bresenhamLine(float x1, float y1, float x2, float y2, WINDOW *wnd, const char &c, RayResult *result);

	static const char *LOOK_FOR_TABLE[];
};
