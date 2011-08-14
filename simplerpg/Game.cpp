#include "Game.h"
#include "GameEntity.h"
#include "GameEntityFactory.h"
#include "Animal.h"

const char *Game::LOOK_FOR_TABLE[] = {"Animal", "Plant"};

Game *Game::CURRENT_GAME = NULL;

Game::Game(int width, int height)
{
	CURRENT_GAME = this;
	mHudWidth = 30;
	
	mMap = NULL;

	mCursorX = -1;
	mCursorY = -1;

	mCursorMode = false;
	mRedisplay = true;
	mUnderCursorDirty = true;

	mSelectedItem = NULL;

	mHud.scrollY(1);
	mHud.addChild(mHudText);

	resize(width, height);

	mMenuLevel = MENU_MAIN;
	mLookFor = 0;
	mSaveCounter = 0;

	mGameRunning = true;
	mGamePaused = false;

	mLastKey = -1;
}

Game::~Game(void)
{
	
}

void Game::resize(int width, int height)
{
	mScreenSize = Rect(0, 0, width - mHudWidth - 1, height);
	mHud.setX(width - mHudWidth);
	mHud.setMaxHeight(height);
	mHud.setMaxWidth(mHudWidth);

	mGameWidth = width;
	mGameHeight = height;
}

void Game::setHudWidth(int width)
{
	if (width < 0)
		width = 0;

	mHudWidth = width;
	resize(mGameWidth, mGameHeight);
}

