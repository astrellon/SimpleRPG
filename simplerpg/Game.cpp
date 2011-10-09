#include "Game.h"
#include "GameEntity.h"
#include "GameEntityFactory.h"
#include "Animal.h"

const char *Game::LOOK_FOR_TABLE[] = {"Animal", "Plant"};

extern const char *GamePropertyNames[] = { "hud_width", "current_time", "current_day",
	"day_length", "camera_location" };

Game *Game::CURRENT_GAME = NULL;

_FindEntityResult::_FindEntityResult()
{
	entity = NULL;
	path = NULL;
}
_FindEntityResult::_FindEntityResult(GameEntity *e, vector<Vector2f> *p)
{
	entity = e;
	path = p;
}
void _FindEntityResult::clear()
{
	entity = NULL;
	if(path != NULL)
	{
		delete path;
		path = NULL;
	}
}

_RayResult::_RayResult()
{
	entity = NULL;
	point.x = -1;
	point.y = -1;
	tile = NULL;
}
_RayResult::_RayResult(const Vector2f &p, GameEntity *e)
{
	point = p;
	entity = e;
	if (e != NULL)
	{
		point = e->getPosition();
	}
	tile = NULL;
}
_RayResult::_RayResult(const Vector2f &p, Tile *t)
{
	entity = NULL;
	point = p;
	tile = t;
}

Game::Game(int width, int height)
{
	CURRENT_GAME = this;
	mHudWidth = 30;
	
	mMap = NULL;
	mTileData = NULL;

	mCursor = Vector2i(0, 0);
	mCursorAngle = 0;
	mCursorLength = 20;

	mCursorMode = false;
	mRedisplay = true;
	mUnderCursorDirty = true;

	mSelectedItem = NULL;

	mWholeHud.addChild(mWholeHudText);
	mWholeHud.addChild(mHud);
	mHud.addChild(mHudText);

	resize(width, height);

	mMenuLevel = MENU_MAIN;
	mLookFor = 0;
	mSaveCounter = 0;

	mGameRunning = true;
	mGamePaused = false;

	mDebugEntity = NULL;
	mInstantMove  = true;
	mDebugPosition = Vector2i(-1, -1);

	mCurrentTime = 0.0f;
	mCurrentDay = 0;
	mDayLength = 600.0f;

	mCheckedTiles = NULL;

	mLastKey = -1;

	mSelection = Rect(-1, -1, 0, 0);

	mTimeScale = 1;

	mUpdating = false;

	mTileTime = mEntityTime = 0;
	mTileTimeFinal = mEntityTimeFinal = 0.0f;
	mTimeCounter = 0;

	mCheckedTilesCounter = 0;
}

Game::~Game(void)
{
	if (CURRENT_GAME == this)
	{
		CURRENT_GAME = NULL;
	}
	delete mMap;
}

