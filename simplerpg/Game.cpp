#include "Game.h"
#include "GameEntity.h"
#include "GameEntityFactory.h"
#include "Animal.h"

char *Game::LOOK_FOR_TABLE[] = {"Animal", "Plant"};

Game::Game(void)
{
	mScreenSize = Rect(0, 0, 60, 25);
	mMap = NULL;

	mCursorX = -1;
	mCursorY = -1;

	mCursorMode = false;
	mRedisplay = true;
	mUnderCursorDirty = true;

	mSelectedItem = NULL;

	mHud.setX(61);
	mHud.setY(1);
	mHud.addChild(mHudText);

	mMenuLevel = 0;
	mLookFor = 0;
	mFoundEntity = NULL;
}

Game::~Game(void)
{
	
}

void Game::keyActions(const int key)
{
	// Numpad 8
	if (key == 56)
	{
		mHud.scrollY(1);
	}
	// Numpad 2
	if (key == 50)
	{
		mHud.scrollY(-1);
	}

	if(mSelectedItem != NULL)
	{
		if(key == 27)
		{
			switchKeyItem(NULL, mHud);
			return;
		}
		else
		{
			mSelectedItem->keyActions(key);
			return;
		}
	}

	if(getCursorMode())
	{
		int cx = mCursorX;
		int cy = mCursorY;

		if (key == 260)
			cx--;
		if (key == 261)
			cx++;
		if (key == 259)
			cy--;
		if (key == 258)
			cy++;

		setCursorPosition(cx, cy);
	}

	switch(mMenuLevel)
	{
	default:
	case 0:
		if (key == 260)
			moveCamera(-1, 0);
		if (key == 261)
			moveCamera(1, 0);
		if (key == 259)
			moveCamera(0, -1);
		if (key == 258)
			moveCamera(0, 1);

		if (key == 'k')
		{
			setCursorMode(true);
			mMenuLevel = 1;
		}

		if (key == 'f')
		{
			setCursorMode(true);
			mMenuLevel = 2;
		}
		break;

	case 1:
		
		if (key == 27 || key == 'k')
		{
			setCursorMode(false);
			mMenuLevel = 0;
		}

		if(key >= '1' && key <= '9')
		{
			EntityList list = getUnderCursor();
			int numPressed = key - '1';
			if(!list.empty() && numPressed < (int)list.size())
			{
				switchKeyItem(list[numPressed], mHud);
			}
		}
		break;
	case 2:
		if (key == 27)
		{
			setCursorMode(false);
			mFoundEntity = NULL;
			mMenuLevel = 0;
		}

		if (key == 'x')
		{
			int l = sizeof(LOOK_FOR_TABLE) / sizeof(char*);
			mLookFor++;
			if (mLookFor >= l)
				mLookFor = 0;
		}

		if(key == 'f')
		{
			mFoundEntity = findClosestEntity(Vector2f(mCursorX, mCursorY), LOOK_FOR_TABLE[mLookFor]);
		}
		break;
	}
}

void Game::displayActions()
{
	mHudText.clearText();
	
	switch (mMenuLevel)
	{
	default:
	case 0:
		mHudText << "Menu\n\n";
		mHudText << "<11>k</>: Look mode.\n";
		mHudText << "<11>f</>: Find closest.";
		break;
	case 1:
		if(mSelectedItem != NULL)
		{
			mSelectedItem->displayActions(mHud);
		}
		else
		{
			displayUnderCursor(mHud);
		}
		break;
	case 2:
		mHudText << "Find closest\n";
		mHudText << "Pos (<12>" << mCursorX << ", " << mCursorY << "</>)\n\n";
		mHudText << "<11>x</>: Toggle P/A (" << LOOK_FOR_TABLE[mLookFor][0] << ")\n";
		mHudText << "<11>f</>: Find.\n\n";

		if(mFoundEntity != NULL)
		{
			mHudText << "Found: <12>" << mFoundEntity->getEntityName() << "</>";
		}
		
		break;

	}
}

void Game::setCursorPosition(int xPos, int yPos)
{
	mCursorX = xPos;
	mCursorY = yPos;

	mUnderCursorDirty = true;
}

