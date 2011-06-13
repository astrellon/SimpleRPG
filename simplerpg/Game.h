#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <curses.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "Map.h"
#include "Rect.h"

class GameEntity;

using namespace std;

class Game
{
public:
	Game(void);
	~Game(void);

	Map *getMap() { return mMap; }
	void setMap(Map *map) { mMap = map; }

	void addEntity(GameEntity* entity) { mEntities.push_back(entity); }
	void removeEntity(GameEntity* entity);

	void update(float dt);
	void render(WINDOW *wnd);

	void moveCamera(int dx, int dy);

	void loadMap(string filename);
	void saveMap(string filename);

	void setCursor(int xPos, int yPos);

protected:
	void clearCanvas();

	Map *mMap;
	Rect mScreenSize;
	vector<GameEntity *> mEntities;

	int mCursorX;
	int mCursorY;

	bool compareStrings(const string &a, const string &b)
	{
		size_t len = a.length();
		if(b.length() != len)
			return false;

		for(size_t i = 0; i < len; i++)
		{
			if(tolower(a[i]) != tolower(b[i]))
			{
				return false;
			}
		}
		return true;
	}

	//HANDLE hConsole;
};