void Game::keyActions(const int key)
{
	mLastKey = key;
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
	// Numpad 5
	if (key == 53)
	{
		mHud.setScrollY(0);
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
	case MENU_MAIN:
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
			mMenuLevel = MENU_LOOK;
		}

		if (key == 'f')
		{
			setCursorMode(true);
			mMenuLevel = MENU_FIND;
		}

		if (key == 's')
		{
			saveMap("test.out");
		}

		if (key == 'q')
		{
			mMenuLevel = MENU_QUIT;
			setGamePaused(true);
		}

		if (key == 'o')
		{
			mMenuLevel = MENU_OPTIONS;
			setGamePaused(true);
		}
		break;

	case MENU_LOOK:
		
		if (key == 27 || key == 'k')
		{
			setCursorMode(false);
			mMenuLevel = MENU_MAIN;
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
	case MENU_FIND:
		if (key == 27)
		{
			setCursorMode(false);
			mFoundEntity.clear();
			mMenuLevel = MENU_MAIN;
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
			mFoundEntity.clear();
			mFoundEntity = findClosestEntity(Vector2i(mCursorX, mCursorY), LOOK_FOR_TABLE[mLookFor]);
		}
		break;
	case MENU_QUIT:
		if (key == 'y' || key == 'q')
		{
			// QUIT!.
			mGameRunning = false;
		}
		
		if (key == 's')
		{
			saveMap("test.out");
			// QUIT!.
			mGameRunning = false;
		}

		if (key == 'n' || key == 27)
		{
			mMenuLevel = MENU_MAIN;
			setGamePaused(false);
		}
		break;
	case MENU_OPTIONS:
		if (key == 27)
		{
			mMenuLevel = MENU_MAIN;
			setGamePaused(false);
		}

		// Numpad -
		if (key == 464)
		{
			setHudWidth(getHudWidth() - 1);
		}
		// Numpad +
		if (key == 465)
		{
			setHudWidth(getHudWidth() + 1);
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
	case MENU_MAIN:
		mHudText << "<15>Menu</>\n\n";
		mHudText << "<11>k</>: Look mode.\n";
		mHudText << "<11>f</>: Find closest.\n";
		mHudText << "<11>o</>: Options.\n";
		mHudText << "<11>s</>: Save.";
		if(mSaveCounter > 0)
		{
			mSaveCounter--;
			mHudText << " (Saved)";
		}
		mHudText << '\n';

		mHudText << "\n<11>q</>: Quit.\n";

#if _DEBUG_RPG
		mHudText << "\n\nLast key: <12>" << mLastKey << "</>\n";
#endif
		
		break;
	case MENU_LOOK:
		if(mSelectedItem != NULL)
		{
			mSelectedItem->displayActions(mHud);
		}
		else
		{
			displayUnderCursor(mHud);
		}
		break;
	case MENU_FIND:
		mHudText << "<15>Find closest</>\n";
		mHudText << "Pos (<12>" << mCursorX << ", " << mCursorY << "</>)\n\n";
		mHudText << "<11>x</>: Toggle P/A (" << LOOK_FOR_TABLE[mLookFor][0] << ")\n";
		mHudText << "<11>f</>: Find.\n";
		
		mHudText << '\n';

		if(mFoundEntity.entity != NULL)
		{
			mHudText << "Found: <12>" << mFoundEntity.entity->getEntityName() << "</>";
		}
		break;
	case MENU_QUIT:
		mHudText << "<15>Are you sure you want to quit?\n\n</>";
		mHudText << "<11>y</>: Quit (no save).\n";
		mHudText << "<11>s</>: Save and quit.\n";
		mHudText << "<11>n</>: Go back.\n";
		
		break;
	case MENU_OPTIONS:
		mHudText << "<15>Options:</>\n\n";
		mHudText << "Menu size: <12>" << getHudWidth() << "</> <11>+</>/<11>-</>\n";
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
		int posX = (int)round(pos.x);
		int posY = (int)round(pos.y);
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
		mHudText << "<15>Tile:</> " << tile->getName() << '\n';
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

vector<Vector2f> *Game::findPath(Vector2i startPosition, Vector2i endPosition)
{
	return getMap()->search(startPosition, endPosition);
}

void Game::saveMap(string filename)
{
	FormattedFile file(filename);
	if(!file.isOpen())
	{
		clog << "Failed to open file " << filename << " for saving." << endl;
		return;
	}

	file << "// Auto generated map file.\n";

	file << "-- Options\n";
	saveOptions(file);

	Map *gameMap = getMap();

	map<char, Tile *> *tileMap = gameMap->getMappedTiles();
	map<Tile *, char> tileLookup;

	file << "\n-- Tiles\n";
	for(map<char, Tile *>::iterator iter = tileMap->begin(); iter != tileMap->end(); iter++)
	{
		file << iter->first << ' ' << iter->second->getCode() << '\n';
		tileLookup[iter->second] = iter->first;
	}

	file << "\n-- Map\n";

	for(int y = 0; y < gameMap->getHeight(); y++)
	{
		for(int x = 0; x < gameMap->getWidth(); x++)
		{
			Tile *tile = gameMap->getTile(x, y);
			file << tileLookup[tile];
		}
		file << '\n';
	}

	file << "\n-- Entities\n";

	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		(*iter)->saveToFile(file);
	}

	file << "\n-- End";
	mSaveCounter = 20;

	file.closeFile();
}

void Game::loadMap(string filename)
{
	ifstream file(filename);
	if(!file.is_open())
	{
		clog << "Failed to open file " << filename << endl;
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

	boost::sregex_token_iterator boostIter(fileStr.begin(), fileStr.end(), readFileRegex, 0), end;

	FormattedFileIterator iter(&boostIter, true);

	static const int STATE_EMPTY = 0;
	static const int STATE_READ_TILES = 1;
	static const int STATE_READ_MAP = 2;
	static const int STATE_READ_ENTITIES = 3;
	static const int STATE_END_OF_FILE = 4;
	static const int STATE_OPTIONS = 5;

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

	while(!iter.atEnd())
	{
		string line;
		if(!endState)
		{
			line = *iter;
			++iter;
		}

		// Comments are ignored by the FormattedFileIterator.
		if(line.length() >= 2)
		{
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
			if(iequals(line, "Tiles"))
			{
				// Change to tiles.
				state = STATE_READ_TILES;
				clog << "Switching to reading tiles." << endl;
			}
			else if(iequals(line, "Map"))
			{
				state = STATE_READ_MAP;
				clog << "Switching to reading map." << endl;
			}
			else if(iequals(line, "Entities"))
			{
				state = STATE_READ_ENTITIES;
				clog << "Switching to reading entities." << endl;
			}
			else if(iequals(line, "End"))
			{
				state = STATE_END_OF_FILE;
				clog << "Switching to end of file state." << endl;
			}
			else if(iequals(line, "Options"))
			{
				state = STATE_OPTIONS;
				clog << "Switch to reading options." << endl;
			}
			else
			{
				clog << "Unknown state " << line << endl;
				break;
			}
			continue;
		}

		if(!endState)
		{
			clog << "Line: >" << line << "<" << endl;
		}
		else
		{
			clog << "Ending state." << endl;
		}

		switch(state)
		{
		default:
		case STATE_EMPTY:
			break;
		case STATE_OPTIONS:
			if(endState)
			{
				clog << "Finished reading options." << endl;
				break;
			}

			loadOptions(line, iter);

			break;
		case STATE_READ_TILES:
			if(endState)
			{
				clog << "Tiles do nothing on end state." << endl;
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
				clog << "Create map and add to game." << endl;
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
				clog << "ERROR With map data, needs to be square!." << endl;
				state = -1;
				break;
			}
			mapData.push_back(line);

			break;
		case STATE_READ_ENTITIES:
			if(endState)
			{
				clog << "End state for entities." << endl;
				break;
			}

			GameEntity *entity = GameEntityFactory::create(this, line);
			if(entity == NULL)
			{
				clog << "Error parsing entity " << line << endl;
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

FindEntityResult Game::findClosestEntity(Vector2f position, const string &entityType)
{
	return findClosestEntity(position, entityType, NULL);
}

FindEntityResult Game::findClosestEntity(Vector2f position, const string &entityType, const GameEntity *ignore)
{
	GameEntity *shortestEntity = NULL;
	vector<Vector2f> *shortestPath = NULL;
	float shortest = 1e30f;
	Map *m = getMap();
	Vector2i positionInt = (Vector2i)position;
	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		GameEntity *entity = *iter;
		if(iequals(entity->getEntityType(), entityType))
		{
			vector<Vector2f> *path = m->search(positionInt, entity->getPosition());
			if(path != NULL)
			{
				float length = lengthOfPath(path);
				if(length < shortest && (entity != ignore && ignore != NULL))
				{
					shortest = length;
					shortestEntity = entity;
					if(shortestPath != NULL)
						delete shortestPath;
					shortestPath = path;
				}
			}
		}
	}
	return FindEntityResult(shortestEntity, shortestPath);
}

void Game::switchKeyItem(IKeyActions *item, UIContainer &hud)
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

void Game::saveOptions(FormattedFile &file)
{
	saveOption(HUD_WIDTH, file);
}

void Game::saveOption(const GameOption &option, FormattedFile &file)
{
	switch(option)
	{
	case HUD_WIDTH:
		file << "hud_width " << mHudWidth << '\n';
		break;
	default:
		break;
	}
}

void Game::loadOptions(string option, FormattedFileIterator &iter)
{
	if(iequals(option, "hud_width"))
	{
		setHudWidth(lexical_cast<int>(*iter));
		++iter;
	}
	else
	{
		clog << "Unknown option '" << option << "'" << endl;
	}
}