EntityList Game::getUnderCursor()
{
	if(!mUnderCursorDirty)
	{
		return mUnderCursor;
	}

	mUnderCursorDirty = false;

	Map *m = getMap();
	if(m == NULL || mCursorX < 0 || mCursorY < 0 || mCursorX > m->getWidth() || mCursorY > m->getHeight())
	{
		return EntityList();
	}

	mUnderCursor.clear();

	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		GameEntity *entity = *iter;
		Vector2f pos = entity->getPosition();
		int posX = math::round(pos.x);
		int posY = math::round(pos.y);
		if(posX == mCursorX && posY == mCursorY)
		{
			mUnderCursor.push_back(entity);
		}
	}

	return mUnderCursor;
}

void Game::addEntity(GameEntity *entity)
{
	mEntities.push_back(entity);
}

void Game::removeEntity(GameEntity *entity)
{
	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		if((*iter) == entity)
		{
			mEntities.erase(iter);
			break;
		}
	}
}

void Game::update(float dt)
{
	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		(*iter)->update(dt);
	}
}

void Game::displayUnderCursor(UIContainer &hud)
{
	getUnderCursor();

	Tile *tile = getMap()->getTile(mCursorX, mCursorY);
	if(tile != NULL)
	{
		mHudText << "Tile: " << tile->getName() << '\n';
	}

	int num = 1;
	for(EntityList::iterator iter = mUnderCursor.begin(); iter != mUnderCursor.end(); iter++)
	{
		mHudText << "<12>" << num++ << "</>: " << (*iter)->getEntityName() << '\n';
	}
}

void Game::render(WINDOW *wnd)
{
	mMap->renderMap(mScreenSize, wnd);

	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		(*iter)->render(mScreenSize, wnd);
	}

	if(mCursorMode)
	{
		int xPos = mCursorX - mScreenSize.getX();
		int yPos = mCursorY - mScreenSize.getY();

		if (xPos >= 0 && xPos < mScreenSize.getWidth() &&
			yPos >= 0 && yPos < mScreenSize.getHeight())
		{
			if (mCursorX >= 0 && mCursorX < mMap->getWidth() &&
				mCursorY >= 0 && mCursorY < mMap->getHeight())
			{
				wattron(wnd, COLOR_PAIR(9));
				wattroff(wnd, A_BOLD);
				mvwaddch(wnd, yPos, xPos, 'X');
			}
		}
	}

	mHud.setWindow(wnd);
	displayActions();
	mHud.render();
}

void Game::moveCamera(int dx, int dy)
{
	mScreenSize.setX(mScreenSize.getX() + dx);
	mScreenSize.setY(mScreenSize.getY() + dy);
}

void Game::saveMap(string filename)
{
	ofstream file(filename);
	if(!file.is_open())
	{
		cout << "Failed to open file " << filename << " for saving." << endl;
		return;
	}

	file << "// Auto generated map file." << endl;

	Map *gameMap = getMap();

	map<char, Tile *> *tileMap = gameMap->getMappedTiles();
	map<Tile *, char> tileLookup;
	
	file << "-- Tiles" << endl;
	for(map<char, Tile *>::iterator iter = tileMap->begin(); iter != tileMap->end(); iter++)
	{
		file << iter->first << ' ' << iter->second->getCode() << endl;
		tileLookup[iter->second] = iter->first;
	}

	file << endl << "-- Map" << endl;

	for(int y = 0; y < gameMap->getHeight(); y++)
	{
		for(int x = 0; x < gameMap->getWidth(); x++)
		{
			Tile *tile = gameMap->getTile(x, y);
			file << tileLookup[tile];
		}
		file << endl;
	}

	file << endl << "-- Entities" << endl;

	
	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		(*iter)->saveToFile(file);
	}

	file << endl << "-- End";

	file.close();
}

