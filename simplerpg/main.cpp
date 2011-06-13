#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <conio.h>
#endif

#include "Map.h"
#include "Tile.h"
#include "Game.h"
#include "Animal.h"
#include "Vector2.h"
#include "Matrix3x3.h"
#include "HUD.h"
#include "SideMenu.h"
#include "keyboard.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

extern "C"
{
	#include <curses.h>
}

#ifdef _WIN32
#include <Windows.h>
inline void msleep(int ms)
{
	Sleep(ms);
}
#else
#include <unistd.h>
inline void msleep(int ms)
{
	usleep(ms * 1000);
}
#endif

using namespace std;

void init_colours()
{
	start_color();
	for(int i = 0; i < 8; i++)
	{
		init_pair(i + 1, i, COLOR_BLACK);
	}
	init_pair(9, 0, 8);
}

WINDOW *wnd;
WINDOW *hud;

IKeyActions *currentItem = NULL;

void switchKeyItem(IKeyActions *item)
{
	if(currentItem != NULL)
	{
		currentItem->clearDisplay();
	}
	currentItem = item;
}

int main()
{
	srand( (unsigned int)time(NULL) );

	Tile::registerDefaults();
	
	Game *game = new Game();
	game->loadMap("map.txt");

	if(game->getMap() == NULL)
	{
		cout << "Unable to load map!" << endl << "Exiting program, press enter." << endl;
		cin.get();
		return 0;
	}

	game->saveMap("test.out");

	initscr();
	curs_set(0);
	init_colours();

	HUD hud;

	wnd = newwin(25, 60, 0, 0);
	cbreak();
	noecho();
	keypad(wnd, true);

	SideMenu menu;

	bool paused = false;
	char buff[20];

	bool cursorMode = false;
	int cx = -1;
	int cy = -1;

	while(true)
	{
		while(kbhit())
		{
			int c = wgetch(wnd);

			if(c == 'k')
			{
				cursorMode = !cursorMode;
				if(!cursorMode)
				{
					game->setCursor(-1, -1);
				}
			}

			if(cursorMode)
			{
				if (c == 260)
					cx--;
				if (c == 261)
					cx++;
				if (c == 259)
					cy--;
				if (c == 258)
					cy++;

				game->setCursor(cx, cy);

				hud.clear();
				Tile *t = game->getMap()->getTile(cx, cy);
				if(t != NULL)
				{
					hud.clear();
					hud << "Tile: " << t->getName();
				}
			}
			else
			{
				if (c == 260)
					game->moveCamera(-1, 0);
				if (c == 261)
					game->moveCamera(1, 0);
				if (c == 259)
					game->moveCamera(0, -1);
				if (c == 258)
					game->moveCamera(0, 1);
			}

			if (c == 56)
				hud.scrollConsole(1);
			if (c == 50)
				hud.scrollConsole(-1);

			if(c == 'p' || c == ' ')
				paused = !paused;

			if(c == 's')
			{
				game->saveMap("test.out");
				hud << "Saved!";
			}
		}

		if(currentItem != NULL)
		{
			currentItem->keyActions();
			currentItem->displayActions(hud);
		}

		if(!paused)
		{
			game->update(0.04f);
		}
		game->render(wnd);

		hud.render();

		if(paused)
		{
			wattron(wnd, A_BOLD);
			wattron(wnd, COLOR_PAIR(5));
			mvwaddstr(wnd, 0, 0, "*PAUSED*");
			wrefresh(wnd);
		}

		msleep(40);
	}

	delwin(wnd);
	endwin();

	return 0;
}
