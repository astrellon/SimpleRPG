#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include <Windows.h>
#include <curses.h>

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

	void update();
	void render(WINDOW *wnd);

	void moveCamera(int dx, int dy);

	void loadMap(char *filename);

protected:
	void clearCanvas();

	Map *mMap;

	Rect mScreenSize;

	//Pixel **mCanvas;
	vector<GameEntity *> mEntities;

	//HANDLE hConsole;
};

