#include "GameEntity.h"
#include "Game.h"

GameEntity::GameEntity(Game *game)
{
	mGame = game;
	mGraphic = Pixel('o', 7, false);
	mName = "GameEntity";
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::move(MathType dx, MathType dy, bool inObjectSpace)
{
	Vector2 v(dx, dy);
	if(inObjectSpace)
		mTransform.transformVectorConst(&v);

	int newPosX = math::round(mTransform.zx + v.x);
	int newPosY = math::round(mTransform.zy + v.y);

	Tile *tile = mGame->getMap()->getTile(newPosX, newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(v);
		return;
	}

	tile = mGame->getMap()->getTile(math::round(mTransform.zx), newPosY);
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(0, v.y);
		return;
	}

	tile = mGame->getMap()->getTile(newPosX, math::round(mTransform.zy));
	if(tile != NULL && tile->getPassable())
	{
		mTransform.translate(v.x, 0);
		return;
	}
}

void GameEntity::move(Vector2 vec, bool inObjectSpace)
{
	move(vec.x, vec.y, inObjectSpace);
}

void GameEntity::turn(MathType angle)
{
	mTransform.rotate(angle);
}

void GameEntity::render(Rect screenSize, WINDOW *wnd)
{
	int xPos = math::round(getX()) - screenSize.getX();
	int yPos = math::round(getY()) - screenSize.getY();

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
	saveProperty(PROPERTY_POSITION, file);
	saveProperty(PROPERTY_NAME, file);
}

void GameEntity::saveProperty(const int &propertyId, ofstream &file)
{
	Vector2 v;

	switch(propertyId)
	{
	case PROPERTY_POSITION:
		v = getPosition();
		file << "position " << v.x << ' ' << v.y << endl;
		break;
	case PROPERTY_NAME:
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
	if(iequals(propertyName, "position"))
	{
		iter++;
		float x = atof(string(*iter++).c_str());
		float y = atof(string(*iter++).c_str());
		move(x, y, false);
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