void Game::loadMap(string filename)
{
	ifstream file(filename);
	if(!file.is_open())
	{
		cout << "Failed to open file " << filename << endl;
		return;
	}

	string fileStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	boost::regex readFileRegex(
		// Multi-line comments.
		"(/\\*[^/\\*]*\\*/)|"
		// Single-line comments.
		"(//[^\n]*\n)|"
		// Double quoted strings.
		"(\"[^\"]*\")|"
		// Single quoted strings.
		"('[^']*')|"
		// Everything that's not whitespace.
		"(\\S+)");

	boost::sregex_token_iterator iter(fileStr.begin(), fileStr.end(), readFileRegex, 0), end;

	static const int STATE_EMPTY = 0;
	static const int STATE_READ_TILES = 1;
	static const int STATE_READ_MAP = 2;
	static const int STATE_READ_ENTITIES = 3;
	static const int STATE_END_OF_FILE = 4;

	vector<string> mapData;
	int width = 0;

	map<char, Tile *> tileMap;
	Map *map;

	int state = STATE_EMPTY;
	char c = '\0';

	// Indicates that we want to change the current state of the file loader.
	bool changeState = false;
	// Indicates to the state machine that the current state is done, call finish code.
	bool endState = false;

	while(iter != end)
	{
		string line;
		if(!endState)
		{
			line = *iter++;
		}

		// Ignore comments
		if(line.length() >= 2)
		{
			if(line[0] == '/' && (line[1] == '/' || line[1] == '*'))
			{
				continue;
			}
			if(line[0] == '-' && line[1] == '-')
			{
				changeState = true;
				if(state != STATE_EMPTY)
				{
					endState = true;
				}
				continue;
			}
		}

		if(changeState && !endState)
		{
			changeState = false;
			if(boost::algorithm::iequals(line, "Tiles"))
			{
				// Change to tiles.
				state = STATE_READ_TILES;
				cout << "Switching to reading tiles." << endl;
			}
			else if(boost::algorithm::iequals(line, "Map"))
			{
				state = STATE_READ_MAP;
				cout << "Switching to reading map." << endl;
			}
			else if(boost::algorithm::iequals(line, "Entities"))
			{
				state = STATE_READ_ENTITIES;
				cout << "Switching to reading entities." << endl;
			}
			else if(boost::algorithm::iequals(line, "End"))
			{
				state = STATE_END_OF_FILE;
				cout << "Switching to end of file state." << endl;
			}
			else
			{
				cout << "Unknown state " << line << endl;
				break;
			}
			continue;
		}

		if(!endState)
		{
			cout << "Line: >" << line << "<" << endl;
		}
		else
		{
			cout << "Ending state." << endl;
		}

		switch(state)
		{
		default:
		case STATE_EMPTY:
			break;
		case STATE_READ_TILES:
			if(endState)
			{
				cout << "Tiles do nothing on end state." << endl;
				break;
			}

			if(c == '\0')
			{
				c = line[0];
			}
			else
			{
				tileMap[c] = Tile::getTile( atoi(line.c_str()));
				c = '\0';
			}

			break;
		case STATE_READ_MAP:
			if(endState)
			{
				cout << "Create map and add to game." << endl;
				map = new Map(width, mapData.size());
				map->setMappedTiles(tileMap);
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
		case STATE_READ_ENTITIES:
			if(endState)
			{
				cout << "End state for entities." << endl;
				break;
			}

			GameEntity *entity = GameEntityFactory::create(this, line);
			if(entity == NULL)
			{
				cout << "Error parsing entity " << line << endl;
			}
			else
			{
				entity->loadFromFile(iter);
				addEntity(entity);
				entity->onAddedToGame();
			}
			
			break;
		}
		endState = false;
	}
}

float lengthOfPath(const vector<Vector2f> *path)
{
	double length = 0.0;
	int size = (int)path->size();
	if (size <= 1)
		return 0.0f;

	for(int i = 1; i < size; i++)
	{
		Vector2f v1 = (*path)[i];
		Vector2f v2 = (*path)[i - 1];
		Vector2f diff = v1.sub(v2);
		length += diff.length();
	}

	return (float)length;
}

GameEntity *Game::findClosestEntity(Vector2f position, string entityType)
{
	GameEntity *shortestEntity = NULL;
	float shortest = 1e30f;
	Map *m = getMap();
	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		GameEntity *entity = *iter;
		if(iequals(entity->getEntityType(), entityType))
		{
			vector<Vector2f> *path = m->search(position, entity->getPosition());
			if(path != NULL)
			{
				float length = lengthOfPath(path);
				if(length < shortest)
				{
					shortest = length;
					shortestEntity = entity;
				}
				delete path;
			}
			
		}
	}
	return shortestEntity;
}