void Game::resize(int width, int height)
{
	mScreenSize = Rect(0, 0, width - mHudWidth - 1, height);
	mHud.setMaxHeight(height);
	mHud.setMaxWidth(mHudWidth);

	mHudText.setMaxWidth(mHudWidth);

	mWholeHud.setX(width - mHudWidth);
	mWholeHud.setMaxHeight(height);
	mWholeHud.setMaxWidth(mHudWidth);

	mWholeHudText.setMaxWidth(mHudWidth);

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

inline TileData *Game::getTileData(const int &x, const int &y)
{
	if (x < 0 || y < 0 || mMap == NULL || 
		x >= mMap->getWidth() || y >= mMap->getHeight())
	{
		return NULL;
	}

	return &mTileData[x][y];
}

inline TileData *Game::getTileData(const Vector2i &position)
{
	return getTileData(position.x, position.y);
}

void Game::keyActions(const int key)
{
	mLastKey = key;
	// Numpad 8
	if (key == 456)
	{
		mHud.scrollY(1);
	}
	// Numpad 2
	if (key == 450)
	{
		mHud.scrollY(-1);
	}
	// Numpad 5
	if (key == 453)
	{
		mHud.setScrollY(0);
	}

	if (key == ' ')
	{
		setGamePaused(!getGamePaused());
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
		int cx = mCursor.x;
		int cy = mCursor.y;

		// Left
		if (key == 260)
			cx--;
		// Right
		else if (key == 261)
			cx++;
		// Up
		else if (key == 259)
			cy--;
		// Down
		else if (key == 258)
			cy++;
		//  Shift Left
		else if (key == 391)
			cx -= 20;
		//  Shift Right
		else if (key == 400)
			cx += 20;
		//  Shift Up
		else if (key == 547)
			cy -= 20;
		//  Shift Down
		else if (key == 548)
			cy += 20;

		setCursorPosition(cx, cy);
	}

	switch(mMenuLevel)
	{
	default:
	case MENU_MAIN:
		// Left
		if (key == 260)
			moveCamera(-1, 0);
		// Right
		else if (key == 261)
			moveCamera(1, 0);
		// Up
		else if (key == 259)
			moveCamera(0, -1);
		// Down
		else if (key == 258)
			moveCamera(0, 1);
		//  Shift Left
		else if (key == 391)
			moveCamera(-20, 0);
		//  Shift Right
		else if (key == 400)
			moveCamera(20, 0);
		//  Shift Up
		else if (key == 547)
			moveCamera(0, -20);
		//  Shift Down
		else if (key == 548)
			moveCamera(0, 20);

		if (key == 'k')
		{
			setCursorMode(true);
			setGamePaused(true);
			mMenuLevel = MENU_LOOK;
		}

		if (key == 'f')
		{
			setCursorMode(true);
			setGamePaused(true);
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

		if (key == 'r')
		{
			mMenuLevel = MENU_RAY;
			setCursorMode(true);
			setGamePaused(true);
		}

		if (key == 'n')
		{
			mMenuLevel = MENU_NEAR;
			setCursorMode(true);
			setGamePaused(true);
		}

		if (key == 'f')
		{
			mMenuLevel = MENU_FOOD;
			setCursorMode(true);
			setGamePaused(true);
		}

		if (key == 'm')
		{
			mMenuLevel = MENU_MOVE;
			setCursorMode(true);
			setGamePaused(true);
		}

		if (key == 'e')
		{
			mMenuLevel = MENU_SELECTION;
			setCursorMode(true);
			setGamePaused(true);
		}

		break;

	case MENU_LOOK:
		
		if (key == 27 || key == 'k')
		{
			setCursorMode(false);
			setGamePaused(false);
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
			setGamePaused(false);
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
			mFoundEntity = findClosestEntity(mCursor, LOOK_FOR_TABLE[mLookFor]);
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

		// Left
		if (key == 260)
		{
			setHudWidth(getHudWidth() - 1);
		}
		// Numpad + or =
		if (key == 465 || key == 61)
		{
			setHudWidth(getHudWidth() + 1);
		}

		if (key == ']')
		{
			setTimeScale(getTimeScale() + 1);
		}
		if (key == '[')
		{
			setTimeScale(getTimeScale() - 1);
		}
		// Shift + ]
		if (key == 125)
		{
			setTimeScale(getTimeScale() + 10);
		}
		// Shift + [
		if (key == 123)
		{
			setTimeScale(getTimeScale() - 10);
		}

		break;

	case MENU_RAY:

		if (key == 27)
		{
			mMenuLevel = MENU_MAIN;
			setCursorMode(false);
			setGamePaused(false);
		}

		// Left
		if (key == 'a')
			mCursorAngle -= 5;
		// Right
		else if (key == 'd')
			mCursorAngle += 5;
		// Up
		else if (key == 'w')
			mCursorLength += 2;
		// Down
		else if (key == 's')
			mCursorLength -= 2;

		if (mCursorLength < 0)
		{
			mCursorLength = 0;
		}

		while(mCursorAngle < -180) 
		{
			mCursorAngle += 360;
		}

		while(mCursorAngle > 180) 
		{
			mCursorAngle -= 360;
		}
		break;

	case MENU_NEAR:

		if (key == 27)
		{
			mMenuLevel = MENU_MAIN;
			setCursorMode(false);
			setGamePaused(false);
		}

		// Up
		else if (key == 'w')
			mCursorLength += 2;
		// Down
		else if (key == 's')
			mCursorLength -= 2;

		if (mCursorLength < 0)
		{
			mCursorLength = 0;
		}
		break;

	case MENU_FOOD:

		// This menu only needs to be the current one and the actual
		// graphic is set from the render function.
		if (key == 27)
		{
			mMenuLevel = MENU_MAIN;
			setCursorMode(false);
			setGamePaused(false);
		}

		break;

	case MENU_MOVE:

		if (key == 27)
		{
			if(mDebugEntity == NULL)
			{
				mMenuLevel = MENU_MAIN;
				setCursorMode(false);
				setGamePaused(false);
			}
			mDebugEntity = NULL;
		}

		if (key == 'x')
		{
			mInstantMove = !mInstantMove;
		}

		if(mDebugEntity == NULL && key >= '1' && key <= '9')
		{
			EntityList list = getUnderCursor();
			int numPressed = key - '1';
			if(!list.empty() && numPressed < (int)list.size())
			{
				mDebugEntity = list[numPressed];
			}
		}

		if(mDebugEntity != NULL && (key == 10 || key == 459))
		{
			Tile *tile = mMap->getTile(mCursor);
			if (tile != NULL && tile->getPassable())
			{
				if(mInstantMove)
				{
					mDebugEntity->move(mCursor.sub(mDebugEntity->getPosition()), false);
				}
				else
				{
					TargetAction *action = new TargetAction(MOVE);
					action->getTarget().setLocation(mCursor);
					mDebugEntity->setCurrentAction(action);
				}
			}
		}
		break;

	case MENU_SELECTION:

		if (key == 27)
		{
			mMenuLevel = MENU_MAIN;
			setCursorMode(false);
			setGamePaused(false);
			mSelection.setRect(-1, -1, 0, 0);
		}

		int sx = mSelection.getX();
		int sy = mSelection.getY();
		int sw = mSelection.getWidth();
		int sh = mSelection.getHeight();

		if(key == 10 || key == 459)
		{
			if(sx < 0 || sy < 0)
			{
				mSelection.setX(mCursor.x);
				mSelection.setY(mCursor.y);
			}
			else if(sw <= 0 || sh <= 0)
			{
				mSelection.setWidth(mCursor.x - sx);
				mSelection.setHeight(mCursor.y - sy);
			}
			else
			{
				mSelection.setRect(mCursor.x, mCursor.y, 0, 0);
			}
		}

		if (sx >= 0 && sy >= 0 && sw > 0 && sh > 0 && key >= '1' && key <= '4')
		{
			// Remove all food from selection.
			for(int x = sx; x < sx + sw; x++)
			{
				for(int y = sy; y < sy + sh; y++)
				{
					TileData *data = getTileData(x, y);
					Tile *tile = mMap->getTile(x, y);
					if(data == NULL)
					{
						continue;
					}

					// Remove food value
					if(key == '1')
					{
						data->setFoodValue(0);
					}
					// Regrow food value
					else if(key == '2' && tile != NULL)
					{
						data->setFoodValue(tile->getMaxFoodValue());
					}
					// Disable regrowth.
					else if(key == '3')
					{
						data->setRegrowthRate(0.0f);
					}
					// Enable regrowth.
					else if(key == '4' && tile != NULL)
					{
						data->setRegrowthRate(tile->getRegrowthRate());
					}
				}
			}
		}

		break;
	}
}

void Game::displayActions()
{
	mHudText.clearText();

	vector<GameEntity *> results;
	TileData *closestFood = NULL;
	string species = "Rabbit";

	mHudText << "<15>Time</>:\t<12>" << mTileTimeFinal << ", " << mEntityTimeFinal << "</>\n\n";

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

		mHudText << "<11>r</>: Ray tester.\n";
		mHudText << "<11>n</>: Nearby tester.\n";
		mHudText << "<11>f</>: Nearest food tile.\n";
		mHudText << "<11>m</>: Move entity.\n";
		mHudText << "<11>e</>: Selection tool.\n";
		
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
			displayUnderCursor(/*mHud*/);
		}
		break;
	case MENU_FIND:
		mHudText << "<15>Find closest</>\n";
		//mHudText << "Pos (" << mCursor.toString(12) << ")\n\n";
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
		mHudText << "<15>Menu size</>:\t<12>" << getHudWidth() << "</> <11>+</>/<11>-</>\n";
		mHudText << "<15>Time scale</>:\t<12>" << getTimeScale() << "</> <11>[</>/<11>]</>\n";

		break;

	case MENU_RAY:
		mHudText << "<15>Ray Test Menu:</>\n\n";
		//mHudText << "<15>Pos</>:\t(" << mCursor.toString(12) << ")\n";
		mHudText << "<15>Angle</>:\t<12>" << mCursorAngle << "</>\n";
		mHudText << "<15>Len</>:\t<12>" << mCursorLength << "</>\n";

		break;

	case MENU_NEAR:

		mHudText << "<15>Nearby Test Menu:</>\n\n";
		//mHudText << "<15>Position</>:\t(" << mCursor.toString(12) << ")\n";
		mHudText << "<15>Radius</>:\t\t<12>" << mCursorLength << "</>\n";

		findNearby(mCursor, (float)mCursorLength, results, NULL);

		mHudText << "<15>Results</>:\t" << results.size() << "\n\n";
		for(size_t i = 0; i < results.size(); i++)
		{
			GameEntity *entity = results[i];
			mHudText << "<12>" << i << "</>: " << entity->getLongName() << '\n';
		}

		results.clear();
		mHudText << "\nNeaby <12>" << species << "</>s\n";

		findNearby(mCursor, (float)mCursorLength, results, species);

		mHudText << "<15>Results</>:\t" << results.size() << "\n\n";
		for(size_t i = 0; i < results.size(); i++)
		{
			GameEntity *entity = results[i];
			mHudText << "<12>" << i << "</>: " << entity->getLongName() << '\n';
		}

	case MENU_FOOD:

		mHudText << "<15>Closest tile with food:</>\n\n";
		//mHudText << "<15>Position</>:\t(" << mCursor.toString(12) << ")\n\n";

		mDebugPosition = findClosestTileWithFood(mCursor);
		closestFood = getTileData(mDebugPosition);
		if(closestFood != NULL)
		{
			mHudText << "<15>Tile</>:\t" << closestFood->getTile()->getName() << '\n'; 
			closestFood->displayData(mHudText);
		}
		else
		{
			mHudText << "None Found\n";
		}

		break;

	case MENU_MOVE:

		mHudText << "<15>Move entity menu:</>\n\n";
		mHudText << "<12>x</>: ";
		if(mInstantMove)
		{
			mHudText << "Instant move.\n";
		}
		else
		{
			mHudText << "Tell to move.\n";
		}
		if(mDebugEntity == NULL)
		{
			mHudText << "No entity selected.\n\n";
			displayUnderCursor();
		}
		else
		{
			mHudText << "<15>Entity</>:\t" << mDebugEntity->getLongName() << '\n';
			mHudText << "<15>Current Position</>:\t" << ((Vector2i)mDebugEntity->getPosition()).toString(12) << '\n';
			mHudText << "<15>New Position</>:\t\t" << mCursor.toString(12) << '\n';
		}
		break;

	case MENU_SELECTION:

		mHudText << "<15>Section menu:</>\n\n";
		mHudText << "<15>Point</>:\t<12>" << mSelection.getX() << "</>, <12>" << mSelection.getY() << "</>\n";
		mHudText << "<15>Size</>:\t<12>" << mSelection.getWidth() << "</>, <12>" << mSelection.getHeight() << "</>\n";

		mHudText << '\n';
		mHudText << "<12>1</>: Remove all food.\n";
		mHudText << "<12>2</>: Regrow all food.\n";
		mHudText << "<12>3</>: Stop all food growth.\n";
		mHudText << "<12>4</>: Reset food growth.\n";

		break;
	}
}

void Game::setCursorPosition(int xPos, int yPos)
{
	if(xPos < -1) xPos = -1;
	if(yPos < -1) yPos = -1;
	
	Map *m = getMap();
	if(m != NULL)
	{
		if(xPos > m->getWidth())  xPos = m->getWidth();
		if(yPos > m->getHeight()) yPos = m->getHeight();
	}
	mCursor.x = xPos;
	mCursor.y = yPos;

	int cameraX = mScreenSize.getX();
	int cameraY = mScreenSize.getY();

	int screenX = mCursor.x - cameraX;
	int screenY = mCursor.y - cameraY;

	if(screenX < 10)
	{
		cameraX -= 10 - screenX;
	}
	else if(screenX > mScreenSize.getWidth() - 10)
	{
		cameraX += screenX - (mScreenSize.getWidth() - 10);
	}

	if(screenY < 10)
	{
		cameraY -= 10 - screenY;
	}
	else if(screenY > mScreenSize.getHeight() - 10)
	{
		cameraY += screenY - (mScreenSize.getHeight() - 10);
	}

	setCamera(cameraX, cameraY);

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
	if(m == NULL || mCursor.x < 0 || mCursor.y < 0 || mCursor.x > m->getWidth() || mCursor.y > m->getHeight())
	{
		return EntityList();
	}

	mUnderCursor.clear();

	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		GameEntity *entity = *iter;
		Vector2i pos = entity->getPosition();
		if(pos.x == mCursor.x && pos.y == mCursor.y)
		{
			mUnderCursor.push_back(entity);
		}
	}

	return mUnderCursor;
}

