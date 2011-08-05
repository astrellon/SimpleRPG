#include "GameEntity.h"
#include "Game.h"

unsigned int GameEntity::sId = 0;

map<unsigned int, GameEntity *> GameEntity::sEntities;

extern const char *EntityPropertyNames[] = {"id", "facing", "position", "destination", "name", "graphic",
	"health", "strength", "dexterity", "intelligence", "running_speed", "walking_speed", "turning_speed",
	"entity_size", "entity_mass", "diet", "damage_base", "amount_eaten", "current_action", "action_history"};

GameEntity::GameEntity(Game *game)
{
	mGame = game;
	mGraphic = Pixel('o', 7, false);
	mName = "GameEntity";
	mAmountEaten = 0.0f;
	mId = nextId();

	mCurrentAction = new Action(IDLE);
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::setCurrentAction(Action *action)
{
	if(mCurrentAction->getAction() != MAX_ID)
	{
		mPastActions.push_back(mCurrentAction);
	}
	mCurrentAction = action;
}

Pixel GameEntity::getGraphic()
{
	return mGraphic;
}
void GameEntity::setGraphic(Pixel graphic)
{
	mGraphic = graphic;
}

float GameEntity::getX()
{
	return mTransform.zx;
}
float GameEntity::getY()
{
	return mTransform.zy;
}

Vector2f GameEntity::getPosition()
{
	return Vector2f(mTransform.zx, mTransform.zy);
}

double GameEntity::getFacing()
{
	return mTransform.getAngle();
}
void GameEntity::setFacing(double facing)
{
	mTransform.setRotation(facing);
}

Matrix3x3f *GameEntity::getTransform()
{
	return &mTransform;
}

void GameEntity::move(float dx, float dy, bool inObjectSpace)
{
	Vector2f v(dx, dy);
	if(inObjectSpace)
		mTransform.transformVectorConst(&v);

	int newPosX = (int)round(mTransform.zx + v.x);
	int newPosY = (int)round(mTransform.zy + v.y);

	Tile *tile = mGame->getMap()->getTile(newPosX, newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(v);
		return;
	}

	tile = mGame->getMap()->getTile((int)round(mTransform.zx), newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(0, v.y);
		return;
	}

	tile = mGame->getMap()->getTile(newPosX, (int)round(mTransform.zy));
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(v.x, 0);
		return;
	}
}

void GameEntity::move(Vector2f vec, bool inObjectSpace)
{
	move(vec.x, vec.y, inObjectSpace);
}

void GameEntity::turn(double angle)
{
	mTransform.rotate(angle);
}

void GameEntity::render(Rect screenSize, WINDOW *wnd)
{
	int xPos = (int)round(getX()) - screenSize.getX();
	int yPos = (int)round(getY()) - screenSize.getY();

	if (xPos < 0 || xPos >= screenSize.getWidth() ||
		yPos < 0 || yPos >= screenSize.getHeight())
	{
		return;
	}

	getGraphic().render(wnd, xPos, yPos);
}

void GameEntity::loadFromFile(boost::sregex_token_iterator &iter)
{
	boost::sregex_token_iterator end;

	while(iter != end)
	{
		string line = *iter;
		if(iequals(line, "end"))
		{
			iter++;
			break;
		}
		loadProperties(iter);
	}
}

void GameEntity::saveToFile(ofstream &file)
{
	file << Game::getOutputTabs() << getEntityType() << endl;
	Game::changeOutputFileLevel(1);
	saveProperties(file);
	Game::changeOutputFileLevel(-1);
	file << Game::getOutputTabs() << "end" << endl << endl;
}

void GameEntity::saveProperties(ofstream &file)
{
	saveProperty(ID, file);
	saveProperty(POSITION, file);
	saveProperty(FACING, file);
	saveProperty(NAME, file);
	saveProperty(AMOUNT_EATEN, file);
	saveProperty(CURRENT_ACTION, file);
	saveProperty(ACTION_HISTORY, file);
}

