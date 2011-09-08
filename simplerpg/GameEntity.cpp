#include "GameEntity.h"
#include "Game.h"

unsigned int GameEntity::sId = 0;

map<unsigned int, GameEntity *> GameEntity::sEntities;

extern const char *EntityPropertyNames[] = { "id", "facing", "position", "destination", "name", "graphic",
	"health", "strength", "dexterity", "intelligence", "running_speed", "walking_speed", "turning_speed",
	"entity_size", "entity_mass", "diet", "damage_base", "amount_eaten", "current_action", "action_history",
	"attack_rate", "attack_cooldown", "energy", "rest_energy_per_day", "species", "species_alignment" };

GameEntity::GameEntity(Game *game)
{
	mGame = game;
	mGraphic = Pixel('o', 7, false);
	mName = "GameEntity";
	mAmountEaten = 0.0f;
	mId = nextId();

	mMass = 1.0f;
	mSize = 1.0f;

	mSpecies = "Unknown";

	mCurrentAction = new Action(IDLE);

	mMenuLevel = 0;
	mMaxMenuLevel = 1;
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

void GameEntity::loadFromFile(FormattedFileIterator &iter)
{
	while(!iter.atEnd())
	{
		string line = *iter;
		if(iequals(line, "end"))
		{
			++iter;
			break;
		}
		loadProperties(iter);
	}
}

void GameEntity::saveToFile(FormattedFile &file)
{
	file << getEntityType() << '\n';
	file.changeTabLevel(1);
	saveProperties(file);
	file.changeTabLevel(-1);
	file << "end\n\n";
}

void GameEntity::saveProperties(FormattedFile &file)
{
	saveProperty(ID, file);
	saveProperty(SPECIES, file);
	saveProperty(POSITION, file);
	saveProperty(FACING, file);
	saveProperty(NAME, file);
	saveProperty(ENTITY_SIZE, file);
	saveProperty(ENTITY_MASS, file);
	saveProperty(AMOUNT_EATEN, file);
	saveProperty(CURRENT_ACTION, file);
	saveProperty(ACTION_HISTORY, file);
}

void GameEntity::saveProperty(const EntityProperty &propertyId, FormattedFile &file)
{
	Vector2f v;

	switch(propertyId)
	{
	case ID:
		file << EntityPropertyNames[ID] << ' ' << getId() << '\n';
		break;
	case SPECIES:
		file << EntityPropertyNames[SPECIES] << " \"" << getSpecies() << "\"\n";
		break;
	case POSITION:
		v = getPosition();
		file << EntityPropertyNames[POSITION] << ' ' << v.x << ' ' << v.y << '\n';
		break;
	case FACING:
		file << EntityPropertyNames[FACING] << ' ' << getFacing() << '\n';
		break;
	case NAME:
		file << EntityPropertyNames[NAME] << " \"" << getEntityName() << "\"\n";
		break;
	case ENTITY_SIZE:
		file << EntityPropertyNames[ENTITY_SIZE] << ' ' << getSize() << '\n';
		break;
	case ENTITY_MASS:
		file << EntityPropertyNames[ENTITY_MASS] << ' ' << getMass() << '\n';
		break;
	case AMOUNT_EATEN:
		file << EntityPropertyNames[AMOUNT_EATEN] << ' ' << getAmountEaten() << '\n';
		break;
	case CURRENT_ACTION:
		file << EntityPropertyNames[CURRENT_ACTION] << '\n';
		getCurrentAction()->saveToFile(file);
		break;
	case ACTION_HISTORY:
		file << EntityPropertyNames[ACTION_HISTORY] << '\n';
		file.changeTabLevel(1);
		for(vector<Action *>::iterator iter = getPastActions()->begin(); iter != getPastActions()->end(); iter++)
		{
			(*iter)->saveToFile(file);
		}
		file.changeTabLevel(-1);
		file << "end\n";
		break;
	default:
		clog << "Unable to save unknown entity property " << propertyId << endl;
		break;
	}
}

void GameEntity::loadProperties(FormattedFileIterator &iter)
{
	string propertyName = *iter;
	++iter;
	if(iequals(propertyName, EntityPropertyNames[ID]))
	{
		setId(lexical_cast<unsigned int>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[SPECIES]))
	{
		string species = *iter; ++iter;
		if(species[0] == '"')
		{
			species = species.substr(1, species.size() - 2);
		}
		setSpecies(species);
	}
	else if(iequals(propertyName, EntityPropertyNames[POSITION]))
	{
		float x = lexical_cast<float>(*iter); ++iter;
		float y = lexical_cast<float>(*iter); ++iter;
		move(x, y, false);
	}
	else if(iequals(propertyName, EntityPropertyNames[FACING]))
	{
		float f = lexical_cast<float>(*iter); ++iter;
		setFacing(f);
	}
	else if(iequals(propertyName, EntityPropertyNames[NAME]))
	{
		string name = *iter; ++iter;
		if(name[0] == '"')
		{
			name = name.substr(1, name.size() - 2);
		}
		setEntityName(name);
	}
	else if(iequals(propertyName, EntityPropertyNames[ENTITY_SIZE]))
	{
		setSize(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[ENTITY_MASS]))
	{
		setMass(lexical_cast<float>(*iter)); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[AMOUNT_EATEN]))
	{
		mAmountEaten = lexical_cast<float>(*iter); ++iter;
	}
	else if(iequals(propertyName, EntityPropertyNames[CURRENT_ACTION]))
	{
		string line = *iter; ++iter;
		Action *action = ActionFactory::create(line);
		if(action == NULL)
		{
			clog << "Error! Could not create action '" << line << "' instance." << endl;
			return;
		}
		action->loadFromFile(iter);
		mCurrentAction = action;
	}
	else if(iequals(propertyName, EntityPropertyNames[ACTION_HISTORY]))
	{
		string line = *iter; ++iter;
		while(!iter.atEnd() && !iequals(line, "end"))
		{
			Action *action = ActionFactory::create(line);
			{
				if(action == NULL)
				{
					clog << "Error! Could not create action '" << line << "' instance." << endl;
					continue;
				}
				action->loadFromFile(iter);
				mPastActions.push_back(action);
			}
			line = *iter; ++iter;
		}
	}
	else 
	{
		clog << "Unable to load unknown property '" << propertyName << '\'' << endl;
	}
}