void Game::addEntity(GameEntity *entity)
{
	EntityList *list = &mEntities;
	if(mUpdating)
	{
		list = &mNewEntities;
	}
	list->push_back(entity);
}

void Game::removeEntity(GameEntity *entity)
{
	if(mUpdating)
	{
		mRemoveEntities.push_back(entity);
	}
	else
	{
		for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
		{
			if((*iter) == entity)
			{
				mRemovedEntities.push_back(*iter);
				mEntities.erase(iter);
				break;
			}
		}
	}
}

void Game::update(float dt)
{
	int previousDay = mCurrentDay;

	advanceTime(dt);

	if(previousDay < mCurrentDay)
	{
		vector<Vector2i> *list = new vector<Vector2i>();
		for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
		{
			list->push_back((*iter)->getPosition());
		}
		mLocationHistory.push_back(list);
	}

	clock_t start = clock();

	int size = (int)mActiveTiles.size();
	for(int i = 0; i < size; i++)
	{
		TileData *tile = mActiveTiles[i];
		tile->update(dt);
		if(!mActiveTiles[i]->getActive())
		{
			mActiveTiles.erase(mActiveTiles.begin() + i);
			size--;
			i--;
		}
	}

	clock_t end = clock();
	mTileTime += (end - start);

	start = clock();

	// Add any new entities.
	for(EntityList::iterator iter = mNewEntities.begin(); iter != mNewEntities.end(); ++iter)
	{
		addEntity(*iter);
	}

	mNewEntities.clear();

	// Update all current entities.
	mUpdating = true;
	size = (int)mEntities.size();
	for(int i = 0; i < size; i++)
	{
		mEntities[i]->update(dt);
	}
	mUpdating = false;

	// Remove dead entities.
	for(EntityList::iterator iter = mRemoveEntities.begin(); iter != mRemoveEntities.end(); ++iter)
	{
		removeEntity(*iter);
	}
	mRemoveEntities.clear();

	end = clock();
	mEntityTime += (end - start);

	mTimeCounter++;
	if(mTimeCounter > mTimeScale)
	{
		mTileTimeFinal = (float)(mTileTime);
		mEntityTimeFinal = (float)(mEntityTime);
		mTileTime = mEntityTime = 0;
		mTimeCounter = 0;
	}
}