void GameEntity::saveProperty(const EntityProperty &propertyId, ofstream &file)
{
	Vector2f v;

	switch(propertyId)
	{
	case ID:
		file << Game::getOutputTabs() << EntityPropertyNames[ID] << ' ' << getId() << endl;
		break;
	case POSITION:
		v = getPosition();
		file << Game::getOutputTabs() << EntityPropertyNames[POSITION] << ' ' << v.x << ' ' << v.y << endl;
		break;
	case FACING:
		file << Game::getOutputTabs() << EntityPropertyNames[FACING] << ' ' << getFacing() << endl;
		break;
	case NAME:
		file << Game::getOutputTabs() << EntityPropertyNames[NAME] << " \"" << getEntityName() << '\"' << endl;
		break;
	case AMOUNT_EATEN:
		file << Game::getOutputTabs() << EntityPropertyNames[AMOUNT_EATEN] << ' ' << getAmountEaten() << endl;
		break;
	case CURRENT_ACTION:
		file << Game::getOutputTabs() << EntityPropertyNames[CURRENT_ACTION] << endl;
		getCurrentAction()->saveToFile(file);
		break;
	case ACTION_HISTORY:
		file << Game::getOutputTabs() << EntityPropertyNames[ACTION_HISTORY] << endl;
		Game::changeOutputFileLevel(1);
		for(vector<Action *>::iterator iter = getPastActions()->begin(); iter != getPastActions()->end(); iter++)
		{
			(*iter)->saveToFile(file);
		}
		Game::changeOutputFileLevel(-1);
		file << Game::getOutputTabs() << "end" << endl;
		break;
	default:
		cout << "Unable to save unknown entity property " << propertyId << endl;
		break;
	}
}

void GameEntity::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter++;
	if(iequals(propertyName, EntityPropertyNames[ID]))
	{
		setId(lexical_cast<unsigned int>(*iter++));
	}
	else if(iequals(propertyName, EntityPropertyNames[POSITION]))
	{
		float x = lexical_cast<float>(*iter++);
		float y = lexical_cast<float>(*iter++);
		move(x, y, false);
	}
	else if(iequals(propertyName, EntityPropertyNames[FACING]))
	{
		float f = lexical_cast<float>(*iter++);
		setFacing(f);
	}
	else if(iequals(propertyName, EntityPropertyNames[NAME]))
	{
		string name = *iter++;
		if(name[0] == '"')
		{
			name = name.substr(1, name.size() - 2);
		}
		setEntityName(name);
	}
	else if(iequals(propertyName, EntityPropertyNames[AMOUNT_EATEN]))
	{
		mAmountEaten = lexical_cast<float>(*iter++);
	}
	else if(iequals(propertyName, EntityPropertyNames[CURRENT_ACTION]))
	{
		string line = *iter++;
		Action *action = ActionFactory::create(line);
		if(action == NULL)
		{
			cout << "Error! Could not create action '" << line << "' instance." << endl;
			return;
		}
		action->loadFromFile(iter);
		mCurrentAction = action;
	}
	else if(iequals(propertyName, EntityPropertyNames[ACTION_HISTORY]))
	{
		string line = *iter++;
		boost::sregex_token_iterator end;
		while(iter != end && !iequals(line, "end"))
		{
			Action *action = ActionFactory::create(line);
			{
				if(action == NULL)
				{
					cout << "Error! Could not create action '" << line << "' instance." << endl;
					continue;
				}
				action->loadFromFile(iter);
				mPastActions.push_back(action);
			}
			line = *iter++;
		}
	}
	else 
	{
		cout << "Unable to load unknown property '" << propertyName << '\'' << endl;
	}
}

float GameEntity::beEaten(GameEntity *eater)
{
	return 0.0f;
}

void GameEntity::displayActions(UIContainer &hud)
{
	if(!mRedisplay)
		return;

	mHudText->clearText();
	*mHudText << "<15>Entity</>: " << getEntityName() << '\n';
	*mHudText << "<15>Facing</>: " << getFacing() << '\n';
	Action *action = getCurrentAction();
	*mHudText << "<15>Action</>: " << Action::EntityActionNames[action->getAction()] << '\n';
	*mHudText << "<15>Step</>:   " << action->getStep() << '\n';

	mRedisplay = false;
}

void GameEntity::clearDisplay(UIContainer &hud)
{
	hud.removeChild(*mHudText);
	mRedisplay = true;

	delete mHudText;
}

void GameEntity::setupDisplay(UIContainer &hud)
{
	hud.removeAllChildren(false);

	mHudText = new UIText();

	mHudText->setY(1);
	hud.addChild(*mHudText);

}