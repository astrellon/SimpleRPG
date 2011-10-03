#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/lexical_cast.hpp>

#include "Map.h"
#include "Rect.h"
#include "TileData.h"
#include "IKeyActions.h"
#include "FormattedFile.h"
#include "FormattedFileIterator.h"

#include "ui/ui.hpp"

using std::vector;
using std::string;
using std::map;
using std::queue;

using boost::math::round;
using boost::algorithm::iequals;
using boost::lexical_cast;

class GameEntity;
class Animal;

enum MenuLevel { MENU_MAIN, MENU_LOOK, MENU_FIND, MENU_QUIT, MENU_OPTIONS, MENU_RAY, 
	MENU_NEAR, MENU_FOOD, MENU_MOVE, MENU_SELECTION };
enum GameOption { HUD_WIDTH, CURRENT_TIME, CURRENT_DAY, DAY_LENGTH, CAMERA_LOCATION };

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
	void setMap(Map *map)
	{
		mMap = map;
		if(map != NULL)
		{
			mCheckedTiles = new long*[map->getWidth()];
			for(int x = 0; x < map->getWidth(); x++)
			{
				mCheckedTiles[x] = new long[map->getHeight()];
				for(int y = 0; y < map->getHeight(); y++)
				{
					mCheckedTiles[x][y] = 0;
				}
			}
		}
	}

	TileData **getTileData() { return mTileData; }
	TileData *getTileData(const int &x, const int &y);
	TileData *getTileData(const Vector2i &position);

	void addEntity(GameEntity* entity);
	void removeEntity(GameEntity* entity);

	void update(float dt);
	void render(WINDOW *wnd);

	void moveCamera(const int &dx, const int &dy);
	void moveCamera(const Vector2i &diff);
	void setCamera(const int &x, const int &y);
	void setCamera(const Vector2i &pos);
	Vector2i getCamera() { return Vector2i(mScreenSize.getX(), mScreenSize.getY()); }

	void loadMap(string filename);
	void saveMap(string filename);

	bool getCursorMode() { return mCursorMode; }
	void setCursorMode(bool mode)
	{
		if(mode)
		{
			mUnderCursorDirty = true;
		}

		mCursorMode = mode;
		mSelectedItem = NULL;

		int screenX = mCursor.x - mScreenSize.getX();
		int screenY = mCursor.y - mScreenSize.getY();

		if (screenX < 2 || screenX > mScreenSize.getWidth() - 2||
			screenY < 2 || screenY > mScreenSize.getHeight() - 2)
		{
			screenX = mScreenSize.getX() + mScreenSize.getWidth() / 2;
			screenY = mScreenSize.getY() + mScreenSize.getHeight() / 2;
			setCursorPosition(screenX, screenY);
		}
	}
	Vector2i getCursorPosition() { return mCursor; }
	void setCursorPosition(int xPos, int yPos);
	void displayUnderCursor();

	virtual void keyActions(const int key);
	virtual void displayActions();

	virtual EntityList getUnderCursor();

	void setDebugAnimal(Animal *animal);

	FindEntityResult findClosestEntity(Vector2f startPosition, const string &entityType);
	FindEntityResult findClosestEntity(Vector2f startPosition, const string &entityType, const GameEntity *ignore);
	FindEntityResult findClosestEntity(Vector2f startPosition, const string &entityType, const GameEntity *ignore, const string &species);
	FindEntityResult findClosestEntity(Vector2f startPosition, const string &entityType, const GameEntity *ignore, const string *species);

	FindEntityResult findBreedingPartner(Animal *origin);

	vector<Vector2f> *findPath(Vector2i startPosition, Vector2i endPosition);

	Vector2i findClosestTileWithFood(Vector2i position);
	Vector2i findClosestTileWithFood(Animal *entity);

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

	virtual string getCurrentTimeString(bool includeDay = false);

	static Game *CURRENT_GAME;

	inline int  getTimeScale() { return mTimeScale; }
	inline void setTimeScale(int scale) 
	{ 
		if(scale < 1)
		{
			scale = 1;
		}
		mTimeScale = scale;
	}

	virtual RayResult fireRay(const Vector2f &point, const Vector2f &direction, const float &length);
	virtual RayResult fireRay(const Vector2f &point, const float &direction, const float &length);
	virtual void findNearby(Vector2f origin, const float &radius, vector<GameEntity *> &results, string &restrictToSpecies);
	virtual void findNearby(Vector2f origin, const float &radius, vector<GameEntity *> &results, string *restrictToSpecies);
	virtual void drawLine(WINDOW *wnd, const char &c, const Vector2f &point, const Vector2f &direction, const float &length);
	virtual void drawLine(WINDOW *wnd, const char &c, const Vector2f &point, const float &direction, const float &length);
	virtual void drawLine(WINDOW *wnd, const int &x1, const int &y1, const int &x2, const int &y2, const char &c);
	virtual void drawLine(WINDOW *wnd, float x1, float y1, float x2, float y2, const char &c);

	void loadNameList(string filename);

	string getRandomName();

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
	TileData **mTileData;
	int **mTileDataUpdate;

	int mTileDataStride;
	int *mStrideUpdateOrder;

	Rect mScreenSize;
	bool mUpdating;
	EntityList mEntities;
	EntityList mNewEntities;
	EntityList mRemoveEntities;
	EntityList mRemovedEntities;
	EntityList mUnderCursor;
	bool mUnderCursorDirty;

	GameEntity *mDebugEntity;
	bool mInstantMove;
	FindEntityResult mFoundEntity;
	IKeyActions *mSelectedItem;

	UIList mWholeHud;
	UIText mWholeHudText;

	UIList mHud;
	UIText mHudText;

	long **mCheckedTiles;
	long mCheckedTilesCounter;

	clock_t mTileTime;
	clock_t mEntityTime;
	float mTileTimeFinal;
	float mEntityTimeFinal;
	int mTimeCounter;

	MenuLevel mMenuLevel;
	Vector2i mCursor;
	int mCursorAngle;
	int mCursorLength;
	bool mCursorMode;
	int mLookFor;

	float mCurrentTime;
	int mCurrentDay;
	float mDayLength;

	int mTimeScale;

	Vector2i mDebugPosition;

	Rect mSelection;

	vector<string> mListOfNames;

	// Used to display the "(Saved)" text in the menu when the game has been saved.
	// Counts down to zero when the graphic will disappear.
	int mSaveCounter;

	bool compareString(const char *data, const string &str);

	void saveOptions(FormattedFile &file);
	void saveOption(const GameOption &option, FormattedFile &file);
	void loadOptions(string option, FormattedFileIterator &iter);

	void renderChar(WINDOW *wnd, const int &x, const int &y, const char &c);

	void switchKeyItem(IKeyActions *item, UIContainer &hud);

	void bresenhamLine(float x1, float y1, float x2, float y2, WINDOW *wnd, const char &c, RayResult *result);

	static const char *LOOK_FOR_TABLE[];

private:

	void checkAdjacentTile(const int &x, const int &y, queue<Vector2i> &openList);
};
