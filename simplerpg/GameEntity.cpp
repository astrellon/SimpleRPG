#include "GameEntity.h"
#include "Game.h"

GameEntity::GameEntity(Game *game)
{
	mGame = game;
	mGraphic = Pixel('o', 7, false);
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::move(double dx, double dy, bool inObjectSpace)
{
	Vector2 v(dx, dy);
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

void GameEntity::move(Vector2 vec, bool inObjectSpace)
{
	move(vec.x, vec.y, inObjectSpace);
}

void GameEntity::turn(double angle)
{
	mTransform.rotate(angle);
}

void GameEntity::update()
{
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