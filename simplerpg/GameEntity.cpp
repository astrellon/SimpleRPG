#include "GameEntity.h"
#include "Game.h"

unsigned int GameEntity::sId = 0;

map<unsigned int, GameEntity *> GameEntity::sEntities;

GameEntity::GameEntity(Game *game)
{
	mGame = game;
	mGraphic = Pixel('o', 7, false);
	mName = "GameEntity";
	mAmountEaten = 0.0f;
	mId = nextId();
}

GameEntity::~GameEntity(void)
{
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

	int newPosX = round(mTransform.zx + v.x);
	int newPosY = round(mTransform.zy + v.y);

	Tile *tile = mGame->getMap()->getTile(newPosX, newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(v);
		return;
	}

	tile = mGame->getMap()->getTile(round(mTransform.zx), newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(0, v.y);
		return;
	}

	tile = mGame->getMap()->getTile(newPosX, round(mTransform.zy));
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
	int xPos = round(getX()) - screenSize.getX();
	int yPos = round(getY()) - screenSize.getY();

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
			break;
		}
		loadProperties(iter);
	}
}

void GameEntity::saveToFile(ofstream &file)
{
	file << getEntityType() << endl;

	saveProperties(file);

	file << "end" << endl << endl;
}

void GameEntity::saveProperties(ofstream &file)
{
	saveProperty(ID, file);
	saveProperty(POSITION, file);
	saveProperty(FACING, file);
	saveProperty(NAME, file);
}

void GameEntity::saveProperty(const EntityProperty &propertyId, ofstream &file)
{
	Vector2f v;

	switch(propertyId)
	{
	case ID:
		file << "id " << getId() << endl;
		break;
	case POSITION:
		v = getPosition();
		file << "position " << v.x << ' ' << v.y << endl;
		break;
	case FACING:
		file << "facing " << getFacing() << endl;
		break;
	case NAME:
		file << "name \"" << getEntityName() << "\"" << endl;
		break;
	default:
		cout << "Unable to save unknown property " << propertyId << endl;
		break;
	}
}

void GameEntity::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, "id"))
	{
		iter++;
		setId(lexical_cast<unsigned int>(*iter++));
	}
	else if(iequals(propertyName, "position"))
	{
		iter++;
		float x = lexical_cast<float>(*iter++);
		float y = lexical_cast<float>(*iter++);
		move(x, y, false);
	}
	else if(iequals(propertyName, "facing"))
	{
		iter++;
		float f = lexical_cast<float>(*iter++);
		setFacing(f);
	}
	else if(iequals(propertyName, "name"))
	{
		iter++;
		string name = *iter++;
		if(name[0] == '"')
		{
			name = name.substr(1, name.size() - 2);
		}
		setEntityName(name);
	}
	else 
	{
		iter++;
		cout << "Unable to load unknown property '" << propertyName << "'" << endl;
	}
}

void GameEntity::displayActions(UIContainer &hud)
{
	if(!mRedisplay)
		return;

	mHudText->clearText();
	*mHudText << "Entity: " << getEntityName() << '\n';
	*mHudText << "Facing: " << getFacing() << '\n';

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