void Game::displayUnderCursor()
{
	getUnderCursor();

	Tile *tile = getMap()->getTile(mCursor.x, mCursor.y);
	if(tile != NULL)
	{
		mHudText << "<15>Tile:</> " << tile->getName() << '\n';
		TileData *data = getTileData(mCursor.x, mCursor.y);
		if(data != NULL)
		{
			data->displayData(mHudText);
		}
	}

	int num = 1;
	for(EntityList::iterator iter = mUnderCursor.begin(); iter != mUnderCursor.end(); ++iter)
	{
		mHudText << "<12>" << num++ << "</>: " << (*iter)->getLongName() << '\n';
	}
}

void Game::render(WINDOW *wnd)
{
	mMap->renderMap(mScreenSize, wnd);

	for(vector<GameEntity *>::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		(*iter)->render(mScreenSize, wnd);
	}

	if(mMenuLevel == MENU_RAY)
	{
		float angle = (float)mCursorAngle * M_PIF / 180.0f;
		RayResult result = fireRay(mCursor, angle, (float)mCursorLength);
		
		wattron(wnd, A_BOLD);
		wattron(wnd, COLOR_PAIR(4));
		drawLine(wnd, '*', mCursor, angle, (float)mCursorLength);

		wattron(wnd, COLOR_PAIR(8));
		renderChar(wnd, (int)result.point.x, (int)result.point.y, '*');
	}

	if(mMenuLevel == MENU_FOOD)
	{
		wattron(wnd, COLOR_PAIR(8));
		wattron(wnd, A_BOLD);
		renderChar(wnd, mDebugPosition.x, mDebugPosition.y, '$');
	}

	if(mMenuLevel == MENU_SELECTION)
	{
		int x = mSelection.getX();
		int y = mSelection.getY();
		int w = mSelection.getWidth();
		int h = mSelection.getHeight();
		
		wattron(wnd, A_BOLD);
		wattron(wnd, COLOR_PAIR(4));

		if ((x >= 0 && y >= 0) && (w <= 0 || h <= 0))
		{
			renderChar(wnd, x, y, '+');
		}
		else if(x >= 0 && y >= 0 && w > 0 && h > 0)
		{
			// Top
			drawLine(wnd, x, y, x + w, y, '+');
			// Right
			drawLine(wnd, x + w, y, x + w, y + h, '+');
			// Left
			drawLine(wnd, x, y, x, y + h, '+');
			// Bottom
			drawLine(wnd, x, y + h, x + w, y + h, '+');

			renderChar(wnd, x + w, y + h, '+');
		}
	}

	if(mCursorMode)
	{
		wattron(wnd, COLOR_PAIR(9));
		wattroff(wnd, A_BOLD);
		renderChar(wnd, mCursor.x, mCursor.y, 'X');
	}

	mWholeHudText.clearText();
	mWholeHudText << "<15>Current Day: </>" << getCurrentDay() << '\n';
	mWholeHudText << "<15>Current Time: </>" << getCurrentTimeString() << " (<12>x" << getTimeScale() << "</>)\n";

	if(getCursorMode())
	{
		mWholeHudText << "<15>Cursor: </>" << mCursor.toString(12) << '\n';
	}

	mWholeHud.setWindow(wnd);
	displayActions();
	mWholeHud.render();
}

