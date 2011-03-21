#include "Game.h"
#include "GameEntity.h"

Game::Game(void)
{
	mScreenSize = Rect(0, 0, 80, 25);
	mMap = NULL;
}

Game::~Game(void)
{
	
}

void Game::removeEntity(GameEntity* entity)
{
	for(vector<GameEntity*>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		if((*iter) == entity)
		{
			mEntities.erase(iter);
			break;
		}
	}
}

void Game::update()
{
	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		(*iter)->update();
	}
}

void Game::render(WINDOW *wnd)
{
	wclear(wnd);
	
	mMap->renderMap(mScreenSize, wnd);

	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		(*iter)->render(mScreenSize, wnd);
	}

	wrefresh(wnd);
}

void Game::moveCamera(int dx, int dy)
{
	mScreenSize.setX(mScreenSize.getX() + dx);
	mScreenSize.setY(mScreenSize.getY() + dy);
}

void Game::loadMap(char *filename)
{
	ifstream file(filename);
	if(!file.is_open())
	{
		cout << "Failed to open file " << filename << endl;
		return;
	}

	string line;
	vector<string> mapData;
	int width = 0;

	map<char, Tile *> tileMap;

	Map *map;

	int state = 0;
	char c;

	while(file.good())
	{
		getline(file, line);
		if(line[line.length() - 1] == '\r')
		{
			line.erase(line.end() - 1);
		}
		switch(state)
		{
		case 0:
			if(line.length() == 0)
				break;
			else
				state++;
		case 1:
			if(line.size() == 0)
			{
				state++;
				break;
			}

			// Get types.
			c = line[0];
			tileMap[c] = Tile::getTile( atoi(line.c_str() + 2));

			break;
		case 2:
			if(line.length() == 0)
			{
				state++;
				map = new Map(width, mapData.size());
				for(unsigned int y = 0; y < mapData.size(); y++)
				{
					string l = mapData[y];
					for(unsigned int x = 0; x < l.size(); x++)
					{
						map->setTile(x, y, tileMap[l[x]]);
					}
				}

				setMap(map);
				break;
			}
			if(width == 0)
			{
				width = line.length();
			}
			else if(line.length() != width)
			{
				cout << "ERROR With map data, needs to be square!." << endl;
				state = -1;
				break;
			}
			mapData.push_back(line);
			break;
		case 3:
			if(line.length() == 0)
				break;
			else
				state++;
		case 4:
			// Load game entites.
			break;
		default:
			// Unknown state.
			break;
		}
	}
}