double GameEntity::distanceToEntity(GameEntity *entity)
{
	if(entity == NULL)
	{
		return 0.0;
	}
	Vector2f otherPos = entity->getPosition();
	Vector2f pos = getPosition();

	return otherPos.sub(pos).length();
}

float GameEntity::beEaten(GameEntity *eater)
{
	return 0.0f;
}

void GameEntity::keyActions(const int key)
{
	if (key >= '0' && key <= '9')
	{
		int num = key - '1';
		if (key == '0')
		{
			num = 9;
		}

		if (num < mMaxMenuLevel)
		{
			mMenuLevel = num;
			mRedisplay = true;
		}
	}
}

void GameEntity::displayActions(UIContainer &hud)
{
	if(!mRedisplay && mGame->getGamePaused())
		return;

	UIText &text = *mHudText;

	text.clearText();

	text << "<15>Entity</>:\t" << getEntityName() << '\n';
	text << "<15>Species</>: " << getSpecies() << '\n';

	if(mMaxMenuLevel > 1)
	{
		text << "<15>[ <12>";

		for(int i = 0; i < mMaxMenuLevel; i++)
		{
			if(i == mMenuLevel)
			{
				text << "<11>" << (i + 1) << "</> ";
			}
			else
			{
				text << (i + 1) << ' ';
			}
		}
		text << "</>]</>\n";
	}

	text << '\n';

	if(mMenuLevel == 0)
	{
		text << "<15>Facing</>:\t" << getFacing() << '\n';
		Action *action = getCurrentAction();
		text << "<15>Action</>:\t" << Action::EntityActionNames[action->getAction()] << '\n';
		text<< "<15>Step</>:\t" << action->getStep() << '\n';
	}
	mRedisplay = false;
}

void GameEntity::clearDisplay(UIContainer &hud)
{
	hud.removeChild(mHudText);
	mRedisplay = true;

	delete mHudText;
}

void GameEntity::setupDisplay(UIContainer &hud)
{
	hud.removeAllChildren(false);

	mHudText = new UIText();
	mHudText->setWordWrap(true);
	mHudText->setMaxWidth(hud.getMaxWidth());

	mHudText->setY(1);
	hud.addChild(mHudText);
}

void GameEntity::setId( unsigned int id )
{
	EntityMap::iterator iter = sEntities.find(id);
	if(iter == sEntities.end())
	{
		iter = sEntities.find(mId);
		if (iter != sEntities.end())
			sEntities.erase(iter);

		sId = max(sId, id) + 1;
		mId = id;

		sEntities[id] = this;
	}
	else
	{
		throw "Two entities attempting to use the same ID!";
	}
}

GameEntity * GameEntity::getEntityById( unsigned int id )
{
	EntityMap::iterator iter = sEntities.find(id);
	if(iter == sEntities.end())
	{
		return NULL;
	}
	return iter->second;
}

void GameEntity::getNearbyEntities(const float &radius, vector<GameEntity *> &result)
{
	getNearbyEntities(radius, result, NULL);
}

void GameEntity::getNearbyEntities(const float &radius, vector<GameEntity *> &result, const string &restrictToSpecies)
{
	getNearbyEntities(radius, result, &restrictToSpecies);
}

void GameEntity::getNearbyEntities(const float &radius, vector<GameEntity *> &result, const string *restrictToSpecies)
{
	// Quick and easy way fof iterating through all entities.

}