void Game::renderChar(WINDOW *wnd, const int &x, const int &y, const char &c)
{
	int xPos = x - mScreenSize.getX();
	int yPos = y - mScreenSize.getY();

	if (xPos >= 0 && xPos < mScreenSize.getWidth() &&
		yPos >= 0 && yPos < mScreenSize.getHeight())
	{
		if (x >= 0 && x < mMap->getWidth() &&
			y >= 0 && y < mMap->getHeight())
		{
			mvwaddch(wnd, yPos, xPos, c);
		}
	}
}

void Game::moveCamera(const int &dx, const int &dy)
{
	mScreenSize.setX(mScreenSize.getX() + dx);
	mScreenSize.setY(mScreenSize.getY() + dy);
}

void Game::moveCamera(const Vector2i &diff)
{
	moveCamera(diff.x, diff.y);
}

void Game::setCamera(const int &x, const int &y)
{
	mScreenSize.setX(x);
	mScreenSize.setY(y);
}

void Game::setCamera(const Vector2i &pos)
{
	setCamera(pos.x, pos.y);
}

vector<Vector2f> *Game::findPath(Vector2i startPosition, Vector2i endPosition)
{
	return getMap()->search(startPosition, endPosition);
}

inline void Game::checkAdjacentTile(const int &x, const int &y, const int &group, queue<Vector2i> &openList)
{
	if (x >= 0 && x < mMap->getWidth() && 
		y >= 0 && y < mMap->getHeight() &&
		mCheckedTiles[x][y] < mCheckedTilesCounter)
	{
		Tile *tile = mMap->getTile(x, y);
		int g = mMap->getGroup(x, y);
		if(g == group && tile != NULL && tile->getPassable())
		{
			mCheckedTiles[x][y] = mCheckedTilesCounter;
			openList.push(Vector2i(x, y));
		}
	}
}

Vector2i Game::findClosestTileWithFood(Vector2i position)
{
	if (position.x < 0 || position.y < 0 || 
		position.x >= mMap->getWidth() || position.y >= mMap->getHeight())
	{
		return Vector2i(-1, -1);
	}
	mCheckedTilesCounter++;

	queue<Vector2i> openList;
	openList.push(position);
	mCheckedTiles[position.x][position.y] = mCheckedTilesCounter;
	Vector2i result(-1, -1);
	int group = mMap->getGroup(position);
	
	while(!openList.empty())
	{
		Vector2i current = openList.front();
		openList.pop();

		TileData *data = getTileData(current);

		float minValue = max(5, data->getMaxFoodValue() * 0.1f);
		if(data->getFoodValue() > 0 && data->getFoodValue() > minValue)
		{
			result = current;
			break;
		}
		else
		{
			// Right
			checkAdjacentTile(current.x + 1, current.y, group, openList);
			// Down
			checkAdjacentTile(current.x, current.y + 1, group, openList);
			// Left
			checkAdjacentTile(current.x - 1, current.y, group, openList);
			// Up
			checkAdjacentTile(current.x, current.y - 1, group, openList);
		}
	}

	return result;
}

Vector2i Game::findClosestTileWithFood(Animal *entity)
{
	Vector2i position = entity->getPosition();

	if (position.x < 0 || position.y < 0 || 
		position.x >= mMap->getWidth() || position.y >= mMap->getHeight())
	{
		return Vector2i(-1, -1);
	}
	
	mCheckedTilesCounter++;

	queue<Vector2i> openList;
	openList.push(position);
	mCheckedTiles[position.x][position.y] = mCheckedTilesCounter;
	Vector2i result(-1, -1);
	int group = mMap->getGroup(position);

	Vector2i average;
	EntityList &list = entity->getSurroundingEntities();
	if(list.size() > 0)
	{
		for(int i = 0; i < list.size(); i++)
		{
			average = average.add(list[i]->getPosition());
		}
		average = average.scale(1.0f / (float)list.size());
	}
	else
	{
		average = position;
		average.x++;
	}

	// Gives a biased random number around 0.0 and between -0.5 and 0.5.
	//float offset = ((math::nextFloat() + math::nextFloat()) * 0.5f - 0.5f) * M_PI * 2;
	
	//float facingX = cos(entity->getFacing() + M_PIF * 0.5f + offset); 
	//float facingY = sin(entity->getFacing() + M_PIF * 0.5f + offset);

	float angle = math::nextFloat() * M_PI * 2;
	int facingX = round(cos(angle));
	int facingY = round(sin(angle));

	//Vector2i toAverage(facingX, facingY);
	//float facingX = 0;
	//float facingY = 1;

	//clog << entity->getEntityName() << " now going to face " << facingX << ", " << facingY << endl;
	
	while(!openList.empty())
	{
		Vector2i current = openList.front();
		openList.pop();

		TileData *data = getTileData(current);

		float minValue = max(5, data->getMaxFoodValue() * 0.1f);
		if(data->getFoodValue() > 0 && data->getFoodValue() > minValue)
		{
			result = current;
			break;
		}
		else
		{
			Vector2f toAverage = ((Vector2f)(current).sub(average));
			double len = toAverage.length();
			if(len < entity->getLineOfSightRadius() * 0.7f)
			{
				toAverage.x = facingX;
				toAverage.y = facingY;
			}
			else
			{
 				toAverage.normalise();
				toAverage.x = round(toAverage.x);
				toAverage.y = round(toAverage.y);
			}
			if(toAverage.x == 0 && toAverage.y == 0)
			{
				toAverage.x = 1;
			}
			// Right
			checkAdjacentTile(current.x + toAverage.x, current.y + toAverage.y, group, openList);
			//checkAdjacentTile(current.x + toAverage.x, current.y - toAverage.y, openList);
			// Down
			checkAdjacentTile(current.x + toAverage.y, current.y - toAverage.x, group, openList);
			//checkAdjacentTile(current.x - toAverage.y, current.y - toAverage.x, openList);
			// Left
			checkAdjacentTile(current.x - toAverage.x, current.y - toAverage.y, group, openList);
			//checkAdjacentTile(current.x - toAverage.x, current.y + toAverage.y, openList);
			// Up
			checkAdjacentTile(current.x - toAverage.y, current.y + toAverage.x, group, openList);
			//checkAdjacentTile(current.x + toAverage.y, current.y + toAverage.x, openList);
		}
	}

	return result;
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
	for(map<char, Tile *>::iterator iter = tileMap->begin(); iter != tileMap->end(); ++iter)
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

	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		(*iter)->saveToFile(file);
	}

	file << "\n-- TileData\n";

	for(int y = 0; y < gameMap->getHeight(); y++)
	{
		for(int x = 0; x < gameMap->getWidth(); x++)
		{
			TileData *data = getTileData(x, y);
			if(data != NULL)
			{
				data->saveToFile(file);
			}
		}
		file << '\n';
	}

	file << "\n-- RemovedEntities\n";

	for(EntityList::iterator iter = mRemovedEntities.begin(); iter != mRemovedEntities.end(); ++iter)
	{
		(*iter)->saveToFile(file);
	}

	file << "\n-- LocationHistory\n";

	for(vector< vector<Vector2i> *>::iterator iter = mLocationHistory.begin(); iter != mLocationHistory.end(); ++iter)
	{
		file << "begin\n";
		file.changeTabLevel(1);
		vector<Vector2i> *list = *iter;

		for(vector<Vector2i>::iterator locIter = list->begin(); locIter != list->end(); ++locIter)
		{
			file << locIter->x << ' ' << locIter->y << "\t";
		}

		file.changeTabLevel(-1);
		file << "\nend\n";
	}

	file << "\n-- End";
	mSaveCounter = 20;

	file.closeFile();
}

