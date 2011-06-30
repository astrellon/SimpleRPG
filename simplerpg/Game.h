#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "curses.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "Map.h"
#include "Rect.h"
//#include "HUD.h"
#include "IKeyActions.h"

#include "UIList.h"
#include "UIText.h"

class GameEntity;
class Animal;

using namespace std;

typedef vector<GameEntity *> EntityList;

class Game// : public IKeyActions
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
	pair<int, int> getCursorPosition() { return pair<int, int>(mCursorX, mCursorY); }
	void setCursorPosition(int xPos, int yPos);
	void displayUnderCursor(UIContainer &hud);

	virtual void keyActions(const int key);
	virtual void displayActions(/*UIContainer &hud*/);
	/*virtual void setupDisplay(UIContainer &hud)
	{
		mHud.addChild(mHudText);
	}
	virtual void clearDisplay(UIContainer &hud)
	{
		mRedisplay = true;
		//hud.removeChild(mHud);
	}*/

	virtual EntityList getUnderCursor();

	void setDebugAnimal(Animal *animal);

protected:
	bool mRedisplay;
	void clearCanvas();

	Map *mMap;
	Rect mScreenSize;
	EntityList mEntities;
	EntityList mUnderCursor;
	bool mUnderCursorDirty;

	Animal *mDebugAnimal;

	IKeyActions *mSelectedItem;

	UIList mHud;
	UIText mHudText;

	int mCursorX;
	int mCursorY;
	bool mCursorMode;

	void switchKeyItem(IKeyActions *item, UIContainer &hud)
	{
		if(mSelectedItem != NULL)
		{
			mSelectedItem->clearDisplay(hud);
		}
		mHud.setScrollY(0);
		mSelectedItem = item;
		if(item != NULL)
		{
			item->setupDisplay(hud);
		}
		else
		{
			mHud.addChild(mHudText);
		}
	}
};