void Game::loadMap(string filename)
{
	FormattedFileIterator iter(filename, true);

	if(!iter.isOpen())
	{
		return;
	}

	static const int STATE_EMPTY = 0;
	static const int STATE_READ_TILES = 1;
	static const int STATE_READ_MAP = 2;
	static const int STATE_READ_ENTITIES = 3;
	static const int STATE_END_OF_FILE = 4;
	static const int STATE_OPTIONS = 5;
	static const int STATE_MAP_DATA = 6;
	static const int STATE_TILE_DATA = 7;
	static const int STATE_REMOVED_ENTITIES = 8;
	static const int STATE_LOCATION_HISTORY = 9;

	vector<TileData> tileData;
	TileData data;

	vector<string> mapData;
	int width = 0;

	map<char, Tile *> tileMap;
	Map *map;

	int state = STATE_EMPTY;
	char c = '\0';

	GameEntity *entity = NULL;

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
			else if(iequals(line, "TileData"))
			{
				state = STATE_TILE_DATA;
				clog << "Switch to reading tile data." << endl;
			}
			else if(iequals(line, "RemovedEntities"))
			{
				state = STATE_REMOVED_ENTITIES;
				clog << "Switch to reading removed entities." << endl;
			}
			else if(iequals(line, "LocationHistory"))
			{
				state = STATE_LOCATION_HISTORY;
				clog << "Switch to reading location history." << endl;
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
				tileMap[c] = Tile::getTile(atoi(line.c_str()));
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
						char t = l[x];
						if(tileMap.find(t) == tileMap.end())
						{
							map->setTile(x, y, &Tile::UNKNOWN_TILE);
						}
						else
						{
							map->setTile(x, y, tileMap[t]);
						}
					}
				}

				map->analyseMapForGroups();
				setMap(map);
				//map->logGroups();
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

			entity = GameEntityFactory::create(this, line);
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

		case STATE_TILE_DATA:

			if(endState)
			{
				clog << "End state for tile data." << endl;
				break;
			}

			data = TileData();
			data.loadFromFile(line, iter);
			tileData.push_back(data);

			break;
		
		case STATE_REMOVED_ENTITIES:

			if(endState)
			{
				clog << "End state for removed entities." << endl;
				break;
			}

			entity = GameEntityFactory::create(this, line);
			if(entity == NULL)
			{
				clog << "Error parsing removed entity " << line << endl;
			}
			else
			{
				entity->loadFromFile(iter);
				mRemovedEntities.push_back(entity);
			}
			
			break;

		case STATE_LOCATION_HISTORY:

			if(endState)
			{
				clog << "End state for location history." << endl;
				break;
			}

			if(iequals(line, "begin"))
			{
				vector<Vector2i> *list = new vector<Vector2i>();
				mLocationHistory.push_back(list);
				line = *iter;
				while(!iequals(line, "end"))
				{
					float x = lexical_cast<float>(line);	++iter;
					line = *iter;
					float y = lexical_cast<float>(line);	++iter;
					line = *iter;
					list->push_back(Vector2i(x, y));
				}
				++iter;
			}

			break;
		}
		endState = false;
	}

	if(mTileData == NULL && mMap != NULL)
	{
		bool loadedTileData = !tileData.empty();
		mTileData = new TileData*[mMap->getWidth()];
		for(int x = 0; x < mMap->getWidth(); x++)
		{
			mTileData[x] = new TileData[mMap->getHeight()];
			for(int y = 0; y < mMap->getHeight(); y++)
			{
				mTileData[x][y].setFromTile(mMap->getTile(x, y));
				int index = x + y * mMap->getWidth();
				if(loadedTileData && index < (int)tileData.size())
				{
					mTileData[x][y].setFromLoaded(tileData[index]);
					//clog << "Index: " << index << " (" << x << ',' << y << ") " << mTileData[x][y].getFoodValue() << endl;
				}
			}
		}
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

FindEntityResult Game::findClosestEntity(Vector2f startPosition, const string &entityType, const GameEntity *ignore)
{
	return findClosestEntity(startPosition, entityType, ignore, NULL);
}

FindEntityResult Game::findClosestEntity(Vector2f startPosition, const string &entityType, const GameEntity *ignore, const string &species)
{
	return findClosestEntity(startPosition, entityType, ignore, &species);
}

FindEntityResult Game::findBreedingPartner(Animal *origin)
{
	Animal *shortestAnimal = NULL;
	vector<Vector2f> *shortestPath = NULL;
	float shortest = 1e30f;
	Map *m = getMap();
	Vector2i positionInt = (Vector2i)origin->getPosition();
	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		Animal *ani = dynamic_cast<Animal*>(*iter);
		if(ani == NULL || ani == origin)
		{
			continue;
		}

		if (iequals(origin->getSpecies(), ani->getSpecies()) &&
			ani->wantsToBreed())
		{
			vector<Vector2f> *path = m->search(positionInt, ani->getPosition());
			if (path != NULL)
			{
				float length = lengthOfPath(path);
				if(length < shortest)
				{
					shortest = length;
					shortestAnimal = ani;
					if(shortestPath != NULL)
					{
						delete shortestPath;
					}
					shortestPath = path;
				}
			}
		}
	}
	return FindEntityResult(shortestAnimal, shortestPath);
}

FindEntityResult Game::findClosestEntity(Vector2f position, const string &entityType, const GameEntity *ignore, const string *species)
{
	GameEntity *shortestEntity = NULL;
	vector<Vector2f> *shortestPath = NULL;
	float shortest = 1e30f;
	Map *m = getMap();
	Vector2i positionInt = (Vector2i)position;
	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		GameEntity *entity = *iter;
		if  (iequals(entity->getEntityType(), entityType) && 
			(species == NULL || (species != NULL && iequals(*species, entity->getSpecies()))))
		{
			vector<Vector2f> *path = m->search(positionInt, entity->getPosition());
			if (path != NULL)
			{
				float length = lengthOfPath(path);
				if(length < shortest && (entity != ignore && ignore != NULL))
				{
					shortest = length;
					shortestEntity = entity;
					if(shortestPath != NULL)
					{
						delete shortestPath;
					}
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
	saveOption(CURRENT_TIME, file);
	saveOption(CURRENT_DAY, file);
	saveOption(DAY_LENGTH, file);
	saveOption(CAMERA_LOCATION, file);
}

void Game::saveOption(const GameOption &option, FormattedFile &file)
{
	switch(option)
	{
	case HUD_WIDTH:
		file << GamePropertyNames[HUD_WIDTH] << ' ' << getHudWidth() << '\n';
		break;
	case CURRENT_TIME:
		file << GamePropertyNames[CURRENT_TIME] << ' ' << getCurrentTime() << '\n';
		break;
	case CURRENT_DAY:
		file << GamePropertyNames[CURRENT_DAY] << ' ' << getCurrentDay() << '\n';
		break;
	case DAY_LENGTH:
		file << GamePropertyNames[DAY_LENGTH] << ' ' << getDayLength() << '\n';
		break;
	case CAMERA_LOCATION:
		file << GamePropertyNames[CAMERA_LOCATION] << ' ' << mScreenSize.getX() << ' ' << mScreenSize.getY() << '\n';
		break;
	default:
		break;
	}
}

void Game::loadOptions(string option, FormattedFileIterator &iter)
{
	if(iequals(option, GamePropertyNames[HUD_WIDTH]))
	{
		setHudWidth(lexical_cast<int>(*iter));
		++iter;
	}
	else if(iequals(option, GamePropertyNames[CURRENT_TIME]))
	{
		setCurrentTime(lexical_cast<float>(*iter));
		++iter;
	}
	else if(iequals(option, GamePropertyNames[CURRENT_DAY]))
	{
		setCurrentDay(lexical_cast<int>(*iter));
		++iter;
	}
	else if(iequals(option, GamePropertyNames[DAY_LENGTH]))
	{
		setDayLength(lexical_cast<float>(*iter));
		++iter;
	}
	else if(iequals(option, GamePropertyNames[CAMERA_LOCATION]))
	{
		int x = lexical_cast<int>(*iter); ++iter;
		int y = lexical_cast<int>(*iter); ++iter;
		moveCamera(x, y);
	}
	else
	{
		clog << "Unknown option '" << option << "'" << endl;
	}
}

void Game::setCurrentTime(float time)
{
	int days = 0;
	while(time > getDayLength())
	{
		time -= getDayLength();
		days++;
	}
	while(time < 0.0f)
	{
		time += getDayLength();
		days--;
	}
	mCurrentTime = time;
	advanceDay(days);
}

void Game::advanceTime(float dt)
{
	setCurrentTime(getCurrentTime() + dt); 
}

string Game::getCurrentTimeString(bool includeDay)
{
	float div = getCurrentTime() * 24 / getDayLength();
	int hours = (int)div;
	int minutes = (int)((div - (float)hours) * 60);

	if(includeDay)
	{
		format fmt("%u %02u:%02u");
		fmt % getCurrentDay() % hours % minutes;

		return fmt.str();
	}
	else
	{
		format fmt("%02u:%02u");
		fmt % hours % minutes;

		return fmt.str();
	}
}

RayResult Game::fireRay(const Vector2f &point, const Vector2f &direction, const float &length)
{
	RayResult result;

	float x1 = point.x;
	float y1 = point.y;

	EntityList lookingAt;

	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		GameEntity *entity = *iter;
		if(math::getDistanceToRay(point, direction, length, entity->getPosition()) < entity->getSize())
		{
			lookingAt.push_back(entity);
		}
	}

	EntityList canSee;

	for(EntityList::iterator iter = lookingAt.begin(); iter != lookingAt.end(); ++iter)
	{
		GameEntity *entity = *iter;
		RayResult test;

		Vector2f pos = entity->getPosition();

		float x2 = pos.x;
		float y2 = pos.y;
		
		bresenhamLine(x1, y1, x2, y2, NULL, '\0', &test);
		
		Vector2f dPos = pos.sub(point);
		Vector2f dPoint = test.point.sub(point);
		if(dPos.length() < dPoint.length())
		{
			canSee.push_back(entity);
		}
	}

	return result;
}

RayResult Game::fireRay(const Vector2f &point, const float &direction, const float &length)
{
	float x1 = point.x;
	float y1 = point.y;

	float x2 = cosf(direction);
	float y2 = sinf(direction);

	return fireRay(Vector2f(x1, y1), Vector2f(x2, y2), length);
}

void Game::findNearby(Vector2f origin, const float &radius, vector<GameEntity *> &results, string &restrictToSpecies)
{
	findNearby(origin, radius, results, &restrictToSpecies);
}

void Game::findNearby(Vector2f origin, const float &radius, vector<GameEntity *> &results, string *restrictToSpecies)
{
	vector<GameEntity *> withinRadius;

	for(EntityList::iterator iter = mEntities.begin(); iter != mEntities.end(); ++iter)
	{
		GameEntity *entity = *iter;
		if(restrictToSpecies != NULL && !iequals(*restrictToSpecies, entity->getSpecies()))
		{
			continue;
		}

		Vector2f diff = origin.sub(entity->getPosition());
		if(diff.length() < radius)
		{
			withinRadius.push_back(entity);
		}
	}

	for(EntityList::iterator iter = withinRadius.begin(); iter != withinRadius.end(); ++iter)
	{
		GameEntity *entity = *iter;
		RayResult cast;
		Vector2f pos = entity->getPosition();
		bresenhamLine(origin.x, origin.y, pos.x, pos.y, NULL, '\0', &cast);
		if(cast.point.x < 0 || cast.point.y < 0)
		{
			results.push_back(entity);
		}
	}
}

void Game::drawLine(WINDOW *wnd, const char &c, const Vector2f &point, const Vector2f &direction, const float &length)
{
	float x1 = point.x;
	float y1 = point.y;

	float x2 = direction.x * length + x1;
	float y2 = direction.y * length + y1;

	bresenhamLine(x1, y1, x2, y2, wnd, c, NULL);
}

void Game::drawLine(WINDOW *wnd, const char &c, const Vector2f &point, const float &direction, const float &length)
{
	float x1 = point.x;
	float y1 = point.y;

	float x2 = cosf(direction) * length + x1;
	float y2 = sinf(direction) * length + y1;

	bresenhamLine(x1, y1, x2, y2, wnd, c, NULL);
}

void Game::drawLine(WINDOW *wnd, const int &x1, const int &y1, const int &x2, const int &y2, const char &c)
{
	drawLine(wnd, (float)x1, (float)y1, (float)x2, (float)y2, c);
}

void Game::drawLine(WINDOW *wnd, float x1, float y1, float x2, float y2, const char &c)
{
	bresenhamLine(x1, y1, x2, y2, wnd, c, NULL);
}

void Game::bresenhamLine(float x1, float y1, float x2, float y2, WINDOW *wnd, const char &c, RayResult *result)
{
	Map *map = getMap();

	Tile *t = NULL;
	if(result != NULL)
	{
		t = map->getTile((int)x1, (int)y1);
		if (t == NULL)
		{
			return;
		}
		if (!t->getTransparent())
		{
			result->point.x = x1;
			result->point.y = y1;
			result->tile = t;
			return;
		}
	}

	// Bresenham's line algorithm

	// When true, this indicates that the algorithm is going from x2,y2 -> x1,y1 instead
	// of x1,y1 -> x2,y2. This means it should take the last result found as the result
	// we want.
	bool swapped = false;
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}
 
	if(x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
		swapped = true;
	}
 
	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);
 
	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)round(y1);
 
	const int maxX = (int)round(x2);
	
	for(int x = (int)round(x1); x < maxX; x++)
	{
		if (result != NULL)
		{
			t = NULL;
			if (steep)
			{
				t = map->getTile(y, x);
			}
			else
			{
				t = map->getTile(x, y);
			}
			if(t != NULL && !t->getTransparent())
			{
				result->point.x = (float)(steep ? y : x);
				result->point.y = (float)(steep ? x : y);
				result->tile = t;
				if(!swapped)
				{
					break;
				}
			}
		}
		else if(wnd != NULL)
		{
			if (steep)
			{
				renderChar(wnd, y, x, c);
			}
			else
			{
				renderChar(wnd, x, y, c);
			}
		}
 
		error -= dy;
		if(error < 0)
		{
			y += ystep;
			error += dx;
		}
	}

	return;
}

void Game::loadNameList(string filename)
{
	ifstream file(filename);
	if(!file.is_open())
	{
		clog << "Unable to open name list '" << filename << "'" << endl;
		return;
	}

	char buff[255];
	while(!file.eof())
	{
		file.getline(buff, 255);
		string line = buff;
		if(line.length() > 0)
		{
			mListOfNames.push_back(line);
		}
	}

	clog << "Loaded " << mListOfNames.size() << " names." << endl;
}

string Game::getRandomName()
{
	if(mListOfNames.size() == 0)
	{
		return "Animal";
	}

	int num = round(math::nextFloat() * (mListOfNames.size() - 1));
	return mListOfNames[num];
}